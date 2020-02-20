//
// Created by afroraydude on 2/20/20.
//
#include <stdio.h>
#include <string.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    printf("Welcome to USPM Extended Suite\n");
    chdir("/var/uspm/storage");

    int result = access("/var/uspm/storage/", W_OK);
    if (result != 0)
    {
        printf("Cannot write to the storage directory, exiting.\n");
        return 1;
    } else {

    }

    if (argc >= 3) {
        if (strcmp(argv[1], "") == 0) {
            printf("install %s\n", argv[2]);
            for (int i = 2; i < argc; i++) {

            }
        }

        else if (strcmp(argv[1], "u") == 0) {
            printf("uninstall %s\n", argv[2]);
            for (int i = 2; i < argc; i++) {

            }
        }

        else if (strcmp(argv[1], "c") == 0) {
            for (int i = 2; i < argc; i++) {

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

