#include <uspm/fm.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    if (argc == 2) {
        char *chksum[16];
        checksum(argv[1], chksum);
        printf("%s:",argv[1]);
        printf(" %s\n",chksum);
    } else if (argc == 3) {
        char *chksum1[16];
        checksum(argv[1], chksum1);
        printf("%s:",argv[1]);
        printf(" %s\n",chksum1);

        char *chksum2[16];
        checksum(argv[2], chksum2);
        printf("%s:",argv[2]);
        printf(" %s\n",chksum2);

        int x = checksum_compare(chksum1, chksum2);
        if (x != 0) {
            printf("Checksum compare returned false.\n");
        } else {
            printf("Checksum compare returned true.\n");
        }
    }
    return 0;
}