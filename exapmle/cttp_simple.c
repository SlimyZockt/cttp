#include "../src/cttp.h"
#include "../src/cttp.c"
#include <stdio.h>

#define S CTTP_S
#define P CTTP_PATH

CTTP_HTTP index(CTTP_Request request) {
    return (CTTP_HTTP){
        
    };
}

int main(int argc, char **argv) {

    printf("Hello \n");

    CTTP_Server server = {0};
    cttp_begin(&server, .port = 8080);

    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("index")), index);
    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("route")), index);
    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("route"), S("$dynamic")), index);

    cttp_end(&server);

    return 0;
}
