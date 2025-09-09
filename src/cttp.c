#include "./cttp.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>

CTTP_Server cttp_begin_opt(CTTP_ServerParam *param) {
    int socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    if ( socket_handle == -1) {
        assert(0 && "Error while creating a socket");
    }

    struct sockaddr_in addr = {
           AF_INET,
           BSWAP32(param->port),       /* Port number */
           0       /* IPv4 address */
    };

    bind(socket_handle, (const struct sockaddr*)(&addr), sizeof(addr));
    return (CTTP_Server){
        .socket = socket_handle,
        .routes = NULL,
        .arena = {0},
    };

}

CTTP_HTTP cttp_to_http(CTTP_Request request) {

    return (CTTP_HTTP){

    };
}

void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_Path path, CTTP_Handle handle) {
    // assert(method != NULL && "method is NULL");
    assert(path.length > 0 && "Path is empty");
    if ( server->routes == NULL ) {
        server->routes = array_init(CTTP_Route, &server->arena);
    }

    array_push(server->routes, &((CTTP_Route){
        .handle = handle,
        .method = method,
        .path = path,
    }));
}

internal
void _cttp_open_route(CTTP_Server *server, CTTP_Route *route) {
    CTTP_Respose response;
    CTTP_Request request;

    CTTP_HTTP http = route->handle(request);

    http = cttp_to_http(request);

    // send(server->socket, http.data, http.len, 0);
}

void cttp_end(CTTP_Server *server) {
    for (u64 i = 0; i < server->routes->length; i++){
        CTTP_Route *route = &server->routes->data[i];
        _cttp_open_route(server, route);
    }

}



