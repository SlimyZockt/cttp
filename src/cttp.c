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
    };
}

typedef struct CTTP_HTTP_Response {
    u8 *data;
    u64 len;
} CTTP_HTTP_Response;

CTTP_HTTP_Response cttp_to_http(CTTP_Request request) {
    return (CTTP_HTTP_Response){

    };
}

void cttp_handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_Path path, CTTP_Handle handle) {
    //TODO: add auto count on use
    
    DefineArray(CTTP_Route);
    CTTP_Route_Array *array = array_init(CTTP_Route, &server->arena);
    // server->arena

}

void cttp__handle(CTTP_Server *server, CTTP_MethodFlag method, CTTP_Path path, CTTP_Handle handle) {
    CTTP_Respose response;
    CTTP_Request request;

    handle(response, request);

    CTTP_HTTP_Response http = cttp_to_http(request);

    send(server->socket, http.data, http.len, 0);
}

void cttp_end(CTTP_Server *server) {

}



