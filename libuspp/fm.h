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

/**
 * concatenate strings
 *
 * @param s1 first string
 * @param s2 second string
 * */
char* concat(const char *s1, const char *s2);

/**
 * Given a filename/file location [file], attempts to parse as JSON
 * and then loads it in as a cJSON object.
 *
 * @param file file to load
 */
cJSON *load_file(char *file);

/**
 * Writes a fresh new packages file
 *
 * @param out the char array to output to the file
 */
void write_packages_file(char *out);

/**
 * Writes a fresh new config file
 *
 * @param out the char array to output to the file
 */
void write_config_file(char *out);

/**
 * Adds a package [packagename] to the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 * @param packagename the Package
 * @param packagedata the internal data
 */
int add_to_packages(char *packagename, cJSON *packagedata);

/**
 * Removes a package [packagename] from the list of packages along with its PACKAGEDATA contents [packagedata]
 *
 *
 * @param packagename package to remove
 */
int remove_from_packages(char *packagename);

/**
 * Downloads a package [package] file from a mirror [mirror]
 *
 *
 * @param mirror the web mirror to load from
 * @param package the package
 */
int download_package(char *mirror, char *package);

/**
 * Checks for the existence of a config file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_config_file();

/**
 * Checks for the existence of a packages file
 *
 * Returns 1 if nonexistant
 * Returns 0 if exists
 */
int check_packages_file();

/**
 * Performs a file [file] checksum and outputs the value to a char array [o]
 *
 * @param filename the file
 * @param o the char array to output the data to
 */
void checksum(char *filename, char *o[16]);

/**
 * Given two [a] and [b] checksums, compares them
 *
 * @param a first file's checksum
 * @param b second file's checksum
 */
int checksum_compare(char *a, char *b);

/**
 * @param json the char array to load as a JSON object
 * @param json
 * @return cJSON
 */
cJSON *load_json(char *json);

/**
 * Gets the package.json file from a mirror [url]
 *
 * @param url the repo
 */
cJSON *get_repo_json(char* url);

/**
 * Verifies the checksum of a package [package] downloaded with the checksum of the
 * mirror [mirror]
 *
 * @param package the package
 * @param mirror the repo to pull from
 */
int verify_checksum(char *mirror, char *package);

/**
 * Checks if a package exists
 * @param mirror the domain that holds the repos
 * @param folder The folder (core, extra, community) that data is held in
 * @param package
 * @return
 */
int check_if_package_exists(char *mirror, char *folder, char *package);

#endif //USPM_FM_H
