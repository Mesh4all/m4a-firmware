#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"

#include "shell.h"
#include "thread.h"
#include "msg.h"

#include "periph/uart.h"
#include "xtimer.h"

#define AT_UART UART_DEV(1)

#define AT_UART_BAUDRATE 115200
#define MAX_SENTENCE_SIZE 128

#define MAIN_QUEUE_SIZE (8)
msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

size_t count = 0;
char uart_buff[MAX_SENTENCE_SIZE] = {0};
char *commands_at[] = {
    "AT+WAP_SSID=",
    "AT+WAP_PASS=",
    "AT+WSTA_SSID=",
    "AT+WSTA_PASS=",
    "AT+WIFI_OFF",
    "AT+WIFI_ON",
    "AT+NVS_RST",
    "AT+WIFI_MODE=",
    "AT+WAP_CHAN=",
    "AT_WAP_AUTH=",
    "AT+WIFI_RST"
};

static void at_uart_rx_cb(void *arg, uint8_t c) {
    if (c != 10) {
        uart_buff[count] = (char)c;
        count += 1;
    } else {
        printf("%s\n", (char *)uart_buff);
        memset(uart_buff, 0, sizeof(uart_buff));
        count = 0;
    }
}

void send_at(char *at_command, char *param_at) {
    char strbuf[64];
    strcpy(strbuf, at_command);
    uart_poweron(AT_UART);
    if (strcmp(param_at, NULL) == 0) {
        uart_write(AT_UART, (uint8_t *)strbuf, sizeof(strbuf));
    } else {
        strcat(strbuf, param_at);
        uart_write(AT_UART, (uint8_t *)strbuf, sizeof(strbuf));
    }
    uart_poweroff(AT_UART);
}

int cmd_uart_at(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: \n--help\n");
        return -1;
    }

    else if (strcmp("on", argv[1]) == 0) {

    } else if (strcmp("off", argv[1]) == 0) {
        /* Manda el AT para apagar el wifi */
        uart_poweroff(AT_UART);
        return 0;

    } else if (strcmp("ap", argv[1]) == 0) /* Configuracion en modo AP */
    {
        if (argc != 3) {
            printf("\nWifi ap settings:\n\tssid\n\tpswd\n\tauth\n\tchannel\n");
            return 0;

        } else if (strcmp(argv[2], "ssid") == 0) {
            if (argc != 4) {
                printf("\nPlease enter your SSID name to Wifi mode ap settings\n*Remember use "
                       "least of 32 characters*\n");
                return 0;

            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
            }

        } else if (strcmp(argv[2], "pswd") == 0) {
            if (argc != 4) {
                printf("\nPlease enter your password to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
            }

        } else if (strcmp(argv[2], "auth") == 0) {
            if (argc != 4) {
                printf("\nDefine the authentication mode to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
            }

        } else if (strcmp(argv[2], "channel") == 0) {
            if (argc != 4) {
                printf("\nChoose the channel <1 ... 7> to Wifi mode ap settings\n");
                return 0;

            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
            }
        }
        return 0;
    } else if (strcmp("sta", argv[1]) == 0) { /* Configuracion del modo STA */
        if (argc != 3) {
            printf("\nWifi ap settings:\n\tssid\n\tpswd\n\tauth\n\tchannel\n");
            return 0;
        } else if (strcmp(argv[2], "ssid") == 0) {
            if (argc != 4) {
                printf("\nPlease enter your SSID name to Wifi mode ap settings\n*Remember use "
                       "least of 32 characters*\n");
                return 0;
            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
            }
        } else if (strcmp(argv[2], "pswd") == 0) {
            if (argc != 4) {
                printf("\nPlease enter your password to Wifi mode ap settings\n");
                return 0;
            } else if (strcmp(argv[3], NULL) != 0) {
                /*Envia por Uart el comando at con argv[3] unido al string at correspondiente*/
                return 0;
            }
        }
    } else if (strcmp("mode", argv[1]) == 0) {
        if (argc != 3) {
            /*Error falta de comando */
        }
        /* Manda el AT para iniciar el wifi */
        return uart_init(AT_UART, AT_UART_BAUDRATE, at_uart_rx_cb, NULL);
    }
    printf("Usage: \nuart start|stop\n");
    return -1;
}

static const shell_command_t shell_commands[] = {
    {"wifi", "start or stop reading the GPS uart port", cmd_uart_at}, {NULL, NULL, NULL}};

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("Welcome to RIOT!");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}