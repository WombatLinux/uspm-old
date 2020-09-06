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

/*
 * Given a package name [package], installs the package file (and, if necessary,
 * downloads) located in /var/uspm/storage
 *
 * @param package the package
 */
int install_package(char *package);

/*
 * Given a package name [package], uninstalled the package
 *
 * @param package the package
 */
int uninstall_package(char *package);

/*
 * Checks if this is the first run
 *
 * If true, creates the necessary files for running the USPM software
 *
 * Returns 0 if true
 * Returns 1 if false
 */
int check_if_first_run();

#endif //USPM_INUNIN_H
