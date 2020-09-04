/*
 *  ___  ___  ________  ________  ________
 * |\  \|\  \|\   ____\|\   __  \|\   __  \
 * \ \  \\\  \ \  \___|\ \  \|\  \ \  \|\  \
 *  \ \  \\\  \ \_____  \ \   ____\ \   ____\
 *   \ \  \\\  \|____|\  \ \  \___|\ \  \___|
 *    \ \_______\____\_\  \ \__\    \ \__\
 *     \|_______|\_________\|__|     \|__|
 *               \|_________|
 *
 *   (U)ltra (S)imple (P)ackage (P)rocessor
 *
 *            (c) 2020 afroraydude
 *
 */

#ifndef USPM_INUNIN_H
#define USPM_INUNIN_H

#include "fm.h"
#include "dephandle.h"

int install_package(char *package);

int uninstall_package(char *package);

int check_dependencies(char *package);

int check_if_first_run();

#endif //USPM_INUNIN_H
