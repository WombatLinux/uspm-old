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
#include "fm.h"
#include "config.h"

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t blank(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

/**
 * concatenate strings
 *
 * @param s1 first string
 * @param s2 second string
 */
char *concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/**
 * Given a filename/file location [file], attempts to parse as JSON
 * and then loads it in as a cJSON object.
 *
 * @param file file to load
 */
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
        printf("No file %s\n", file);
        return NULL;
    }
}

/**
 * Write data to a file
 * @param filename the file to write to
 * @param text the text to write to the file
 */
void write_file(char *filename, char *text) {
    FILE *ptr;

    ptr = fopen(filename, "w");

    fprintf(ptr, "%s", text);
    fclose(ptr);
}

/**
 * Writes a fresh new packages file
 *
 * @param out the char array to output to the file
 */
void write_packages_file(char *out) {
    char *outfile = concat(out, "\n");

    write_file("packages.json", outfile);
}

/**
 * Writes a fresh new config file
 *
 * @param out the char array to output to the file
 */
void write_config_file(char *out) {
    char *outfile = concat(out, "\n");

    write_file("config.json", outfile);
}

/**
 * Adds a package [packagename] to the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 * @param packagename the Package
 * @param packagedata the internal data
 */
int add_to_packages(char *packagename, cJSON *packagedata) {
    chdir("/var/uspm/storage/");

    cJSON *root = load_file("packages.json");


    cJSON_AddItemToObject(root, packagename, packagedata);

    char *out = cJSON_Print(root);

    write_packages_file(out);

    free(out);

    return 0;
}

/**
 * Removes a package [packagename] from the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 *
 * @param packagename package to remove
 */
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

int check_if_package_exists(char *mirror, char *folder, char *package) {
    char *url = concat(mirror, folder);

    cJSON *json = get_repo_json(url);

    cJSON *packagejson = cJSON_GetObjectItem(json, package);

#ifdef _REPO_LOOKUP_
    if (packagejson == NULL) {
        return 1;
    }
#endif
    return 0;
}

/**
 * Downloads a package [package] file from a mirror [mirror]
 *
 *
 * @param mirror the web mirror to load from
 * @param package the package
 */
int download_package(char *mirror, char *package) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    int folder = 0;

    char *url = mirror;

    if (check_if_package_exists(mirror, "core/", package) == 0) {
        folder = 1;

    } else if (check_if_package_exists(mirror, "extra/", package) == 0) {
        folder = 2;
    } else if (check_if_package_exists(mirror, "community/", package) == 0) {
        folder = 3;
    }

    switch(folder) {
        case (1): url = concat(url, "core/"); break;
        case (2): url = concat(url, "extra/"); break;
        case (3): url = concat(url, "community/"); break;
        default: return 1;
    }

    mirror = url;

    url = concat(url, package);


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

        if(res != CURLE_OK) {
            printf("fail");
            return 1;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
        {
            // success do not thing
        }
        else
        {
            printf("Failed to download file\n");
            curl_easy_cleanup(curl);
            return 1;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    free(url);

#ifdef _CHECKSUM_
    if (verify_checksum(mirror, package) != 0) return 1;
#endif
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
    cJSON_AddItemToObject(root, "mirror", cJSON_CreateString(("http://repo.wombatlinux.org/")));

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_config_file(out);

    free(out);
}

/**
 * Checks for the existence of a packages file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_packages_file() {
    if (access("packages.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        return 0;
        // printf("%s\n", json);
    } else {
        create_packages_file();
        return 1;
    }
}

/**
 * Checks for the existence of a config file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_config_file() {
    if (access("config.json", F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        return 0;
        // printf("%s\n", json);
    } else {
        create_config_file();
        return 1;
    }
}

/**
 * Loads JSON from a char array [json]
 *
 * @param json the char array to load as a JSON object
 */
cJSON *load_json(char *json) {
    cJSON *out = cJSON_Parse(json);

    return out;
}

/**
 * Performs a file [file] checksum and outputs the value to a char array [o]
 *
 * @param filename the file
 * @param o the char array to output the data to
 */
void checksum(char *filename, char *o[16]) {
    unsigned char c[MD5_DIGEST_LENGTH];
    int i;
    FILE *inFile = fopen(filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf("%s can't be opened.\n", filename);
    }


    MD5_Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, inFile)) != 0)
        MD5_Update(&mdContext, data, bytes);
    MD5_Final(c, &mdContext);
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
        char *part[2];
        sprintf(part, "%02x", c[i]);
        //sprintf(o, "%02x", c[i]);
        sprintf(o, "%s%s", o, part);
    }
    fclose(inFile);
}

/**
 * Given two [a] and [b] checksums, compares them
 *
 * @param a first file's checksum
 * @param b second file's checksum
 */
int checksum_compare(char *a, char *b) {
    int test = strcmp(a, b);
    if (test != 0) {
        return 1;
    }
    return 0;
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/**
 * Gets the package.json file from a mirror [url]
 *
 * @param url the repo
 */
cJSON *get_repo_json(char *url) {
    CURL *curl_handle;
    CURLcode res;

    /* 2020-11-07: 
    * IDK why this is here, but I am too afraid
    * to break something if I delete this. So it
    * stays in the code
    *
    * - afroraydude
    */
    chdir("adsfijo");

    url = concat(url, "packages.json");

    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    } else {
#ifdef _DEBUG_
        printf("%lu bytes retrieved\n", (unsigned long) chunk.size);
        printf("data: %s\n", chunk.memory);
#endif
    }


    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    cJSON *data = cJSON_Parse(chunk.memory);

    free(chunk.memory);

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return data;
}

/**
 * Verifies the checksum of a package [package] downloaded with the checksum of the
 * mirror [mirror]
 *
 * @param package the package
 * @param mirror the repo to pull from
 */
int verify_checksum(char *mirror, char *package) {
    cJSON *repoJSON = get_repo_json(mirror);
    char *packageChecksum = cJSON_GetObjectItem(repoJSON, package)->valuestring;

    char *filename = concat(package, ".uspm");
    char *fileChecksum[16];
    checksum(filename, fileChecksum);

    if (checksum_compare((char *) fileChecksum, packageChecksum) != 0) {
#ifdef _DEBUG_
        printf("%s - %s\n", fileChecksum, packageChecksum);
        printf("bad package file\n");
#endif
        return 1;
    } else {
#ifdef _DEBUG_
        printf("package file OK\n");
#endif
        return 0;
    }
}
