#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "esp_wifi.h"
#include "net/gnrc/netif.h"
#include "net/netif.h"

#define EXAMPLE_ESP_WIFI_SSID "KevinAPP"
#define EXAMPLE_ESP_WIFI_PASS "1234567890"
#define EXAMPLE_ESP_WIFI_CHANNEL   1
#define EXAMPLE_MAX_STA_CONN       4


int init_sta (int argc, char **argv) 
{
   
    if(argc < 2){
        printf("Error: the correct parameters are init_sta <ssid> and <password> \n");
        return -1;
    }
    printf("password is:%s ssid is:%s \n",(char*)argv[2], (char*)argv[1]);

    if(esp_wifi_set_mode(WIFI_MODE_STA) < 0) {
        printf("Error: setting sta mode \n");
        return -1;
    }
     
    wifi_config_t wifi_cconfig;
    
    bzero(&wifi_cconfig, sizeof(wifi_config_t));
    memcpy(wifi_cconfig.sta.ssid, argv[1], 33);
    memcpy(wifi_cconfig.sta.password, argv[2], 65);


    if(esp_wifi_set_config(WIFI_IF_STA, &wifi_cconfig) < 0) {
        printf("Error: setting config \n");
        return -1;
    }


    if( esp_wifi_connect() < 0) {
        printf("Error: to the connect with sta mode \n");
        return -1;
    } 

    return 0;
}


int disconnect_wifi(int argc, char **argv)
{
   if(esp_wifi_disconnect() < 0) {
       printf("error to the disconnect \n");
       return -1;
   }

   return 0;
}


int init_ap_mode (int argc, char **argv) {
    
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
        }
    };

    if(esp_wifi_set_mode(WIFI_MODE_AP) < 0)
    {
        printf("Error: couldn't set AP mode");
    }

    if(esp_wifi_set_config(WIFI_IF_AP, &wifi_config) < 0) 
    {
         printf("Error: couldn't set wifi_config");
    }


    esp_wifi_start();


    return 0;
}



static const shell_command_t shell_commands[] = {
    { "init_sta", "init sta mode and connect", init_sta },
    { "disconnect", "disconnect wifi", disconnect_wifi },
    { "connect", "connect ap mode", init_ap_mode},
    { NULL, NULL, NULL }
};


int main (void)
{
    printf("executed sta and ap \n");

    // esp_wifi_disconnect();

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}