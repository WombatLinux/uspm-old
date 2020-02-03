//
// Created by afroraydude on 2/2/20.
//
#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "parser.h"

void create_config();

int check_config() {
    if (access("config.json",F_OK) != -1) {
        FILE *f = fopen("config.json", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        char *json = malloc(fsize + 1);
        fread(json, 1, fsize, f);
        fclose(f);

        printf("%s\n", json);
    } else {
        create_config();
    }
    return 0;
}

void create_config() {
    char *out;


    /* create root node and package for uspm package */
    cJSON *root, *uspm;
    root = cJSON_CreateObject();
    uspm = cJSON_CreateObject();

    /* add data to uspm package */
    cJSON_AddItemToObject(uspm, "version", cJSON_CreateString(("1.0.0")));
    cJSON_AddItemToObject(uspm, "dependencies", cJSON_CreateArray());

    /* add data to root object */
    cJSON_AddItemToObject(root, "uspm", uspm);

    out = cJSON_Print(root);
    printf("%s\n", out);

    FILE *ptr;

    ptr = fopen("config.json","w");

    fprintf(ptr,"%s",out);
    fclose(ptr);

    free(out);

    /* free all objects under root and root itself */
    cJSON_Delete(root);


}