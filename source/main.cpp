/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#include "mbed-drivers/mbed.h"
#include "ws2812.h"

#define PIXEL_WIDTH  8
#define PIXEL_HEIGHT 8

static DigitalOut g_led(LED1);
static Serial g_pc(USBTX, USBRX);
static WS2812 g_rgb(PTD2, PIXEL_WIDTH, PIXEL_HEIGHT);

static void frame_update(void) {
    /* blink LED to show activity */
    g_led = !g_led;
    g_pc.printf("frame_update\n\r");

    /* transmit RGB frame */
    g_rgb.send();
}

void app_start(int, char**){
    int i;

    /* set 115200 baud rate for stdout */
    g_pc.baud(115200);

    /* set example pixels */
    for(i=0; i<PIXEL_WIDTH; i++)
    {
        g_rgb.set(i,i, 0x400000);
        g_rgb.set(PIXEL_WIDTH-1-i, i, 0x004000);
    }

    minar::Scheduler::postCallback(frame_update)
        .period(minar::milliseconds(50))
        .tolerance(minar::milliseconds(1));
}

