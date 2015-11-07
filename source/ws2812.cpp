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

    /* update SPI cmd */
    memset(&m_cmd, 0, sizeof(m_cmd));
    m_cmd.isEndOfQueue = false;
    m_cmd.isChipSelectContinuous = true;
    /* used data CTAR by default */
    m_cmd.whichCtar = kDspiCtar0;

    /* configure SPI settings */
    format(16, 0, SPI_MSB);
    /* data  CTAR */
    frequency(12600000, kDspiCtar0);
    /* reset CTAR */
    frequency(300000, kDspiCtar1);
}

void WS2812::tx_raw(uint16_t value)
{
    /* wait for TX buffer */
    while(!DSPI_HAL_GetStatusFlag(_spi.spi.address, kDspiTxFifoFillRequest));
    DSPI_HAL_WriteDataMastermode(_spi.spi.address, &m_cmd, value);
    DSPI_HAL_ClearStatusFlag(_spi.spi.address, kDspiTxFifoFillRequest);
}

void WS2812::frequency(int hz, dspi_ctar_selection_t ctar)
{
    uint32_t busClock;

    /* update HZ only for CTAR0 */
    if(ctar == kDspiCtar0)
        _hz = hz;

    CLOCK_SYS_GetFreq(kBusClock, &busClock);
    DSPI_HAL_SetBaudRate(_spi.spi.address, ctar, (uint32_t)hz, busClock);
}

void WS2812::tx(uint32_t value)
{
    int i;

    /* transmit RGB sequence for one WD2812B pixel */
    for(i=0; i<24; i++)
    {
        tx_raw((value & 0x800000) ? 0xFF80 : 0xF800 );
        value <<= 1;
    }
}

void WS2812::tx_reset(void)
{
    /* send reset CTAR */
    m_cmd.whichCtar = kDspiCtar1;
    tx_raw(0x00);
    /* switch back to data CTAR */
    m_cmd.whichCtar = kDspiCtar0;
}


void WS2812::send(void)
{
    tx_reset();

    tx(0xFF0000);
    tx(0x00FF00);
    tx(0x0000FF);

    tx_reset();
}
