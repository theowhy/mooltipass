cmake_minimum_required(VERSION 3.9)
# This file should not be called alone.
# It requires the firmware/CMakelists.txt file

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../tool)
message(STATUS "::${CMAKE_MODULE_PATH}")
include(bundle)

set(bitmap_images
BITMAP = $(wildcard [0-9]*.png)
BITMAP_IMAGES = $(BITMAP_PNG:.png=.img)

FONT_IMAGES = $(wildcard [0-9]*font*.img)
LUT_IMAGES = $(wildcard [0-9]*lut*.img)


image_convert(standard
	SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/0_Mooltipass.png
		${CMAKE_CURRENT_SOURCE_DIR}/1_login_select.png
		${CMAKE_CURRENT_SOURCE_DIR}/2_login_right_arrow.png
		${CMAKE_CURRENT_SOURCE_DIR}/3_yes_no_frame.png
		${CMAKE_CURRENT_SOURCE_DIR}/4_tick.png
		${CMAKE_CURRENT_SOURCE_DIR}/5_cross.png
		${CMAKE_CURRENT_SOURCE_DIR}/6_information.png
		${CMAKE_CURRENT_SOURCE_DIR}/8_insert_smartcard.png
		${CMAKE_CURRENT_SOURCE_DIR}/9_main_screen.png
		${CMAKE_CURRENT_SOURCE_DIR}/10_settings_screen.png
		${CMAKE_CURRENT_SOURCE_DIR}/11_left_arrow.png
		${CMAKE_CURRENT_SOURCE_DIR}/12_right_arrow.png
		${CMAKE_CURRENT_SOURCE_DIR}/13_pin_lines.png
		${CMAKE_CURRENT_SOURCE_DIR}/14_sleep_symbol.png
		${CMAKE_CURRENT_SOURCE_DIR}/15_find_login.png
		${CMAKE_CURRENT_SOURCE_DIR}/40_yes_no_frame_int_l.png
		${CMAKE_CURRENT_SOURCE_DIR}/41_yes_no_frame_int_r.png
		${CMAKE_CURRENT_SOURCE_DIR}/42_zzz_symbol_locked.png
		${CMAKE_CURRENT_SOURCE_DIR}/43_tutorial1.png
		${CMAKE_CURRENT_SOURCE_DIR}/44_tutorial2.png
		${CMAKE_CURRENT_SOURCE_DIR}/45_tutorial3.png
		${CMAKE_CURRENT_SOURCE_DIR}/46_tutorial4.png
		${CMAKE_CURRENT_SOURCE_DIR}/47_tutorial5.png
		${CMAKE_CURRENT_SOURCE_DIR}/48_tutorial6.png
		${CMAKE_CURRENT_SOURCE_DIR}/49_egg.png
		${CMAKE_CURRENT_SOURCE_DIR}/50_blank.png
)

firmware_bundle(standard
	OUTPUT_NAME ${CMAKE_CURRENT_SOURCE_DIR}/bundle.img
	STRING_FILE ${CMAKE_CURRENT_SOURCE_DIR}/fw_strings.txt
	SOURCES
		0_Mooltipass.png
		1_login_select.png
		2_login_right_arrow.png
		3_yes_no_frame.png
		4_tick.png
		5_cross.png
		6_information.png
		8_insert_smartcard.png
		9_main_screen.png
		10_settings_screen.png
		11_left_arrow.png
		12_right_arrow.png
		13_pin_lines.png
		14_sleep_symbol.png
		15_find_login.png



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
		${CMAKE_CURRENT_SOURCE_DIR}/lut/51_CZ_QWERTY_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/52_EN_DV_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/53_FR_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/54_FR_CH_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/55_DE_CH_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/56_DE_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/lut/57_US_MAC_keyb_lut.img
		${CMAKE_CURRENT_SOURCE_DIR}/58_space_filler.img
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
		${CMAKE_CURRENT_SOURCE_DIR}/96_space_filler.img

)
