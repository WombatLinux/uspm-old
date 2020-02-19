//
// Created by afroraydude on 2/19/20.
//
#include <stdio.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

int createpkg();

int main(int argc, char *argv[]) {
    printf("Welcome to uspm-mkpkg. Please make sure this is running in the package's folder and the name of the folder is the same as the package's name\n\n");

    createpkg();
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator

    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void write_file(char *filename, char *out) {
    char *outfile = concat(out, "\n");

    FILE *ptr;

    ptr = fopen(filename,"w");

    fprintf(ptr,"%s",outfile);
    fclose(ptr);

    free(outfile);
}

int createpkg() {
    char name[64], version[64], setbool[64], depName[64], depVer[64];
    cJSON *root, *dependencies;
    int *dependenciesDone = 1;

    root = cJSON_CreateObject();
    dependencies = cJSON_CreateObject();

    // create package details
    printf("What is the package name? \n");
    scanf("%s", name);
    printf("Enter the version: \n");
    scanf("%s", version);
    printf("%s\n", version);

    printf("Are there dependencies? (y/n) \n");
    scanf("%s", setbool);
    while(dependenciesDone == 1) {
        if (strcmp(setbool, "n") == 0) {
            dependenciesDone = 0;
        } else if (strcmp(setbool, "y") == 0) {
            printf("Enter the name for the dependency: \n");
            scanf("%s", depName);
            printf("Enter the minimum version for the dependency: \n");
            scanf("%s", depVer);

            cJSON_AddStringToObject(dependencies, depName, depVer);

            printf("Are there any more dependencies? (y/n) \n");
            scanf("%s", setbool);
        } else {
            printf("Invalid. Use \"y\" or \"n\" (lowercase, without quotes) \n");
        }
    }

    cJSON_AddStringToObject(root, "dependencies", dependencies);
    cJSON_AddStringToObject(root, "version", version);

    char *out = cJSON_Print(root);
    write_file("PACKAGEDATA", out);

    out = "#!/bin/sh\ncd INSERTFOLDERHERE\n\nif [ $1 == 'install' ]\nthen\n\tINSERT INSTALL CODE HERE\nelif [ $1 == 'uninstall' ]\nthen\n\tINSERT UNINSTALL CODE HERE\nfi\n";
    write_file("PACKAGECODE", out);
    system("vim PACKAGECODE");

    chdir("..");

    char *packageFile = concat(name, ".uspm");
    char *command = concat("tar cf ", packageFile);
    command = concat(command, " ");

    command = concat(command, name);

    system(command);

    return 0;
}