# WS2812 RGB array example for FRDM-K64F

This repository contains example software for controlling a WS2812 RGB LED array via network.

Supported platforms:
- [Freescale FRDM-K64F](http://developer.mbed.org/platforms/FRDM-K64F/) ([GCC ARM Embedded toolchain](https://launchpad.net/gcc-arm-embedded)).

### How To

The following section explains how to build, flash and run the uVisor on OS X, Linux and Windows.

### Pre-Requisites

Please install the following:

* [yotta](https://github.com/ARMmbed/yotta). Please note that yotta has its own set of dependencies, listed in the [installation instructions](http://armmbed.github.io/yotta/#installing-on-windows).

#### Build

First, download the sources and navigate to the directory containing your source files:

```bash
git clone https://github.com/ARMmbed/example-mbedos-ws2812-rgb.git
cd example-mbedos-ws2812-rgb
```

yotta must know which platform (target) it is building to. So we declare the target, then build.

```bash
yotta target frdm-k64f-gcc
yotta build -r
```

for subsequent builds you only need to run 

```bash
yotta build -r
```
The resulting binary file will be located in
`./build/frdm-k64f-gcc/source/example-mbedos-ws2812-rgb.bin`

#### Flash

Connect your board to your computer USB port and simply drag & drop the binary file from the previous step into the `MBED` device listed in the file browser.

#### Run

Hit the reset button after flashing to start program execution. The application will be running right after you reset the processor. The red LED starts blinking with 100ms period.
