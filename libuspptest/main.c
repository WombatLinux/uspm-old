//
// Created by afroraydude on 9/1/20.
//
#include <cjson/cJSON.h>
#include <uspm/fm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    cJSON *root = get_repo_json("http://localhost:5000/packages.json");

    char *output = cJSON_Print(root);

    printf("%s\n", output);

    return 0;
}