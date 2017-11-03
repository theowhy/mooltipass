
function(firmware_bundle)
	set(options)
	set(oneValueArgs STRING_FILE OUTPUT_NAME)
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_custom_command(OUTPUT ${ARG_OUTPUT_NAME} PRE_BUILD
		COMMENT "Create ${NAME} bundle"
		COMMAND python2 ./bundle.py -s "${ARG_STRING_FILE}" -o ${ARG_OUTPUT_NAME} ${ARG_SOURCES}
		SOURCES ${ARG_STRING_FILE}
		WORKING_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}"
		DEPENDS ${ARG_STRING_FILE}
		#BYPRODUCTS ${ARG_OUTPUT_NAME}
	)
endfunction()

add_subdirectory(bitmaps)

firmware_bundle(
	OUTPUT_NAME ${CMAKE_CURRENT_BINARY_DIR}/mini_bundle.img
	STRING_FILE ${CMAKE_CURRENT_SOURCE_DIR}/strings/strings_mini.txt
	SOURCES
		bitmaps/mini/00_mooltipass.img
		bitmaps/mini/03_approve.png
		bitmaps/mini/04_deny.png
		bitmaps/mini/05_no_card.png
		bitmaps/mini/06_pin_slot1.png
		bitmaps/mini/07_pin_slot2.png
		bitmaps/mini/08_pin_slot3.png
		bitmaps/mini/09_pin_slot4.png
		bitmaps/mini/10_scroll_bg.png
		bitmaps/mini/12_login_left_pane.png
		bitmaps/mini/13_zzz.png
		bitmaps/mini/14_mini_update.png
		${CMAKE_CURRENT_SOURCE_DIR}/15_space_filler.img
		${CMAKE_CURRENT_SOURCE_DIR}/16_space_filler.img
		${CMAKE_CURRENT_SOURCE_DIR}/17_space_filler.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/18_EN_US_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/19_FR_FR_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/20_ES_ES_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/21_DE_DE_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/22_ES_AR_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/23_EN_AU_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/24_FR_BE_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/25_PO_BR_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/26_EN_CA_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/27_CZ_CZ_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/28_DA_DK_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/29_FI_FI_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/30_HU_HU_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/31_IS_IS_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/32_IT_IT_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/33_NL_NL_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/34_NO_NO_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/35_PO_PO_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/36_MAC_UK_keyb_lut.img
#		${CMAKE_CURRENT_SOURCE_DIR}/lut/36_RO_RO_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/37_SL_SL_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/38_FRDE_CH_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/39_EN_UK_keyb_lut.img
		bitmaps/mini/40_lock_full.png
		bitmaps/mini/41_lock_empty.png
		bitmaps/mini/42_pac_full.png
		bitmaps/mini/43_pac_right.png
		bitmaps/mini/44_pac_right2.png
		bitmaps/mini/45_pac_bot.png
		bitmaps/mini/46_pac_bot2.png
		bitmaps/mini/47_pac_left.png
		bitmaps/mini/48_pac_left2.png
		bitmaps/mini/49_pac_up.png
		bitmaps/mini/50_pac_up2.png
		${CMAKE_CURRENT_SOURCE_DIR}/lut/51_CZ_QWERTY_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/52_EN_DV_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/53_FR_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/54_FR_CH_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/55_DE_CH_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/56_DE_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/57_US_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/58_space_filler.img
		bitmaps/mini/59_tutorial_1.png
		bitmaps/mini/60_tutorial_2.png
		bitmaps/mini/61_tutorial_3.png
		bitmaps/mini/62_tutorial_4.png
		bitmaps/mini/63_tutorial_5.png
		bitmaps/mini/64_main_lock.png
		bitmaps/mini/65_main_lock_t1.png
		bitmaps/mini/66_main_lock_t2.png
		bitmaps/mini/67_main_lock_t3.png
		bitmaps/mini/68_main_login.png
		bitmaps/mini/69_main_login_t1.png
		bitmaps/mini/70_main_login_t2.png
		bitmaps/mini/71_main_login_t3.png
		bitmaps/mini/72_main_favorites.png
		bitmaps/mini/73_main_favorites_t1.png
		bitmaps/mini/74_main_favorites_t2.png
		bitmaps/mini/75_main_favorites_t3.png
		bitmaps/mini/76_main_settings.png
		bitmaps/mini/77_main_settings_t1.png
		bitmaps/mini/78_main_settings_t2.png
		bitmaps/mini/79_main_settings_t3.png
		bitmaps/mini/80_settings_change_pin.png
		bitmaps/mini/81_settings_change_pin_t1.png
		bitmaps/mini/82_settings_change_pin_t2.png
		bitmaps/mini/83_settings_change_pin_t3.png
		bitmaps/mini/84_settings_clone.png
		bitmaps/mini/85_settings_clone_t1.png
		bitmaps/mini/86_settings_clone_t2.png
		bitmaps/mini/87_settings_clone_t3.png
		bitmaps/mini/88_settings_home.png
		bitmaps/mini/89_settings_home_t1.png
		bitmaps/mini/90_settings_home_t2.png
		bitmaps/mini/91_settings_home_t3.png
		bitmaps/mini/92_settings_erase.png
		bitmaps/mini/93_settings_erase_t1.png
		bitmaps/mini/94_settings_erase_t2.png
		bitmaps/mini/95_settings_erase_t3.png
		${CMAKE_CURRENT_SOURCE_DIR}/96_space_filler.img
)

firmware_bundle(
	OUTPUT_NAME ${CMAKE_CURRENT_BINARY_DIR}/standard_bundle.img
	STRING_FILE ${CMAKE_CURRENT_SOURCE_DIR}/strings/strings_standard.txt
	SOURCES
		00_Mooltipass.png
		01_login_select.png
		02_login_right_arrow.png
		03_yes_no_frame.png
		04_tick.png
		05_cross.png
		06_information.png
		08_insert_smartcard.png
		09_main_screen.png
		10_settings_screen.png
		11_left_arrow.png
		12_right_arrow.png
		13_pin_lines.png
		14_sleep_symbol.png
		15_find_login.png
		40_yes_no_frame_int_l.png
		41_yes_no_frame_int_r.png
		42_zzz_symbol_locked.png
		43_trailer.img
		43_tutorial1.png
		44_tutorial2.png
		45_tutorial3.png
		46_tutorial4.png
		47_tutorial5.png
		48_tutorial6.png
		49_egg.png
		50_blank.png
		58_trailer.img
)

add_custom_target(bundles
	SOURCES
		${CMAKE_CURRENT_BINARY_DIR}/mini_bundle.img
		${CMAKE_CURRENT_BINARY_DIR}/standard_bundle.img
)
