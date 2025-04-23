upload: build
	arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno

build:
	arduino-cli compile --fqbn arduino:avr:uno
	

