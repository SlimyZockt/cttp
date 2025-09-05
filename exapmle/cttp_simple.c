#include "../src/cttp.h"
#include "../src/cttp.c"
#include <stdio.h>

b8 index(CTTP_Respose respose, CTTP_Request request) {

    return 1;
}

int main(int argc, char **argv) {

    printf("Hello");

    CTTP_Server server = cttp_start();

    cttp_handle(&server, CTTP_MethodFlag_GET, cttp_path("index","xx"), index);

    cttp_end(&server);

    return 0;
}
