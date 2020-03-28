//
// Created by afroraydude on 2/20/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <cjson/cJSON.h>
#include "../uspm/uspm.h"

int main(int argc, char *argv[])
{
    printf("Welcome to USPM Extended Suite\n");
    chdir(rootdir);

    int result = access(rootdir, W_OK);
    if (result != 0)
    {
        printf("Cannot write to the storage directory, exiting.\n");
        return false;
    }

    if (argc == 2)
    {
        if (strcmp(argv[1], "c") == 0)
        {
            for (int i = 2; i < argc; i++)
            {
                system("rm *.uspm");
            }
        }

        else if (strcmp(argv[1], "u") == 0)
        {
            cJSON *root = load_file(pkgfile);

            cJSON *package = root->child;

            while (package)
            {
                char *command = concat("uspm i ", package->string);

                system(command);

                package = package->next;
            }
        }

        else if (strcmp(argv[1], "p") == 0)
        {
            cJSON *root = load_file(pkgfile);

            cJSON *package = root->child;

            while (package)
            {
                char *command = concat("uspm u ", package->string);

                system(command);

                package = package->next;
            }
        }

        else if (strcmp(argv[1], "l") == 0)
        {
            printf("list packages\n");
            cJSON *root = load_file(pkgfile);

            cJSON *package = root->child;

            while (package)
            {
                printf("%s\n", package->string);

                package = package->next;
            }
        }

        else if (strcmp(argv[1], "y") == 0)
        {
            int updates = 0;

            /* get internal data */
            cJSON *config = load_file("config.json");
            cJSON *internal = load_file(pkgfile);
            char *mirror = cJSON_GetObjectItem(config, "mirror")->valuestring;

            /* download mirror */
            CURL *curl;
            FILE *fp;
            CURLcode res;
            char *url = concat(mirror, "mirrorpackages.json");
            free(config);
            url = concat(url, ".uspm");
            printf("%s\n", url);
            char outfilename[FILENAME_MAX] = "mirrorpackages.json";
            curl = curl_easy_init();
            if (curl)
            {
                fp = fopen(outfilename, "wb");
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                res = curl_easy_perform(curl);
                /* always cleanup */
                curl_easy_cleanup(curl);
                fclose(fp);
            }

            /* load mirror packagelist */
            cJSON *remote = load_file("mirrorpackages.json");

            /* compare */
            cJSON *package = internal->child;

            if (package != NULL) {
                cJSON *remotepackage = cJSON_GetObjectItem(remote, package->string);

                if (remotepackage != NULL) {
                    /* if installed version < remote version, update */
                    if (check_version(package->valuestring, remotepackage->valuestring) < 0) updates++;
                }

                package = package->next;
            }

            free(url);
            return true;
        }

        else
        {
            printf("command not found");
        }
    }
    else
    {
        printf("No command found");
    }
    return true;
}
