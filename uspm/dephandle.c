/*
* dephandle.c - Dependency Handler
*/
#include <string.h>
#include <cjson/cJSON.h>
#include <libtar.h>
#include <fcntl.h>
#include "parser.h"

int install_dependency(char *, char *minversion);

int check_dependencies(char *package) {
    printf("Checking dependencies...\n");

    char *file = concat("./", package);
    file = concat(file, "/PACKAGEDATA");

    cJSON *packagedata = load_file(file);

    cJSON *root = load_file(pkgfile);

    cJSON *dependencies = cJSON_GetObjectItem(packagedata, "dependencies");

    cJSON *dependency = dependencies->child;

    while (dependency) {

        cJSON *dependency_internal = cJSON_GetObjectItem(root, dependency->string);
        char *minversion = dependency->valuestring;

        if (dependency_internal != NULL) {

            char *version = cJSON_GetObjectItem(dependency_internal, "version")->valuestring;

            if (check_version(version, minversion) < 0) {
                printf("%s (missing)...installing first\n", dependency->string);
                if (install_dependency(dependency->string, minversion) != 0) {
                    return false ;
                }
            } else {
                printf("%s\n", dependency->string);
            }
        } else {
            printf("Dependency not installed\n");
            if (install_dependency(dependency->string, minversion) != 0) {
                return false;
            }
        }
        // do what we need to do
        dependency = dependency->next;
    }

    printf("No more dependencies found\n");
    return true;
}

int install_dep_file(char *package, char *minversion) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) != -1) {
        printf("File exists\n");
        TAR *tar;
        //char *command = concat("tar -xf ", filename);

        tar_open(&tar, filename, 0, O_RDONLY, 0, 0);

        tar_extract_all(tar, rootdir);
        //system(command);
        if (access(concat(package, "/PACKAGEDATA"),F_OK) == -1) {
            printf("FILE EXTRACT FAILED\n");
            return false;
        }

        filename = concat(package, "/PACKAGEDATA");

        cJSON *root = load_file(filename);

        char *test = cJSON_Print(root);

        printf("%s\n", test);

        free(tar);

        remove(filename);
        char *version = cJSON_GetObjectItem(root, "version")->valuestring;

        if (check_version(version, minversion) < 0) {
            printf("No good version of dependency found. Aborting.\n");
            return false;
        }

        check_dependencies(package);

        char *command = concat("sh ./", package);
        command = concat(command, "/PACKAGECODE install");

        system(command);

        free(command);

        return true;
    } else {
        printf("Failed to extract package file");
        return false;
    }
}

int install_dependency(char *package, char *minversion) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) == -1) {
        cJSON *config = load_file("config.json");
        download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, package);
    }


    if (install_dep_file(package, minversion) == 0) {
        char *file = concat("./", package);
        file = concat(file, "/PACKAGEDATA");

        cJSON *packagedata = load_file(file);

        free(file);

        add_to_packages(package, packagedata);

        return true;
    } else {
        return false;
    }
}