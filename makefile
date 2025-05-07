ARDUINO=arduino:avr:uno
PORT=/dev/ttyACM0

BIN_PATH=/usr/bin
HW_PATH=/usr/share/arduino/hardware/arduino/avr
TOOLS_PATH=/usr/share/arduino/hardware/tools

FLAGS_ASM=-c -g -x assembler-with-cpp -flto -MMD -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR
FLAGS_GPP=-c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I${HW_PATH}/cores/arduino -I${HW_PATH}/variants/standard -I./includes
FLAGS_GCC=-c -g -Os -w -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -I${HW_PATH}/cores/arduino -I${HW_PATH}/variants/standard -I./includes

CORES_DIR=${HW_PATH}/cores/arduino
AVR_GCC=${BIN_PATH}/avr-gcc
AVR_GPP=${BIN_PATH}/avr-g++
AVR_AR=${BIN_PATH}/avr-gcc-ar
AVR_OBJCPY=${BIN_PATH}/avr-objcopy


LIBS=$(shell ls libs/*)
LIBS_BIN=$(patsubst %,build/bin/%.o,$(LIBS))

TESTS=$(shell ls tests/*)
TESTS_BIN=$(patsubst %,build/bin/%.o,$(TESTS))

# I would prefer to just ls into this directory, but certain files are unnecessary and simply cannot be compiled.
CORES_CPP=hooks.c wiring.c wiring_pulse.c wiring_pulse.S wiring_digital.c wiring_shift.c wiring_analog.c WInterrupts.c Stream.cpp HardwareSerial.cpp HardwareSerial0.cpp HardwareSerial1.cpp HardwareSerial2.cpp HardwareSerial3.cpp CDC.cpp WString.cpp Print.cpp main.cpp WMath.cpp IPAddress.cpp USBCore.cpp Tone.cpp
CORES_BIN=$(patsubst %,build/bin/cores/%.o,${CORES_CPP})


SKETCH=flying_camera
SKETCH_OBJ=build/bin/target/$(SKETCH).o
SKETCH_OUT=build/out/$(SKETCH)
DEPS=build/bin/deps.a

.PHONY:

dirs:
	mkdir -p build/bin/cores
	mkdir -p build/bin/libs
	mkdir -p build/bin/tests
	mkdir -p build/bin/target
	mkdir -p build/tests
	mkdir -p build/out

# Cores building rules
build/bin/cores/%.S.o: $(CORES_DIR)/%.S
	${AVR_GCC} $(FLAGS_ASM) $< -o $@

build/bin/cores/%.c.o: $(CORES_DIR)/%.c
	${AVR_GCC} $(FLAGS_GCC) $< -o $@

build/bin/cores/%.cpp.o: $(CORES_DIR)/%.cpp
	${AVR_GPP} $(FLAGS_GPP) $< -o $@


# Library building rules
build/bin/%.c.o: %.c
	${AVR_GCC} $(FLAGS_GCC) $< -o $@

build/bin/%.cpp.o: %.cpp
	${AVR_GPP} $(FLAGS_GPP) $< -o $@


$(SKETCH_OBJ): $(SKETCH).cpp
	${AVR_GPP} $(FLAGS_GPP) $< -o $@

upload: build
	$(AVR_OBJCPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(SKETCH_OUT).elf $(SKETCH_OUT).eep 
	$(AVR_OBJCPY) -O ihex -R .eeprom $(SKETCH_OUT).elf $(SKETCH_OUT).hex 
	$(TOOLS_PATH)/avrdude -C$(TOOLS_PATH)/avrdude.conf -v -patmega328p -carduino -P$(PORT) -b115200 -D -Uflash:w:$(SKETCH_OUT).hex:i


$(DEPS): $(CORES_BIN) $(LIBS_BIN) $(SKETCH_OBJ)
	rm -f $(DEPS)
	${AVR_AR} rcs $(DEPS) $^

build: dirs $(DEPS)
	${AVR_GCC} -Os -Wl,-Map,avr.map,--gc-sections -mmcu=atmega328p -o $(SKETCH_OUT).elf $(SKETCH_OBJ) $(DEPS) -lm 

clean:
	rm -rf build
	# Yes, I am one of those people who clears their terminal every 0.2 seconds, how'd you know?
	clear

build-tests: dirs $(DEPS) $(TESTS_BIN)
