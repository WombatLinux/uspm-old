//
// Created by afroraydude on 9/1/20.
//
#include <uspm/fm.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    unsigned char *x = get_checksum("uspm", x);
    printf("%s\n", x);
    unsigned char *y = get_checksum("uspm", y);
    printf("%s\n", y);


    if(compare_checksum(x, y) != 0) {
        printf("Checksum compare returned false\n");
    } else {
        printf("Checksum compare returned true\n");
    }
}