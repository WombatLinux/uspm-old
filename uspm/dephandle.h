//
// Created by afroraydude on 8/31/20.
//

#ifndef USPM_DEPHANDLE_H
#define USPM_DEPHANDLE_H

int install_dependency(char *, char *minversion);

int check_version(char* version1, char* version2);

int check_dependencies_and_install(char *package);

#endif //USPM_DEPHANDLE_H
