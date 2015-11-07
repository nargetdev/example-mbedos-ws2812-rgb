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

#include <stdint.h>
#include "mbed-drivers/mbed_assert.h"
#include "fsl_clock_manager.h"
#include "fsl_dspi_hal.h"

#include "ws2812.h"

WS2812::WS2812(PinName pin) :
    SPI(pin, NC, NC),
    m_width(1),
    m_height(1)
{
    init();
};

WS2812::WS2812(PinName pin, int width) :
    SPI(pin, NC, NC),
    m_width(width),
    m_height(1)
{
    init();
};

WS2812::WS2812(PinName pin, int width, int height) :
    SPI(pin, NC, NC),
    m_width(width),
    m_height(height)
{
    init();
};

void WS2812::init(void)
{
    if(m_width<1)
        m_width = 1;
    if(m_height<1)
        m_height = 1;

    /* allocate output buffer */
    m_buffer = new int [m_width*m_height];

    /* configure SPI settings */
    format(16, 0, SPI_MSB);
    frequency(12600000);
}

void WS2812::tx(uint32_t value)
{
    int i;
    dspi_command_config_t cmd;

    /* prepare SPI command */
    memset(&cmd, 0, sizeof(cmd));
    cmd.isEndOfQueue = false;
    cmd.isChipSelectContinuous = true;

    for(i=0; i<24; i++)
    {
        /* wait for TX buffer */
        while(!DSPI_HAL_GetStatusFlag(_spi.spi.address, kDspiTxFifoFillRequest));
        DSPI_HAL_WriteDataMastermode(_spi.spi.address, &cmd, (value & 0x800000) ? 0xF800 : 0xFF80);
        DSPI_HAL_ClearStatusFlag(_spi.spi.address, kDspiTxFifoFillRequest);

        value <<= 1;
    }
}

void WS2812::send(void)
{
    tx(0xFF0000);
    tx(0x00FF00);
    tx(0x0000FF);
}
