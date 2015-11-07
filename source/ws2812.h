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
#include "fsl_clock_manager.h"
#include "fsl_dspi_hal.h"

class WS2812 : protected mbed::SPI {
    protected:
        int m_width, m_height;
        int *m_buffer;
        dspi_command_config_t m_cmd;

        void init(void);
        void tx(uint32_t value);
        void tx_raw(uint16_t value);
        void tx_reset(void);
        void frequency(int hz, dspi_ctar_selection_t ctar);

    public:
        WS2812(PinName pin);
        WS2812(PinName pin, int width);
        WS2812(PinName pin, int width, int height);

        void send(void);
};

#endif/*__WS2812__*/