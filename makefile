ARDUINO = arduino:avr:uno
PORT = /dev/ttyACM0

LIBS = $(shell ls src/*.c)
OBJS = $(patsubst %, $(shell pwd)/build/sketch/%.o, $(LIBS))

.PHONY: 

upload: build
	arduino-cli upload -p $(PORT) --fqbn $(ARDUINO) build

build: .PHONY
	mkdir -p build
	arduino-cli compile -p $(PORT) --fqbn $(ARDUINO) --build-path build --libraries libs --build-property "build.extra_flags=-I./includes"

	

