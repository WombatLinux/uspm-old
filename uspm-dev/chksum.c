#include <uspm/fm.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("Current working dir: %s\n", cwd);
        printf("filename: ");
        printf(argv[1]);
        printf("\n");
        unsigned char *x;
        get_checksum(argv[1], x);
        printf("%s\n", x);
    } else if (argc == 3) {

    }

    return 0;
}