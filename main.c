#include <stdio.h>
#include "parser.h"
#include "parser.c"

int main(int argc, char *argv[]) {
    printf("Welcome to USPM\n");
    check_packages_file();
    //cJSON *test = load_packages_file();

    if (argc == 3) {
        printf("%s\n", argv[1]);
        printf("%s\n", argv[2]);
    }
    return 0;
}
