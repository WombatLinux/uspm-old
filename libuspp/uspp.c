/*The main package handling file.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include "fm.h"
#include "dephandle.h"
#include "uspp.h"
#include "config.h"

/**
 * Given a package name [package], installs the package file (and, if necessary,
 * downloads) located in /var/uspm/storage
 *
 * @param package the package
 */
int install_package_file(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) != -1) {
        char *command;
        if (access(package, F_OK) == -1) {
            command = concat("tar -xf ", filename);
            system(command);
        }

        if (check_for_dependencies(package) != 0) {
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

/**
 * Given a package name [package], installs the package file (and, if necessary,
 * downloads) located in /var/uspm/storage
 *
 * @param package the package
 */
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

/**
 * Given a package name [package], uninstalled the package
 *
 * @param package the package
 */
int uninstall_package(char *package) {
    char *command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE uninstall");

    system(command);

    free(command);

    remove_from_packages(package);

    return 0;
}

/**
 * Checks if this is the first run
 *
 * If true, creates the necessary files for running the USPM software
 *
 * @return int 0 if first run 1 if not first run
 */
int check_if_first_run() {
    int response = 0;
    if (check_config_file() != 0) response = 1;
    if (check_packages_file() != 0) response = 1;
    return response;
}
