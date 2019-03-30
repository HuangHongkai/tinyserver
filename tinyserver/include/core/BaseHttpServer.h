//
// Created by hhk on 18-12-13.
//

#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <bits/stdc++.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "StringUtils.h"
#include "IHttpServer.h"

#define LISTING 5
#define ADDRESS "0.0.0.0"
#define PORT 8888
#define MAX_MSG_SIZE 1024
#define USERSIZE 10

using namespace std;

namespace WebServer {
    using std::pair;
    using std::vector;
    /**
     * 基于 select 的单线程同步非阻塞io服务器
     */
    class BaseHttpServer : public IHttpServer {

    private:
        typedef struct {
            in_addr addr; // 客户端ip地址
            in_port_t port; // 客户端端口
            int fd; // 客户端fd
        }client_info_t;

        typedef struct {
            int cli_cnt; // 当前客户端数目
            client_info_t cli[USERSIZE];
            fd_set allfs; // 当前fd集合
            int maxfd; // 当前最大的fd
        } server_context_st;

        server_context_st *s_srv_ctx{};
        int server_fd{};
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        vector<pair<regex, VIEW_HANDLER>> router_table;
        ERROR_HANDLER handle_401, handle_403, handle_404, handle_500, handle_501, handle_502, handle_503;

        void server_init();

    public:
        BaseHttpServer();

        bool enrol(const string& url_rule, VIEW_HANDLER func) override;

        void run() override;

    private:
        /**
         * 匹配url
         * @param url
         * @return 返回一个pair,第一个为函数地址,第二个为匹配的结果
         */
        pair<VIEW_HANDLER, vector<string>> match(const string& url);

        /**
         * 接受客户端连接
         * @param srvfd 客户端的fd
         */
        void accept_client(int srvfd);

        /**
         * 响应客户端请求
         * @param cli
         * @param msg
         */
        void handle_client_msg(client_info_t* cli, char *msg);

        /**
         * 接受客户端消息
         * @param readfds 客户端集合
         */
        void recv_client_msg(fd_set *readfds);

        /**
         * 空闲时间做的事情
         */
        void idle();
    };

}
#endif //HTTPSERVER_H
