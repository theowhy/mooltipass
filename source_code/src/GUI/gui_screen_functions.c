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
/*!  \file     gui_screen_functions.c
*    \brief    General user interface - screen functions
*    Created:  22/6/2014
*    Author:   Mathieu Stephan
*/
#include "smart_card_higher_level_functions.h"
#include "touch_higher_level_functions.h"
#include "gui_smartcard_functions.h"
#include "gui_screen_functions.h"
#include "gui_basic_functions.h"
#include "gui_pin_functions.h"
#include "userhandling.h"
#include "defines.h"
#include "oledmp.h"
#include "delays.h"
#include "anim.h"
#include "gui.h"

// Our current screen
uint8_t currentScreen = SCREEN_DEFAULT_NINSERTED;


/*! \fn     getCurrentScreen(void)
*   \brief  Get the current screen
*   \return The current screen
*/
uint8_t getCurrentScreen(void)
{
    return currentScreen;
}

/*! \fn     guiSetCurrentScreen(uint8_t screen)
*   \brief  Set current screen
*   \param  screen  The screen
*/
void guiSetCurrentScreen(uint8_t screen)
{
    currentScreen = screen;
}

/*! \fn     guiGetBackToCurrentScreen(void)
*   \brief  Get back to the current screen
*/
void guiGetBackToCurrentScreen(void)
{
    switch(currentScreen)
    {
        case SCREEN_DEFAULT_NINSERTED :
        {
            oledBitmapDrawFlash(0, 0, BITMAP_HAD, OLED_SCROLL_UP);
            break;
        }
        case SCREEN_DEFAULT_INSERTED_LCK :
        {
            oledBitmapDrawFlash(0, 0, BITMAP_HAD, OLED_SCROLL_UP);
            break;
        }
        case SCREEN_DEFAULT_INSERTED_NLCK :
        {
            oledBitmapDrawFlash(0, 0, BITMAP_MAIN_SCREEN, OLED_SCROLL_UP);
            break;
        }
        case SCREEN_DEFAULT_INSERTED_INVALID :
        {
            guiDisplayInformationOnScreen(PSTR("Please Remove The Card"));
            break;
        }
        case SCREEN_SETTINGS :
        {
            oledBitmapDrawFlash(0, 0, BITMAP_SETTINGS_SC, OLED_SCROLL_UP);
            break;
        }
        default : break;
    }
}

/*! \fn     guiScreenLoop(uint8_t touch_detect_result)
*   \brief  Function called to handle screen changes
*   \param  touch_detect_result Touch detection result
*/
void guiScreenLoop(uint8_t touch_detect_result)
{
    uint8_t state_machine_val = currentScreen;
    
    // If no press, you can return!
    if (!(touch_detect_result & TOUCH_PRESS_MASK))
    {
        return;
    }
    
    // Current screen is codded in the first 8 bytes, so we set the lowest 8 bytes to the detection quarter
    if (touch_detect_result & RETURN_WHEEL_PRESSED)
    {
        state_machine_val |= getWheelTouchDetectionQuarter();
    }
    else
    {
        state_machine_val |= 0x0F;
    }
    
    if (currentScreen == SCREEN_DEFAULT_NINSERTED)
    {
        // No smart card inserted, ask the user to insert one
        guiDisplayInsertSmartCardScreenAndWait();
    }
    else if (currentScreen == SCREEN_DEFAULT_INSERTED_LCK)
    {
        // Locked screen and a detection happened....
        
        // Check that the user hasn't removed his card, launch unlocking process
        if ((cardDetectedRoutine() == RETURN_MOOLTIPASS_USER) && (validCardDetectedFunction() == RETURN_OK))
        {
            // User approved his pin
            currentScreen = SCREEN_DEFAULT_INSERTED_NLCK;
        }
        
        // Go to the new screen
        guiGetBackToCurrentScreen();
    }
    else
    {
        switch(state_machine_val)
        {
            case (SCREEN_DEFAULT_INSERTED_NLCK|TOUCHPOS_WHEEL_BRIGHT) :
            {
                // User wants to lock his mooltipass
                currentScreen = SCREEN_DEFAULT_INSERTED_LCK;
                guiHandleSmartcardRemoved();
                guiGetBackToCurrentScreen();
                break;
            }
            case (SCREEN_DEFAULT_INSERTED_NLCK|TOUCHPOS_WHEEL_TRIGHT) :
            {
                // User wants to go to the settings menu
                currentScreen = SCREEN_SETTINGS;
                guiGetBackToCurrentScreen();
                break;
            }
            case (SCREEN_SETTINGS|TOUCHPOS_WHEEL_BRIGHT) :
            {
                // User wants to clone his smartcard
                uint16_t pin_code;
                
                // Reauth user
                if ((removeCardAndReAuthUser() == RETURN_OK) && (guiGetPinFromUser(&pin_code, PSTR("PIN for card?")) == RETURN_OK) && (cloneSmartCard(pin_code) == RETURN_OK))
                {
                    // Well, it's done
                }
                else
                {
                    currentScreen = SCREEN_DEFAULT_INSERTED_LCK;
                    guiDisplayInformationOnScreen(PSTR("Failed!"));
                }
                userViewDelay();
                guiGetBackToCurrentScreen();
                break;
            }
            case (SCREEN_SETTINGS|TOUCHPOS_WHEEL_TLEFT) :
            {
                // User wants to go to the main menu
                currentScreen = SCREEN_DEFAULT_INSERTED_NLCK;
                guiGetBackToCurrentScreen();
                break;
            }
            case (SCREEN_SETTINGS|TOUCHPOS_WHEEL_TRIGHT) :
            {
                // User wants to change his PIN code
                
                // Reauth user
                if (removeCardAndReAuthUser() == RETURN_OK)
                {
                    // User approved his pin, ask his new one
                    uint16_t pin1;
                    uint16_t pin2;
                    
                    if ((guiGetPinFromUser(&pin1, PSTR("New PIN ?")) == RETURN_OK) && (guiGetPinFromUser(&pin2, PSTR("Confirm PIN")) == RETURN_OK) && (pin1 == pin2))
                    {
                        // Both pins are the same, valid input, change pin
                        writeSecurityCode(pin1);
                        // Inform of success
                        guiDisplayInformationOnScreen(PSTR("PIN changed!"));
                    }
                    else
                    {
                        // Inform of fail
                        guiDisplayInformationOnScreen(PSTR("Not changed!"));
                    }
                    userViewDelay();
                }
                else
                {
                    currentScreen = SCREEN_DEFAULT_INSERTED_LCK;
                }
                guiGetBackToCurrentScreen();
                break;
            }
            default : break;
        }
    }    
}

/*! \fn     guiDisplayProcessingScreen(void)
*   \brief  Inform the user the mooltipass is busy
*/
void guiDisplayProcessingScreen(void)
{
    guiDisplayInformationOnScreen(PSTR("Processing..."));
}

/*! \fn     guiDisplayInformationOnScreen(const char* string)
*   \brief  Display text information on screen
*   \param  string  Pointer to the string to display
*/
void guiDisplayInformationOnScreen(const char* string)
{
    // Draw information bitmap & wait for user input
    oledClear();
    oledBitmapDrawFlash(2, 17, BITMAP_INFO, 0);
    oledPutstrXY_P(10, 24, OLED_CENTRE, string);
    oledFlipBuffers(0,0);
}

/*! \fn     guiAskForConfirmation(const char* string)
*   \brief  Ask for user confirmation for different things
*   \param  nb_args     Number of text lines (must be either 1 2 or 4)
*   \param  text_object Pointer to the text object if more than 1 line, pointer to progrem string if not
*   \return User confirmation or not
*/
RET_TYPE guiAskForConfirmation(uint8_t nb_args, confirmationText_t* text_object)
{
    // Draw asking bitmap
    oledClear();
    oledBitmapDrawFlash(0, 0, BITMAP_YES_NO, 0);
    
    // If more than one line
    if (nb_args == 1)
    {
        // Yeah, that's a bit dirty
        oledPutstrXY_P(0, 24, OLED_CENTRE, (const char*)text_object);
    }
    else
    {
        oledPutstrXY_P(0, 4, OLED_CENTRE, text_object->line1);
        if (nb_args >= 2)
        {
            oledPutstrXY(0, 21, OLED_CENTRE, text_object->line2);
        }
        if (nb_args >= 4)
        {
            oledPutstrXY_P(0, 36, OLED_CENTRE, text_object->line3);
            oledPutstrXY(0, 52, OLED_CENTRE, text_object->line4);
        }
    }
    
    // Display result
    oledFlipBuffers(0,0);
    
    // Wait for user input
    if(getTouchedPositionAnswer(LED_MASK_WHEEL) == TOUCHPOS_RIGHT)
    {
        return RETURN_OK;
    }
    else
    {
        return RETURN_NOK;
    }
}