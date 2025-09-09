#include "../src/cttp.h"
#include "../src/cttp.c"
#include <stdio.h>

#define S cttp_S
#define P cttp_path

CTTP_HTTP index(CTTP_Request request) {
    return (CTTP_HTTP){
        
    };
}

int main(int argc, char **argv) {

    printf("Hello \n");

    CTTP_Server server = cttp_begin();

    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("index")), index);
    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("route")), index);
    cttp_handle(&server, CTTP_MethodFlag_GET, P(S("route"), S("$dynamic")), index);

    // CTTP_String str = S("index");

    // cttp_handle(&server, CTTP_MethodFlag_GET, S("index"), index);

    cttp_end(&server);

    return 0;
}
