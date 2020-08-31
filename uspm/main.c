#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "fm.h"
#include "inunin.h"


int main(int argc, char *argv[]) {
    printf("Welcome to USPM\nuspm [command] [package]\n\nCommands\n\ti - install package\n\tu - uninstall packag\n\n");
    chdir("/var/uspm/storage");

    if (access("/var/uspm/storage/", W_OK) != 0 || access("/etc", W_OK) != 0 || access("/usr", W_OK) != 0) 
    {
        printf("Insufficient permissions for one of the required directories (/var/uspm/storage /usr or /etc). \nAre you running as root?\n");
        return 1;
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

        else {
            printf("command not found");
        }
    } else {
        printf("No command found");
    }
    return 0;
}
