//
// Created by afroraydude on 8/31/20.
//

#include <unistd.h>
#include <stdlib.h>
#include "fm.h"
#include "dephandle.h"
#include "uspm.h"

int install_package_file(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) != -1) {
        char *command;
        if (access(package, F_OK) == -1) {
            command = concat("tar -xf ", filename);
            system(command);
        }

        if (check_dependencies_and_install(package) != 0) {
            printf("Installation failed\n");
            return 1;
        }

        command = concat("sh ./", package);
        command = concat(command, "/PACKAGECODE install");

        system(command);
        command = concat("rm -rf ./", package);
        command = concat(command, "/files");
        system(command);

        free(command);
        remove(filename);

        return 0;
    } else {
        printf("Failed to extract package file");
        return 1;
    }
}

int install_package(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) == -1) {
        cJSON *config = load_file("config.json");
        if (download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, package) != 0) return 1;
    }

    if (install_package_file(package) == 0) {
        char *file = concat("./", package);
        file = concat(file, "/PACKAGEDATA");

        cJSON *packagedata = load_file(file);

        free(file);

        add_to_packages(package, packagedata);

        return 0;
    } else {
        return 1;
    }
}

int uninstall_package(char *package) {
    char *command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE uninstall");

    system(command);

    free(command);

    remove_from_packages(package);

    return 0;
}

int check_if_first_run() {
    check_config_file();
    check_packages_file();
    return 0;
}
