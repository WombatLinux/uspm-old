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

/* concatenate strings */
char* concat(const char *s1, const char *s2);

/*
 * Given a filename/file location [file], attempts to parse as JSON
 * and then loads it in as a cJSON object.
 */
cJSON *load_file(char *file);

/*
 * Writes a fresh new packages file
 */
void write_packages_file(char *out);

/*
 * Writes a fresh new config file
 */
void write_config_file(char *out);

/*
 * Adds a package [packagename] to the list of packages along with its PACKAGEDATA contents [packagedata]
 */
int add_to_packages(char *packagename, cJSON *packagedata);

/*
 * Removes a package [packagename] from the list of packages along with its PACKAGEDATA contents [packagedata]
 */
int remove_from_packages(char *packagename);

/*
 * Downloads a package [package] file from a mirror [mirror]
 */
int download_package(char *mirror, char *package);

/*
 * Checks for the existence of a config file
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_config_file();

/*
 * Checks for the existence of a packages file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_packages_file();

/*
 * Performs a file [file] checksum and outputs the value to a char array [o]
 */
void checksum(char *filename, char *o[16]);

/*
 * Given two [a] and [b] checksums, compares them
 */
int checksum_compare(char *a, char *b);

/*
 * Loads JSON from a char array [json]
 */
cJSON *load_json(char *json);

/*
 * Gets the package.json file from a mirror [url]
 */
cJSON *get_repo_json(char* url);

/*
 * Verifies the checksum of a package [package] downloaded with the checksum of the
 * mirror [mirror]
 */
int verify_checksum(char *mirror, char *package);

#endif //USPM_FM_H
