//
// Created by afroraydude on 2/20/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <cjson/cJSON.h>
#include "../uspm/parser.h"

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
        if (strcmp(argv[1], "c") == 0) {
            printf("clean %s\n", argv[2]);
            for (int i = 2; i < argc; i++) {
                system("rm *.uspm");
            }
        }

        else if (strcmp(argv[1], "u") == 0) {
            printf("upgrade %s\n", argv[2]);
            for (int i = 2; i < argc; i++) {
                cJSON *root = load_file("packages.json");

               cJSON *package = root->child;

               while (package) {
                   char *command = concat("uspm i ", package->string);

                   system(command);

                   package = package->next;
               }
            }
        }

        else if (strcmp(argv[1], "p") == 0) {
            for (int i = 2; i < argc; i++) {
                cJSON *root = load_file("packages.json");

                cJSON *package = root->child;

                while (package) {
                    char *command = concat("uspm u ", package->string);

                    system(command);

                    package = package->next;
                }
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

