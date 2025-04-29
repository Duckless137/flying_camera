ARDUINO = arduino:avr:uno
PORT = /dev/ttyACM0


upload: build
	arduino-cli upload -p $(PORT) --fqbn $(ARDUINO) build

build:
	mkdir -p build
	arduino-cli $(PORT) --fqbn $(ARDUINO) --build-path build
	

