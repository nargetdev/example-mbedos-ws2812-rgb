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
#include "sal-iface-eth/EthernetInterface.h"
#include "sal-stack-lwip/lwipv4_init.h"
#include "sal/socket_api.h"
#include "sockets/UDPSocket.h"
#include "ws2xxx-rgb-led/ws2xxx.h"

#define IP "10.200.1.20"
#define MASK "255.255.255.0"
#define GATEWAY "10.200.1.200"

#define STR_NUM(x) #x
#define STR(x) STR_NUM(x)

using namespace mbed::Sockets::v0;

#define UDP_SERVER_PORT 7890
// #define PIXEL_WIDTH 8
// #define PIXEL_HEIGHT 8
#define STRIP_LENGTH 6
#define SERVER_RESPONSE "{\"type\": \"RAINBOW_MATRIX\", \"width\": " STR(PIXEL_WIDTH) ",\"height\": " STR(PIXEL_HEIGHT) "}\n\r"

static UDPSocket *g_udp_server;
static EthernetInterface g_eth;
static DigitalOut g_led(LED1);
static Serial g_pc(USBTX, USBRX);
// static WS2xxx g_rgb(PTD2, PIXEL_WIDTH, PIXEL_HEIGHT);
static WS2xxx g_rgb(PTC6, STRIP_LENGTH);
static const char g_id_string[] = SERVER_RESPONSE;

static void ActivityLedToggle(void) {
    /* blink LED to show activity */
    g_led = !g_led;
}

static void NetworkError(Socket *s, socket_error_t err) {
    (void) s;
    printf("NET: Socket Error: %s (%d)\r\n", socket_strerror(err), err);
    minar::Scheduler::stop();
}

static void NetworkReceive(Socket *s) {
    SocketAddr addr;
    int pos;
    uint16_t port;
    // uint8_t buffer[PIXEL_WIDTH*PIXEL_HEIGHT*3], *p;
    uint8_t buffer[STRIP_LENGTH*3], *p;
    size_t len = sizeof(buffer);

    /* Recieve the packet */
    socket_error_t err = s->recv_from(buffer, &len, &addr, &port);
    if (!s->error_check(err) && len) {

        g_pc.printf("NET: rx[%i]\r\n", len);

        /* send the packet */
        err = s->send_to(&g_id_string, sizeof(g_id_string), &addr,port);
        if (err != SOCKET_ERROR_NONE) {
            NetworkError(s, err);
        }

        /* update RGB buffer with received data */
        pos = 0;
        p = buffer;
        /* clear output buffer */
        g_rgb.clear();
        while(len>=3)
        {
            g_rgb.set( pos++,
                (((uint32_t)p[0]) << 16) |
                (((uint32_t)p[1]) <<  8) |
                (((uint32_t)p[2]) <<  0)
            );
            p += 3;
            len -= 3;
        }
        /* transmit RGB frame */
        g_rgb.send();
    }
}

static void NetworkInit(void){
    /* Initialise with DHCP, connect, and start up the stack */
    g_eth.init(IP,MASK,GATEWAY);
    g_eth.connect();

    socket_error_t err = lwipv4_socket_init();
    if (err) {
        g_pc.printf("ERROR: Failed to initialize socket stack (%d)\r\n", err);
        return;
    }
    g_pc.printf("NET: UDP Server IP Address is %s:%d\r\n", g_eth.getIPAddress(), UDP_SERVER_PORT);

    /* bind server socket */
    g_udp_server = new UDPSocket(SOCKET_STACK_LWIP_IPV4);
    g_udp_server->setOnError(UDPSocket::ErrorHandler_t(NetworkError));
    g_udp_server->open(SOCKET_AF_INET4);
    err = g_udp_server->bind("0.0.0.0", UDP_SERVER_PORT);
    g_udp_server->error_check(err);
    g_udp_server->setOnReadable(UDPSocket::ReadableHandler_t(NetworkReceive));

    g_pc.printf("NET: Waiting for packet...\r\n");
}

void app_start(int, char**){
    int i;

    /* set 115200 baud rate for stdout */
    g_pc.baud(115200);

    // while (1)
    // {
    //     /* set initial pixels */
        for (i = 0; i < STRIP_LENGTH; i++)
        {
            g_rgb.set(i, 0x0);
            // g_rgb.set(PIXEL_WIDTH-1-i, i, 0x00FF00);
        }
        /* transmit first RGB frame */
        g_rgb.send();
    //     wait(.5);

    //     for (i = 0; i < STRIP_LENGTH; i++)
    //     {
    //         g_rgb.set(i, 0x0);
    //         // g_rgb.set(PIXEL_WIDTH-1-i, i, 0x00FF00);
    //     }
    //     /* transmit first RGB frame */
    //     g_rgb.send();
    //     wait(.5);
    // }

    /* initialize network */
    NetworkInit();

    /* initialize activity led */
    minar::Scheduler::postCallback(ActivityLedToggle)
        .period(minar::milliseconds(1000))
        .tolerance(minar::milliseconds(10));
}

