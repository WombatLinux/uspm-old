/*
 * fm - manages the json files and stuff
 * few extras added
 * HEADER
 */

#ifndef USPM_FM_H
#define USPM_FM_H

#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>


char* concat(const char *s1, const char *s2);

int compareSubstr(char *substr_version1, char *substr_version2,
                  int len_substr_version1, int len_substr_version2);

cJSON *load_file(char *file);

void write_packages_file(char *out);

void write_config_file(char *out);

int add_to_packages(char *packagename, cJSON *packagedata);

int remove_from_packages(char *packagename);

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

int download_package(char *mirror, char *package);

int check_config_file();

int check_packages_file();

#endif //USPM_FM_H
