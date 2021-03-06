/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	ToDoListEnum.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	This file defines screen, string, and image id of to do list application.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: ToDoListEnum.h

  	PURPOSE		: To Do List application

 

	AUTHOR		: Arvind

	DATE		: 20th Oct,2003

**************************************************************/
 
#ifndef _COOLSAND_TODO_LIST_ENUM_H
#define _COOLSAND_TODO_LIST_ENUM_H

#include "mmi_features.h"
#ifdef __MMI_TODOLIST__

#include "stdc.h"
#include "mmi_data_types.h"

// Define the All screen Id of Data Account

typedef enum 
{
	SCR_TDL_TASK_LIST =	TO_DO_LIST_BASE + 1,
	SCR_TDL_OPTION_LIST,
	SCR_TDL_TASK_VIEW,
	SCR_TDL_TASK_EDIT,
	SCR_TDL_EDIT_NOTE,
	SCR_TDL_NOTE_OPTION,
	SCR_TDL_LIST_TEMPLATE,
	SCR_TDL_REMINDER,
	SCR_TDL_CONFIRMATION,
	SCR_TDL_SUB_REMINDER
} TDL_SCR_ID_ENUM ;


// Define the All image Id of Data Account

typedef enum 
{
	IMG_TODO_LIST_CAPTION =	TO_DO_LIST_BASE + 1,
	IMG_TODO_LIST_LSK_EDIT,

	IMG_TODO_LIST_DATE,
	IMG_TODO_LIST_TIME,
	IMG_TODO_LIST_NOTE,
	IMG_TODO_LIST_ALARM,
	IMG_TODO_LIST_REPEAT,

	IMG_TODO_LIST_DELETE,
	IMG_TODO_LIST_EMPTY,
	IMG_TODO_LIST_SAVED,
	ORGANIZER_MENU_TODOLIST_IMAGEID

} IMG_TODO_LIST;


// Define the All string Id of Data Account

typedef enum 
{

	STR_TODO_LIST_MAIN =	TO_DO_LIST_BASE + 1,

	STR_TODO_LIST_LSK_EDIT,
	STR_TODO_LIST_EMPTY,
	STR_TODO_LIST_ADD_TASK,
	STR_TODO_LIST_VIEW_TASK,

	STR_TODO_LIST_CAP_TASK_LIST,
	STR_TODO_LIST_CAP_OPTION,
	STR_TODO_LIST_CAP_VIEW,
	STR_TODO_LIST_CAP_NOTE,
	STR_TODO_LIST_CAP_TEMPLATE,
	STR_TODO_LIST_CAP_REMINDER,

	STR_TODO_LIST_VIEW,
	STR_TODO_LIST_ADD,
	STR_TODO_LIST_EDIT,
	STR_TODO_LIST_DELETE,
	STR_TODO_LIST_DELETE_ALL,
	STR_TODO_LIST_SAVE,
	STR_TODO_LIST_USE_TEMPLATE,
	STR_TODO_LIST_INPUT_METHOD,

	STR_TODO_LIST_NOTE,
	STR_TODO_LIST_ALARM,
	STR_TODO_LIST_REPEAT,
	STR_TODO_LIST_DAYS,
	STR_TODO_LIST_TIME,

	STR_TODO_LIST_DELETE_ONE_TASK_QUERY,
	STR_TODO_LIST_DELETE_ALL_TASK_QUERY,
	STR_TODO_LIST_SAVE_TASK_QUERY,

	STR_TODO_LIST_DELETE_ONE_MSG,
	STR_TODO_LIST_DELETE_ALL_MSG,
	STR_TODO_LIST_SAVING_MSG,
	STR_TODO_LIST_EMPTY_NOTE_MSG,
	STR_TODO_LIST_WRONG_TIME_MSG,
	STR_TODO_LIST_WRONG_DATE_MSG,
	STR_TODO_LIST_LIST_FULL_MSG,

	STR_TODO_TEMPLATE_1,
	STR_TODO_TEMPLATE_2,
	STR_TODO_TEMPLATE_3,
	STR_TODO_TEMPLATE_4,
	STR_TODO_TEMPLATE_5,
	STR_TODO_TEMPLATE_6,
	STR_TODO_TEMPLATE_7,
	STR_TODO_TEMPLATE_8,
	STR_TODO_TEMPLATE_9,
	STR_TODO_TEMPLATE_10,

	STR_TODO_SUN,
	STR_TODO_MON,
	STR_TODO_TUE,
	STR_TODO_WED,
	STR_TODO_THU,
	STR_TODO_FRI,
	STR_TODO_SAT,

	STR_TODO_ON	,
	STR_TODO_OFF,
	STR_TODO_ALARM,
	STR_TODO_NOTE,
	STR_TODO_REPEAT,
	STR_TODO_ONCE,
	STR_TODO_EVERY_DAY,
	STR_TODO_DAY,
	STR_TODO_WEEKLY,
	STR_TODO_MONTHLY,
	ORGANIZER_MENU_TODOLIST_STRINGID,
	CAL_STRING_VIEW,
	STR_TDL_TOTAL
 } STR_TODO_LIST ;

#endif
#endif /* _COOLSAND_TODO_LIST_ENUM_H */
 


