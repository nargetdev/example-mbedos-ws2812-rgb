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
 
#ifndef __WS2812__
#define __WS2812__

#include "mbed-drivers/SPI.h"

class WS2812 {
    protected:
        int m_width, m_height;
        int *m_buffer;
        mbed::SPI m_spi;

        void init(void);

    public:
        WS2812(PinName pin);
        WS2812(PinName pin, int width);
        WS2812(PinName pin, int width, int height);
};

#endif/*__WS2812__*/
