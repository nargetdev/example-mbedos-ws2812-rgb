TARGET:=frdm-k64f-gcc
PROJECT:=example-mbedos-ws2812-rgb

#
# derived settings
#
TARGET_BIN:=build/$(TARGET)/source/$(PROJECT).bin
TARGET_MOUNT:=/run/media/$(USER)/MBED

all: release

release: images
	yt build -r
	ls -l $(TARGET_BIN)
	cp $(TARGET_BIN) $(TARGET_MOUNT)/firmware.bin
	sync

.rgb.png:
	convert $^ $@

images: docs/smiley-happy.rgb docs/smiley-sad.rgb

docs/smiley-happy.rgb: docs/smiley-happy.png
	convert $^ $@

docs/smiley-sad.rgb: docs/smiley-sad.png
	convert $^ $@

clean:
	yt target $(TARGET)
	yt clean

clean_all: clean
	rm -f docs/*.rgb
