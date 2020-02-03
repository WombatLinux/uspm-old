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

int download_package(char *mirror, char *package) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = concat(mirror, package);
    url = concat(url, ".uspm");
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

    command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE install");

    system(command);

    free(command);

    return 0;
}

int install_package(char *package) {
    if (access("packages.json",F_OK) == -1) {
        download_package("http://packages.afroraydude.com/uspm/", package);
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