#include "./cttp.h"
#include "base/base_core.h"
#include <assert.h>
#include <errno.h>
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

    struct epoll_event epoll_event;
    epoll_event.events = EPOLLIN;
    epoll_event.data.fd = server->socket;
    epoll_ctl(epoll_handle, EPOLL_CTL_ADD, server->socket, &epoll_event);

    printf("Start server at http://localhost:%lu\n", server->port);
    struct epoll_event events[CTTP_MAX_EVENTS];

    while (1) {
        int n = epoll_wait(epoll_handle, events, CTTP_MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            cttp_error("Epoll Error");
            break;
        }
     for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            if (fd == server->socket) {
                // New client
                int client_fd = accept(server->socket, NULL, NULL);
                if (client_fd < 0) { perror("accept"); continue; }

                // Register client fd in epoll
                struct epoll_event client_ev;
                client_ev.events = EPOLLIN;
                client_ev.data.fd = client_fd;
                epoll_ctl(epoll_handle, EPOLL_CTL_ADD, client_fd, &client_ev);

            } else {
                // Handle client data
                char buffer[4096];
                int bytes = read(fd, buffer, sizeof(buffer) - 1);

                if (bytes <= 0) {
                    // client closed connection
                    close(fd);
                    epoll_ctl(epoll_handle, EPOLL_CTL_DEL, fd, NULL);
                } else {
                    buffer[bytes] = '\0';
                    printf("Request from fd=%d:\n%s\n", fd, buffer);

                    // for (u64 i = 0; i < server->routes->length; i++) {
                    //     CTTP_Route *route = &server->routes->data[i];
                    // }
                    //
                    const char *response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 15\r\n"
                        "\r\n"
                        "Hello, epoll!\n";

                    write(fd, response, strlen(response));

                    // Close for simplicity (HTTP/1.0 style)
                    close(fd);
                    epoll_ctl(epoll_handle, EPOLL_CTL_DEL, fd, NULL);
                }
            }
        }
    }
}
