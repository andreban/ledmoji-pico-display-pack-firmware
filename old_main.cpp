#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico_display.hpp"
#include "drivers/st7789/st7789.hpp"
#include "lwip/apps/mqtt.h"

using namespace pimoroni;

/* MQTT Types and Functions  */

#define MQTT_SERVER "test.mosquitto.org"

typedef struct MQTT_CLIENT_T_ {
    ip_addr_t remote_addr;
    mqtt_client_t *mqtt_client;
    u32_t received;
    u32_t counter;
    u32_t reconnect;
} MQTT_CLIENT_T;

err_t mqtt_test_connect(MQTT_CLIENT_T *state);

// Perform initialisation
static MQTT_CLIENT_T* mqtt_client_init(void) {
    MQTT_CLIENT_T *state = new MQTT_CLIENT_T;
    if (!state) {
        printf("failed to allocate state\n");
        return NULL;
    }
    state->received = 0;
    return state;
}

ST7789 st7789(PicoDisplay::WIDTH, PicoDisplay::HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));
PicoGraphics_PenRGB332 graphics(st7789.width, st7789.height, nullptr);

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect\n");
        return 1;
    }

    graphics.set_pen(255, 0, 0);

//    while(true) {
        graphics.pixel(Point(0, 0));
//
//        // now we've done our drawing let's update the screen
        st7789.update(&graphics);
//    }

    /* sleep a bit to let usb stdio write out any buffer to host */
    sleep_ms(100);

    cyw43_arch_deinit();
    printf("All done\n");
}