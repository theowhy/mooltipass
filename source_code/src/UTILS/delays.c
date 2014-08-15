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
/*!  \file     delays.c
*    \brief    Different delays used in the mooltipass
*    Created:  09/6/2014
*    Author:   Mathieu Stephan
*/ 
#include <util/delay.h>


void userViewDelay(void)
{
    _delay_ms(2000);
}

void smartcardHPulseDelay(void)
{
    _delay_us(2);
}

void pluginMessageRetryDelay(void)
{
    _delay_us(200);
}

void smartcardPowerDelay(void)
{
    _delay_ms(300);
}

void smartcardTchpDelay(void)
{
    _delay_ms(4);
}