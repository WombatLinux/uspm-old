#include <uspm/fm.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    if (argc == 2) {
        printf("filename: ");
        printf(argv[1]);
        printf("\n");
        unsigned char *x = get_checksum(argv[1]);
        printf("Checksum: %s\n", x);
    } else if (argc == 3) {
        unsigned char *x = get_checksum(argv[1]);
        printf("Checksum of file 1: %s\n", x);
        unsigned char *y = get_checksum(argv[2]);
        printf("Checksum of file 2: %s\n", y);

        if(compare_checksum(x, y, 16) != 0) {
            printf("Checksum compare returned false\n");
        } else {
            printf("Checksum compare returned true\n");
        }
    }

    return 0;
}