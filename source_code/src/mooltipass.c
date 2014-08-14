/* CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at src/license_cddl-1.0.txt
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at src/license_cddl-1.0.txt
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*! \file   mooltipass.c
 *  \brief  main file
 *  Copyright [2014] [Mathieu Stephan]
 */
#include <util/atomic.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "smart_card_higher_level_functions.h"
#include "touch_higher_level_functions.h"
#include "gui_smartcard_functions.h"
#include "gui_screen_functions.h"
#include "gui_basic_functions.h"
#include "eeprom_addresses.h"
#include "watchdog_driver.h"
#include "usb_cmd_parser.h"
#include "timer_manager.h"
#include "userhandling.h"
#include "mooltipass.h"
#include "interrupts.h"
#include "smartcard.h"
#include "flash_mem.h"
#include "node_mgmt.h"
#include "node_test.h"
#include "defines.h"
#include "entropy.h"
#include "oledmp.h"
#include "utils.h"
#include "tests.h"
#include "touch.h"
#include "anim.h"
#include "spi.h"
#include "pwm.h"
#include "usb.h"

// Define the bootloader function
bootloader_f_ptr_type start_bootloader = (bootloader_f_ptr_type)0x3800;
// Flag to inform if the caps lock timer is armed
volatile uint8_t wasCapsLockTimerArmed = FALSE;


/*! \fn     disable_jtag(void)
*   \brief  Disable the JTAG module
*/
void disable_jtag(void)
{
    unsigned char temp;

    temp = MCUCR;
    temp |= (1<<JTD);
    MCUCR = temp;
    MCUCR = temp;
}

/*! \fn     main(void)
*   \brief  Main function
*/
int main(void)
{
    uint8_t usb_buffer[RAWHID_TX_SIZE];
    RET_TYPE flash_init_result;
    RET_TYPE touch_init_result;
    RET_TYPE card_detect_ret;
    
    // Check if we were resetted and want to go to the bootloader
    if (eeprom_read_word((uint16_t*)EEP_BOOTKEY_ADDR) == BOOTLOADER_BOOTKEY)
    {
        // Disable WDT
        wdt_reset();
        wdt_clear_flag();
        wdt_change_enable();
        wdt_stop();
        // Store correct bootkey
        eeprom_write_word((uint16_t*)EEP_BOOTKEY_ADDR, CORRECT_BOOTKEY);
        // Jump to bootloader
        start_bootloader();
    }

    /* Check if a card is inserted in the Mooltipass to go to the bootloader */
    #ifdef AVR_BOOTLOADER_PROGRAMMING
        disable_jtag();                 // Disable JTAG to gain access to pins
        DDR_SC_DET &= ~(1 << PORTID_SC_DET);
        PORT_SC_DET |= (1 << PORTID_SC_DET);
        _delay_ms(100);    
        #if defined(HARDWARE_V1)
        if (PIN_SC_DET & (1 << PORTID_SC_DET))
        #elif defined(HARDWARE_OLIVIER_V1)
        if (!(PIN_SC_DET & (1 << PORTID_SC_DET)))
        #endif
        {
            start_bootloader();
        }
    #endif

    CPU_PRESCALE(0);                    // Set for 16MHz clock
    _delay_ms(500);                     // Let the power settle
    disable_jtag();                     // Disable JTAG to gain access to pins
    initPortSMC();                      // Initialize smart card port
    initPwm();                          // Initialize PWM controller
    initIRQ();                          // Initialize interrupts    
    initUsb();                          // Initialize USB controller
    initI2cPort();                      // Initialize I2C interface
    entropyInit();                      // Initialize avrentropy library
    while(!isUsbConfigured());          // Wait for host to set configuration
    spiUsartBegin(SPI_RATE_8_MHZ);      // Start USART SPI at 8MHz

    // Launch the before flash initialization tests
    #ifdef TESTS_ENABLED
        beforeFlashInitTests();
    #endif
    
    // Check if we can initialize the Flash memory
    flash_init_result = initFlash();
    
    // Launch the after flash initialization tests
    #ifdef TESTS_ENABLED
        afterFlashInitTests();
    #endif
    
    // Set up OLED now that USB is receiving full 500mA.
    oledBegin(FONT_DEFAULT);

    // OLED screen is reversed on Olivier's design
    #ifdef HARDWARE_OLIVIER_V1
        oledSetRemap(OLED_REMAP_NIBBLES|OLED_REMAP_COL_ADDR);
    #endif
    
    oledSetColour(15);
    oledSetBackground(0);
    oledSetContrast(0xFF);
    oledSetScrollSpeed(3);
    oledWriteActiveBuffer();
    
    // First time initializations
    if (eeprom_read_word((uint16_t*)EEP_BOOTKEY_ADDR) != CORRECT_BOOTKEY)
    {
        // Erase everything non graphic in flash
        eraseFlashUsersContents();
        // Erase # of cards and # of users
        firstTimeUserHandlingInit();
        // Set bootloader password bool to FALSE
        eeprom_write_byte((uint8_t*)EEP_BOOT_PWD_SET, FALSE);
        // Store correct bootkey
        eeprom_write_word((uint16_t*)EEP_BOOTKEY_ADDR, CORRECT_BOOTKEY);
    }
    
    // Check if we can initialize the touch sensing element
    touch_init_result = initTouchSensing();
    
    // Launch the after touch initialization tests
    #ifdef TESTS_ENABLED
        afterTouchInitTests();
    #endif
    
    // Test procedure to check that all HW is working
    //#define HW_TEST_PROC
    #ifdef HW_TEST_PROC
        oledSetXY(0,0);
        RET_TYPE temp_rettype;    
        if (flash_init_result == RETURN_OK)
        {
            printf_P(PSTR("FLASH OK\r\n"));
        } 
        else
        {
            printf_P(PSTR("PB FLASH\r\n"));
        }
        if (touch_init_result == RETURN_OK)
        {
            printf_P(PSTR("TOUCH OK\r\n"));            
        } 
        else
        {
            printf_P(PSTR("PB TOUCH\r\n"));
        }
        printf_P(PSTR("Bring hand close, touch left, wheel, right\r\n"));
        while(!(touchDetectionRoutine() & RETURN_PROX_DETECTION));
        printf_P(PSTR("Det, "));
        activateGuardKey();
        while(!(touchDetectionRoutine() & RETURN_LEFT_PRESSED));
        printf_P(PSTR("left, "));
        while(!(touchDetectionRoutine() & RETURN_WHEEL_PRESSED));
        printf_P(PSTR("wheel, "));
        while(!(touchDetectionRoutine() & RETURN_RIGHT_PRESSED));
        printf_P(PSTR("right!\r\n"));
        printf_P(PSTR("Insert card\r\n"));
        while(isCardPlugged() != RETURN_JDETECT);
        temp_rettype = cardDetectedRoutine();
        if ((temp_rettype == RETURN_MOOLTIPASS_BLANK) || (temp_rettype == RETURN_MOOLTIPASS_USER))
        {
            printf_P(PSTR("CARD OK\r\n"));
        } 
        else
        {
            printf_P(PSTR("PB CARD\r\n"));
        }
        printf_P(PSTR("Check LEDs!\r\n"));
        switchOnButtonWheelLeds();
        setPwmDc(MAX_PWM_VAL);
        if ((flash_init_result == RETURN_OK) && (touch_init_result == RETURN_OK) && ((temp_rettype == RETURN_MOOLTIPASS_BLANK) || (temp_rettype == RETURN_MOOLTIPASS_USER)))
        {
            printf_P(PSTR("---- TEST OK !!! ----\r\n"));
        }
        else
        {
            printf_P(PSTR("---- TEST NOT OK ----\r\n"));
        }
        while(1);
    #endif
    
    // Stop the Mooltipass if we can't communicate with the flash or the touch interface
    #if defined(HARDWARE_OLIVIER_V1) && !defined(V2_DEVELOPERS_BOTPCB_BOOTLOADER_SETUP)
        while ((flash_init_result != RETURN_OK) && (touch_init_result != RETURN_OK));
    #endif

    // Write inactive buffer & default bitmap
    oledWriteInactiveBuffer();
    oledBitmapDrawFlash(0, 0, BITMAP_HAD, OLED_SCROLL_UP);
        
    // Launch the after HaD logo display tests
    #ifdef TESTS_ENABLED
        afterHadLogoDisplayTests();  
    #endif
    
    // Let's fade in the LEDs
    for (uint16_t i = 0; i < MAX_PWM_VAL; i++)
    {
        setPwmDc(i);
        _delay_us(888);
    }
    activityDetectedRoutine();
    launchCalibrationCycle();
    _delay_ms(2000);
    
    while (1)
    {        
        // Call GUI routine
        guiMainLoop();
        
        // Check if a card just got inserted / removed
        card_detect_ret = isCardPlugged();
        
        // Do appropriate actions on smartcard insertion / removal
        if (card_detect_ret == RETURN_JDETECT)
        {
            // Light up the Mooltipass and call the dedicated function
            activityDetectedRoutine();
            guiHandleSmartcardInserted();
        }
        else if (card_detect_ret == RETURN_JRELEASED)
        {
            // Light up the Mooltipass and call the dedicated function
            activityDetectedRoutine();
            guiHandleSmartcardRemoved();
            
            // Set correct screen
            guiDisplayInformationOnScreen(PSTR("Card removed"));
            guiSetCurrentScreen(SCREEN_DEFAULT_NINSERTED);
            _delay_ms(2000);
            guiGetBackToCurrentScreen();
        }
        
        // Process possible incoming data
        if(usbRawHidRecv(usb_buffer, USB_READ_TIMEOUT) == RETURN_COM_TRANSF_OK)
        {
            usbProcessIncoming(usb_buffer);
        }  
        
        // Two quick caps lock presses wakes up the device        
        if ((hasTimerExpired(TIMER_CAPS, FALSE) == TIMER_EXPIRED) && (getKeyboardLeds() & HID_CAPS_MASK) && (wasCapsLockTimerArmed == FALSE))
        {
            wasCapsLockTimerArmed = TRUE;
            activateTimer(TIMER_CAPS, CAPS_LOCK_DEL);
        }
        else if ((hasTimerExpired(TIMER_CAPS, FALSE) == TIMER_RUNNING) && !(getKeyboardLeds() & HID_CAPS_MASK))
        {
            activityDetectedRoutine();
        }
        else if ((hasTimerExpired(TIMER_CAPS, FALSE) == TIMER_EXPIRED) && !(getKeyboardLeds() & HID_CAPS_MASK))
        {
            wasCapsLockTimerArmed = FALSE;            
        }
    }
}
