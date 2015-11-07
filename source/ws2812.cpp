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

#include "ws2812.h"

WS2812::WS2812(PinName pin) :
    m_width(1),
    m_height(1),
    m_spi(pin, NC, NC)
{
    init();
};

WS2812::WS2812(PinName pin, int width) :
    m_width(width),
    m_height(1),
    m_spi(pin, NC, NC)
{
    init();
};

WS2812::WS2812(PinName pin, int width, int height) :
    m_width(width),
    m_height(height),
    m_spi(pin, NC, NC)
{
    init();
};

void WS2812::init(void)
{
    MBED_ASSERT(m_width>0 && m_height>0);

    m_buffer = new int [m_width*m_height];
}

