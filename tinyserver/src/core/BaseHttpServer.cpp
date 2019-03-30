//
// Created by hhk on 18-12-18.
//

#include "core/BaseHttpServer.h"
#include "core/HandleError.h"
#include "core/BaseHttpServer.h"
#include <fcntl.h>
#include <errno.h>
#include "utils/systool.h"
#include "tinytemplate.h"

void WebServer::BaseHttpServer::server_init() {
    s_srv_ctx = (server_context_st *) malloc(sizeof(server_context_st));
    memset(s_srv_ctx, 0, sizeof(server_context_st));
    for (auto &cli : s_srv_ctx->cli) {
        cli.fd = -1;
    }
    int fd;
    struct sockaddr_in servaddr{};
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        fprintf(stderr, "create sock error\n");
        exit(0);
    }

    /*一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。*/
    int reuse = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ADDRESS, &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT);
    if (bind(fd, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
        fprintf(stderr, "bind error\n");
        exit(0);
    }
    listen(fd, LISTING);
    server_fd = fd;
}

bool WebServer::BaseHttpServer::enrol(const string& url_rule, WebServer::IHttpServer::VIEW_HANDLER func) {
    router_table.emplace_back(std::make_pair(regex(url_rule), func));
    return true;
}

void WebServer::BaseHttpServer::run() {
    int srvfd = server_fd;
    int clifd = -1;
    int ret_val = 0;
    fd_set *read_fds = &s_srv_ctx->allfs;

    while (true) {
        FD_ZERO(read_fds); // 每次select之后文件描述符都会被修改
        FD_SET(srvfd, read_fds); // 服务端read，等待客户端连接
        s_srv_ctx->maxfd = srvfd;
        struct timeval tv{};
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        for (int i = 0; i < s_srv_ctx->cli_cnt; i++) {
            clifd = s_srv_ctx->cli[i].fd;
            if (clifd != -1) {
                FD_SET(clifd, read_fds); // 客户端send
            }
            s_srv_ctx->maxfd = (clifd > s_srv_ctx->maxfd ? clifd : s_srv_ctx->maxfd);
        }
        ret_val = select(s_srv_ctx->maxfd + 1, read_fds, nullptr, nullptr, &tv); // 寻找可以处理的socket
        if (ret_val == -1) {
            fprintf(stderr, "select error %s\n", strerror(errno));
            exit(-1);
        }
        if (ret_val == 0) {
            fprintf(stderr, "select timeout\n");
            idle();
            continue;
        }
        if (FD_ISSET(srvfd, read_fds)) { // 检查集合中指定的文件描述符是否可以读写
            // fd为服务端，说明是客户端请求连接
            accept_client(srvfd);
        } else {
            // 客户端发来了消息
            recv_client_msg(read_fds);
        }
    }
}

pair<WebServer::IHttpServer::VIEW_HANDLER , vector<string>> WebServer::BaseHttpServer::match(const string& url) {
    smatch result;
    vector<string> ret;
    for(auto& item: router_table) {
        if(regex_match(url, result, item.first)) {
            for(size_t i=1;i < result.size();i++)
                ret.emplace_back(result.str(i));
            return {item.second, ret};
        }
    }
    return {nullptr, {}};
}

void WebServer::BaseHttpServer::accept_client(int srvfd) {
    sockaddr_in cliaddr{};
    int clifd;
    int i;
    socklen_t socklen = sizeof(cliaddr);
    clifd = accept(srvfd, (struct sockaddr *) &cliaddr, &socklen);
    if (clifd == -1) {
        fprintf(stderr, "accept fail, error %s \n", strerror(errno));
        return;
    }
    fprintf(stdout, "accept a new client, %s:%d, current clients: %d\n", inet_ntoa(cliaddr.sin_addr),
            cliaddr.sin_port, s_srv_ctx->cli_cnt + 1);
    for (i = 0; i < USERSIZE; i++) {
        if (s_srv_ctx->cli[i].fd < 0) {
            s_srv_ctx->cli[i].fd = clifd;
            s_srv_ctx->cli[i].addr = cliaddr.sin_addr;
            s_srv_ctx->cli[i].port = cliaddr.sin_port;
            s_srv_ctx->cli_cnt++;
            break;
        }
    }
    if (i == USERSIZE) {
        fprintf(stderr, "too many client\n");
        close(clifd);
        return;
    }
}

void WebServer::BaseHttpServer::handle_client_msg(client_info_t* cli, char *msg) {
    int clifd = cli->fd;
    HttpRequest request;
    HttpResponse response;
    string url = request.parseRequest(msg).get_url();
    request.set_ip(cli->addr);
    request.set_port(cli->port);

    if(url.empty()) {
        printf("url 解析失败\n");
        handle_500(response, "解析http头部失败");
    } else {
        // TODO request中间件

        /* 视图处理部分 */
        auto match_res = match(url);
        auto func = match_res.first;  // url匹配函数
        request.set_url_parm(match_res.second); // url匹配参数

        if (func != nullptr) {
            try {
                func(request, response);
            } catch (Exception404& e) {
                cerr << "错误信息: " << e.what() << endl;
                print_trace();
                handle_404(response, e.what());
            } catch (Exception500& e) {
                cerr << "错误信息: " << e.what() << endl;
                print_trace();
                handle_500(response, e.what());
            }
            catch (TinyTemplate::TemplateParseException& e) {
                cerr << e.what() << endl;
                handle_500(response, e.what());
            } catch (BaseException& e) {
                // TODO 自定义exception处理
                cerr << "错误信息: " << e.what() << endl;

            } catch(exception& e) {
                cerr << "错误信息: " << e.what() << endl;
                print_trace();
                handle_500(response, "内部服务器错误");
            } catch (...) {
                cerr << "未知错误" << endl;
                print_trace();
            }
        } else {
            handle_404(response, "请求的资源有误");
        }
    }

    try {
        // TODO response中间件
        response.write_to_fd(clifd);
    } catch(exception& e) {
        cerr << e.what() << endl;
        print_trace();
    } catch(...) {
        cerr << "未知错误" << endl;
        print_trace();
    }
    close(clifd);

    cout << str_toupper(request.get_header().get_action()) << "  " << response.get_response_code() << "  " << url << endl;

    /* 释放资源 */
    pthread_mutex_lock(&mutex);
    cli->fd = -1;
    s_srv_ctx->cli_cnt--;
    pthread_mutex_unlock(&mutex);
}

void WebServer::BaseHttpServer::recv_client_msg(fd_set *readfds) {
    int i = 0;
    int clifd;
    size_t size = 0;
    ssize_t n = 0;
    char buf[MAX_MSG_SIZE] = {0};
    for (i = 0; i < USERSIZE; i++) {
        clifd = s_srv_ctx->cli[i].fd;
        if (clifd < 0) {
            continue;
        }
        if (FD_ISSET(clifd, readfds)) {
            fcntl(clifd, F_SETFL, fcntl(clifd,F_GETFL,0) | O_NONBLOCK);
            while(true) {
                // TODO 请求头部和请求的payload读取优化
                // TODO 支持文件上传
                n=read(clifd, buf + size, MAX_MSG_SIZE - size);
                if(n <= 0)
                    break;
                else
                    size += n;
                usleep(100000); // 100 ms
            }
            if(size == 0 || size >= MAX_MSG_SIZE) {
                close(clifd);
                return;
            }
            /*
            if (n < 0) {
                FD_CLR(clifd, &s_srv_ctx->allfs);
                close(clifd);
                pthread_mutex_lock(&mutex);
                s_srv_ctx->cli[i].fd = -1;
                s_srv_ctx->cli_cnt--;
                pthread_mutex_unlock(&mutex);
                fprintf(stdout, "client exit, current clients: %d \n", s_srv_ctx->cli_cnt);
                return;
            }
            */
            handle_client_msg(&s_srv_ctx->cli[i], buf);
        }
    }
}

void WebServer::BaseHttpServer::idle() {
    int clifd;
    for (auto &i : s_srv_ctx->cli) {
        clifd = i.fd;
        if (clifd < 0) {
            continue;
        }
        close(clifd);
        FD_CLR(clifd, &s_srv_ctx->allfs);
        pthread_mutex_lock(&mutex);
        i.fd = -1;
        s_srv_ctx->cli_cnt--;
        pthread_mutex_unlock(&mutex);
        fprintf(stdout, "client exit, current clients: %d \n", s_srv_ctx->cli_cnt);
    }
}

WebServer::BaseHttpServer::BaseHttpServer() {
    server_init();
    RegisterErrorHandler(401);
    RegisterErrorHandler(403);
    RegisterErrorHandler(404);
    RegisterErrorHandler(500);
    RegisterErrorHandler(501);
    RegisterErrorHandler(502);
    RegisterErrorHandler(503);
    cout << "server init at " << ADDRESS << ":" << PORT << " pid: " << getpid() << endl;
}
