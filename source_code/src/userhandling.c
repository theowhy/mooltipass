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
/*!  \file     userhandling.c
*    \brief    Logic for user handling
*    Created:  09/6/2014
*    Author:   Mathieu Stephan
*/
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/io.h>
#include "eeprom_addresses.h"
#include "userhandling.h"
#include "smartcard.h"
#include "defines.h"
#include "usb.h"

// Credential timer valid flag
volatile uint8_t credential_timer_valid = FALSE;
// Credential timer value
volatile uint16_t credential_timer = 0;
// Selected login flag (the plugin selected a login)
uint8_t selected_login_flag = FALSE;
// Context valid flag (eg we know the current service / website)
uint8_t context_valid_flag = FALSE;


/*! \fn     setCurrentContext(uint8_t* name, uint8_t length)
*   \brief  Set our current context
*   \param  name    Name of the desired service / website
*   \param  length  Length of the string
*   \return If we found the context
*/
RET_TYPE setCurrentContext(uint8_t* name, uint8_t length)
{
    uint8_t reg;
    
    // Look for name inside our flash
    if (TRUE)
    {
        context_valid_flag = TRUE;
        return RETURN_OK;
    } 
    else
    {
        cli();
        credential_timer = 0;
        context_valid_flag = FALSE;
        selected_login_flag = FALSE;
        credential_timer_valid = FALSE;
        SREG = reg;                     // restore original interrupt state (may already be disabled)
        return RETURN_NOK;
    }
}

/*! \fn     getLoginForContext(uint8_t* buffer)
*   \brief  Get login for current context
*   \param  buffer  Buffer to store the login
*   \return If login was entered
*/
RET_TYPE getLoginForContext(uint8_t* buffer)
{
    if (context_valid_flag == FALSE)
    {
        // Context invalid
        return RETURN_NOK;
    } 
    else
    {
        if (credential_timer_valid == FALSE)
        {
            // Ask the user for approval
            return RETURN_NOK;
        } 
        else
        {
            // Fetch the login and send it
            // bla bla bla
            // Send it to the computer via HID
            // usbKeybPutStr((char*)buffer);
            return RETURN_OK;
        }
    }
}

/*! \fn     getPasswordForContext(void)
*   \brief  Get password for current context
*   \return If password was entered
*/
RET_TYPE getPasswordForContext(void)
{
    uint8_t reg = SREG;

    if ((context_valid_flag == TRUE) && (credential_timer_valid == TRUE))
    {
        // Fetch password and send it over USB
        //usbKeybPutStr();
        // Clear credential timer
        cli();
        credential_timer = 0;
        credential_timer_valid = FALSE;
        SREG = reg;                     // restore original interrupt state (may already be disabled)
        return RETURN_OK; 
    } 
    else
    {
        return RETURN_NOK;
    }
}

/*! \fn     setLoginForContext(uint8_t* name, uint8_t length)
*   \brief  Set login for current context
*   \param  name    String containing the login
*   \param  length  String length
*   \return Operation success or not
*/
RET_TYPE setLoginForContext(uint8_t* name, uint8_t length)
{
    if (context_valid_flag == FALSE)
    {
        return RETURN_NOK;
    } 
    else
    {
        // Look for given login in the flash
        if (TRUE)
        {
            // Select it
            selected_login_flag = TRUE;
            return RETURN_OK;
        } 
        else
        {
            // If doesn't exist, ask user for confirmation to add to flash
            if (TRUE)
            {
                selected_login_flag = TRUE;
                return RETURN_OK;
            } 
            else
            {
                selected_login_flag = FALSE;
                return RETURN_NOK;
            }
        }
    }
}

/*! \fn     setPasswordForContext(uint8_t* password, uint8_t length)
*   \brief  Set password for current context
*   \param  password    String containing the password
*   \param  length      String length
*   \return Operation success or not
*/
RET_TYPE setPasswordForContext(uint8_t* password, uint8_t length)
{
    if ((selected_login_flag == FALSE) || (context_valid_flag == FALSE))
    {
        // Login not set
        return RETURN_NOK;
    } 
    else
    {
        // Ask for password changing approval
        if (TRUE)
        {
            // Store password
            selected_login_flag = FALSE;
            return RETURN_OK;
        } 
        else
        {
            return RETURN_NOK;
        }
    }
}

/*! \fn     firstTimeUserHandlingInit(void)
*   \brief  First time required intialization
*/
void firstTimeUserHandlingInit(void)
{
    eeprom_write_byte((uint8_t*)EEP_NB_KNOWN_CARDS_ADDR, 0);
    eeprom_write_byte((uint8_t*)EEP_NB_KNOWN_USERS_ADDR, 0);
}

/*! \fn     getNumberOfKnownUsers(void)
*   \brief  Get the number of know users
*   \return The number of users
*/
uint8_t getNumberOfKnownUsers(void)
{
    return eeprom_read_byte((uint8_t*)EEP_NB_KNOWN_USERS_ADDR);
}

/*! \fn     getNumberOfKnownCards(void)
*   \brief  Get the number of know cards
*   \return The number of cards
*/
uint8_t getNumberOfKnownCards(void)
{
    return eeprom_read_byte((uint8_t*)EEP_NB_KNOWN_CARDS_ADDR);
}

/*! \fn     findUserId(uint8_t userid)
*   \brief  Find a given user ID
*   \param  userid  The user ID
*   \return Yes or No...
*/
RET_TYPE findUserId(uint8_t userid)
{
    uint8_t i;
    
    for (i = 0; i < getNumberOfKnownCards(); i++)
    {
        if (eeprom_read_byte((uint8_t*)EEP_SMC_IC_USER_MATCH_START_ADDR+i*SMCID_UID_MATCH_ENTRY_LENGTH+SMARTCARD_CPZ_LENGTH) == userid)
        {
            return RETURN_OK;
        }
    }
    
    return RETURN_NOK;    
}

/*! \fn     getUserIdFromSmartCardCPZ(uint8_t* buffer, uint8_t* userid)
*   \brief  Get a user ID from card CPZ
*   \param  buffer  Buffer containing the CPZ
*   \param  userid  pointed to where to store the user id
*   \return If we found the CPZ
*/
RET_TYPE getUserIdFromSmartCardCPZ(uint8_t* buffer, uint8_t* userid)
{
    uint8_t temp_bool;
    uint8_t i,j;
    
    for (i = 0; i < getNumberOfKnownCards(); i++)
    {
        temp_bool = TRUE;
        for (j = 0; j < SMARTCARD_CPZ_LENGTH; j++)
        {
            if (buffer[j] != eeprom_read_byte((uint8_t*)EEP_SMC_IC_USER_MATCH_START_ADDR+i*SMCID_UID_MATCH_ENTRY_LENGTH+j))
            {
                temp_bool = FALSE;
            }
        }
        if (temp_bool == TRUE)
        {
            *userid = eeprom_read_byte((uint8_t*)EEP_SMC_IC_USER_MATCH_START_ADDR+i*SMCID_UID_MATCH_ENTRY_LENGTH+SMARTCARD_CPZ_LENGTH);
            return RETURN_OK;
        }
    }
    
    return RETURN_NOK;
}

/*! \fn     writeSmartCardCPZForUserId(uint8_t* buffer, uint8_t userid)
*   \brief  Add a CPZ<>User id entry
*   \param  buffer  Buffer containing the CPZ
*   \param  userid  user id
*   \return If we could add the entry
*/
RET_TYPE writeSmartCardCPZForUserId(uint8_t* buffer, uint8_t userid)
{
    uint8_t i;
    
    // Check that we still have space to store
    if (((getNumberOfKnownCards()+1)*SMCID_UID_MATCH_ENTRY_LENGTH) + EEP_SMC_IC_USER_MATCH_START_ADDR >= EEPROM_SIZE)
    {
        return RETURN_NOK;
    }
    else if (getUserIdFromSmartCardCPZ(buffer, &i) == RETURN_OK)
    {
        return RETURN_NOK;
    }
    else
    {
        if (findUserId(userid) == RETURN_NOK)
        {
            eeprom_write_byte((uint8_t*)EEP_NB_KNOWN_USERS_ADDR, getNumberOfKnownUsers()+1);
        }
        for (i = 0; i < SMARTCARD_CPZ_LENGTH; i++)
        {
            eeprom_write_byte((uint8_t*)EEP_SMC_IC_USER_MATCH_START_ADDR + getNumberOfKnownCards()*SMCID_UID_MATCH_ENTRY_LENGTH + i, buffer[i]);
        }
        eeprom_write_byte((uint8_t*)EEP_SMC_IC_USER_MATCH_START_ADDR + getNumberOfKnownCards()*SMCID_UID_MATCH_ENTRY_LENGTH + SMARTCARD_CPZ_LENGTH, userid);
        eeprom_write_byte((uint8_t*)EEP_NB_KNOWN_CARDS_ADDR, getNumberOfKnownCards()+1);
        return RETURN_OK;
    }
}