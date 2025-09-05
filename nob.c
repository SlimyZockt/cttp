#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"
#define EXAMPLE_FOLDER "exapmle/"
#define LIB_FOLDER "lib/"

int main(int argc, char **argv) {

    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) {
        return 1;
    }


    nob_cc(&cmd);
    nob_cmd_append(&cmd, "-std=c11");
    // nob_cc_flags(&cmd);
    nob_cc_inputs(&cmd, EXAMPLE_FOLDER "cttp_simple.c");
    nob_cc_output(&cmd, BUILD_FOLDER "cttp");

    if (!nob_cmd_run(&cmd)) {
        return 1;
    }

    nob_cmd_append(&cmd, BUILD_FOLDER "cttp");

    if (!nob_cmd_run(&cmd)) {
        return 1;
    }

    return 0;
}
