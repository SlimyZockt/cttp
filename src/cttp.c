#include "./cttp.h"
#include "base/base_assert.h"
#include "base/base_inc.h"
#include "base/base_string.h"
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#define CTTP_MAX_EVENTS 25

#define S CTTP_S

void cttp_begin_opt(CTTP_Server *server, CTTP_Server_Parameter *parameter) {
    if (server->routes == NULL) {
        server->routes = array_init(CTTP_Route, &server->arena);
    }

    if (parameter == NULL) return;

    if (parameter->port != 0) {
        server->port = parameter->port;
    }
}

void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_String_Array path, CTTP_Handle handle) {
void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_String_Array path, CTTP_Handle handle) {
    cttp_ensure(path.length > 0, "Path is empty");

    array_push(server->routes, &((CTTP_Route){
                                   handle,
                                   method,
                                   path,
                               }));
}


typedef struct _HTTP_NODE {
   struct _HTTP_Node *left; 
   struct _HTTP_Node *right; 
   CTTP_String token;
} _HTTP_NODE;



b8 _cttp_parse_path(CTTP_Request *request_out, CTTP_String *token, Arena *arena) {
    request_out->path = array_init(CTTP_String, arena);
    CTTP_String_Builder sb = {0};

    if (token->len == 1) {
        arena_sb_append_cstr(arena, &sb, "index.html");
        array_push(request_out->path, (&(CTTP_String){sb.items, sb.count}));
        return 1;
    } 

    u16 token_len = 0;

    for (u64 i = 1; i < token->len; i++) {
        if (token->str[i] == '/') {
            arena_sb_append_null(arena, &sb);

            cttp_warn("%s", sb.items);
            array_push(request_out->path, (&(CTTP_String){sb.items, sb.count}));

            token_len = 0;
            sb.count = 0;
            sb.capacity = 0;
            sb.items = 0;
            continue;
        }

        arena_da_append(arena, &sb, token->str[i]);
        token_len += 1;
    }

    if (token_len > 0) {
        arena_sb_append_null(arena, &sb);

        cttp_warn("%s", sb.items);
        array_push(request_out->path, (&(CTTP_String){sb.items, sb.count}));

        token_len = 0;
        sb.count = 0;
        sb.capacity = 0;
        sb.items = 0;
    }

    return 1;
}

int _cttp_parse_http(CTTP_Request *request_out, CTTP_String *request, Arena *arena) {
    u64 last_token_pos = 0;
    u16 token_len = 0;
    u16 token_paresed = 0;
    // CTTP_String_Builder sb = {0};

    for (u64 i = 0; i < request->len; i++) {
        if (!cttp_char_is_space(request->str[i])) {
            token_len += 1;
            continue;
        }

        CTTP_String token = {
           &request->str[i-token_len],
           token_len
        };

        if (cttp_str_cmp(token, S("GET"))) {
            request_out->method = CTTP_MethodFlag_GET;
        } else if (cttp_str_cmp(token, S("POST"))) {
            request_out->method = CTTP_MethodFlag_POST;
        }

        if (token_paresed == 1) {
            _cttp_parse_path(request_out, &token, arena);
        }

        token_len = 0;
        token_paresed += 1;
    }

    return 0;
}

void cttp_end(CTTP_Server *server) {
    int socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    cttp_ensure(socket_handle >= 0, "Error while creating a socket");
    server->socket = socket_handle;

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

    Arena request_arena = {0};

    while (1) {
        int n = epoll_wait(epoll_handle, events, CTTP_MAX_EVENTS, -1);

        if (n < 0) {
            if (errno == EINTR)
                continue;
            cttp_error("Epoll Error", NULL);
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if (fd == server->socket) {
                // New client
                int client_fd = accept(server->socket, NULL, NULL);
                if (client_fd < 0) {
                    cttp_error("accept failed");
                    continue;
                }

                // Register client fd in epoll
                struct epoll_event client_ev;
                client_ev.events = EPOLLIN;
                client_ev.data.fd = client_fd;
                epoll_ctl(epoll_handle, EPOLL_CTL_ADD, client_fd, &client_ev);
                continue;
            }
            // Handle client data
            char buffer[4096];

            int bytes = read(fd, buffer, sizeof(buffer) - 1);
            if (bytes <= 0) {
                // client closed connection
                close(fd);
                epoll_ctl(epoll_handle, EPOLL_CTL_DEL, fd, NULL);
                continue;
            }

            buffer[bytes] = '\0';
            cttp_debug("%s", buffer);

            CTTP_Request request = {0};
            _cttp_parse_http(&request, &(CTTP_String){buffer, 4096}, &request_arena);

            const char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 14\r\n"
                "\r\n"
                "Hello, epoll!\n";

            for (u64 i = 0; i < server->routes->length; i++) {
                CTTP_Route *route = &server->routes->data[i];
                if (route->path.length != request.path->length) continue;

                u8 found = 1;

                for (size_t j = 0; j < request.path->length; j++) {
                    found = found && cttp_str_cmp(request.path->data[j], route->path.data[j]);
                }

                if (found) {
                response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 14\r\n"
                    "\r\n"
                    "Hello, index !\n"; 

                break;
                }
            }

            write(fd, response, strlen(response));

            // Close for simplicity (HTTP/1.0 style)
            close(fd);
            epoll_ctl(epoll_handle, EPOLL_CTL_DEL, fd, NULL);
        }

        arena_reset(&request_arena);
    }
    close(server->socket);
    close(epoll_handle);
}

