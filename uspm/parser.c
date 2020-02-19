/*
 * parser - parses JSON files used by the program
 */
#include <zconf.h>
#include <stdlib.h>
#include "parser.h"

void create_packages_file();
void create_config_file();

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

int check_config_file() {
    if (access("config.json",F_OK) != -1) {
        cJSON *root = load_file("packages.json");

        char *json = cJSON_Print(root);
        // printf("%s\n", json);
    } else {
        create_config_file();
    }
    return 0;
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