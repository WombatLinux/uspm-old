#include <stdio.h>
#include "parser.h"
#include "parser.c"
#include "iu.c"
#include <string.h>

int main(int argc, char *argv[]) {
    printf("Welcome to USPM\n");
    chdir(rootdir);

    if (access(rootdir, W_OK) != 0) 
    {
        printf("Insufficient permissions for one of the required directories (/var). Are you running as root?\n");
        return 1;
    } else {

    }

    check_packages_file();
    check_config_file();

    if (argc >= 3) {
        if (strcmp(argv[1], "i") == 0) {
            printf("install packages\n");
            for (int i = 2; i < argc; i++) {
                install_package(argv[i]);
            }
        }

        else if (strcmp(argv[1], "u") == 0) {
            printf("uninstall packages\n");
            for (int i = 2; i < argc; i++) {
                uninstall_package(argv[i]);
            }
        }

        else if (strcmp(argv[1], "c") == 0) {
            printf("check dependencies\n");
            for (int i = 2; i < argc; i++) {
                get_dependencies(argv[i]);
            }
        }

        else {
            printf("command not found");
        }
    } else {
        printf("No command found");
    }
    return 0;
}
