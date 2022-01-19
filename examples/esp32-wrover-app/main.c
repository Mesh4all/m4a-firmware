/*
 * Copyright (c) 2021 Mesh4all <mesh4all.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @brief       Main example file
 *
 * @author      xkevin190 <kevinvelasco190@gmail.com>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "shell_commands.h"
#include "msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "log.h"
#include "ztimer.h"
#include "timex.h"
#include "udpf.h"
#include "net/gnrc/netif.h"
#include "net/netif.h"
#include "cjson.h"
#include "lvgl/lvgl.h"
#include "lvgl_riot.h"
#include "ili9341.h"
#include "ili9341_params.h"
#include "disp_dev.h"
#include "ili9341_disp_dev.h"

#define EXAMPLE_ESP_WIFI_SSID      "Chicho"
#define EXAMPLE_ESP_WIFI_PASS      "Pirulin0312"
#define EXAMPLE_ESP_MAXIMUM_RETRY  1
#define ESP_STA_IFACE (8)

#define LOCHA_COLOR     "F8931C"
#define RIOT_R_COLOR    "BC1A29"
#define RIOT_G_COLOR    "3FA687"

static const char *TAG = "wifi station";
char *port = "3000";
udpf_payload payload;
static ili9341_t s_disp_dev;
static screen_dev_t s_screen;

///// SCREEN ////////
static lv_style_t style_box;
static lv_obj_t *win;
static uint32_t btn_cnt = 1;
static screen_dev_t s_screen;
lv_obj_t * table;

void receive_callback(char* message);

int dummy_cmd(int argc, char **argv)
{

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "udp", "send udp packets", udp_send },
    { NULL, NULL, NULL }
};

int xinit_sta (void)
{
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
        }
    };

    if (esp_wifi_set_mode(WIFI_MODE_STA) < 0) {
        printf("Error: setting sta mode \n");
        return -1;
    }

    if (esp_wifi_set_config(WIFI_IF_STA, &wifi_config) < 0) {
        printf("Error: setting config \n");
        return -1;
    }
    return 0;
}

int set_global_ipv6 (void)
{
    gnrc_netif_t *wifi_iface = gnrc_netif_get_by_pid(ESP_STA_IFACE);
    if (wifi_iface == NULL) {
        printf("Error: ESP32 WiFi STA interface doesn't exists.\n");
        return -1;
    }

    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, "2001:db8:1::2") == NULL){
        printf("Error: Invalid IPv6 global address\n");
        return -1;
    }

    /* Add node IPv6 global address */
    if (gnrc_netif_ipv6_addr_add(wifi_iface, &addr,
                                 64,
                                 GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID) < 0) {
        printf("Error: Couldn't add IPv6 global address\n");
        return -1;
    }

    return 0;
}

void receive_callback(char* message)
{
    printf("receive %s \n", message);
    cJSON* req_root = cJSON_Parse(message);
    cJSON* bitcoin = cJSON_GetObjectItemCaseSensitive(req_root, "bitcoin");
    cJSON* eth = cJSON_GetObjectItemCaseSensitive(req_root, "eth");
    const char* bitcoin_char = bitcoin->valuestring;
    const char* eth_char =  eth->valuestring;

    // printf("value bitcoin %c \n" , *bitcoin_char);
    if (table != NULL){
        printf("inside here \n");
        lv_table_set_cell_value(table, 1, 1, bitcoin_char);
        lv_table_set_cell_value(table, 2, 1, eth_char);
        lvgl_wakeup();
    }

    cJSON_Delete(req_root);
}

void screen (void)
{
    lv_style_init(&style_box);
    lv_style_set_value_align(&style_box, LV_STATE_DEFAULT, LV_ALIGN_OUT_TOP_LEFT);
    lv_style_set_value_ofs_y(&style_box, LV_STATE_DEFAULT, -LV_DPX(15));
    lv_style_set_margin_top(&style_box, LV_STATE_DEFAULT, LV_DPX(5));

    win = lv_win_create(lv_disp_get_scr_act(NULL), NULL);
    lv_win_set_title(win, "Crypto challenge");
    lv_win_set_layout(win, LV_LAYOUT_PRETTY_TOP);

    // List container
    table = lv_table_create(win, NULL);
    lv_table_set_col_cnt(table, 2);
    lv_table_set_row_cnt(table, 4);
    lv_obj_align(table, NULL, LV_ALIGN_CENTER, 0, 0);

    /*Make the cells of the first row center aligned */
    lv_table_set_cell_align(table, 0, 0, LV_LABEL_ALIGN_CENTER);
    lv_table_set_cell_align(table, 0, 1, LV_LABEL_ALIGN_CENTER);

    /*Align the price values to the right in the 2nd column*/
    lv_table_set_cell_align(table, 1, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 2, 1, LV_LABEL_ALIGN_RIGHT);
    lv_table_set_cell_align(table, 3, 1, LV_LABEL_ALIGN_RIGHT);

    lv_table_set_cell_type(table, 0, 0, 2);
    lv_table_set_cell_type(table, 0, 1, 2);

    /*Fill the first column*/
    lv_table_set_cell_value(table, 0, 0, "Name");
    lv_table_set_cell_value(table, 1, 0, "BTC");
    lv_table_set_cell_value(table, 2, 0, "ETH");

    /*Fill the second column*/
    lv_table_set_cell_value(table, 0, 1, "Price");
    lv_table_set_cell_value(table, 1, 1, "$57000");
    lv_table_set_cell_value(table, 2, 1, "$4500");

    lv_table_ext_t * ext = lv_obj_get_ext_attr(table);
    ext->row_h[0] = 20;

}

int scree_init(void)
{
    disp_dev_backlight_on();
    s_screen.display = (disp_dev_t *)&s_disp_dev;
    s_screen.display->driver = &ili9341_disp_dev_driver;

   if (ili9341_init(&s_disp_dev, &ili9341_params[0]) < 0 ){
       printf("Error: ili9341\n");
   }

   screen();

   lvgl_run();

   return 0;
}

int main(void)
{
    LOG_INFO("~~ Welcome to Mesh4all ~~\n");
    LOG_INFO("[APP] Free memory: %d bytes", esp_get_free_heap_size());
    /* Start shell */

    printf("init STA wifi\n");
    if (init_sta() < 0){
        printf("Error: init_sta was failed\n");
    };

    LOG_INFO("[APP] Free memory: %d bytes", esp_get_free_heap_size());

    printf("setting global ipv6\n");
    if (set_global_ipv6() < 0){
        printf("Error: could not  set global address\n");
    }
    ztimer_sleep(ZTIMER_MSEC, 5 * MS_PER_SEC);
    printf("starting udp server\n");

    payload.port = port;
    payload.callback = &receive_callback;

    if (udp_server(2, &payload) < 0){
        printf("Error: udp server couldn't started\n\n");
    }

    if (scree_init() < 0){
        printf("Error: screen not inicializated");
    }

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
