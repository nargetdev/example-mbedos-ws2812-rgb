TARGET:=frdm-k64f-gcc
PROJECT:=example-mbedos-ws2812-rgb

#
# derived settings
#
TARGET_BIN:=build/$(TARGET)/source/$(PROJECT).bin
TARGET_MOUNT:=/run/media/$(USER)/MBED

all: release

release:
	yt build -r
	ls -l $(TARGET_BIN)
	cp $(TARGET_BIN) $(TARGET_MOUNT)/firmware.bin
	sync

clean:
	yt target $(TARGET)
	yt clean