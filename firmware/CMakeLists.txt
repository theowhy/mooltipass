cmake_minimum_required(VERSION 3.0)

include_directories(
	src
	src/GUI
	src/CARD
	src/FLASH
	src/USB
	src/SPI
	src/OLEDMP
	src/UTILS
	src/AES
	src/NODEMGMT
	src/RNG
	src/PWM
	src/TOUCH
	src/LOGIC
	src/OLEDMINI
	src/MINI
	/usr/avr/include
)

option(DEBUG "Build in debug mode" OFF)

if(DEBUG)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -g")
else()
	add_definitions(-DNDEBUG)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
endif()

macro(info_dump TARGET)
	add_custom_command(TARGET ${TARGET}
		POST_BUILD
		COMMAND avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $<TARGET_FILE:${TARGET}> $<TARGET_FILE_NAME:${TARGET}>.hex
		COMMAND avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex $<TARGET_FILE:${TARGET}> $<TARGET_FILE_NAME:${TARGET}>.eep
	)
	add_custom_command(TARGET ${TARGET}
		POST_BUILD
		COMMAND avr-size  --format=avr --mcu=atmega32u4 $<TARGET_FILE:${TARGET}>
	)
endmacro()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -fuse-linker-plugin -Wall -mmcu=atmega32u4 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -Werror -std=gnu99 -mcall-prologues -fno-inline-small-functions -mrelax -MD -MP")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto -fuse-linker-plugin -Wl,--gc-sections -mrelax -Wl,--start-group -Wl,-lm -Wl,--end-group -Wl,-Map=mooltipass.map")

add_definitions(-DF_CPU=16000000UL -DF_USB=16000000UL -DNDEBUG)
add_executable(mooltipass_bootloader
	src/bootloader_main.c
	src/FLASH/flash_mem.c
	src/SPI/spi.c
	src/AES/aes256_ctr.c
	src/AES/aes.c
)
info_dump(mooltipass_bootloader)

add_executable(mooltipass
	src/mooltipass.c
	src/stack.c
	src/interrupts.c
	src/functional_testing.c
	src/tests.c
	src/timer_manager.c
	src/GUI/mini_gui_credentials_functions.c
	src/GUI/mini_gui_basic_functions.c
	src/GUI/standard_gui_credentials_functions.c
	src/GUI/mini_gui_smartcard_functions.c
	src/GUI/mini_gui_screen_functions.c
	src/GUI/standard_gui_basic_functions.c
	src/GUI/standard_gui_smartcard_functions.c
	src/GUI/standard_gui_screen_functions.c
	src/GUI/mini_gui_pin_functions.c
	src/GUI/standard_gui_pin_functions.c
	src/CARD/smartcard.c
	src/CARD/smart_card_higher_level_functions.c
	src/FLASH/flash_test.c
	src/FLASH/flash_mem.c
	src/USB/hid_defines.c
	src/USB/usb_cmd_parser.c
	src/USB/usb.c
	src/USB/usb_descriptors.c
	src/SPI/spi.c
	src/OLEDMP/anim.c
	src/OLEDMP/oledmp.c
	src/OLEDMP/bitstream.c
	src/UTILS/delays.c
	src/UTILS/utils.c
	src/AES/aes256_ctr.c
	src/AES/aes256_nessie_test.c
	src/AES/aes256_ctr_test.c
	src/AES/aes.c
	src/NODEMGMT/node_mgmt.c
	src/RNG/rng.c
	src/PWM/pwm.c
	src/TOUCH/touch_higher_level_functions.c
	src/TOUCH/touch.c
	src/LOGIC/logic_eeprom.c
	src/LOGIC/logic_fwflash_storage.c
	src/LOGIC/logic_aes_and_comms.c
	src/LOGIC/logic_smartcard.c
	src/OLEDMINI/oledmini.c
	src/OLEDMINI/bitstreammini.c
	src/MINI/mini_leds.c
	src/MINI/mini_inputs.c
)

info_dump(mooltipass)
