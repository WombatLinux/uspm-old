/*
 * parser - parses JSON files used by the program
 */
#include <zconf.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "parser.h"

void create_packages_file();

int check_packages_file() {
    if (access("packages.json",F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        char *json = cJSON_Print(root);
        // printf("%s\n", json);
    } else {
        create_packages_file();
    }
    return 0;
}

void create_packages_file() {
    char *out;
    cJSON *root, *uspm;

    root = cJSON_CreateObject();
    uspm = cJSON_CreateObject();

    /* add data to uspm package */
    cJSON_AddItemToObject(uspm, "version", cJSON_CreateString(("1.0.0")));
    cJSON_AddItemToObject(uspm, "dependencies", cJSON_CreateArray());

    /* add data to root object */
    cJSON_AddItemToObject(root, "uspm", uspm);

    out = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);

    write_packages_file(out);

    free(out);
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