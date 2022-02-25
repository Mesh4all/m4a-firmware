#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "storage.h"
#include "wifi.h"

void app_main(void)
{

    // unity_run_all_tests();
    printf("Starting interactive test menu\n");
    unity_run_menu();
}
