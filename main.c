#include <stdio.h>
#include "parser.h"
#include "parser.c"
#include "iu.c"

int main(int argc, char *argv[]) {
    printf("Welcome to USPM\n");
    chdir("/var/uspm/storage");

    check_packages_file();

    if (argc == 3) {
        if (strcmp(argv[1], "i") == 0) {
            printf("install %s\n", argv[2]);
            install_package(argv[2]);
        }

        else if (strcmp(argv[1], "u") == 0) {
            printf("uninstall %s\n", argv[2]);
            uninstall_package(argv[2]);
        }

        else {
            printf("command not found");
        }
    } else {
        printf("No command found");
    }
    return 0;
}
