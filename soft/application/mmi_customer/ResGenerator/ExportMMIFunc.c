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

/**************************************************************

	FILENAME	: ExportMMIFunc.c

  	PURPOSE		: Functions From MMI

	REMARKS		: nil

	AUTHOR		: Yogesh

	DATE		: August-26-2003

**************************************************************/

#include "cs_types.h"
#include "unicodedcl.h"
#include "exportmmifunc.h"
#include "custresdef.h"

HWND hWnd;
S8 CustImgPath[]=CUST_IMG_PATH;
S8 CustAdoPath[]=CUST_ADO_PATH;/*101205 audio resource Calvin Add */

void InitUnicodeSupport(void)
{
 
#ifdef __UCS2_ENCODING


	pfnUnicodeToEncodingScheme = UnicodeToUCS2Encoding;
	pfnEncodingSchemeToUnicode = UCS2EncodingToUnicode;
	pfnUnicodeStrlen  = UCS2Strlen;
	pfnUnicodeStrcpy  = UCS2Strcpy;
	pfnUnicodeStrcmp  = UCS2Strcmp;
	pfnUnicodeStrncpy = UCS2Strncpy;
	pfnUnicodeStrncmp = UCS2Strncmp;
	pfnUnicodeStrcat  = UCS2Strcat;
#endif



#ifdef __ASCII


	pfnUnicodeToEncodingScheme = UnicodeToUCS2Encoding;
	pfnEncodingSchemeToUnicode = UCS2EncodingToUnicode;
	pfnUnicodeStrlen  = AsciiStrlen;
	pfnUnicodeStrcpy  = AsciiStrcpy;
	pfnUnicodeStrcmp  = AsciiStrcmp;
	pfnUnicodeStrncpy = AsciiStrncpy;
	pfnUnicodeStrncmp = AsciiStrncmp;
	pfnUnicodeStrtok  = AsciiStrtok;
	pfnUnicodeStrcat  = AsciiStrcat;
#endif
}
void ShutdownSystemOperation(void)
{
}

void set_MMI_theme(MMI_theme *t){}
void MMI_apply_current_theme(void){}
S8* GetImageFileSystem(U16 ImageId){return 0;}
void PhnsetReinitHomeCityIdx(void){}

