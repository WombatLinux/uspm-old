//
// Created by afroraydude on 9/1/20.
//
#include <uspm/fm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);

    char *file1[16];
    checksum("uspm", file1);
    printf("%s\n", file1);

    char *file2[16];
    checksum("Makefile", file2);
    printf("%s\n", file2);
    int x = checksum_compare(file1, file2);
    if (x != 0) {
        printf("Checksum compare returned false.\n");
    } else {
        printf("Checksum compare returned true.\n");
    }
    //printf("%s\n", x);
    //unsigned char *y = get_checksum("uspm", y);
    //printf("%s\n", y);


    /*if(compare_checksum(x, y, 16) != 0) {
        printf("Checksum compare returned false\n");
    } else {
        printf("Checksum compare returned true\n");
    }*/
    return 0;
}