//
// Created by afroraydude on 8/31/20.
//

#ifndef USPM_DEPHANDLE_H
#define USPM_DEPHANDLE_H

/**
 * given a (dependency) package name [package], it downloads and checks the version, and
 *         then compares it with the given minimum version [minversion]
 *
 * returns 0 for success and 1 for failure
 *
 * @param package the package file
 * @param minversion the minimum version
 */
int install_dependency(char *package, char *minversion);

/**
 * given versions [version1] and [version2] it compares the two versions
 *
 * returns > 0 if [version1] is newer
 * returns < 0 if [version2] is newer
 * returns 0 if the versions are the same
 *
 * uses semantic versioning, may work for non-semantic versioning.
 *
 * @param version1 current version
 * @param version2 version to test against
 * 
 * @return int
 */
int check_version(char* version1, char* version2);

/**
 * Given [package] looks in the PACKAGEDATA and looks for any dependencies
 * required for installation or runtime.
 *
 * @param package the package file
 * 
 * @return int true if task performed successfully
 */
int check_for_dependencies(char *package);

#endif //USPM_DEPHANDLE_H
