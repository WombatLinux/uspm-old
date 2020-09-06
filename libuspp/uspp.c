/*The main package handling file.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include "fm.h"
#include "dephandle.h"
#include "uspp.h"
#include <archive.h>
#include <archive_entry.h>

/*
 * Given a package name [package], installs the package file (and, if necessary,
 * downloads) located in /var/uspm/storage
 *
 * @param package the package
 */
int
copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    la_int64_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}

int extract(const char *filename)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, filename, 10240)))
        return 1;
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            return 1;
        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                return 1;
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            return 1;
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return 0;
}

int install_package_file(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) != -1) {
        char *command;
        if (access(package, F_OK) == -1) {
            extract(filename);
        }

        if (check_for_dependencies(package) != 0) {
            printf("Installation failed\n");
            return 1;
        }

        command = concat("sh ./", package);
        command = concat(command, "/PACKAGECODE install");

        system(command);
        command = concat("rm -rf ./", package);
        command = concat(command, "/files");
        system(command);

        free(command);
        remove(filename);

        return 0;
    } else {
        printf("Failed to extract package file");
        return 1;
    }
}

/*
 * Given a package name [package], installs the package file (and, if necessary,
 * downloads) located in /var/uspm/storage
 *
 * @param package the package
 */
int install_package(char *package) {
    char *filename = concat(package, ".uspm");
    if (access(filename,F_OK) == -1) {
        cJSON *config = load_file("config.json");
        if (download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, package) != 0) return 1;
    }

    if (install_package_file(package) == 0) {
        char *file = concat("./", package);
        file = concat(file, "/PACKAGEDATA");

        cJSON *packagedata = load_file(file);

        free(file);

        add_to_packages(package, packagedata);

        return 0;
    } else {
        return 1;
    }
}

/*
 * Given a package name [package], uninstalled the package
 *
 * @param package the package
 */
int uninstall_package(char *package) {
    char *command = concat("sh ./", package);
    command = concat(command, "/PACKAGECODE uninstall");

    system(command);

    free(command);

    remove_from_packages(package);

    return 0;
}

int check_if_first_run() {
    if (check_config_file() == 1 ||
            check_packages_file() == 1) return 1;
    return 0;
}
