//
// Created by afroraydude on 2/2/20.
//

#ifndef USPM_PARSER_H
#define USPM_PARSER_H

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <zconf.h>

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
            return 1;

        /* copy all the text into the buffer */
        fread(buffer, sizeof(char), numbytes, infile);
        fclose(infile);

        /* confirm we have read the file by
        outputing it to the console */
        //printf("The file contains this text\n\n%s", buffer);

        cJSON *root = cJSON_Parse(buffer);

        cJSON_Print(root);

        /* free the memory we used for the buffer */
        free(buffer);

        return root;
    } else {
        printf("No file\n");
        return NULL;
    }
}

#endif //USPM_PARSER_H
