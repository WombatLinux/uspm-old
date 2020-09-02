#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <libuspp/uspm.h>


int main(int argc, char *argv[]) {
    printf("Welcome to USPM\nuspm [command] [package]\n\nCommands\n\ti - install package\n\tu - uninstall packag\n\n");

    int result;

    result = access("/var/uspm", F_OK);

    if (result != 0) {
        mkdir("/var/uspm", 0744);
        printf("Main USPM directory noneexistant");
    }

    result = access("/var/uspm/storage", F_OK);

    if (result != 0) {
        mkdir("/var/uspm/storage", 0744);
        printf("USPM storage directory noneexistant");
    }

    chdir("/var/uspm/storage");

    /* check for access to typical file locations */

    result = access("/var/uspm/storage", W_OK);
    if (result != 0) {
        printf("Insufficient permissions for one of the required directory /var/uspm/storage. \nAre you running as root?\n");
        return 1;
    }

    result = access("/etc", W_OK);
    if (result != 0) {
        printf("Insufficient permissions for one of the required directory /etc. \nAre you running as root?\n");
        return 1;
    }

#ifdef __APPLE__
    result = access("/usr/local", W_OK);
    if (result != 0) {
        printf("Insufficient permissions for one of the required directory /usr/local. \nAre you running as root?\n");
        return 1;
    }
#elif __linux__
    result = access("/usr", W_OK);
    if (result != 0) {
        printf("Insufficient permissions for one of the required directory /usr. \nAre you running as root?\n");
        return 1;
    }
#endif
    check_if_first_run();

    if (argc >= 3) {
        if (strcmp(argv[1], "i") == 0) {
            printf("install packages\n");
            for (int i = 2; i < argc; i++) {
                install_package(argv[i]);
            }
        } else if (strcmp(argv[1], "u") == 0) {
            printf("uninstall packages\n");
            for (int i = 2; i < argc; i++) {
                uninstall_package(argv[i]);
            }
        } else {
            printf("Command not found\n");
        }
    }
    return 0;
}
