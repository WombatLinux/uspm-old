//
// Created by afroraydude on 8/31/20.
//

#ifndef USPM_INUNIN_H
#define USPM_INUNIN_H

#include "fm.h"
#include "dephandle.h"

int install_package(char *package);

int uninstall_package(char *package);

int check_dependencies(char *package);

#endif //USPM_INUNIN_H
