/**
 * This program tests multiple functions of libuspp.
 */

#include <uspm/uspp.h>
#include <uspm/fm.h>

int perform_test(cJSON *config) {
    if (download_package(cJSON_GetObjectItem(config, "mirror")->valuestring, "test")) {
        
    } else {
        return 1;
    }
}

int main() {
    int returnVal = 0;

    check_if_first_run();

    cJSON *config = load_file("config.json");

    perform_test(config)

    return returnVal;
}