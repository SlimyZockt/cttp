#include "./cttp.h"
#include "base/base_core.h"
#include <assert.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define CTTP_MAX_EVENTS 25

void cttp_begin_opt(CTTP_Server *server, CTTP_Server_Parameter *parameter){
    if (server->routes == NULL) {
        server->routes = array_init(CTTP_Route, &server->arena);
    }

    if (parameter == NULL) return;

    if (parameter->port != 0) {
        server->port = parameter->port;
    }
}

CTTP_HTTP cttp_to_http(CTTP_Request request) {
    return (CTTP_HTTP){
    };
}

void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_Path path,
                 CTTP_Handle handle) {
    cttp_ensure(path.length > 0, "Path is empty");

    array_push(server->routes, &((CTTP_Route){
                                handle,
                                method,
                                path,
                               }));
}

void cttp_end(CTTP_Server *server) {
    int socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    cttp_ensure(socket_handle >= 0, "Error while creating a socket");

    int opt = 1;
    setsockopt(socket_handle, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server->port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int err = bind(socket_handle, (const struct sockaddr *)(&addr), sizeof(addr));
    cttp_ensure(err == 0, "Bind failed");

    err = listen(socket_handle, SOMAXCONN);
    cttp_ensure(err == 0, "Listen failed");

    int epoll_handle = epoll_create1(0);
    cttp_ensure(epoll_handle > 0, "epoll creation error");

    while (1) {
        for (u64 i = 0; i < server->routes->length; i++) {
            CTTP_Route *route = &server->routes->data[i];
        }
    }
}
