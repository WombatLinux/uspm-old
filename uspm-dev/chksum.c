#include <uspm/fm.h>
#include <zconf.h>

int main(int argc, char *argv[]) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);


    return 0;
}