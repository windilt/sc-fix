/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * DataAccount.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for �K.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef _COOLSAND_DATA_ACCOUNT_H
#define _COOLSAND_DATA_ACCOUNT_H

typedef enum
{
    SCR_ID_DTCNT_LIST = DATA_ACCOUNT_BASE + 1,
    SCR_ID_DTCNT_EDIT,
    SCR_ID_DTCNT_NOTIFICATION,
    SCR_ID_DTCNT_IN_PROGRESS,
    SCR_ID_DTCNT_MAIN,
#ifdef __MMI_GPRS_FEATURES__
    SCR_ID_DTCNT_GPRS_LIST,
    SCR_ID_DTCNT_GPRS_EDIT,
    SCR_ID_DTCNT_GPRS_NOTIFICATION,
#endif /* __MMI_GPRS_FEATURES__ */ 
    SCR_ID_DTCNT_POPUP
} SCR_DATA_ACCOUNT;

typedef enum
{
    IMG_ID_DTCNT_MAIN = DATA_ACCOUNT_BASE + 1,
    IMG_ID_DTCNT_CAPTION,
    IMG_ID_DTCNT_LSK_EDIT,
    IMG_ID_DTCNT_LSK_OK,
    IMG_ID_DTCNT_RSK_BACK,
    IMG_ID_DTCNT_AC_NAME,
    IMG_ID_DTCNT_DIAL_NUM,
    IMG_ID_DTCNT_USER_NAME,
    IMG_ID_DTCNT_PASSWD,
    IMG_ID_DTCNT_DIAL_TYPE,
    IMG_ID_DTCNT_DATA_RATE,
    IMG_ID_DTCNT_DNS_ADDRESS,
#ifdef __MMI_GPRS_FEATURES__
    IMG_ID_DTCNT_GPRS_AC_NAME,
    IMG_ID_DTCNT_GPRS_APN,
    IMG_ID_DTCNT_GPRS_USER_NAME,
    IMG_ID_DTCNT_GPRS_PASSWD,
    IMG_ID_DTCNT_GPRS_CONNTYPE,
#endif /* __MMI_GPRS_FEATURES__ */ 
    IMG_ID_DTCNT_SAVE,
    IMG_ID_DATAACCOUNT_ICON,
    IMG_ID_DTCNT_IN_PROGRESS
} IMG_DATA_ACCOUNT;

typedef enum
{
    STR_ID_DTCNT_MAIN = DATA_ACCOUNT_BASE + 1,
    STR_ID_DTCNT_CAPTION,
    /* STR_ID_DTCNT_LSK_EDIT, */
    STR_ID_DTCNT_RSK_BACK,
    /* STR_ID_DTCNT_EMPTY, */
    STR_ID_DTCNT_RENAME,
    STR_ID_DTCNT_DIAL_NUMBER,
    STR_ID_DTCNT_USER_NAME,
    STR_ID_DTCNT_PASSWD,
    STR_ID_DTCNT_DIAL_TYPE,
    STR_ID_DTCNT_DATA_RATE,
    STR_ID_DTCNT_DNS_ADDRESS,
    STR_ID_DTCNT_EDIT,
    //STR_ID_DTCNT_SAVING,
    //STR_ID_DTCNT_SAVE_QUERY,
#ifdef __MMI_GPRS_FEATURES__
    STR_ID_DTCNT_GPRS_PAP_AUTH,
    STR_ID_DTCNT_GPRS_CHAP_AUTH,
    STR_ID_DTCNT_GPRS_APN,
    STR_ID_DTCNT_GPRS_AUTH_TYP,
#endif /* __MMI_GPRS_FEATURES__ */ 
    STR_ID_DTCNT_ANALOGUE_STR,
    STR_ID_DTCNT_ISDN_STR,
    STR_ID_DTCNT_RATE_STR_ONE,
    STR_ID_DTCNT_RATE_STR_TWO,
    STR_ID_DTCNT_RATE_STR_THREE,
    STR_ID_DTCNT_NAME,
    STR_ID_DTCNT_NAME_ONE,
    STR_ID_DTCNT_NAME_TWO,
    STR_ID_DTCNT_NAME_THREE,
    STR_ID_DTCNT_NAME_FOUR,
    STR_ID_DTCNT_NAME_FIVE,
    /* micha1230 */
    STR_ID_DTCNT_NAME_SIX,
    STR_ID_DTCNT_NAME_SEVEN,
    STR_ID_DTCNT_NAME_EIGHT,
    STR_ID_DTCNT_NAME_NINE,
    STR_ID_DTCNT_NAME_TEN
} STR_DATA_ACCOUNT;

#endif /* _COOLSAND_DATA_ACCOUNT_H */ 

