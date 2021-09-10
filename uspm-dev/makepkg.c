
#include <stdio.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#include <uspm/fm.h>


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
    int dependenciesDone = 1;

    root = cJSON_CreateObject();
    dependencies = cJSON_CreateObject();

    /* package details */
    printf("What is the package name? \n");
    scanf("%s", name);
    printf("Enter the version: \n");
    scanf("%s", version);
    printf("%s\n", version);

    /* dependencies */
    /* use semantic pls */
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

    /* go into folder */
    chdir(name);

    cJSON_AddItemToObject(root, "dependencies", dependencies);
    cJSON_AddStringToObject(root, "version", version);

    char *out = cJSON_Print(root);
    write_file("PACKAGEDATA", out);

    char *out1 = "#!/bin/sh\ncd ";
    char *out2 = "\n\nif [ $1 == 'install' ]\nthen\n\t# INSERT INSTALL CODE HERE\nelif [ $1 == 'uninstall' ]\nthen\n\t# INSERT UNINSTALL CODE HERE\nfi\n";
    out = concat(concat(out1, name), out2); /* stupid new concat system */
    write_file("PACKAGECODE", out);
    system("vim PACKAGECODE"); /* assuming they have vim, brave */

    chdir("..");

    /* create file for package */
    char *packageFile = concat(name, ".uspm");
    char *command = concat("tar cf ", packageFile);
    command = concat(command, " ");
    command = concat(command, name);
    system(command);

    return 0;
}

int main(int argc, char *argv[]) {
    printf("Welcome to uspm-mkpkg.\nPlease make sure that:\n\t- This is running in the parent directory of the package's folder, and\n\t- The name of the folder is the same as the package's name\n\n");
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    createpkg();
}
