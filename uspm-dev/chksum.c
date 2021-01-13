#include <uspm/fm.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    /* if just getting for one file */
    if (argc == 2) {
        char *chksum[16];
        checksum(argv[1], chksum);
        printf("%s:",argv[1]);
        printf(" %s\n",chksum);
    } 
    /* if comparing two files */
    else if (argc == 3) { 
        char *chksum1[16];
        checksum(argv[1], chksum1);
        printf("%s:",argv[1]);
        printf(" %s\n",chksum1);

        char *chksum2[16];
        checksum(argv[2], chksum2);
        printf("%s:",argv[2]);
        printf(" %s\n",chksum2);

        /* use libuspp to comare */
        int x = checksum_compare(chksum1, chksum2);
        if (x != 0) {
            printf("Checksum compare returned false.\n");
        } else {
            printf("Checksum compare returned true.\n");
        }
    }
    return 0;
}