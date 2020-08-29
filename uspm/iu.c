/*
 * iu - Installer/Uninstaller
 */
#include <curl/curl.h>
#include "fm.h"
#include "dephandle.c"

int install_package_file(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) != -1) {
        printf("File exists\n");
        char *command = concat("tar -xf ", filename);

        system(command);

        if (check_dependencies_and_install(package) != 0) {
            printf("Installation failed\n");
            return 1;
        }

        command = concat("sh ./", package);
        command = concat(command, "/PACKAGECODE install");

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
        download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, package);
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


int check_dependencies(char *package) {
    printf("Checking dependencies...\n");

    char *file = concat("./", package);
    file = concat(file, "/PACKAGEDATA");

    cJSON *packagedata = load_file(file);

    cJSON *root = load_file("packages.json");

    cJSON *dependencies = cJSON_GetObjectItem(packagedata, "dependencies");

    cJSON *dependency = dependencies->child;

    while (dependency) {

        cJSON *dependency_internal = cJSON_GetObjectItem(root, dependency->string);

        if (dependency_internal != NULL) {
            char *version = dependency_internal->valuestring;
            char *minversion = dependency->valuestring;

            if (check_version(version, minversion) >= 0) {
                printf("%s (missing)\n", dependency->string);
            } else {
                printf("%s\n", dependency->string);
            }
        } else {
            printf("Dependency not installed\n");
        }
        // do what we need to do
        dependency = dependency->next;
    }

    printf("No more dependencies found\n");

    return 0;
}