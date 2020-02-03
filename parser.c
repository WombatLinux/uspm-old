/*
 * parser - parses JSON files used by the program
 */
#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "parser.h"

void create_packages_file();

int check_packages_file() {
    if (access("packages.json",F_OK) != -1) {
        cJSON *root = load_file("packages.json");

    } else {
        create_packages_file();
    }
    return 0;
}

void create_packages_file() {
    char *out;

    /* create root node and package for uspm package */
    cJSON *root, *uspm;
    FILE *ptr;
    root = cJSON_CreateObject();
    uspm = cJSON_CreateObject();

    /* add data to uspm package */
    cJSON_AddItemToObject(uspm, "version", cJSON_CreateString(("1.0.0")));
    cJSON_AddItemToObject(uspm, "dependencies", cJSON_CreateArray());

    /* add data to root object */
    cJSON_AddItemToObject(root, "uspm", uspm);

    out = cJSON_Print(root);
    printf("%s\n", out);

    ptr = fopen("packages.json","w");

    fprintf(ptr,"%s",out);
    fclose(ptr);

    free(out);
    free(ptr);

    /* free all objects under root and root itself */
    cJSON_Delete(root);
    free(root);
}

int add_to_packages(package) {
    cJSON *root = load_file("packages.json");

    return 0;
}