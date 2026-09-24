/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * RGB matrix WS2812 bit-bang driver (A7).
 *
 * SPI + DMA is not usable on this board: on STM32F072 the SPI WS2812 driver
 * needs the SCK pin in alternate-function mode, and both SPI1 SCK pins
 * (A5, B3) are key matrix columns.
 *
 * Only transmits when the LED buffer changed (flush_rgb_leds), which keeps
 * the data line quiet while LED power is off.
 */

#include "ws2812_driver.h"
#include "uart.h"

ws2812_led_t ws2812_leds[WS2812_LED_COUNT];
bool         flush_rgb_leds = 0;

static void send_byte(uint8_t byte) {
    // WS2812 protocol wants most significant bits first
    for (unsigned char bit = 0; bit < 8; bit++) {
        bool is_one = byte & (1 << (7 - bit));
        // using something like wait_ns(is_one ? T1L : T0L) here throws off timings
        if (is_one) {
            gpio_write_pin_high(WS2812_DI_PIN);
            wait_ns(WS2812_T1H);
            gpio_write_pin_low(WS2812_DI_PIN);
            wait_ns(WS2812_T1L);
        } else {
            gpio_write_pin_high(WS2812_DI_PIN);
            wait_ns(WS2812_T0H);
            gpio_write_pin_low(WS2812_DI_PIN);
            wait_ns(WS2812_T0L);
        }
    }
}

void ws2812_init(void) {
    palSetLineMode(WS2812_DI_PIN, WS2812_OUTPUT_MODE);
}

void ws2812_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    if (index < 0 || index >= WS2812_LED_COUNT) { return; }
    if (ws2812_leds[index].r != red || ws2812_leds[index].g != green || ws2812_leds[index].b != blue) {
        flush_rgb_leds = true;
    }
    ws2812_leds[index].r = red;
    ws2812_leds[index].g = green;
    ws2812_leds[index].b = blue;
}

void ws2812_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        ws2812_set_color(i, red, green, blue);
    }
}

void ws2812_flush(void) {
    // A full frame masks UART interrupts. Let the RF receive task drain
    // already-pending bytes first, then refresh on a later RGB matrix tick.
    if (!flush_rgb_leds || uart_available()) { return; }

    // this code is very time dependent, so we need to disable interrupts
    chSysLock();
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        // WS2812 protocol dictates grb order
        send_byte(ws2812_leds[i].g);
        send_byte(ws2812_leds[i].r);
        send_byte(ws2812_leds[i].b);
    }
    wait_ns(WS2812_RES);
    chSysUnlock();

    flush_rgb_leds = false;
}
