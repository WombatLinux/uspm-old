/*
 *   ___  ___  ________  ________  _____ ______
 *  |\  \|\  \|\   ____\|\   __  \|\   _ \  _   \
 *  \ \  \\\  \ \  \___|\ \  \|\  \ \  \\\__\ \  \
 *   \ \  \\\  \ \_____  \ \   ____\ \  \\|__| \  \
 *    \ \  \\\  \|____|\  \ \  \___|\ \  \    \ \  \
 *     \ \_______\____\_\  \ \__\    \ \__\    \ \__\
 *      \|_______|\_________\|__|     \|__|     \|__|
 *                \|_________|
 *                Extended Suite of Commands
 *              Can be a replacement for USPM
 *
 *                   (c) 2020 afroraydude
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <cjson/cJSON.h>
#include <uspm/uspp.h>
#include <uspm/fm.h>

int main(int argc, char * argv[]) {
  printf("Welcome to USPM Extended Suite\n");
  chdir("/var/uspm/storage");

  int result = access("/var/uspm/storage/", W_OK);
  if (result != 0) {
    printf("Cannot write to the storage directory, exiting.\n");
    return 1;
  }

  if (argc >= 2) {
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
      }
    } else {

      if (strcmp(argv[1], "clean") == 0) {
        for (int i = 2; i < argc; i++) {
          system("rm *.uspm");
        }
      } else if (strcmp(argv[1], "upgrade") == 0) {
        cJSON * root = load_file("packages.json");

        cJSON * package = root -> child;

        while (package) {
          install_package(package -> string);

          package = package -> next;
        }
      } else if (strcmp(argv[1], "purge") == 0) {
        cJSON * root = load_file("packages.json");

        cJSON * package = root -> child;

        while (package) {
          /*
          char *command = concat("uspm u ", package->string);

          system(command);
          */

          uninstall_package(package -> string);

          package = package -> next;
        }
      } else if (strcmp(argv[1], "l") == 0) {
        printf("list packages\n");
        cJSON * root = load_file("packages.json");

        cJSON * package = root -> child;

        while (package) {
          printf(package -> string);
          printf("\n");

          package = package -> next;
        }
      } else {
        printf("command not found");
      }
    }
  } else {
    printf("No command found");
  }
  return 0;
}
