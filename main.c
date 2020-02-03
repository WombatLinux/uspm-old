#include <stdio.h>
#include "parser.h"
#include "parser.c"

int main() {
    printf("Welcome to USPM\n");
    check_packages_file();
    //cJSON *test = load_packages_file();
    return 0;
}
