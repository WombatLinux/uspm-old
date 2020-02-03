/*
 * iu - Installer/Uninstaller
 */

#include "iu.h"
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int downloadPackage(char *mirror, char *package) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = mirror + package + '.uspm';
    char outfilename[FILENAME_MAX] = package + '.uspm';
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

int installPackage() {

    return 0
}