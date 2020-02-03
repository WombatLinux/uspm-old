//
// Created by afroraydude on 2/2/20.
//

#ifndef USPM_PARSER_H
#define USPM_PARSER_H

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

cJSON *load_file(char *file) {
    if (access(file,F_OK) != -1) {
        /* declare a file pointer */
        FILE    *infile;
        char    *buffer;
        long    numbytes;

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
        buffer = (char*)calloc(numbytes, sizeof(char));

        /* memory error */
        if(buffer == NULL)
            return NULL;

        /* copy all the text into the buffer */
        fread(buffer, sizeof(char), numbytes, infile);
        fclose(infile);

        /* confirm we have read the file by
        outputing it to the console */
        //printf("The file contains this text\n\n%s", buffer);

        cJSON *root = cJSON_Parse(buffer);

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

    ptr = fopen("packages.json","w");

    fprintf(ptr,"%s",outfile);
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

#endif //USPM_PARSER_H
