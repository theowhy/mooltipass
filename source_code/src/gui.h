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
/*!  \file     gui.h
*    \brief    General user interface
*    Created:  22/6/2014
*    Author:   Mathieu Stephan
*/


#ifndef GUI_H_
#define GUI_H_

#include "node_mgmt.h"

/* Defines */
#define SCREEN_TIMER_DEL    60000
#define LIGHT_TIMER_DEL     10000
#define USER_INTER_DEL      6000

/* Prototypes */
uint16_t guiAskForLoginSelect(mgmtHandle* h, pNode* p, cNode* c, uint16_t parentNodeAddress);
RET_TYPE guiAskForPasswordSet(char* name, char* password, char* service);
RET_TYPE guiAskForImportExportConfirmation(const char* string);
RET_TYPE guiAskForLoginAddApproval(char* name, char* service);
RET_TYPE guiAskForDomainAddApproval(char* name);
void informGuiOfCurrentContext(char* context);
void activityDetectedRoutine(void);
void guiTimerTick(void);
void guiMainLoop(void);

#endif /* GUI_H_ */