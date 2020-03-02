//
// Created by afroraydude on 2/2/20.
//

#ifndef USPM_PARSER_H
#define USPM_PARSER_H

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>

#define rootdir "/var/uspm/storage"
#define false 1
#define true 0
#define pkgfile "packages.json"



char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


int notZero(char *p) {
    while (*p != '\0') {
        if (*p != '0' && *p != '.') return 1;
        ++p;
    }
    return 0;
}

// utility function to compare each substring of version1 and
// version2
int compareSubstr(char *substr_version1, char *substr_version2,
                  int len_substr_version1, int len_substr_version2)
{
    // if length of substring of version 1 is greater then
    // it means value of substr of version1 is also greater
    if (len_substr_version1 > len_substr_version2)
        return 1;

    else if (len_substr_version1 < len_substr_version2)
        return -1;

        // when length of the substrings of both versions is same.
    else
    {
        int i = 0, j = 0;

        // compare each character of both substrings and return
        // accordingly.
        while (i < len_substr_version1)
        {
            if (substr_version1[i] < substr_version2[j]) return -1;
            else if (substr_version1[i] > substr_version2[j]) return 1;
            i++, j++;
        }
        return 0;
    }
}

// function to compare two versions.
// i found this throughout a few stackoverflow things
// could have just used the method I did on uspm.py
int check_version(char* version1, char* version2)
{
    int len_version1 = strlen(version1);
    int len_version2 = strlen(version2);

    char *substr_version1 = (char *) malloc(sizeof(char) * 1000);
    char *substr_version2 = (char *) malloc(sizeof(char) * 1000);

    // loop until both strings are exhausted.
    // and extract the substrings from version1 and version2
    int i = 0, j = 0;
    while (i < len_version1 || j < len_version2)
    {
        int p = 0, q = 0;

        // skip the leading zeros in version1 string.
        while (version1[i] == '0' )
            i++;

        // skip the leading zeros in version2 string.
        while (version2[j] == '0' )
            j++;

        // extract the substring from version1.
        while (version1[i] != '.' && i < len_version1)
            substr_version1[p++] = version1[i++];

        //extract the substring from version2.
        while (version2[j] != '.' && j < len_version2)
            substr_version2[q++] = version2[j++];

        int res = compareSubstr(substr_version1,
                                substr_version2, p, q);

        // if res is either -1 or +1 then simply return.
        if (res)
            return res;
        i++;
        j++;
    }

    // here both versions are exhausted it implicitly
    // means that both strings are equal.
    return 0;
}

cJSON *load_file(char *file) {
    if (access(file,F_OK) != -1) {
        /* declare a file pointer */
        FILE    *infile;
        char    *buffer;
        long    numbytes;

        /* open an existing file for reading */
        infile = fopen(file, "r");

        /* Get the number of bytes */
        fseek(infile, 0L, SEEK_END);
        numbytes = ftell(infile);

        /* reset the file position indicator to
        the beginning of the file */
        fseek(infile, 0L, SEEK_SET);

        /* grab sufficient memory for the
        buffer to hold the text */
        buffer = (char*)calloc(numbytes, sizeof(char));

        /* memory error */
        if(buffer == NULL)
            return NULL;

        /* copy all the text into the buffer */
        fread(buffer, sizeof(char), numbytes, infile);
        fclose(infile);

        /* confirm we have read the file by
        outputing it to the console */
        //printf("The file contains this text\n\n%s", buffer);

        cJSON *root = cJSON_Parse(buffer);

        /* free the memory we used for the buffer */
        free(buffer);

        return root;
    } else {
        printf("No file\n");
        return NULL;
    }
}

void write_packages_file(char *out) {
    char *outfile = concat(out, "\n");

    FILE *ptr;

    ptr = fopen(pkgfile,"w");

    fprintf(ptr,"%s",outfile);
    fclose(ptr);

    free(outfile);
}

void write_config_file(char *out) {
    char *outfile = concat(out, "\n");

    FILE *ptr;

    ptr = fopen("config.json","w");

    fprintf(ptr,"%s",outfile);
    fclose(ptr);

    free(outfile);
}

int add_to_packages(char *packagename, cJSON *packagedata) {
    cJSON *root = load_file(pkgfile);

    cJSON_AddItemToObject(root, packagename, packagedata);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

int remove_from_packages(char *packagename) {
    cJSON *root = load_file(pkgfile);
    cJSON_DeleteItemFromObject(root, packagename);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

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

#endif //USPM_PARSER_H
