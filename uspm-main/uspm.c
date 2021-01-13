/*
 *   ___  ___  ________  ________  _____ ______
 *  |\  \|\  \|\   ____\|\   __  \|\   _ \  _   \
 *  \ \  \\\  \ \  \___|\ \  \|\  \ \  \\\__\ \  \
 *   \ \  \\\  \ \_____  \ \   ____\ \  \\|__| \  \
 *    \ \  \\\  \|____|\  \ \  \___|\ \  \    \ \  \
 *     \ \_______\____\_\  \ \__\    \ \__\    \ \__\
 *      \|_______|\_________\|__|     \|__|     \|__|
 *                \|_________|
 *
 *                   (c) 2020 afroraydude
 *
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <libuspp/uspp.h>

int main(int argc, char *argv[]) {
    printf("Welcome to USPM\nuspm command package [packages...]\n\nCommands\n\ti - install package(s)\n\tu - uninstall package(s)\n\n");

    int result;

    /* create any required folders early */
    result = access("/var/uspm", F_OK);

    if (result != 0) {
        printf("Main USPM directory noneexistant...");
        mkdir("/var/uspm", 0744);
        printf("created\n");
    }

    result = access("/var/uspm/storage", F_OK);

    if (result != 0) {
        printf("USPM storage directory noneexistant...");
        mkdir("/var/uspm/storage", 0744);
        printf("created\n");
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

    /*
     * apple and linux go to different places cause of
     * permission issues regarding apple's macos system
     * this checks for apple during compilation to fix that
     */
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
    /*
     * we don't care about the output,
     * we just care about if you have
     * all the required files already.
     */
    check_if_first_run();

    /* the actual program */
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

