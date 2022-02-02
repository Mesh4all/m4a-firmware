#include <string.h>
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "subsys_uart.h"

static const char *TAG = "subsys_uart";
static const int RX_BUF_SIZE = 1024;

#define ARRAY_SIZE  11

#define TXD_PIN (GPIO_NUM_10)
#define RXD_PIN (GPIO_NUM_9)

/** this are commands for tests this will be removed in the future */
char *COMMAND[11] = {"AT+WAP_SSID=kevin","AT+WAP_PASS=hiiii",
                    "AT+WSTA_SSID=new_ssid", "AT+WSTA_PASS=new_passoword",
                    "AT+WIFI_OFF", "AT+WIFI_ON", "AT+NVS_RST", "AT+WIFI_MODE=1",
                    "AT+WAP_CHAN=1", "AT+WAP_AUTH=1", "AT+WIFI_RST"};

char *default_keys[ARRAY_SIZE] = {
    "NVS_RST", "WIFI_OFF", "WAP_SSID", "WAP_PASS",
    "WAP_CHAN", "WAP_AUTH", "WIFI_ON",  "WIFI_MODE",
    "WIFI_RST", "WSTA_SSID", "WSTA_PASS",
};

esp_err_t setting_uart(void)
{
    esp_err_t err;
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    err = uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    if(err != ESP_OK) {
        return err;
    }
    err = uart_param_config(UART_NUM_1, &uart_config);
    if(err != ESP_OK) {
        return err;
    }
    err = uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if(err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}

int sendData(const char* logName, char* data)
{
    const int len = strlen(data);
    printf("before send %s \n", data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

esp_err_t parse_at_message(uint8_t* at_comant, at_request_t* output)
{
    char* commands = (char*)at_comant;
    char *key = strtok(commands, "+");
    char firts_step[100];
    char* key_value = NULL;
    char* value = NULL;

    int i = 0;
    while( key != NULL ) {
      if(i == 1){
        memcpy(firts_step, key, sizeof(firts_step));
      }
      i++;

      key = strtok(NULL, "+");
    }

    char *key2 = strtok(firts_step, "=");
    i = 0;
    while( key2 != NULL ) {
      if(i == 0){
        key_value = (char *) malloc(strlen(key2) +1);

        memset(key_value, 0, strlen(key2)+ 1);
        memcpy(key_value, key2, strlen(key2) + 1);
      }
      if(i == 1){
        value = (char *) malloc(strlen(key2) +1);
        memcpy(value, key2, strlen(key2) +1);
      }
      i++;
      key2 = strtok(NULL, "+");
    }

    /* cppcheck-suppress nullPointerRedundantCheck
    * (reason: <is necessary verify that this data is not null>) */
    memcpy(&output->key, key_value, strlen(key_value) +1);
    if (value != NULL) {
        memcpy(&output->value, value, strlen(value) + 1);
    }

    if(key_value == NULL && value == NULL ){
        return ESP_FAIL;
    }

    free(value);
    free(key_value);

    return ESP_OK;
}

int at_cmd_to_name (char* key)
{
    for (size_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (strcmp(default_keys[i], key) == 0){
            return i;
        }
    }

    return -1;
}

void at_handler(uint8_t* at_comant)
{
    char* commands = (char*)at_comant;
    at_request_t payload;

    payload.key = malloc(32);
    payload.value = malloc(32);
    parse_at_message(at_comant, &payload);

    enum at_keys_n at_key = at_cmd_to_name(payload.key);

    switch (at_key)
    {
    case (NVS_RST):
        printf("nvs rst \n");
        break;
    case (WIFI_OFF):
        printf("wifi off \n");
        break;
    case (WAP_SSID):
        printf("wifi_wap_ssid \n");
        break;
    case (WAP_PASS):
        printf("wifi_wap_pass \n");
         break;
    case (WAP_CHAN):
        printf("wifi_wap_ch \n");
         break;
    case (WAP_AUTH):
        printf("wifi_wap_auth \n");
        break;
    case (WIFI_ON):
        printf("wifi_on \n");
        break;
    case (WIFI_MODE):
        printf("wifi_mode \n");
        break;
    case (WIFI_RST):
        printf("wifi_rst \n");
        break;
    case (WSTA_SSID):
        printf("wifi_wsta_ssid \n");
        break;
    case (WSTA_PASS):
        printf("wifi_wsta_pass \n");
        break;
    default:
        printf("unhandler event %i \n", at_key);
        break;
    }
    ESP_LOGI(TAG, "KEY: '%s' ", payload.key);

    free(payload.key);
    free(payload.value);

}

void received_sensor_data (uint8_t* values)
{
    char* message = (char*)values;
    ESP_LOGI(TAG, "Read sensor data: '%s' ", message);
}

void tx_send_loop(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    int counter = 0;
    while (1) {
        if(counter == 4){
            counter = 0;
        } else {
            counter = counter + 1;
        }
        sendData(TX_TASK_TAG, COMMAND[counter]);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void rx_receive(void *arg)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            if(rxBytes > 2 && data[0] == 0x41 && data[1] == 0x54 && data[2] == 0x2b){
                at_handler(data);
            } else {
                received_sensor_data(data);
            }
            ESP_LOG_BUFFER_HEXDUMP(TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

esp_err_t init_uart()
{
    esp_err_t err = setting_uart();
    if(err != ESP_OK) {
        return err;
    }
    xTaskCreate(rx_receive, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    return ESP_OK;
}
