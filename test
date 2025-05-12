#!/bin/bash
ARDUINO=arduino:avr:uno
PORT=/dev/ttyACM0

BIN_PATH=/usr/bin
TOOLS_PATH=/usr/share/arduino/hardware/tools

AVR_GCC=$BIN_PATH/avr-gcc
AVR_OBJCPY=$BIN_PATH/avr-objcopy

DEPS=build/bin/deps.a

# I ain't reading all that
make build-tests > /dev/null

function runtest {
    test_obj=build/bin/tests/$1.o
    test_file=build/tests/$1

    $AVR_GCC -Os -Wl,-Map,avr.map,--gc-sections -mmcu=atmega328p -o $test_file.elf $test_obj $DEPS -lm 
	$AVR_OBJCPY -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $test_file.elf $test_file.eep 
	$AVR_OBJCPY -O ihex -R .eeprom $test_file.elf $test_file.hex 
	$TOOLS_PATH/avrdude -C$TOOLS_PATH/avrdude.conf -v -patmega328p -carduino -P$PORT -b115200 -D -Uflash:w:$test_file.hex:i
}

if [[ ! -n "$1" ]]; then
    echo "Must specify a test name. Options:"
    for file in $(ls tests); do
        printf "\t$file\n"
    done
    exit 2
elif [[ "$1" == "all" ]]; then
    tests_list=$(ls tests)
    tests_list_len=$(echo "$tests_list" | wc -w)
    for file in $tests_list; do
        runtest $file
        echo "Running test $file... (1/$tests_list_len)"
        read -s -n 1 -p "Press any key to continue to next... "
    done
else
    if [ -f "tests/$1" ]; then
        runtest $1
    else
        echo "No test named \"$1\""
        exit 3
    fi
fi

# That was... fun to write.
# How HIGH were the people who made
# Bash's syntax? I ask this every time
# I write a bash script...