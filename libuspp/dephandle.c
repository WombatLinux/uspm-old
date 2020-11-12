//
// Created by afroraydude on 2/19/20.
//
#include <string.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include "fm.h"
#include "dephandle.h"
#include <stdlib.h>
#include "uspp.h"
#include "config.h"

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

/**
 * given versions [version1] and [version2] it compares the two versions
 *
 * returns > 0 if [version1] is newer
 * returns < 0 if [version2] is newer
 * returns 0 if the versions are the same
 *
 * uses semantic versioning, may work for non-semantic versioning.
 *
 * @param version1 current version
 * @param version2 version to test against
 */
int check_version(char *version1, char *version2) {
    int len_version1 = strlen(version1);
    int len_version2 = strlen(version2);

    char *substr_version1 = (char *) malloc(sizeof(char) * 1000);
    char *substr_version2 = (char *) malloc(sizeof(char) * 1000);

    // loop until both strings are exhausted.
    // and extract the substrings from version1 and version2
    int i = 0, j = 0;
    while (i < len_version1 || j < len_version2) {
        int p = 0, q = 0;

        // skip the leading zeros in version1 string.
        while (version1[i] == '0')
            i++;

        // skip the leading zeros in version2 string.
        while (version2[j] == '0')
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

/**
 * Given [package] looks in the PACKAGEDATA and looks for any dependencies
 * required for installation or runtime.
 *
 * @param package the package file
 */
int check_for_dependencies(char *package) {
    printf("Checking dependencies...\n");

    char *file = concat("./", package);
    file = concat(file, "/PACKAGEDATA");

    cJSON *packagedata = load_file(file);

    cJSON *root = load_file("packages.json");

    cJSON *dependencies = cJSON_GetObjectItem(packagedata, "dependencies");

    cJSON *dependency = dependencies->child;

    while (dependency) {
        /* try to load dependency from packages.json */
        cJSON *dependency_internal = cJSON_GetObjectItem(root, dependency->string);

        /* load min version */
        char *minversion = dependency->valuestring;

        /* if we have that package installed already */
        if (dependency_internal != NULL) {
            char *version = cJSON_GetObjectItem(dependency_internal, "version")->valuestring;

            if (check_version(version, minversion) < 0) {
                printf("%s (outdated)...installing first\n", dependency->string);
                if (install_dependency(dependency->string, minversion) != 0) {
                    return 1;
                }
            } else {
                printf("%s\n", dependency->string);
            }
        } else {
            printf("Dependency not installed\n");
            if (install_dependency(dependency->string, minversion) != 0) {
                return 1;
            }
        }
        /* do what we need to do */
        dependency = dependency->next;
    }

    printf("No more dependencies found\n");
    return 0;
}

/**
 * given a (dependency) package name [package], it downloads and checks the version, and
 * then compares it with the given minimum version [minversion]
 *
 * returns 0 for success and 1 for failure
 *
 * @param package the package file
 * @param minversion the minimum version
 
 * TODO: Rewrite to utilize repo json
 */
int install_dependency(char *package, char *minversion) {
    char *filename = concat(package, ".uspm");

    if (access(filename, F_OK) == -1) {
        cJSON *config = load_file("config.json");
        if (download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, package) != 0) return 1;
    }

    char *command = concat("tar -xf ", filename);
    system(command);

    filename = concat(package, "/PACKAGEDATA");
    cJSON *root = load_file(filename);
    char *version = cJSON_GetObjectItem(root, "version")->valuestring;
    if (check_version(version, minversion) < 0) {
        printf("No good version of dependency found. Aborting.\n");
        return 1;
    }

    if (install_package(package) != 0) {
        return 1;
    }

    return 0;
}
