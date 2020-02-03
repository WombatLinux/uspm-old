/*
 * iu - Installer/Uninstaller
 */
#include <curl/curl.h>
#include <string.h>
#include "parser.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int check_dependencies_and_install(char *package);

int download_package(char *mirror, char *package) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = concat(mirror, package);
    url = concat(url, ".uspm");
    printf("%s\n", url);

    char outfilename[FILENAME_MAX] = "";
    strcpy(outfilename, concat(package, ".uspm"));
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    free(url);
    return 0;
}

int install_package_file(char *package) {
    char *command = concat("tar -xf ", package);
    command = concat(command, ".uspm");

    system(command);

    check_dependencies_and_install(package);

    command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE install");

    system(command);

    free(command);

    return 0;
}

int install_package(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) == -1) {
        download_package("http://packages.afroraydude.com/", package);
    }

    install_package_file(package);

    char *file = concat("./", package);
    file = concat(file, "/PACKAGEDATA");

    cJSON *packagedata = load_file(file);

    free(file);

    add_to_packages(package, packagedata);

    return 0;
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


int check_dependencies_and_install(char *package) {
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

            char *version = cJSON_GetObjectItem(dependency_internal, "version")->valuestring;
            char *minversion = dependency->valuestring;

            if (check_version(version, minversion) <= 0) {
                printf("%s (missing)...installing first\n", dependency->string);
                install_package(dependency->string);
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