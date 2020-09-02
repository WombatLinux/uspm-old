/*
 * fm - manages the json files and stuff
 * few extras added
 */
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <openssl/md5.h>
#include <string.h>
#include "fm.h"

struct url_data {
    size_t size;
    char* data;
};

char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// utility function to compare each substring of version1 and
// version2
int compareSubstr(char *substr_version1, char *substr_version2,
                  int len_substr_version1, int len_substr_version2) {
    // if length of substring of version 1 is greater then
    // it means value of substr of version1 is also greater
    if (len_substr_version1 > len_substr_version2)
        return 1;

    else if (len_substr_version1 < len_substr_version2)
        return -1;

        // when length of the substrings of both versions is same.
    else {
        int i = 0, j = 0;

        // compare each character of both substrings and return
        // accordingly.
        while (i < len_substr_version1) {
            if (substr_version1[i] < substr_version2[j]) return -1;
            else if (substr_version1[i] > substr_version2[j]) return 1;
            i++, j++;
        }
        return 0;
    }
}

cJSON *load_file(char *file) {
    if (access(file, F_OK) != -1) {
        /* declare a file pointer */
        FILE *infile;
        char *buffer;
        long numbytes;

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
        buffer = (char *) calloc(numbytes, sizeof(char));

        /* memory error */
        if (buffer == NULL)
            return NULL;

        /* copy all the text into the buffer */
        fread(buffer, sizeof(char), numbytes, infile);
        fclose(infile);

        /* confirm we have read the file by
        outputing it to the console */
        //printf("The file contains this text\n\n%s", buffer);

        cJSON *root = load_json(buffer);

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

    ptr = fopen("packages.json", "w");

    fprintf(ptr, "%s", outfile);
    fclose(ptr);

    free(outfile);
}

void write_config_file(char *out) {
    char *outfile = concat(out, "\n");

    FILE *ptr;

    ptr = fopen("config.json", "w");

    fprintf(ptr, "%s", outfile);
    fclose(ptr);

    free(outfile);
}

int add_to_packages(char *packagename, cJSON *packagedata) {
    cJSON *root = load_file("packages.json");

    cJSON_AddItemToObject(root, packagename, packagedata);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

int remove_from_packages(char *packagename) {
    cJSON *root = load_file("packages.json");
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
        fp = fopen(outfilename, "wb");
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

void create_packages_file() {
    char *out;
    cJSON *root, *uspm;

    root = cJSON_CreateObject();
    uspm = cJSON_CreateObject();

    printf("Creating packages file with default values\n");

    /* add data to uspm package */
    cJSON_AddItemToObject(uspm, "version", cJSON_CreateString(("1.0.0")));
    cJSON_AddItemToObject(uspm, "dependencies", cJSON_CreateObject());

    /* add data to root object */
    cJSON_AddItemToObject(root, "uspm", uspm);

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_packages_file(out);

    free(out);
}

void create_config_file() {
    char *out;
    cJSON *root;
    printf("Creating config file with default values\n");
    root = cJSON_CreateObject();

    /* add data to uspm package */
    cJSON_AddItemToObject(root, "mirror", cJSON_CreateString(("http://packages.afroraydude.com/uspm/")));

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_config_file(out);

    free(out);
}

int check_packages_file() {
    if (access("packages.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        char *json = cJSON_Print(root);
        // printf("%s\n", json);
    } else {
        create_packages_file();
    }
    return 0;
}

int check_config_file() {
    if (access("config.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        char *json = cJSON_Print(root);
        // printf("%s\n", json);
    } else {
        create_config_file();
    }
    return 0;
}

unsigned char *get_checksum(char *filename) {
    unsigned char result[2*MD5_DIGEST_LENGTH];
    unsigned char hash[MD5_DIGEST_LENGTH];
    int i;
    FILE *f = fopen(filename,"rb");
    MD5_CTX mdContent;
    int bytes;
    unsigned char data[1024];

    if(f == NULL){
        printf("%s couldn't open file\n",filename);
        exit(1);
    }

    MD5_Init(&mdContent);
    while((bytes = fread(data, 1, 1024, f)) != 0){

        MD5_Update(&mdContent, data, bytes);
    }

    MD5_Final(hash,&mdContent);
#ifdef DEBUG
    for(i=0;i<MD5_DIGEST_LENGTH;i++){
        printf("%02x",hash[i]);
    }
    printf("\n");
#endif
/** if you want to see the plain text of the hash */
    for(i=0; i < MD5_DIGEST_LENGTH;i++){
        sprintf((char *)&(result[i*2]), "%02x",hash[i]);
    }

    unsigned char *out = result;
#ifdef DEBUG
    printf("%s\n", result);
    printf("%s\n", output);
#endif
    fclose(f);

    return out;
}

int compare_checksum(unsigned char *a, unsigned char *b, int size) {
    int i;

    unsigned char *x = (unsigned char *)"disajfisajdjfsij";

    unsigned char t1 = a;
    unsigned char t2 = b;

    printf((const char *) t1);
    printf((const char *) t2);
    printf("x %s\n", x);

    for(i=0;i<size;i++) {
        printf("a %u\n", a[i]);
        printf("b %u\n", b[i]);
        printf("x %u\n", x[i]);

        if(a[i]!=b[i])
            return 1;
    }

    return 0;
}

cJSON *load_json(char *json) {
    cJSON *out = cJSON_Parse(json);

    return out;
}

cJSON *get_repo_json(char* url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }



        curl_easy_cleanup(curl);

    }
    return data.data;
}