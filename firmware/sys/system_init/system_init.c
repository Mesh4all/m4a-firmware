#include "stdio.h"
#include "ds18_sensor.h"
#include "moisture_sensor.h"
#include "xtimer.h"
#include "serialization.h"
#include "udp_m4a.h"
#include "board.h"
#include "thread.h"
#include "system_init.h"
#include "net/gnrc/netif.h"
#include "net/netif.h"

#ifdef CPU_ESP32
#include "esp_system.h"
#define SLIP_IFACE (10)
#else
#define SLIP_IFACE (0)
#endif

#ifdef DS18B20_PIN
#define DS18_PIN  DS18B20_PIN
#else
#define DS18_PIN  0  /* REMOVE THIS SOON. is recommended add DS18B20 PIN since the settings of your board,
                         this variable is for avoid compilations error in other boards. */
#endif

#define SERVER_BUFFER_SIZE      (100)

uint8_t buffer[SERVER_BUFFER_SIZE];

char get_sensor_event[THREAD_STACKSIZE_DEFAULT];

void *init_loop (void *args)
{
    int port = 3000;
    char address[30] = "::1";
    sensor_data payload;
    available_sensors_t* available_periperals = (available_sensors_t*)args;
    while (1) {
        if (available_periperals->moisture_sensor_is_available != 0) {
            if (get_moisture_value(&payload.soil_moisture) < 0) {
                payload.soil_moisture = 0;
                printf("Error to the obtain the moisture \n");
            }
        }
        if (available_periperals->ds18_is_available != 0) {
            if (get_temperature_value(&payload.temperature) < 0) {
                payload.temperature = 0;
                printf("Error to the obtain the temperature \n");
            }
        }

        if (available_periperals->ds18_is_available != 0 ||
            available_periperals->moisture_sensor_is_available != 0) {
             printf("the soil_moisture is %i and temperature is %i \n",
                            payload.soil_moisture, payload.temperature);
             size_t len_cbor = sizeof(sensor_data);
             if (cbor_enconde_message(&payload, buffer, &len_cbor) < 0) {
                 printf("error to encode payload with cbor");
             }
             else {
                udp_send(&port, address, buffer, &len_cbor);
             }
        }
        xtimer_sleep(60);
    }
}

int set_wired_ipv6 (void)
{
    uint8_t mac[6] = {0x42, 0x55, 0xbb, 0xfe, 0x17, 0x2 };
    #ifdef CPU_ESP32
    if (esp_base_mac_addr_get(mac) < 0) {
        printf("error with the mac \n");
        return -1;
    }
    #endif

    gnrc_netif_t *wifi_iface = gnrc_netif_get_by_pid(SLIP_IFACE);
    if ( wifi_iface == NULL) {
        printf("Error: ESP32 WiFi STA interface doesn't exists.\n");
        return -1;
    }

    ipv6_addr_t addr = {
        .u8 = { 0xff, 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,mac[2], mac[3], mac[4], mac[5]}
    };

    /* Add node IPv6 global address */
    if (gnrc_netif_ipv6_addr_add(wifi_iface, &addr,
                                 64,
                                 GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID) < 0) {
        printf("Error: Couldn't add IPv6 global address\n");
        return -1;
    }

    return 0;
}

int init_initial_params (void)
{
    int16_t temp = 0;
    int soil_moisture = 0;
    available_sensors_t available ={
        .ds18_is_available = 1,
        .moisture_sensor_is_available = 1,
    };
    if (init_moisture() < 0) {
        available.ds18_is_available = 0;
        printf("Error to the init the moisture sensor");
    }

    if (init_temperature_sensor(DS18_PIN) < 0) {
        available.moisture_sensor_is_available = 0;
        printf("Error to the init ds18 sensor");
    }

    xtimer_sleep(5);
    if (get_moisture_value(&soil_moisture) < 0) {
        available.moisture_sensor_is_available = 0;
        printf("Error to the obtain the moisture \n");
    }

    if (get_temperature_value(&temp) < 0) {
        available.ds18_is_available = 0;
        printf("Error to the obtain the temperature \n");
    }

    printf("moisture_sensor_is_avaliable %i \n", soil_moisture);

    if (set_wired_ipv6() < 0) {
        printf("Error: could not add wired address \n");
        return -1;
    }

    if (thread_create(get_sensor_event, sizeof(get_sensor_event), THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST, *init_loop, &available, "Sensors event") <= KERNEL_PID_UNDEF ) {
        printf("peripherals event loop could not init \n");
        return -1;
    }

    return 0;
}
