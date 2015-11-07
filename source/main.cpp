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


static DigitalOut g_led(LED1);
static Serial g_pc(USBTX, USBRX);
static WS2812 g_rgb(PTD2);

static void blinky(void) {

    g_led = !g_led;
    g_pc.printf("LED = %d \n\r",g_led.read());

    /* transmit RGB frame */
    g_rgb.send();
}

void app_start(int, char**){
    // set 115200 baud rate for stdout
    g_pc.baud(115200);

    minar::Scheduler::postCallback(blinky)
        .period(minar::milliseconds(100))
        .tolerance(minar::milliseconds(1));
}

