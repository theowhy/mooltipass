cmake_minimum_required(VERSION 3.9)
# This file should not be called alone.
# It requires the firmware/CMakelists.txt file

function(image_convert NAME)
	set(options)
	set(oneValueArgs)
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	add_custom_target(image_convert_mini_all COMMENT "Build all images for ${NAME}")
	foreach(SOURCE IN ITEMS ${ARG_SOURCES})
		get_filename_component(SOURCE_NAME "${SOURCE}" NAME_WE)
		add_custom_target(image_convert_${NAME}_${SOURCE_NAME}
			COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/imagetoimg.py -q -o "${NAME}/${SOURCE_NAME}.img" "${SOURCE}"

			DEPENDS "${SOURCE}"
			SOURCES "${SOURCE}"
			COMMENT "Building ${SOURCE}"
			BYPRODUCTS "${NAME}//${SOURCE_NAME}.img"
		)

		add_dependencies(image_convert_mini_all image_convert_${NAME}_${SOURCE_NAME})
	endforeach()

endfunction()

function(firmware_bundle NAME)
	set(options)
	set(oneValueArgs STRING_FILE OUTPUT_NAME)
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_custom_target(firmware_bundle_${NAME} COMMENT "Create ${NAME} bundle"
		COMMAND python2 ./bundle.py -s "${ARG_STRING_FILE}" -o ${ARG_OUTPUT_NAME} ${ARG_SOURCES}
		SOURCES ${ARG_STRING_FILE}
		WORKING_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}"
		DEPENDS ${ARG_STRING_FILE}
		BYPRODUCTS ${ARG_OUTPUT_NAME}
	)
endfunction()

image_convert(mini
	SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/0_mooltipass.png
		${CMAKE_CURRENT_SOURCE_DIR}/3_approve.png
		${CMAKE_CURRENT_SOURCE_DIR}/4_deny.png
		${CMAKE_CURRENT_SOURCE_DIR}/5_no_card.png
		${CMAKE_CURRENT_SOURCE_DIR}/6_pin_slot1.png
		${CMAKE_CURRENT_SOURCE_DIR}/7_pin_slot2.png
		${CMAKE_CURRENT_SOURCE_DIR}/8_pin_slot3.png
		${CMAKE_CURRENT_SOURCE_DIR}/9_pin_slot4.png
		${CMAKE_CURRENT_SOURCE_DIR}/10_scroll_bg.png
		${CMAKE_CURRENT_SOURCE_DIR}/12_login_left_pane.png
		${CMAKE_CURRENT_SOURCE_DIR}/13_zzz.png
		${CMAKE_CURRENT_SOURCE_DIR}/14_mini_update.png
		${CMAKE_CURRENT_SOURCE_DIR}/40_lock_full.png
		${CMAKE_CURRENT_SOURCE_DIR}/41_lock_empty.png
		${CMAKE_CURRENT_SOURCE_DIR}/42_pac_full.png
		${CMAKE_CURRENT_SOURCE_DIR}/43_pac_right.png
		${CMAKE_CURRENT_SOURCE_DIR}/44_pac_right2.png
		${CMAKE_CURRENT_SOURCE_DIR}/45_pac_bot.png
		${CMAKE_CURRENT_SOURCE_DIR}/46_pac_bot2.png
		${CMAKE_CURRENT_SOURCE_DIR}/47_pac_left.png
		${CMAKE_CURRENT_SOURCE_DIR}/48_pac_left2.png
		${CMAKE_CURRENT_SOURCE_DIR}/49_pac_up.png
		${CMAKE_CURRENT_SOURCE_DIR}/50_pac_up2.png
		${CMAKE_CURRENT_SOURCE_DIR}/59_tutorial_1.png
		${CMAKE_CURRENT_SOURCE_DIR}/60_tutorial_2.png
		${CMAKE_CURRENT_SOURCE_DIR}/61_tutorial_3.png
		${CMAKE_CURRENT_SOURCE_DIR}/62_tutorial_4.png
		${CMAKE_CURRENT_SOURCE_DIR}/63_tutorial_5.png
		${CMAKE_CURRENT_SOURCE_DIR}/64_main_lock.png
		${CMAKE_CURRENT_SOURCE_DIR}/65_main_lock_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/66_main_lock_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/67_main_lock_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/68_main_login.png
		${CMAKE_CURRENT_SOURCE_DIR}/69_main_login_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/70_main_login_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/71_main_login_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/72_main_favorites.png
		${CMAKE_CURRENT_SOURCE_DIR}/73_main_favorites_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/74_main_favorites_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/75_main_favorites_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/76_main_settings.png
		${CMAKE_CURRENT_SOURCE_DIR}/77_main_settings_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/78_main_settings_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/79_main_settings_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/80_settings_change_pin.png
		${CMAKE_CURRENT_SOURCE_DIR}/81_settings_change_pin_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/82_settings_change_pin_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/83_settings_change_pin_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/84_settings_clone.png
		${CMAKE_CURRENT_SOURCE_DIR}/85_settings_clone_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/86_settings_clone_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/87_settings_clone_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/88_settings_home.png
		${CMAKE_CURRENT_SOURCE_DIR}/89_settings_home_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/90_settings_home_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/91_settings_home_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/92_settings_erase.png
		${CMAKE_CURRENT_SOURCE_DIR}/93_settings_erase_t1.png
		${CMAKE_CURRENT_SOURCE_DIR}/94_settings_erase_t2.png
		${CMAKE_CURRENT_SOURCE_DIR}/95_settings_erase_t3.png
)

firmware_bundle(mini
	OUTPUT_NAME ${CMAKE_CURRENT_SOURCE_DIR}/bundle.img
	STRING_FILE ${CMAKE_CURRENT_SOURCE_DIR}/fw_strings.txt
	SOURCES
		mini/0_mooltipass.png
		mini/3_approve.png
		mini/4_deny.png
		mini/5_no_card.png
		mini/6_pin_slot1.png
		mini/7_pin_slot2.png
		mini/8_pin_slot3.png
		mini/9_pin_slot4.png
		mini/10_scroll_bg.png
		mini/12_login_left_pane.png
		mini/13_zzz.png
		mini/14_mini_update.png
		mini/40_lock_full.png
		mini/41_lock_empty.png
		mini/42_pac_full.png
		mini/43_pac_right.png
		mini/44_pac_right2.png
		mini/45_pac_bot.png
		mini/46_pac_bot2.png
		mini/47_pac_left.png
		mini/48_pac_left2.png
		mini/49_pac_up.png
		mini/50_pac_up2.png
		mini/59_tutorial_1.png
		mini/60_tutorial_2.png
		mini/61_tutorial_3.png
		mini/62_tutorial_4.png
		mini/63_tutorial_5.png
		mini/64_main_lock.png
		mini/65_main_lock_t1.png
		mini/66_main_lock_t2.png
		mini/67_main_lock_t3.png
		mini/68_main_login.png
		mini/69_main_login_t1.png
		mini/70_main_login_t2.png
		mini/71_main_login_t3.png
		mini/72_main_favorites.png
		mini/73_main_favorites_t1.png
		mini/74_main_favorites_t2.png
		mini/75_main_favorites_t3.png
		mini/76_main_settings.png
		mini/77_main_settings_t1.png
		mini/78_main_settings_t2.png
		mini/79_main_settings_t3.png
		mini/80_settings_change_pin.png
		mini/81_settings_change_pin_t1.png
		mini/82_settings_change_pin_t2.png
		mini/83_settings_change_pin_t3.png
		mini/84_settings_clone.png
		mini/85_settings_clone_t1.png
		mini/86_settings_clone_t2.png
		mini/87_settings_clone_t3.png
		mini/88_settings_home.png
		mini/89_settings_home_t1.png
		mini/90_settings_home_t2.png
		mini/91_settings_home_t3.png
		mini/92_settings_erase.png
		mini/93_settings_erase_t1.png
		mini/94_settings_erase_t2.png
		mini/95_settings_erase_t3.png
}
