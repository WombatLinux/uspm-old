/*
 * iu - Installer/Uninstaller
 */

#include "iu.h"
#include <curl/curl.h>
#include <string.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int downloadPackage(char *mirror, char *package) {
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
    return 0;
}

int installPackage(char *package) {
    char *command = concat("tar -xf ", *package);
    command = concat(command, '.uspm');

    system(command);

    command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE install");

    system(command);

    return 0;
}