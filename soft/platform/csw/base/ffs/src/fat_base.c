
#include "dsm_cf.h"
#include "fs.h"
#include "dsm_dbg.h"
#include "fat_local.h"
#include "fat_base.h"
#include "fs_base.h"



FAT_CACHE* g_FatCache = NULL;
UINT32 g_FatCahcheNum = 0;

INT32 NameFat2User(PCSTR fatName, PSTR userName,UINT8 entryReseved)
{
    INT32 i;
    UINT8 *p = NULL;

    p = userName;
    if(NULL == fatName )
    {
        return _ERR_FAT_PARAM_ERROR;
    }

    if(NULL == userName)
    {
        return _ERR_FAT_PARAM_ERROR;
    }

    //
    // Deal with the prefix
    //
    for(i = 0; i < 8 && fatName[i] != ' '; ++i, ++p)
    {
		if(0x08 & entryReseved)
		{
			*p = (fatName[i] >= 'A' && fatName[i] <= 'Z') ? (fatName[i] + 'a' - 'A') : fatName[i];
		}
		else
		{
			*p = fatName[i];
		}	
    }

    //
    // If there is postfix
    //
    if(fatName[8+0] != ' ')
    {
        *p = '.';
        ++p;

        //
        // Deal with the postfix
        //
        for(i = 0; i < 3 && fatName[8+i] != ' '; ++i, ++p)
        {
            *p = fatName[8+i];
			if(0x10 & entryReseved)
			{
				*p = (fatName[8+i] >= 'A' && fatName[8+i] <= 'Z') ? (fatName[8+i] + 'a' - 'A') : fatName[8+i];
			}
			else
			{
				*p = fatName[8+i];
			}	
        }
    }

    *p = '\0';

    return ERR_SUCCESS;
}


INT32 NameUser2Fat(PCSTR userName, PSTR fatName)
{
    UINT32 i;
    UINT8* dotpos;
    UINT32 iDotCount = 0;
    UINT32 userNameLen = 0;
    
    userNameLen = DSM_StrLen(userName);
    
    if(NULL == fatName )
    {
        D( ( DL_FATERROR,"In  NameUser2Fat, fatName is NULL\n"));
        return _ERR_FAT_PARAM_ERROR;
    }

    if(NULL == userName)
    {
        D( ( DL_FATERROR,"In  NameUser2Fat, userName is NULL\n"));
        return _ERR_FAT_PARAM_ERROR;
    }

    if( 0 == userNameLen || userNameLen > FAT_MAX_NAME_LEN + 1)
    {
        D( ( DL_FATERROR,"In  NameUser2Fat, userNameLen = %d\n",userNameLen));
        return _ERR_FAT_PARAM_ERROR;
    }
    // FAT 8-3 name validating.
    // The following characters are not legal in any bytes of DIR_Name:
    // Values less than 0x20 except for the special case of 0x05 in DIR_Name[0] described above.
    // 0x22, 0x2A, 0x2B, 0x2C, 0x2E, 0x2F, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x5B, 0x5C, 0x5D,
    // and 0x7C.
    if(userName[0] <= 0x20)
    {
        D( ( DL_FATERROR,"In  NameUser2Fat, userName[0] <= 0x20 userName[0] = 0x%x\n",userName[0] ));
        return _ERR_FAT_PARAM_ERROR;
    }
    for(i = 0 ; i < userNameLen ; i++)
    {
        if(userName[i] == 0x3a || userName[i] == 0x22 ||
           userName[i] == 0x3b || userName[i] == 0x2a || 
           userName[i] == 0x3c || userName[i] == 0x2b ||
           userName[i] == 0x3d || userName[i] == 0x2c ||
           userName[i] == 0x3e || userName[i] == 0x3f || 
           userName[i] == 0x2f || userName[i] == 0x5b || 
           userName[i] == 0x5c || userName[i] == 0x5d ||
           userName[i] == 0x7c
           )
        {
            D( ( DL_FATERROR,"In  NameUser2Fat, there is illegal character in userName, userName[%d] = 0x%x\n",i,userName[i]));
            return _ERR_FAT_PARAM_ERROR;
        }
        if(0x2e == userName[i])
        {
            if(iDotCount >= 1)
            {
                D( ( DL_FATERROR,"In  NameUser2Fat, there is too many dots in userName!\n"));
                return _ERR_FAT_PARAM_ERROR;
            }
            iDotCount ++;
        }
    }

    DSM_MemSet(fatName, ' ', FAT_MAX_NAME_LEN);
    dotpos = (UINT8*)DSM_StrChr(userName, '.');

    if(NULL == dotpos)
    {
        UINT32 len = DSM_StrLen(userName);
        if(len > 8)
        {
            D( ( DL_FATERROR,"In  NameUser2Fat, dotpos is NULL, len = %d!\n",len));
            return _ERR_FAT_PARAM_ERROR;
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fatName[i] = userName[i];
            }
        }
    }
    else
    {
        UINT32 prefixlen = dotpos - userName;
        UINT32 postfixlen = userNameLen - (prefixlen + 1);
        if(prefixlen > 8 || postfixlen > 3)
        {
            D( ( DL_FATERROR,"In  NameUser2Fat, prefixlen = %d, postfixlen = %d\n",prefixlen,postfixlen));
            return _ERR_FAT_PARAM_ERROR;
        }
        else
        {
            for(i=0; i<prefixlen; i++)
            {
                fatName[i] = userName[i];
            }
            for(i=0; i<postfixlen; i++)
            {
                fatName[i+8] = dotpos[i+1];
            }
        }
    }

    DSM_StrUpr(fatName);
    
    return ERR_SUCCESS;
}

VOID FBR2Buf(FAT_BOOT_RECORD *fbr, UINT8 *buf)
{
    DSM_MemCpy(&buf[FBR_JMP_BOOT_POS], &(fbr->BS_jmpBoot), FBR_JMP_BOOT_SIZE);
    DSM_MemCpy(&buf[FBR_OEM_NAME_POS], &(fbr->BS_OEMName), FBR_OEM_NAME_SIZE);
    DSM_MemCpy(&buf[FBR_BYTES_PER_SEC_POS], &(fbr->BPB_BytesPerSec), FBR_BYTES_PER_SEC_SIZE);
    buf[FBR_SEC_PER_CLUS_POS] = fbr->BPB_SecPerClus;
    DSM_MemCpy(&buf[FBR_RSVD_SEC_CNT_POS], &(fbr->BPB_RsvdSecCnt), FBR_RSVD_SEC_CNT_SIZE);
    buf[FBR_NUM_FATS_POS] = fbr->BPB_NumFATs;
    DSM_MemCpy(&buf[FBR_ROOT_ENT_CNT_POS], &(fbr->BPB_RootEntCnt), FBR_ROOT_ENT_CNT_SIZE);
    DSM_MemCpy(&buf[FBR_TOT_SEC16_POS], &(fbr->BPB_TotSec16), FBR_TOT_SEC16_SIZE);
    buf[FBR_MEDIA_POS] = fbr->BPB_Media;
    DSM_MemCpy(&buf[FBR_FAT_SZ16_POS], &(fbr->BPB_FATSz16), FBR_FAT_SZ16_SIZE);
    DSM_MemCpy(&buf[FBR_SEC_PER_TRK_POS], &(fbr->BPB_SecPerTrk), FBR_SEC_PER_TRK_SIZE);
    DSM_MemCpy(&buf[FBR_NUM_HEADS_POS], &(fbr->BPB_NumHeads), FBR_NUM_HEADS_SIZE);
    DSM_MemCpy(&buf[FBR_HIDD_SEC_POS], &(fbr->BPB_HiddSec), FBR_HIDD_SEC_SIZE);
    DSM_MemCpy(&buf[FBR_TOL_SEC32_POS], &(fbr->BPB_TotSec32), FBR_TOL_SEC32_SIZE);
    if(0 == fbr->BPB_FATSz16)
    {
        DSM_MemCpy(&buf[FBR_FATSZ32_POS],&(fbr->u.sub_rec32.BPB_FATSz32),BPB_FATSZ32_SIZE);
        DSM_MemCpy(&buf[BPB_EXTFLAGS_POS],&(fbr->u.sub_rec32.BPB_ExtFlags),BPB_EXTFLAGS_SIZE);
        DSM_MemCpy(&buf[BPB_FSVER_POS],&(fbr->u.sub_rec32.BPB_FSVer),BPB_FSVER_SIZE);
        DSM_MemCpy(&buf[BPB_ROOTCLUS_POS],&(fbr->u.sub_rec32.BPB_RootClus),BPB_ROOTCLUS_SIZE);
        DSM_MemCpy(&buf[BPB_FSINFO_POS],&(fbr->u.sub_rec32.BPB_FSInfo),BPB_FSINFO_SIZE);
        DSM_MemCpy(&buf[BPB_BKBOOTSEC_POS],&(fbr->u.sub_rec32.BPB_BkBootSec),BPB_BKBOOTSEC_SIZE);
        DSM_MemCpy(&buf[BPB_RESERVED_POS],&(fbr->u.sub_rec32.BPB_Reserved),BPB_RESERVED_SIZE);
        buf[BS_DRVNUM_POS] = fbr->u.sub_rec32.BS_DrvNum;
        buf[BS_RESERVED1_POS] = fbr->u.sub_rec32.BS_Reserved1;
        buf[BS_BOOTSIG_POS] = fbr->u.sub_rec32.BS_BootSig;
        DSM_MemCpy(&buf[BS_VOLID_POS],&(fbr->u.sub_rec32.BS_VolID),BS_VOLID_SIZE);
        DSM_MemCpy(&buf[BS_VOLLAB_POS],&(fbr->u.sub_rec32.BS_VolLab),BS_VOLLAB_SIZE);
        DSM_MemCpy(&buf[BS_FILSYSTYPE_POS],&(fbr->u.sub_rec32.BS_FilSysType),BS_FILSYSTYPE_SIZE);
    }
    else
    {    
        buf[FBR_DRV_NUM_POS] = fbr->u.sub_rec1216.BS_DrvNum;
        buf[FBR_RESERVED1_POS] = fbr->u.sub_rec1216.BS_Reserved1;
        buf[FBR_BOOT_SIG_POS] = fbr->u.sub_rec1216.BS_BootSig;
        DSM_MemCpy(&buf[FBR_VOL_ID_POS], &(fbr->u.sub_rec1216.BS_VolID), FBR_VOL_ID_SIZE);
        DSM_MemCpy(&buf[FBR_VOL_LAB_POS], &(fbr->u.sub_rec1216.BS_VolLab), FBR_VOL_LAB_SIZE);
        DSM_MemCpy(&buf[FBR_FIL_SYS_TYPE_POS], &(fbr->u.sub_rec1216.BS_FilSysType), FBR_FIL_SYS_TYPE_SIZE);
    }
}

VOID Buf2MBR(UINT8 *buf, MBR_BOOT_RECORD *mbr)
{
	DSM_MemSet(mbr,0,SIZEOF(MBR_BOOT_RECORD));
	mbr->active_partition = (UINT8)buf[MBR_ACTIVE_PARTION_POS]; 
	mbr->start_head        = (UINT8)buf[MBR_START_HEAD_POS];
	mbr->start_sector      = (UINT16)buf[MBR_START_SECTOR_POS]&0x3F;
	mbr->start_cylinder   = (UINT16)((buf[MBR_START_SECTOR_POS]&0xc0)<<2)|buf[MBR_START_SECTOR_POS + 1];
	mbr->part_type_indicator = (UINT8)buf[MBR_PART_TYPE_INDICATOR_POS];
	mbr->end_head        = (UINT8)buf[MBR_END_HEAD_POS];
	mbr->end_sector      = (UINT16)buf[MBR_END_SECTOR_POS]&0x3F;
	mbr->end_cylinder    = (UINT16)((buf[MBR_END_SECTOR_POS]&0xc0)<<2)|buf[MBR_END_SECTOR_POS + 1];
	
       mbr->sectors_preceding = MAKEFATUINT32((buf)[MBR_SECTORS_PRECEDING_POS],
						  (buf)[MBR_SECTORS_PRECEDING_POS+1],
	                                     (buf)[MBR_SECTORS_PRECEDING_POS+2],
	                                     (buf)[MBR_SECTORS_PRECEDING_POS+3]);
       mbr->setcors_partion = MAKEFATUINT32((buf)[MBR_SECTORS_PARTION_POS],
		            		         (buf)[MBR_SECTORS_PARTION_POS+1],
		                              (buf)[MBR_SECTORS_PARTION_POS+2],
		                              (buf)[MBR_SECTORS_PARTION_POS+3]);	
}

VOID Buf2FBR(UINT8 *buf, FAT_BOOT_RECORD *fbr)
{
    DSM_MemSet(fbr,0,SIZEOF(FAT_BOOT_RECORD));
    
    DSM_MemCpy(fbr->BS_jmpBoot, &buf[FBR_JMP_BOOT_POS], FBR_JMP_BOOT_SIZE);
    DSM_MemCpy(fbr->BS_OEMName, &buf[FBR_OEM_NAME_POS], FBR_OEM_NAME_SIZE);
    fbr->BPB_BytesPerSec = MAKEFATUINT16((buf)[FBR_BYTES_PER_SEC_POS], 
                                         (buf)[FBR_BYTES_PER_SEC_POS+1]
                                         );
    fbr->BPB_SecPerClus = (UINT8)buf[FBR_SEC_PER_CLUS_POS];
    fbr->BPB_RsvdSecCnt = MAKEFATUINT16((buf)[FBR_RSVD_SEC_CNT_POS], 
                                        (buf)[FBR_RSVD_SEC_CNT_POS+1]);
    fbr->BPB_NumFATs = buf[FBR_NUM_FATS_POS];
    fbr->BPB_RootEntCnt = MAKEFATUINT16((buf)[FBR_ROOT_ENT_CNT_POS], 
                                        (buf)[FBR_ROOT_ENT_CNT_POS+1]);

    fbr->BPB_TotSec16 = MAKEFATUINT16((buf)[FBR_TOT_SEC16_POS], 
                                      (buf)[FBR_TOT_SEC16_POS+1]);
    
    fbr->BPB_SecPerTrk = MAKEFATUINT16((buf)[FBR_SEC_PER_TRK_POS], 
                                       (buf)[FBR_SEC_PER_TRK_POS+1]);

    fbr->BPB_NumHeads = (UINT8)buf[FBR_NUM_HEADS_POS];

    fbr->BPB_HiddSec = MAKEFATUINT32((buf)[FBR_HIDD_SEC_POS],
                                     (buf)[FBR_HIDD_SEC_POS+1],
                                     (buf)[FBR_HIDD_SEC_POS+2],
                                     (buf)[FBR_HIDD_SEC_POS+3]);
    
    fbr->BPB_TotSec32 = MAKEFATUINT32((buf)[FBR_TOL_SEC32_POS],
                                     (buf)[FBR_TOL_SEC32_POS+1],
                                     (buf)[FBR_TOL_SEC32_POS+2],
                                     (buf)[FBR_TOL_SEC32_POS+3]);

    fbr->BPB_Media = (UINT8)buf[FBR_MEDIA_POS];
    
    fbr->BPB_FATSz16 = MAKEFATUINT16((buf)[FBR_FAT_SZ16_POS], 
                                     (buf)[FBR_FAT_SZ16_POS+1]);
    if(0 == fbr->BPB_FATSz16)
    {
        DSM_MemSet(&(fbr->u.sub_rec1216),0,SIZEOF(FAT_BOOT_SUB_RECORD_1216));
        fbr->u.sub_rec32.BPB_FATSz32 = MAKEFATUINT32((buf)[FBR_FATSZ32_POS],
                                                     (buf)[FBR_FATSZ32_POS+1],
                                                     (buf)[FBR_FATSZ32_POS+2],
                                                     (buf)[FBR_FATSZ32_POS+3]);  
        fbr->u.sub_rec32.BPB_RootClus = MAKEFATUINT32((buf)[BPB_ROOTCLUS_POS],
                                                      (buf)[BPB_ROOTCLUS_POS+1],
                                                      (buf)[BPB_ROOTCLUS_POS+2],
                                                      (buf)[BPB_ROOTCLUS_POS+3]);  
        fbr->u.sub_rec32.BS_VolID = MAKEFATUINT32((buf)[BS_VOLID_POS],
                                                  (buf)[BS_VOLID_POS+1],
                                                  (buf)[BS_VOLID_POS+2],
                                                  (buf)[BS_VOLID_POS+3]);  
        fbr->u.sub_rec32.BPB_ExtFlags = MAKEFATUINT16((buf)[BPB_EXTFLAGS_POS],
                                                      (buf)[BPB_EXTFLAGS_POS+1]);  
        fbr->u.sub_rec32.BPB_FSVer = MAKEFATUINT16((buf)[BPB_FSVER_POS],
                                                   (buf)[BPB_FSVER_POS+1]); 
        fbr->u.sub_rec32.BPB_FSInfo = MAKEFATUINT16((buf)[BPB_FSINFO_POS],
                                                   (buf)[BPB_FSINFO_POS+1]); 
        fbr->u.sub_rec32.BPB_BkBootSec = MAKEFATUINT16((buf)[BPB_BKBOOTSEC_POS],
                                                   (buf)[BPB_BKBOOTSEC_POS+1]);  
        fbr->u.sub_rec32.BS_DrvNum = (UINT8)buf[BS_DRVNUM_POS];
        fbr->u.sub_rec32.BS_Reserved1 = (UINT8)buf[BS_RESERVED1_POS];
        fbr->u.sub_rec32.BS_BootSig = (UINT8)buf[BS_BOOTSIG_POS];
        DSM_MemCpy(fbr->u.sub_rec32.BPB_Reserved, &buf[BPB_RESERVED_POS], BPB_RESERVED_SIZE);
        DSM_MemCpy(fbr->u.sub_rec32.BS_VolLab, &buf[BS_VOLLAB_POS], BS_VOLLAB_SIZE);
        DSM_MemCpy(fbr->u.sub_rec32.BS_FilSysType, &buf[BS_FILSYSTYPE_POS], BS_FILSYSTYPE_SIZE);
      
    }
    else
    {   
        DSM_MemSet(&(fbr->u.sub_rec32),0,SIZEOF(FAT_BOOT_SUB_RECORD_32));
        fbr->u.sub_rec1216.BS_DrvNum = (UINT8)buf[FBR_DRV_NUM_POS];
        fbr->u.sub_rec1216.BS_Reserved1 = (UINT8)buf[FBR_RESERVED1_POS];
        fbr->u.sub_rec1216.BS_BootSig = (UINT8)buf[FBR_BOOT_SIG_POS];
        fbr->u.sub_rec1216.BS_VolID = MAKEFATUINT32((buf)[FBR_VOL_ID_POS], 
            (buf)[FBR_VOL_ID_POS+1],
            (buf)[FBR_VOL_ID_POS+2],
            (buf)[FBR_VOL_ID_POS+3]);
        
        DSM_MemCpy(fbr->u.sub_rec1216.BS_VolLab, &buf[FBR_VOL_LAB_POS], FBR_VOL_LAB_SIZE);
        DSM_MemCpy(fbr->u.sub_rec1216.BS_FilSysType, &buf[FBR_FIL_SYS_TYPE_POS], FBR_FIL_SYS_TYPE_SIZE);
    }
}

VOID FDE2Buf(FAT_DIR_ENTRY *fde, UINT8 *buf)
{
    DSM_MemCpy(&buf[FDE_DIR_NAME_POS], &(fde->DIR_Name), FDE_DIR_NAME_SIZE);
    buf[FDE_DIR_ATTR_POS] = fde->DIR_Attr;
    buf[FDE_DIR_NTRES_POS] = fde->DIR_NTRes;
    buf[FDE_DIR_CRT_TIME_TENTH_POS] = fde->DIR_CrtTimeTenth;
    DSM_MemCpy(&buf[FDE_DIR_CRT_TIME_POS], &(fde->DIR_CrtTime), FDE_DIR_CRT_TIME_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_CRT_DATA_POS], &(fde->DIR_CrtDate), FDE_DIR_CRT_DATA_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_LST_ACC_DATA_POS], &(fde->DIR_LstAccDate), FDE_DIR_LST_ACC_DATA_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_FST_CLUS_HI_POS], &(fde->DIR_FstClusHI), FDE_DIR_FST_CLUS_HI_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_WRT_TIME_POS], &(fde->DIR_WrtTime), FDE_DIR_CRT_TIME_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_WRT_DATA_POS], &(fde->DIR_WrtDate), FDE_DIR_WRT_DATA_SIZE);
    DSM_MemCpy(&buf[FDE_DIR_FST_CLUS_LO_POS], &(fde->DIR_FstClusLO), FDE_DIR_FST_CLUS_LO_SIZE);    
    DSM_MemCpy(&buf[FDE_DIR_FILE_SIZE_POS], &(fde->DIR_FileSize), FDE_DIR_FILE_SIZE_SIZE);
}

VOID Buf2FDE(UINT8 *buf, FAT_DIR_ENTRY *fde)
{
    DSM_MemCpy(fde->DIR_Name, &buf[FDE_DIR_NAME_POS], FDE_DIR_NAME_SIZE);
    fde->DIR_Attr = buf[FDE_DIR_ATTR_POS];
    fde->DIR_NTRes = buf[FDE_DIR_NTRES_POS];
    fde->DIR_CrtTimeTenth = buf[FDE_DIR_CRT_TIME_TENTH_POS];

    fde->DIR_CrtTime = MAKEFATUINT16((buf)[FDE_DIR_CRT_TIME_POS],
                                     (buf)[FDE_DIR_CRT_TIME_POS+1]);
    
    fde->DIR_CrtDate = MAKEFATUINT16((buf)[FDE_DIR_CRT_DATA_POS],
                                     (buf)[FDE_DIR_CRT_DATA_POS+1]);

    fde->DIR_LstAccDate = MAKEFATUINT16((buf)[FDE_DIR_LST_ACC_DATA_POS],
                                        (buf)[FDE_DIR_LST_ACC_DATA_POS+1]);

    fde->DIR_FstClusHI = MAKEFATUINT16((buf)[FDE_DIR_FST_CLUS_HI_POS],
                                       (buf)[FDE_DIR_FST_CLUS_HI_POS+1]);

    fde->DIR_WrtTime = MAKEFATUINT16((buf)[FDE_DIR_WRT_TIME_POS],
                                     (buf)[FDE_DIR_WRT_TIME_POS+1]);

    fde->DIR_WrtDate = MAKEFATUINT16((buf)[FDE_DIR_WRT_DATA_POS],
                                     (buf)[FDE_DIR_WRT_DATA_POS+1]);

    fde->DIR_FstClusLO = MAKEFATUINT16((buf)[FDE_DIR_FST_CLUS_LO_POS],
                                       (buf)[FDE_DIR_FST_CLUS_LO_POS+1]);

    fde->DIR_FileSize = MAKEFATUINT32((buf)[FDE_DIR_FILE_SIZE_POS],
                                      (buf)[FDE_DIR_FILE_SIZE_POS+1],
                                      (buf)[FDE_DIR_FILE_SIZE_POS+2],
                                      (buf)[FDE_DIR_FILE_SIZE_POS+3]);
}

VOID Buf2FLDE(UINT8 *buf, FAT_LONG_DIR_ENTRY *flde)
{ 
    flde->LDIR_Ord = buf[FLDE_LDIR_ORD_POS];
    DSM_MemCpy(flde->LDIR_Name1,&buf[FLDE_LDIR_NAME1_POS],FLDE_LDIR_NAME1_SIZE);
    fat_name_change(flde->LDIR_Name1,FLDE_LDIR_NAME1_SIZE);

    flde->LDIR_Attr = buf[FLDE_LDIR_ATTR_POS];
    flde->LDIR_Type = buf[FLDE_LDIR_TYPE_POS];
    flde->LDIR_Chksum= buf[FLDE_LDIR_CHKSUM_POS];
    DSM_MemCpy(flde->LDIR_Name2,&buf[FLDE_LDIR_NAME2_POS],FLDE_LDIR_NAME2_SIZE);
    fat_name_change(flde->LDIR_Name2,FLDE_LDIR_NAME2_SIZE);

    flde->LDIR_FstClusLO = MAKEFATUINT16(buf[FLDE_LDIR_FSTCLUSLO_POS],
                                         buf[FLDE_LDIR_FSTCLUSLO_POS]);
    DSM_MemCpy(flde->LDIR_Name3,&buf[FLDE_LDIR_NAME3_POS],FLDE_LDIR_NAME3_SIZE);
    fat_name_change(flde->LDIR_Name3,FLDE_LDIR_NAME3_SIZE);
}

VOID FLDE2Buf(FAT_LONG_DIR_ENTRY *flde,UINT8 *buf)
{
    DSM_MemSet(buf,0,FLDE_LONG_ENTRY_SIZE);
    buf[FLDE_LDIR_ORD_POS] = flde->LDIR_Ord;
    DSM_MemCpy(&buf[FLDE_LDIR_NAME1_POS], flde->LDIR_Name1, FLDE_LDIR_NAME1_SIZE);
    buf[FLDE_LDIR_ATTR_POS] = flde->LDIR_Attr;
    buf[FLDE_LDIR_TYPE_POS] = flde->LDIR_Type;
    buf[FLDE_LDIR_CHKSUM_POS] = flde->LDIR_Chksum;
    DSM_MemCpy(&buf[FLDE_LDIR_NAME2_POS],flde->LDIR_Name2,FLDE_LDIR_NAME2_SIZE);
    DSM_MemCpy(&buf[FLDE_LDIR_FSTCLUSLO_POS],&(flde->LDIR_FstClusLO),FLDE_LDIR_FSTCLUSLO_SIZE);    
    DSM_MemCpy(&buf[FLDE_LDIR_NAME3_POS],flde->LDIR_Name3,FLDE_LDIR_NAME3_SIZE);
}   
    
INT32 fat_GetFATSz(UINT32 iMatedType,
                   UINT32 iTotalSec,
                   UINT32 iSecPerClus,
                   UINT32 iRootDirSec
                   )
{
    
    UINT32 iFATSz;
    UINT32 iResvdSec;
    UINT32 iDataSec;
    UINT32 iTmpVal1,iTmpVal2;

    if(FAT12 == iMatedType)
    {
        // reserved sector count
        iResvdSec = FAT_RESERVED_SECTOR_COUNT_16;
        
        // data sector count
        iDataSec = iTotalSec - (1 + FAT_FAT_COUNT +  iRootDirSec);
        iFATSz = ((iDataSec*3)/2)/DEFAULT_SECSIZE + 1;	
        //iFATSz = iFATSz > 0 ? iFATSz : 1;
    }
    else
    {
     
        iResvdSec = FAT32 == iMatedType ? FAT_RESERVED_SECTOR_COUNT_32 : FAT_RESERVED_SECTOR_COUNT_16;
        iTmpVal1 = iTotalSec - (iResvdSec + iRootDirSec);
        iTmpVal2 = (256 * iSecPerClus) + FAT_FAT_COUNT;
        // mated fat type   
        if(iMatedType == FAT32)
             iTmpVal2 = iTmpVal2 / 2;
        
        iFATSz = (iTmpVal1 + (iTmpVal2 - 1)) / iTmpVal2;
    }
   
     return iFATSz;
}


INT32 fat_GetRandNum()
{
	//return DSM_GetRandNum();
	return 730823;
}

VOID fat_GetFormatInfo(UINT32 iDiskTotSec,
            UINT8* vollab,
            UINT8 vollab_len,
            UINT8* oemname,
            UINT8 oemname_len,
            FAT_BOOT_RECORD *pFbr,
            UINT32* pType,
            UINT32* pDataClus,
            UINT32* pRootDirSects)
{	

	UINT32 iRootDirSec;
       UINT32 iFATSz;
	UINT32 iCountOfClus;
	UINT32 iDataSec;
	UINT32 iTotSec;
	UINT8 iSecPerClus;
       UINT32 iType =  0;

    iTotSec = iDiskTotSec;
    iType = fat_GetFatTypeForFormat(iDiskTotSec);
    if(FAT32 == iType)
    {
        iRootDirSec = 0;
   	    iSecPerClus = fat_GetSecNumPerClus_32(iDiskTotSec);
    }
    else
    {
        if(FAT16 == iType)
       {
             iRootDirSec = ((FAT_ROOT_ENTRY_COUNT_16 * 32) + (DEFAULT_SECSIZE- 1)) / DEFAULT_SECSIZE;
             iSecPerClus = fat_GetSecNumPerClus_16(iDiskTotSec);
       }
	else
       {
             iRootDirSec = ((FAT_ROOT_ENTRY_COUNT_12 * 32) + (DEFAULT_SECSIZE- 1)) / DEFAULT_SECSIZE;
             iSecPerClus = fat_GetSecNumPerClus_12(iDiskTotSec);
	}
    }
    iFATSz = fat_GetFATSz(iType,iTotSec,iSecPerClus,iRootDirSec);
    iDataSec = iTotSec - ( (FAT32 == iType ? FAT_RESERVED_SECTOR_COUNT_32 : 
                    FAT_RESERVED_SECTOR_COUNT_16)+ (FAT_FAT_COUNT * iFATSz) + iRootDirSec );
    iCountOfClus = iDataSec/iSecPerClus;    
    
    *pDataClus = iCountOfClus;
    *pType = iType;
      
    *pRootDirSects = iRootDirSec;

    // Set the fbr stuct.
    DSM_MemSet(pFbr,0,SIZEOF(FAT_BOOT_RECORD)); 	
    
    // Index  FieldName Offset Size(byte)
    // 1 BS_jmpBoot              0   3
    if(FAT32 == iType)
    {
        pFbr->BS_jmpBoot[0] = 0xEB;
        pFbr->BS_jmpBoot[1] = 0x58;
        pFbr->BS_jmpBoot[2] = 0x90;    
    }
    else
    {
        pFbr->BS_jmpBoot[0] = 0xEB;
        pFbr->BS_jmpBoot[1] = 0x3C;
        pFbr->BS_jmpBoot[2] = 0x90;
    }    
    
    // 2 BS_OEMName              3   8
    DSM_MemCpy(pFbr->BS_OEMName,oemname,oemname_len);   

    // 3 BPB_BytesPerSec        11   2
    pFbr->BPB_BytesPerSec = DEFAULT_SECSIZE; 

    // 4 BPB_SecPerClus         13   1
    pFbr->BPB_SecPerClus = iSecPerClus;

    // 5 BPB_RsvdSecCnt         14   2
    pFbr->BPB_RsvdSecCnt = (UINT16)((FAT32 == iType) ? \
              FAT_RESERVED_SECTOR_COUNT_32 : FAT_RESERVED_SECTOR_COUNT_16);

    // 6 BPB_RsvdSecCnt         16   1
    pFbr->BPB_NumFATs = FAT_FAT_COUNT;  

    // 7 BPB_RootEntCnt         17   2    
    if(FAT32 == iType)
    {
       pFbr->BPB_RootEntCnt = 0;	   
    }
    else if(FAT16 == iType)
    {
       pFbr->BPB_RootEntCnt = FAT_ROOT_ENTRY_COUNT_16;	   
    }
    else
    {
       pFbr->BPB_RootEntCnt = FAT_ROOT_ENTRY_COUNT_12;
    }

    // 8 pFbr->BPB_TotSec16     19   2
    pFbr->BPB_TotSec16 = (UINT16)((FAT32 == iType || 0x10000 <= iTotSec) ? 0 : iTotSec);

    // 9 BPB_Media              21   1
    pFbr->BPB_Media = 0xF8;   

    // 10 BPB_FATSz16           22   2 
    pFbr->BPB_FATSz16 = (UINT16)((FAT32 == iType) ? 0 : iFATSz);

    // 11 BPB_SecPerTrk         24   2    
    pFbr->BPB_SecPerTrk = 32; 

    // 12 BPB_NumHeads          26   2
    pFbr->BPB_NumHeads = 64; 

    // 13 BPB_HiddSec           28   4 
    pFbr->BPB_HiddSec = FAT_HIDE_SECTOR_COUNT;  

    // 14 BPB_TotSec32          32   4
    pFbr->BPB_TotSec32 = (FAT32 == iType || 0x10000 <= iTotSec) ? iTotSec : 0;

    // padded.
    pFbr->pad = 0x00;      
    
    if(FAT32 == iType)
    {
        // 15  BPB_FATSz32          36   4  
        pFbr->u.sub_rec32.BPB_FATSz32 = iFATSz;

        // 16  BPB_ExtFlags         40   2  
        pFbr->u.sub_rec32.BPB_ExtFlags = 0;

        // 17  BPB_FSVer            42   2
        pFbr->u.sub_rec32.BPB_FSVer = FAT_VER;  

        // 18  BPB_RootClus         44   4  
        pFbr->u.sub_rec32.BPB_RootClus = 2;

        // 19  BPB_FSInfo           48   2
        pFbr->u.sub_rec32.BPB_FSInfo = 1;

        // 20  BPB_BkBootSec        50   2
        pFbr->u.sub_rec32.BPB_BkBootSec = 6;

        // 21  BPB_Reserved         52   12  
        DSM_MemSet(pFbr->u.sub_rec32.BPB_Reserved,0,BPB_RESERVED_SIZE);

        // 22  BS_DrvNum            64   1
        pFbr->u.sub_rec32.BS_DrvNum = 0;

        // 23  BS_Reserved1         65   1
        pFbr->u.sub_rec32.BS_Reserved1 = 0;

        // 24  BS_BootSig           66   1
        pFbr->u.sub_rec32.BS_BootSig = 0x29;

        // 25  BS_VolID             67   4
        pFbr->u.sub_rec32.BS_VolID = fat_GetRandNum();

        // 26  BS_VolLab            71   11
        DSM_MemSet(pFbr->u.sub_rec32.BS_VolLab,' ',FBR_VOL_LAB_SIZE - 1);
        DSM_MemCpy(pFbr->u.sub_rec32.BS_VolLab,vollab,vollab_len);
        
        // 27  BS_FilSysType        82   8
        DSM_MemCpy(pFbr->u.sub_rec32.BS_FilSysType,"FAT32  ",FBR_FIL_SYS_TYPE_SIZE);
    }    
    else
    {
        // 15  BS_DrvNum            36   1
        pFbr->u.sub_rec1216.BS_DrvNum = 0;

        // 16  BS_Reserved1         37   1
        pFbr->u.sub_rec1216.BS_Reserved1 = 0; 

        // 17  BS_BootSig           38   1
        pFbr->u.sub_rec1216.BS_BootSig = 0x29;        

        // 18  BS_VolID             39   4
        pFbr->u.sub_rec1216.BS_VolID = fat_GetRandNum();

        // 19  BS_VolLab            43   11
        DSM_MemSet(pFbr->u.sub_rec1216.BS_VolLab,' ',FBR_VOL_LAB_SIZE - 1);
        DSM_MemCpy(pFbr->u.sub_rec1216.BS_VolLab,vollab,vollab_len);

        // 20  BS_FilSysType        54   8     
        if(FAT12 == iType)
        {
            DSM_MemCpy(pFbr->u.sub_rec1216.BS_FilSysType,"FAT12  ",FBR_FIL_SYS_TYPE_SIZE - 1); 
            // make to even.
            *pDataClus = (*pDataClus/2)*2;
        }
        else
        {
            DSM_MemCpy(pFbr->u.sub_rec1216.BS_FilSysType,"FAT16  ",FBR_FIL_SYS_TYPE_SIZE - 1);                
        }           
    }
}


UINT8 fat_GetFatType(UINT32 iCountOfClus)
{
    UINT8 iType;

    if(4085 > iCountOfClus)
    {
        iType = FAT12;
    }
    else if(65525 > iCountOfClus)
    {
        iType = FAT16;
    }
    else
    {
        iType = FAT32;
    }
    return iType;
}

UINT8 fat_GetFatTypeForFormat(UINT32 iDiskTotSec)
{
    UINT8 iType;

    if(8400 > iDiskTotSec)
    {
        iType = FAT12;
    }
    else if(1048576 > iDiskTotSec)
    {
        iType = FAT16;
    }
    else
    {
        iType = FAT32;
    }
    return iType;
}


UINT8 fat_GetSecNumPerClus_12(UINT32 iDiskTotSec)
{
    UINT32 iCountOfSec;
    UINT8 iSecPerCluster = 0;

    iCountOfSec = iDiskTotSec;
   
    // disks up to 4k, the 0 value for SecPerClusVal trips an error    
    if(iCountOfSec <= 8) 
    {
        iSecPerCluster = 0;
    }
    
    // disks up to 1 MB, 0.5K cluster
    else if(iCountOfSec > 8 && iCountOfSec <= 2100)
    {
        iSecPerCluster = 1;
    }

    // disks up to 2 MB, 1K cluster
    else if(iCountOfSec > 2100 && iCountOfSec <= 4200)
    {
        iSecPerCluster = 2;
    }
    
    // disks up to 4 MB, 2k cluster
    else if(iCountOfSec > 4200 && iCountOfSec <= 8400)
    {
        iSecPerCluster = 4;
    }    
    
    // any disk greater than 8MB, 0 value for SecPerClusVal trips an error
    else
    {
         iSecPerCluster = 0;
    }
    return iSecPerCluster;
}


UINT8 fat_GetSecNumPerClus_16(UINT32 iDiskTotSec)
{
    UINT32 iCountOfSec;
    UINT8 iSecPerCluster = 0;

    iCountOfSec = iDiskTotSec;
    
    // disks up to 4.1 MB, the 0 value for SecPerClusVal trips an error    
    if(iCountOfSec <= 8400) 
    {
        iSecPerCluster = 0;
    }
    
    // disks up to 16 MB, 1k cluster
    else if(iCountOfSec > 8400 && iCountOfSec <= 32680)
    {
        iSecPerCluster = 2;
    }
    // disks up to 128 MB, 2k cluster
    else if(iCountOfSec > 32680 && iCountOfSec <= 262144)
    {
        iSecPerCluster = 4;
    }
    // disks up to 256 MB, 4k cluster
    else if(iCountOfSec > 262144 && iCountOfSec <= 524288)
    {
        iSecPerCluster = 8;
    }
    // isks up to 512 MB, 8k cluster
    else if(iCountOfSec > 524288 && iCountOfSec <= 1048576)
    {
        iSecPerCluster = 16;
    }
    // disks up to 1 GB, 16k cluster'
    else if(iCountOfSec > 1048576 && iCountOfSec <= 2097152)
    {
        iSecPerCluster = 32;
    }
    // disks up to 2 GB, 32k cluste
    else if(iCountOfSec > 2097152 && iCountOfSec <= 4194304)
    {
        iSecPerCluster = 64;
    }
    // any disk greater than 2GB, 0 value for SecPerClusVal trips an error
    else if(iCountOfSec > 4194304 && iCountOfSec <= 0xFFFFFFFF)
    {
        iSecPerCluster = 0;
    }
 
    return iSecPerCluster;
}


UINT8 fat_GetSecNumPerClus_32(UINT32 iDiskTotSec)
{
    UINT32 iCountOfSec;
    UINT8 iSecPerCluster = 0;

    iCountOfSec = iDiskTotSec;
    // disks up to 32.5 MB, the 0 value for SecPerClusVal trips an error 
    if( iCountOfSec <= 66600) 
    {
        iSecPerCluster = 0;
    }
    // disks up to 260 MB, .5k cluster
    else if(iCountOfSec > 66600 && iCountOfSec <= 532480)
    {
        iSecPerCluster = 1;
    }
    // disks up to 8 GB, 4k cluster
    else if(iCountOfSec > 532480 && iCountOfSec <= 16777216)
    {
        iSecPerCluster = 8;
    }
    // disks up to 16 GB, 8k cluster
    else if(iCountOfSec >= 16777216 && iCountOfSec <= 33554432)
    {
        iSecPerCluster = 16;
    }
    // disks up to 32 GB, 16k cluster
    else if(iCountOfSec >= 33554432 && iCountOfSec <= 67108864)
    {
        iSecPerCluster = 32;
    }    
    // disks greater than 32GB, 32k cluster
    else if(iCountOfSec >= 67108864 && iCountOfSec <= 0xFFFFFFFF)
    {
        iSecPerCluster = 64;
    }
    return iSecPerCluster;
}

//********************************************************************************
// Function:
//   This function read the sb information.
// Parameter:
//   sb[out], output the sb information.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      _ERR_FAT_BPB_ERROR:          The BPB Struct is error.
//      _ERR_FAT_MALLOC_FAILED:      allocte memory failed.
//      _ERR_FAT_NOT_SUPPORT:        not support the fat type.
//*******************************************************************************
INT32 fat_read_sb_info(struct super_block * sb)
{
	FAT_SB_INFO* sb_info;
	FAT_BOOT_RECORD boot_rec;
	UINT8* secbuf;
	UINT32 TotSec;
	UINT32 DataSec;
	UINT32 CountofClusters;
	UINT16 iTrailSig;
	INT32 iResult;
	INT32 iRet;
	// BOOL bIsFAT32 = FALSE;
	UINT32 nSecNo = 0;
	MBR_BOOT_RECORD   mbr_rec;
    
	sb_info = &(sb->u.fat_sb);
	sb_info->sb = sb;
	iRet = _ERR_FAT_SUCCESS;

	secbuf = FAT_SECT_BUF_ALLOC();
	if(NULL == secbuf)
	{
		D((DL_FATERROR, "fat_read_sb_info: FAT_SECT_BUF_ALLOC failed."));
              DSM_ASSERT(0,"fat_read_sb_info: FAT_SECT_BUF_ALLOC failed.");
		return _ERR_FAT_MALLOC_FAILED;        
	}	   

	 // Check the sector 0 to find the MBR.    
        iResult = DRV_BLOCK_READ( sb->s_dev, nSecNo, secbuf );      
        if(_ERR_FAT_SUCCESS != iResult)
        {
            D((DL_FATERROR, "fat_read_sb_info(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
            iRet = _ERR_FAT_READ_SEC_FAILED;
            goto end;
        } 
		
        // check the boot trailsig--'55aa'			
	iTrailSig = 0;
	DSM_MemCpy(&iTrailSig,(secbuf + DEFAULT_SECSIZE - SIZEOF(UINT16)),SIZEOF(UINT16));

	if(FAT_FSI_TRAILSIG != iTrailSig) 
	{         
		D((DL_FATWARNING, "Search boot sector,boot trailsig error,maybe hide sector.iTrailSig = 0x%x,Sector No = %d",iTrailSig,nSecNo));					
		iRet = _ERR_FAT_BPB_ERROR;
		goto end; 
	} 

	//check MBR 
	Buf2MBR(secbuf, &mbr_rec);
	if(((mbr_rec.active_partition == 0x00)       ||
            (mbr_rec.active_partition == 0x80))          &&
           ((mbr_rec.part_type_indicator == 0x01) ||
            (mbr_rec.part_type_indicator == 0x06) ||
            (mbr_rec.part_type_indicator == 0x0b) ||
            (mbr_rec.part_type_indicator == 0x0c) ||
            (mbr_rec.part_type_indicator == 0x0e) ||
            (mbr_rec.part_type_indicator == 0x04))    &&
            (mbr_rec.sectors_preceding > 0) &&
            (mbr_rec.setcors_partion > 0)
         )
	{
		 nSecNo = mbr_rec.sectors_preceding;
		 iResult = DRV_BLOCK_READ( sb->s_dev, nSecNo, secbuf );      
	        if(_ERR_FAT_SUCCESS != iResult)
	        {
	            D((DL_FATERROR, "fat_read_sb_info(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
	            iRet = _ERR_FAT_READ_SEC_FAILED;
	            goto end;
	        } 
			
		iTrailSig = 0;
		DSM_MemCpy(&iTrailSig,(secbuf + DEFAULT_SECSIZE - SIZEOF(UINT16)),SIZEOF(UINT16));

		if(FAT_FSI_TRAILSIG != iTrailSig) 
		{         
			D((DL_FATWARNING, "Search boot sector,boot trailsig error,maybe hide sector.iTrailSig = 0x%x,Sector No = %d",iTrailSig,nSecNo));					
			iRet = _ERR_FAT_BPB_ERROR;
			goto end; 
		} 
	}
       else
       {           
            if(!((mbr_rec.active_partition == 0x00) ||
               (mbr_rec.active_partition == 0x80)))
            {
                 CSW_TRACE(BASE_DSM_TS_ID,"read sb info:active_parition = 0x%x,",                                           
                                            mbr_rec.active_partition
                                            );
            }

            if(!((mbr_rec.part_type_indicator == 0x01) ||
               (mbr_rec.part_type_indicator == 0x06) ||
               (mbr_rec.part_type_indicator == 0x0b) ||
               (mbr_rec.part_type_indicator == 0x0c) ||
               (mbr_rec.part_type_indicator == 0x0e)))
            {
                 CSW_TRACE(BASE_DSM_TS_ID,"read sb info:part_type_indicator = 0x%x,",                                           
                                            mbr_rec.part_type_indicator
                                            );
            }
            if(!((mbr_rec.part_type_indicator == 0x01) ||
               (mbr_rec.part_type_indicator == 0x06) ||
               (mbr_rec.part_type_indicator == 0x0b) ||
               (mbr_rec.part_type_indicator == 0x0c) ||
               (mbr_rec.part_type_indicator == 0x0e)))
            {
                 CSW_TRACE(BASE_DSM_TS_ID,"read sb info:part_type_indicator = 0x%x,",                                           
                                            mbr_rec.part_type_indicator
                                            );
            }
            if(!(mbr_rec.sectors_preceding > 0))
            {
                 CSW_TRACE(BASE_DSM_TS_ID,"read sb info:sectors_preceding = 0x%x,",                                           
                                            mbr_rec.sectors_preceding
                                            );
            }
            if(!(mbr_rec.setcors_partion > 0))
            {
                 CSW_TRACE(BASE_DSM_TS_ID,"read sb info:setcors_partion = 0x%x,",                                           
                                            mbr_rec.setcors_partion
                                            );
            }            
       }
       
       
	Buf2FBR(secbuf, &boot_rec);
       if(nSecNo == 0)
       {
           boot_rec.BPB_HiddSec = 0;
       }
	D((DL_FATDETAIL, "BPB_BytesPerSec = %d,",boot_rec.BPB_BytesPerSec));
	D((DL_FATDETAIL, "BPB_TotSec32 = %d,",boot_rec.BPB_TotSec32));
	D((DL_FATDETAIL, "BPB_TotSec16 = %d,",boot_rec.BPB_TotSec16));
	D((DL_FATDETAIL, "BPB_RsvdSecCnt = %d,",boot_rec.BPB_RsvdSecCnt));
	D((DL_FATDETAIL, "BPB_RootEntCnt = %d,",boot_rec.BPB_RootEntCnt));
	D((DL_FATDETAIL, "BPB_FATSz16 = %d,",boot_rec.BPB_FATSz16));
	D((DL_FATDETAIL, "BPB_FATSz32 = %d,",boot_rec.u.sub_rec32.BPB_FATSz32));
	D((DL_FATDETAIL, "BPB_SecPerTrk = %d,",boot_rec.BPB_SecPerTrk));
	D((DL_FATDETAIL, "BPB_NumHeads = %d,",boot_rec.BPB_NumHeads));
	D((DL_FATDETAIL, "BPB_SecPerClus = %d,",boot_rec.BPB_SecPerClus));
	D((DL_FATDETAIL, "BPB_NumFATs = %d,",boot_rec.BPB_NumFATs));
	D((DL_FATDETAIL, "BPB_Media = %d.",boot_rec.BPB_Media));
#if 0    
       if(DSM_GetDevType(sb_info->sb->s_dev) == DSM_MEM_DEV_TFLASH)
       {
            sxr_Sleep(100);
            CSW_TRACE(BASE_DSM_TS_ID,"BPB_BytesPerSec = %d,",boot_rec.BPB_BytesPerSec);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_TotSec32 = %d,",boot_rec.BPB_TotSec32);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_TotSec16 = %d,",boot_rec.BPB_TotSec16);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_RsvdSecCnt = %d,",boot_rec.BPB_RsvdSecCnt);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_RootEntCnt = %d,",boot_rec.BPB_RootEntCnt);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_FATSz16 = %d,",boot_rec.BPB_FATSz16);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_FATSz32 = %d,",boot_rec.u.sub_rec32.BPB_FATSz32);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_SecPerTrk = %d,",boot_rec.BPB_SecPerTrk);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_NumHeads = %d,",boot_rec.BPB_NumHeads);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_SecPerClus = %d,",boot_rec.BPB_SecPerClus);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_NumFATs = %d,",boot_rec.BPB_NumFATs);
            CSW_TRACE(BASE_DSM_TS_ID, "BPB_Media = %d.",boot_rec.BPB_Media);       
            sxr_Sleep(100);
       }
#endif

	if(DEFAULT_SECSIZE != boot_rec.BPB_BytesPerSec
						|| 0 == boot_rec.BPB_TotSec32+boot_rec.BPB_TotSec16
						|| boot_rec.BPB_RsvdSecCnt < 1            
						||(boot_rec.BPB_FATSz16 == 0 && boot_rec.u.sub_rec32.BPB_FATSz32 == 0)
						// || 0 == boot_rec.BPB_SecPerTrk 
						// || 0 == boot_rec.BPB_NumHeads
						|| boot_rec.BPB_SecPerClus < 1
						|| boot_rec.BPB_NumFATs < 1
						|| 0 == boot_rec.BPB_Media 
						)	      //||nSecNo != boot_rec.BPB_HiddSec
	{
			D((DL_FATERROR, "fat_read_sb_info(),get hidden sector number error. ErrorCode = %d",iResult));
			iRet = _ERR_FAT_BPB_ERROR;
			goto end;				
	}

        // Set the sb_info begin.
         if(0 == boot_rec.BPB_HiddSec)
         {
             boot_rec.BPB_HiddSec += nSecNo;    
         }
        
        sb_info->iHiddSec = boot_rec.BPB_HiddSec;
        sb_info->iFATStartSec = boot_rec.BPB_RsvdSecCnt + boot_rec.BPB_HiddSec; 
        sb_info->iRsvdSecCnt  = boot_rec.BPB_RsvdSecCnt;
        sb_info->iBytesPerSec = boot_rec.BPB_BytesPerSec;
        sb_info->iSecPerClus  = boot_rec.BPB_SecPerClus;
        sb_info->iRootEntCnt  = boot_rec.BPB_RootEntCnt;
	 // support long name. 	
        sb_info->iSuppLName = FAT_SUPPORT_LONG_NAME;
  	
        if(0 != boot_rec.BPB_FATSz16) // FAT12/FAT16
        {
            if(boot_rec.BPB_RootEntCnt < DEFAULT_SECSIZE/DEFAULT_DIRENTRYSIZE)
            {
                D((DL_FATERROR, "fat_read_sb_info(),boot_rec.BPB_RootEntCnt_1 = 0x%x",boot_rec.BPB_RootEntCnt));
                iRet = _ERR_FAT_BPB_ERROR;
                goto end;
            }      
            sb_info->iFATSize = boot_rec.BPB_FATSz16;
            sb_info->iFat32RootClus = 0;
	        sb_info->iRootDirStartSec = boot_rec.BPB_RsvdSecCnt +
							     boot_rec.BPB_HiddSec +
							     (boot_rec.BPB_NumFATs * sb_info->iFATSize);
            
	        sb_info->iFATn = DSM_StrNCaselessCmp(boot_rec.u.sub_rec1216.BS_FilSysType,"FAT16",5) == 0 ? FAT16 : FAT12;
        }
        else // FAT32      
        {
             if(0 != boot_rec.BPB_RootEntCnt)
            {
                D((DL_FATERROR, "fat_read_sb_info(),boot_rec.BPB_RootEntCnt_0 = 0x%x",boot_rec.BPB_RootEntCnt));
                iRet = _ERR_FAT_BPB_ERROR;
                goto end;
            }                  
            sb_info->iFATSize = boot_rec.u.sub_rec32.BPB_FATSz32;
            sb_info->iFat32RootClus = boot_rec.u.sub_rec32.BPB_RootClus  ;
   	     sb_info->iRootDirStartSec = 0;	    	 
		 	                                    // sb_info->iFat32RootClus + boot_rec.BPB_RsvdSecCnt + 
   	                                                  // boot_rec.BPB_HiddSec +
                                                         // (boot_rec.BPB_NumFATs * sb_info->iFATSize) ;
            // bIsFAT32 = TRUE;
            sb_info->iFATn = FAT32;												
			
        }
		
	    sb_info->iFAT2StartSec = sb_info->iFATStartSec + sb_info->iFATSize;		

        if(0 != boot_rec.BPB_TotSec16)
        {
            TotSec = boot_rec.BPB_TotSec16;
        }
        else if( 0 != boot_rec.BPB_TotSec32)
        {
            TotSec = boot_rec.BPB_TotSec32;
        }
        else
        {
            D((DL_FATERROR, "fat_read_sb_info(),boot_rec.BPB_TotSec16 = 0x%x,boot_rec.BPB_TotSec32 = 0x%x",boot_rec.BPB_TotSec16,boot_rec.BPB_TotSec32));
            iRet = _ERR_FAT_BPB_ERROR;
            goto end;
        }
	 
        
        sb_info->iRootDirSecCnt = 
            ((boot_rec.BPB_RootEntCnt * 32) + (boot_rec.BPB_BytesPerSec - 1)) 
            / boot_rec.BPB_BytesPerSec;

        sb_info->iDataStartSec = boot_rec.BPB_RsvdSecCnt + 
   	     boot_rec.BPB_HiddSec +
            (boot_rec.BPB_NumFATs * sb_info->iFATSize) + 
            sb_info->iRootDirSecCnt ;
        
        //DataSec = TotSec - sb_info->iDataStartSec;   
        // TotSec is the sector count on this partition,unincluded hide or other partition sector count.
        DataSec = TotSec - sb_info->iDataStartSec + boot_rec.BPB_HiddSec;
        CountofClusters = DataSec / boot_rec.BPB_SecPerClus;

        sb_info->iFATn = sb_info->iFATn == FAT32 ? FAT32 : fat_GetFatType(CountofClusters);
        
        D((DL_FATDETAIL, "CountofClusters = %d,",CountofClusters));
        D((DL_FATDETAIL, "DataSec = %d,",DataSec));   
        D((DL_FATDETAIL, "sb_info->iFATn = %d,",sb_info->iFATn)); 
        
 	 // if fat type is fat12, make to even.	
	    sb_info->iDataSec = (sb_info->iFATn == FAT12) ? (DataSec/2)*2 : DataSec; 	
        // Set the sb_info End.
#if 0    
      if(DSM_GetDevType(sb_info->sb->s_dev) == DSM_MEM_DEV_TFLASH)
       {
           sxr_Sleep(100);	     
	      CSW_TRACE(BASE_DSM_TS_ID,"sb:");
		 CSW_TRACE(BASE_DSM_TS_ID,"iHiddSec = 0x%x,",sb_info->iHiddSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFATStartSec = 0x%x,",sb_info->iFATStartSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFAT2StartSec = 0x%x,",sb_info->iFAT2StartSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFATSize = 0x%x,",sb_info->iFATSize);
		  sxr_Sleep(100);	
		 CSW_TRACE(BASE_DSM_TS_ID,"iRootDirStartSec = 0x%x,",sb_info->iRootDirStartSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iDataStartSec = 0x%x,",sb_info->iDataStartSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iRootDirSecCnt = 0x%x,",sb_info->iRootDirSecCnt);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFATn = 0x%x,",sb_info->iFATn);
		 CSW_TRACE(BASE_DSM_TS_ID,"iRsvdSecCnt = 0x%x,",sb_info->iRsvdSecCnt);
		  sxr_Sleep(100);	
		 CSW_TRACE(BASE_DSM_TS_ID,"iBytesPerSec = 0x%x,",sb_info->iBytesPerSec);
		 CSW_TRACE(BASE_DSM_TS_ID,"iRootEntCnt = 0x%x,",sb_info->iRootEntCnt);
		 CSW_TRACE(BASE_DSM_TS_ID,"iSecPerClus = 0x%x,",sb_info->iSecPerClus);
		 CSW_TRACE(BASE_DSM_TS_ID,"iSuppLName = 0x%x,",sb_info->iSuppLName);
		  sxr_Sleep(100);	
		 CSW_TRACE(BASE_DSM_TS_ID,"iDataSec = 0x%x,",sb_info->iDataSec    );
		 CSW_TRACE(BASE_DSM_TS_ID,"iFat32RootClus = 0x%x,",sb_info->iFat32RootClus);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFreeCluCnt = 0x%x,",sb_info->iFreeCluCnt);
		 CSW_TRACE(BASE_DSM_TS_ID,"iFreeCnterInitFlag = 0x%x,",sb_info->iFreeCnterInitFlag);	
               sxr_Sleep(100);
       }
#endif

        iRet = _ERR_FAT_SUCCESS;
        
 end:
    
    if(NULL != secbuf)
    {
        FAT_SECT_BUF_FREE((SECT_BUF*)secbuf);
    }
    return iRet;
}


//********************************************************************************
// Function:
//   This function read a sector in specify cluster.
// Parameter:
//   sb_info[in], Potint to the FAT_SB_INFO struct.
//   clunum[in], cluster number.
//   offset[in], offset of sector in the cluster.
//   secbuf[out], Output the sector data.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//*******************************************************************************
INT32 fat_read_cluster(FAT_SB_INFO *sb_info, UINT32 clunum, 
    UINT32 offset, UINT8 *secbuf)
{
    UINT32 rsecnum;
    INT32 iResult;
    INT32 iRet;

    
    if(NULL == secbuf)
    {
        D( ( DL_FATERROR, "In fat_read_cluster paramert error.secbuf == NULL.\n") );
        return _ERR_FAT_PARAM_ERROR;
    }
    
    if (clunum > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || clunum < 2 || offset >= sb_info->iSecPerClus)
    {
        D((DL_FATERROR, "fat_read_cluster: clunum = 0x%x, SecOffset = %d, _ERR_FAT_PARAM_ERROR!\n", clunum, offset));
        return _ERR_FAT_ERROR;
    }
    
    iRet = _ERR_FAT_SUCCESS;
 
     rsecnum = sb_info->iDataStartSec + ((clunum-2) * sb_info->iSecPerClus)+ offset;    

    

    iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, rsecnum, secbuf );
    if(_ERR_FAT_SUCCESS == iResult)
    {
        iRet = _ERR_FAT_SUCCESS;
         D( ( DL_FATDETAIL, 
            "sb_info->iSecPerClus = %d,clunum = %d,sb_info->iDataStartSec = %d.\n",
            sb_info->iSecPerClus,
            clunum,
            sb_info->iDataStartSec
            ));
    }
    else
    {
        iRet = iResult;
        D( ( DL_FATERROR, "DRV_BLOCK_READ() failed!!! Local error code:%d,\n",iResult) );
        D( ( DL_FATERROR, "sb_info->sb->s_dev = %d, rsecnum = %d, secbuf = 0x%x,dev handle = 0x%x.\n",
            sb_info->sb->s_dev, 
            rsecnum, 
            secbuf,
            g_pDevDescrip[ sb_info->sb->s_dev ].dev_handle));
        D( ( DL_FATERROR, 
            "sb_info->iSecPerClus = %d,clunum = %d,sb_info->iDataStartSec = %d,offset = %d...\n",
            sb_info->iSecPerClus,
            clunum,
            sb_info->iDataStartSec,
            offset));      
        D( ( DL_FATERROR, 
            "sb_info->iDataSec = %d,sb_info->iDataStartSec = %d,max data sector = %d.\n",
            sb_info->iDataSec,
            sb_info->iDataStartSec,
            sb_info->iDataSec + sb_info->iDataStartSec));       
    }
    return iRet;
}


//********************************************************************************
// Function:
//   This function write a sector in specify cluster.
// Parameter:
//   sb_info[in], Potint to the FAT_SB_INFO struct.
//   clunum[in], cluster number.
//   offset[in], offset of sector in the cluster.
//   secbuf[in], The sector data.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_WRITE_SEC_FAILED:    Write the sector failed.
//*******************************************************************************
INT32 fat_write_cluster(FAT_SB_INFO *sb_info, UINT32 clunum, 
    UINT32 offset, UINT8 *secbuf)
{
    INT32 iResult;
    INT32 iRet;
    UINT32 wsecnum;
    UINT32 iTime = 0;

    if(NULL == secbuf)
    {
        D( ( DL_FATERROR, "In fat_write_cluster paramert error.secbuf == NULL.\n") );
        return _ERR_FAT_PARAM_ERROR;
    }

    if (clunum > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || clunum < 2 || offset >= sb_info->iSecPerClus)
    {
        D((DL_FATERROR, "fat_write_cluster: clunum = 0x%x, SecOffset = %d, _ERR_FAT_PARAM_ERROR!\n", clunum, offset));
        DSM_HstSendEvent(0xca000001);
		DSM_HstSendEvent(clunum);
		DSM_HstSendEvent(((sb_info->iDataSec / sb_info->iSecPerClus) + 1));
		DSM_HstSendEvent(offset);
		return _ERR_FAT_ERROR;
    }
    
    iRet = _ERR_FAT_SUCCESS;
    wsecnum = sb_info->iDataStartSec + (clunum-2) * sb_info->iSecPerClus + offset;
   do
    {
        iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, wsecnum, secbuf );
        if(_ERR_FAT_SUCCESS == iResult)
        {
            iRet = _ERR_FAT_SUCCESS;
            break;
        }
        else
        {
            DSM_HstSendEvent(0xca000002);
			DSM_HstSendEvent(iResult);
			DSM_HstSendEvent(0-iResult);
            D( ( DL_FATERROR, "In fat_write_cluster: DRV_BLOCK_WRITE() return failed. ErrorCode = %d\n",iResult) );
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
        }
        iTime ++;
    }while(iTime <= 3);
    return iRet;    
}

//********************************************************************************
// Function:
//   This function revert a sector in specify cluster.
// Parameter:
//   sb_info[in], Potint to the FAT_SB_INFO struct.
//   clunum[in], cluster number.
//   offset[in], offset of sector in the cluster.
//   secbuf[in], The sector data.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_WRITE_SEC_FAILED:    Write the sector failed.
//*******************************************************************************
INT32 fat_revert_cluster(FAT_SB_INFO *sb_info, UINT32 clunum, 
    UINT32 offset)
{
    INT32 iResult;
    INT32 iRet;
    UINT32 wsecnum;
    UINT32 iTime = 0;

  
    if (clunum > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || clunum < 2 || offset >= sb_info->iSecPerClus)
    {
        D((DL_FATERROR, "fat_revert_cluster: clunum = 0x%x, SecOffset = %d, _ERR_FAT_PARAM_ERROR!\n", clunum, offset));
        return _ERR_FAT_ERROR;
    }
    
    iRet = _ERR_FAT_SUCCESS;
    wsecnum = sb_info->iDataStartSec + (clunum-2) * sb_info->iSecPerClus + offset;
   do
    {
        iResult = DRV_BLOCK_REVERT( sb_info->sb->s_dev, wsecnum);
        if(_ERR_FAT_SUCCESS == iResult)
        {
            iRet = _ERR_FAT_SUCCESS;
            break;
        }
        else
        {
            D( ( DL_FATERROR, "In fat_revert_cluster: DRV_BLOCK_REVERT() return failed. err_code = %d\n",iResult) );
            iRet = _ERR_FAT_REVERT_SEC_FAILED;
        }
        iTime ++;
    }while(iTime <= 1);
    return iRet;    
}


//********************************************************************************
// Function:
//   retrieve directory entry according the entry name.
// Parameter:
//   sb_info[in],  the pointer to the FAT_SB_INFO struct.
//   startclu[in], the looking up start cluster.
//   filename[in][out], the entry name to be looked up.
//   d_entry[out], the directory entry content retrieved.
//   bsec[out],    the sector number where the 'd_entry' stays.
//   eoff[out],    the entry offset in the sector where the 'd_entry' stays.
//   decount[out], the entry count,include the short entry and long entries.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//       And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_PARAM_ERROR:        The input paramert error. 
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      _ERR_FAT_MALLOC_FAILED:      Memory alloc failed.
//      _ERR_FAT_FILE_NOT_EXISTED    The file or dir not existed.
//*******************************************************************************
INT32 fat_lookup_entry(FAT_SB_INFO* sb_info,
                       UINT32 startclu, 
                       WCHAR* filename, 
                       FAT_DIR_ENTRY* d_entry, 
                       UINT32* bclu, 
                       UINT16* bsec, 
                       UINT16* eoff,
                       UINT16* decount,
                       UINT32 iFlag,
                       UINT32 strtype)
{
          
    INT32 iResult;                    // call function result.
    INT32 iRet = _ERR_FAT_FILE_NOT_EXISTED;    // Retrun value.
    
    UINT32 iCurClus = 0;              // Current cluster.
    UINT32 iBegClus = 0;              // Begin cluster.
    UINT16 iBegSec = 0;               // 指sector在整个磁盘上的index；

    UINT16 iCurSec = 0;               // Current sector.
    UINT16 iBegEOff = 0;              // Begin Entry offset.
    UINT16 iCurEOff = 0;              // Current entry offset.
    UINT32 iMaxSecCount;              // max sector count to read.
    UINT8 pSecBuf[DEFAULT_SECSIZE]; 
    UINT8  szFatName[FAT_MAX_NAME_LEN + 1];      // Fat name buffer.
    UINT8  szUseName[FAT_MAX_USER_NAME_LEN + 1]; // Use name buffer.   
    WCHAR* pszUniName = NULL;                    // Point to unicode name,need malloc.  
    UINT8* pShortOemName = NULL;                    // Point to unicode name,need malloc.  
    UINT8* pShortUniName = NULL;                   // Point to the unicode short name, malloc in DSM_OEM2Uincode(), need free.
    UINT32 iULen = 0;                            // Unicode name length. 
    WCHAR* pFile = NULL;                         // Point to file name to storage the input file name(long), need malloc.

    FAT_DIR_ENTRY  sDe;                          // to storage the entry(long or short).
    FAT_LONG_DIR_ENTRY* psLDe =NULL;            // Point to a long dir entry struct.
    UINT8 iCheckSum = 0;                         // check sum number. 
    BOOL bLMatch = FALSE;                        // long name is matched.
    UINT16 iLCount = 0;                          // long dir entry count.
    UINT32 iLIndex = 0;                          // long dir entry ord index.
    UINT32 i = 0;   
   // INT32 tst_result = 0;
   
    psLDe = (FAT_LONG_DIR_ENTRY*)&sDe;
    *decount = 0;
    iCurClus = startclu;
    iBegClus = iCurClus;
    iBegSec = *bsec;
    iCurEOff = *eoff;
    
    // Check input paramert.
    if (FAT32 == sb_info->iFATn)
    {       
        if(fat_is_last_cluster(sb_info, startclu) || 2 > startclu )
        {
            D( ( DL_FATERROR, "In fat_lookup_entry paramert1 error.startclu  == %d\n",startclu) );
            return _ERR_FAT_PARAM_ERROR;
        }
    }
    else
    {
        if(fat_is_last_cluster(sb_info, startclu) || 1 == startclu)
        {
            D( ( DL_FATERROR, "In fat_lookup_entry paramert2 error.startclu  == %d\n",startclu) );
            return _ERR_FAT_PARAM_ERROR;
        }
    }

    // look sub dir iMaxSector 
    // look up fat32 root dir.
    if(FAT32 == sb_info->iFATn || 2 <= startclu) 
    {
        iMaxSecCount = sb_info->iSecPerClus;  
        iBegSec = *bsec;
    }
    else // root dir for FAT12 | FAT16.
    {
        if(sb_info->iBytesPerSec == 0)
        {
             D( ( DL_FATERROR,
            		"in fat_lookup_entry() sb_info->iBytesPerSec = %d.\n",
            		sb_info->iBytesPerSec)); 
             return _ERR_FAT_PARAM_ERROR;
        }
        iMaxSecCount = sb_info->iRootDirStartSec + sb_info->iRootDirSecCnt;//((sb_info->iRootEntCnt * DEFAULT_DIRENTRYSIZE)/sb_info->iBytesPerSec);
        iBegSec = (UINT16)(sb_info->iRootDirStartSec + *bsec); 
    }

    iCurSec = iBegSec;
    do
    {   
        // loop read sector,if root dir, loop count = 32 else loop count = secPerClus.
        for(iCurSec = iCurSec; iCurSec < iMaxSecCount; iCurSec++)
        {
            
            DSM_MemSet(pSecBuf, 0x00, DEFAULT_SECSIZE);
            if(FAT32 == sb_info->iFATn || 2 <= startclu) // FAT12/FAT16 sub dir or FAT32 dir.
            {
               iResult = fat_read_cluster(sb_info, iCurClus, (UINT8)iCurSec, pSecBuf);                
            }
            else // FAT12/FAT16 root dir    
            {
             //cluster是就fat的File And Directory region而言的。对Root Directory Region
             //没有cluster的概念
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, iCurSec, pSecBuf);
            }
            
            if(_ERR_FAT_SUCCESS != iResult)
            {                
                D( ( DL_FATERROR, "In fat_lookup_entry DRV_BLOCK_READ error ErrorCode = %d \n",iResult) );
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end_lookup;
            }  
            
            // 从当前entry开始遍历本sector中的诸dir-entry.
            for(i = iCurEOff; i < DEFAULT_SECSIZE; i += DEFAULT_DIRENTRYSIZE)
            {
                if(FAT_IS_FREE_ENTRY(pSecBuf+i)) // free entry, continue.
                {
                    DSM_MemSet(d_entry, 0x00, SIZEOF(*d_entry));
                    D( ( DL_FATBRIEF, "In fat_lookup_entry FAT_IS_FREE_ENTRY : i = 0x%x\n",i) );
                    iRet = _ERR_FAT_FILE_NOT_EXISTED;
                    goto end_lookup;
                }
                else if(FAT_IS_DELETE_ENTRY(pSecBuf+i)) // delete entry continue.         
                {   
                    iLIndex = 0;
                    iLCount = 0;
                    bLMatch = FALSE;                   
                    continue;
                }
                else if(FAT_IS_SHORT_ENTRY(pSecBuf+i))   // short entry  
                {
                    if(FAT_IS_JAP_DELETE_ENTRY(pSecBuf + i)) // Japonic delete entry continue
                    {
                        iLIndex = 0;
                        iLCount = 0;
                        bLMatch = FALSE;
                        continue;
                    }
                    Buf2FDE((UINT8*)(pSecBuf+i), d_entry);
                          
                    DSM_MemCpy(szFatName, d_entry->DIR_Name, FAT_MAX_NAME_LEN);
                    szFatName[FAT_MAX_NAME_LEN] = '\0';                     
                    
                    if(_ERR_FAT_SUCCESS != NameFat2User(szFatName,szUseName,*(pSecBuf+i + 0x0c)))
                    {
                        iLIndex = 0;
                        iLCount = 0;
                        bLMatch = FALSE;
                        continue;                        
                    }

                    // Long name matching
                    if(TRUE == bLMatch)
                    {
                        // Check the chksum. check pass,output the long name.
                        if(iCheckSum == fat_ChkSum(szFatName))                        
                        {
                            if(iFlag & FAT_LOOK_GET_NAME)
                            {
    				  	      DSM_TcStrCpy(filename, pszUniName); 
                            }
                            *decount = (UINT16)(iLCount + 1);  
                            iRet = _ERR_FAT_SUCCESS;
                            goto end_lookup;
                        }
                        // check not pass,indicated the long name is invalid,
                        // will to match the short name.                        
                    }
                    else
                    {
                        if(iLCount > 0 && FAT_STR_TYPE_UNICODE == strtype)
                        {                            
                            iLIndex = 0;
                            iLCount = 0;
                            bLMatch = FALSE;      
                            continue; 
                        }

                    }
                    // make the input name to short name,to match.                      
                    if(NULL == pFile)
                    {
                        pFile = NAME_BUF_ALLOC();     
                        if(NULL == pFile)
                        {
                            D( ( DL_FATERROR, "In fat_lookup_entry 1. malloc pFile failed.") ); 
                            DSM_ASSERT(0, "In fat_lookup_entry 1. malloc pFile failed.");
                            iRet = _ERR_FAT_MALLOC_FAILED;                            
                            goto end_lookup;
                        }
                        else
                        {
                            if(FAT_STR_TYPE_UNICODE == strtype)
                            {
                                DSM_TcStrCpy(pFile, filename); 
                                // fat_Trim(pFile);
                            }
                            else
                            {
                                DSM_StrCpy(pFile, filename); 
                                // fat_TrimOem((UINT8*)pFile);
                            }
                        }
                    }
                    
                    if(FAT_STR_TYPE_UNICODE == strtype)
                    {       
                        
                        // Because the point of pShortUniName malloced in DSM_OEM2Uincode, we need free it after called.                   
                        if(NULL == pShortOemName)
                        {                                         
                            iResult = DSM_Unicode2OEM((UINT8*)pFile, (UINT32)DSM_UnicodeLen((UINT8*)pFile), &pShortOemName, &iULen,NULL);
                            if(_ERR_FAT_SUCCESS != iResult)
                            {
                                D((DL_FATWARNING, "fat_lookup_entry: DSM_Unicode2Oem() failed-3."));
                                iLIndex = 0;
                                iLCount = 0;
                                bLMatch = FALSE;      
                                continue; 
                            }
                        }

                        iResult = pattern_match_oem((UINT8*)szUseName,(UINT8*)pShortOemName);
                        
                    }
                    else
                    {                        
                        iResult = pattern_match_oem((UINT8*)szUseName, (UINT8*)pFile);    
                    }                    
                    
                    // Short name matching 
                    if(PATTERN_CMP_MATCH == iResult)
                    {  
                        // if short name is match 
                        // long entry count equal to zero or check sum not pass,
                        // will output the short name else output long name.
                        if((0 == iLCount) ||
                           (iCheckSum != fat_ChkSum(szFatName)))
                        {                            
                            iBegSec = iCurSec;
                            iBegEOff = (UINT16)i;  
                            if(iFlag & FAT_LOOK_GET_NAME)
                            {                                
                                 if(NULL != pShortUniName)
                                 {
                                     DSM_Free(pShortUniName);
                                     pShortUniName = NULL;
                                 }  
                                 iResult = DSM_OEM2Uincode(szUseName, (UINT16)DSM_StrLen(szUseName), &pShortUniName, &iULen,NULL);
                                 if(_ERR_FAT_SUCCESS != iResult)
                                 {
                                     D((DL_FATWARNING, "fat_lookup_entry: DSM_OEM2Uincode() failed-2."));
                                     iLIndex = 0;
                                     iLCount = 0;
                                     bLMatch = FALSE;    
                                     continue; 
                                 }
                                 DSM_TcStrCpy((PWSTR)filename,(PCWSTR)pShortUniName);                                
                            }
                        }
                        else
                        {                            
                            if(0 == (iFlag & FAT_LOOK_EXISTED))
                            {
                                iLIndex = 0;
                                iLCount = 0;
                                bLMatch = FALSE;   
                                continue;
                            }                                                            
                        }                            
                        *decount = (UINT16)(iLCount + 1);
                        iBegClus = iCurClus;
                        iRet = _ERR_FAT_SUCCESS;
                        goto end_lookup;
                    }                      
                    // long name not matched and short name not matched,get next dir entry to match,
                    iLIndex = 0;
                    iLCount = 0;
                    bLMatch = FALSE;      
                }
                else if(FAT_IS_FIRST_LONG_ENTRY(pSecBuf+i) || FAT_IS_MIDDLE_LONG_ENTRY(pSecBuf+i) || FAT_IS_LAST_LONG_ENTRY(pSecBuf+i))
                {
                    // 如果系统不支持长文件名，跳过长目录项。
                    if(FAT_NOT_SUPPORT_LONG_NAME == sb_info->iSuppLName)
                    {
                        continue;
                    } 
                    // 将BUF转换为目录项结构
                    Buf2FLDE((UINT8*)(pSecBuf+i), psLDe);
                    if(iLCount == 0)
                    {
                        if(FAT_IS_LAST_LONG_ENTRY(pSecBuf+i))
                        {
                            iLCount = (UINT16)(psLDe->LDIR_Ord & 0x3f);   
                            if(0 == iLCount)
                            {
                                continue;
                            }

                            if(iLCount >= FAT_MAX_LONG_NAME_LEN /FAT_LONG_PER_ENTRY_NAME_LEN)
                            {
                                D( ( DL_FATERROR, "In fat_lookup_entry psLDe->LDIR_Ord  = 0x%x,error  \n",psLDe->LDIR_Ord) );
                                continue;
                            }
                            
                            // if pszUniName is null ,malloc 
                            if(NULL == pszUniName)
                            {
                                pszUniName = NAME_BUF_ALLOC();    
                                if(NULL == pszUniName)
                                {
                                    D( ( DL_FATERROR, "In fat_lookup_entry 2. malloc pszUniName failed \n") );
                                    DSM_ASSERT(0, "In fat_lookup_entry 2. malloc pszUniName failed.");
                                    iRet = _ERR_FAT_MALLOC_FAILED;
                                    goto end_lookup;
                                }
                            }                            
                            DSM_MemSet(pszUniName, 0, (FAT_MAX_LONG_NAME_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
                            
                            // 按照长目录项ord从大到小的次序组装长文件名.
                            iULen = fat_get_long_name(psLDe, (UINT8*)pszUniName);  
                            iLIndex = (psLDe->LDIR_Ord & 0x3f);                            
                            iLIndex --;
                            iBegSec = iCurSec;
                            iBegEOff = (UINT16)i;
                            bLMatch = FALSE;
                            iCheckSum = psLDe->LDIR_Chksum;
                            iBegClus = iCurClus;
                        }
                        else
                        {
                            iLIndex = 0;
                            iLCount = 0;
                            bLMatch = FALSE;
                            continue;
                        }
                    }
                    else
                    {
                        if(psLDe->LDIR_Ord != iLIndex || 
                           psLDe->LDIR_Ord == 0)
                        {
                            iLIndex = 0;
                            iLCount = 0;
                            bLMatch = FALSE;
                            continue;
                        }
                        if(iCheckSum != psLDe->LDIR_Chksum)
                        {
                            iLIndex = 0;
                            iLCount = 0;
                            bLMatch = FALSE;
                            continue;
                        }
                        
                        iULen = (UINT16)(iULen + fat_get_long_name(psLDe, (UINT8*)pszUniName));  
                        iLIndex--;                        
                    }

                    // parse the first long entry,to match long name.                 
                    if(0 == iLIndex)
                    {
                        fat_Trim(pszUniName);
                        
                        // malloc a buff to storage the file name.
                        // iLen =  DSM_StrLen(filename);
                        if(NULL == pFile)
                        {
                            pFile = NAME_BUF_ALLOC(); 
                            if(NULL == pFile)
                            {
                                D( ( DL_FATERROR, "In fat_lookup_entry 3 malloc pFile failed \n") );
                                DSM_ASSERT(0, "In fat_lookup_entry 3. malloc pFile failed.");
                                iRet = _ERR_FAT_MALLOC_FAILED;                            
                                goto end_lookup;
                            }                     
                            else
                            {                        
                                //  DSM_MemSet(pFile,0,FAT_MAX_USER_NAME_LEN + 1);
                                if(FAT_STR_TYPE_UNICODE == strtype)
                                {
                                    DSM_TcStrCpy(pFile, filename); 
                                    fat_Trim(pFile);
                                }
                                else
                                {
                                    DSM_StrCpy(pFile, filename); 
                                    fat_TrimOem((UINT8*)pFile);
                                }
                                
                            }
                        }
                        if(FAT_STR_TYPE_UNICODE == strtype)
                        {
                             iResult = pattern_match(pszUniName, pFile);                                   
                        }
                        else
                        {
                      
                            // Because the point of pShortUniName malloced in DSM_OEM2Uincode, we need free it after called.                   
                            if(NULL == pShortUniName)
                            {                                
                                iResult = DSM_OEM2Uincode((UINT8*)pFile, (UINT32)DSM_StrLen((UINT8*)pFile), &pShortUniName, &iULen,NULL);
                                if(_ERR_FAT_SUCCESS != iResult)
                                {
                                   // iLIndex = 0;
                                   // iLCount = 0;
                                   // bLMatch = FALSE; 
                                   D((DL_FATWARNING, "fat_lookup_entry: DSM_Unicode2Oem() failed-1."));
                                   iResult = PATTERN_CMP_UNMATCH;
                                   //continue; 
                                }
                            }
                            iResult = pattern_match(pszUniName, (WCHAR*)pShortUniName); 

                        }
                        if(PATTERN_CMP_MATCH == iResult)
                        {
                            bLMatch = TRUE;                            
                        }
                        else
                        {
                            bLMatch = FALSE;

                        }
                    } 
                }
            }
            iCurEOff = 0;
        }   	       
        
        if(FAT32 == sb_info->iFATn || 2 <= startclu) // FAT12/FAT16 sub dir or FAT32 dir.
        {
	    // FAT12/FAT16 sub dir look up
	    // FAT32 dir look up. 
	    iRet = fat_get_next_cluster(sb_info, iCurClus, &iCurClus);
	    if (iRet != _ERR_FAT_SUCCESS)
	    {
		 D((DL_FATERROR, "fat_lookup_entry: fat_get_next_cluster() return %d", iRet));
		 goto end_lookup;
	    }
        }
	 else
	 {
            // FAT12 or FAT16 root dir look up.	
            iRet = _ERR_FAT_FILE_NOT_EXISTED;
            goto end_lookup;
        }	
        iCurSec = 0;
        iCurEOff = 0;
    }while(!fat_is_last_cluster(sb_info, iCurClus));

    iRet = _ERR_FAT_FILE_NOT_EXISTED;
    
end_lookup:
    if(_ERR_FAT_SUCCESS == iRet)
    {
        if(FAT32 == sb_info->iFATn || 2 <= startclu) // FAT12/FAT16 sub dir or FAT32 dir.
        {
            // FAT12/FAT16 sub dir look up
 	     // FAT32 dir look up. 
            *bsec = iBegSec;         
        }
        else
        {
             // FAT12 or FAT16 root dir look up.	 
            *bsec = (UINT16)(iBegSec - sb_info->iRootDirStartSec);
        }
        *eoff = (UINT16)iBegEOff;
        *bclu = iBegClus;
    }
    else
    {        
          *decount = 0;
    }
    
    if(NULL != pszUniName)
    {
        NAME_BUF_FREE(pszUniName);
        pszUniName = NULL;
    }
    
    if(NULL != pFile)
    {
        NAME_BUF_FREE(pFile);   
        pFile = NULL;
    }   
    if(NULL != pShortUniName)
    {
        DSM_Free(pShortUniName);
        pShortUniName = NULL;
    }
    if(NULL != pShortOemName)
    {
        DSM_Free(pShortOemName);
        pShortOemName = NULL;
    }
    return iRet;    
}

#if 0
BOOL fat_FileNameMaching(WCHAR* wcFileName, UINT8* pSecBuff, FAT_DIR_ENTRY* d_entry,UINT32* piEOff,UINT32 iFlag, UINT32 strtype)
{
    typedef enum FAT_FNM_STATE
    { 
         FNM_INIT,
         FNM_SHORT_ONLY,
         FNM_LONG_LAST,
         FNM_LONG_MID,
         FNM_LONG_FIRST,
         FNM_LONG_MACHING,
         FNM_LONG_MACHING_SHORT,
         FNM_LONG_NOT_MACHING,
         FNM_LONG_NOT_MACHING_SHORT,
         FNM_NOT_MACHING_ENTRY,
         FNM_MACHING_END,
         FNM_NOT_MACHING_END,
    };
    INT32 iResult;
    FAT_FNM_STATE eState = FNM_INIT;
    UINT32 iEOff = *piEOff;
    UINT32 iLIndex = 0;
    UINT32 iLCount = 0;
     
    UINT8  szFatName[FAT_MAX_NAME_LEN + 1];             // Fat name buffer.
    UINT8  szUseName[FAT_MAX_USER_NAME_LEN + 1];   // Use name buffer.   
    WCHAR* pszUniName = NULL;                                   // Point to unicode name,need malloc.  
    UINT8* pShortOemName = NULL;                              // Point to unicode name,need malloc.  
    UINT8* pShortUniName = NULL;                                // Point to the unicode short name, malloc in DSM_OEM2Uincode(), need free.
    UINT32 iULen = 0;                                                     // Unicode name length. 
    WCHAR* pFile = NULL;                                              // Point to file name to storage the input file name(long), need malloc.
    FAT_DIR_ENTRY sDEntry;
    UINT8* p = NULL;
    
    while(iEOff < DEFAULT_SECSIZE)
    { 
        p = pSecBuff + iEOff;
        switch(eState)
        {
            case FNM_INIT:
                 iLIndex = 0;
                 iLCount = 0;
                if(FAT_IS_SHORT_ENTRY(p))
                {
                    eState = FNM_SHORT_ONLY;
                    continue;
                }
                else if(FAT_IS_LAST_LONG_ENTRY(p))
                {
                    eState = FNM_LONG_LAST;
                    continue;
                }
                else if(FAT_IS_FIRST_LONG_ENTRY(p))
                {
                    eState = FNM_SHORT_ONLY;
                    continue;
                }
                // include free,delete and other conditions.                 
                else
                {
                    // only jump this dir entry(size:32 byte).
                }
                break;
            case FNM_SHORT_ONLY:
                if(FAT_IS_JAP_DELETE_ENTRY(p)) // Japonic delete entry continue
                {
                    eState = FNM_INIT;
                    continue;
                }
                Buf2FDE((UINT8*)(p), &sDEntry);

                DSM_StrNCpy(szFatName, d_entry->DIR_Name, FAT_MAX_NAME_LEN);                                 

                if(_ERR_FAT_SUCCESS != NameFat2User(szFatName,szUseName,*(p + 0x0c)))
                {                        
                    eState = FNM_INIT;
                    continue;                       
                }
                if(FAT_STR_TYPE_UNICODE == strtype)
                {       
                    // Because the point of pShortUniName malloced in DSM_OEM2Uincode, we need free it after called.                   
                    if(NULL == pShortOemName)
                    {                                         
                        iResult = DSM_Unicode2OEM((UINT8*)wcFileName, (UINT32)DSM_UnicodeLen((UINT8*)wcFileName), &pShortOemName, &iULen,NULL);
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            D((DL_FATWARNING, "fat_lookup_entry: DSM_Unicode2Oem() failed-3."));                          
                            continue; 
                        }                        
                    }
                    iResult = pattern_match_oem((UINT8*)szUseName,(UINT8*)pShortOemName);
                }
                else
                {                        
                    iResult = pattern_match_oem((UINT8*)szUseName, (UINT8*)pFile);    
                }   
                // Short name matching 
                if(PATTERN_CMP_MATCH == iResult)
                {  
                    // if short name is match 
                    // long entry count equal to zero or check sum not pass,
                    // will output the short name else output long name.
                    iBegEOff = (UINT16)i;  
                    if(iFlag & FAT_LOOK_GET_NAME)
                    {                                
                        if(NULL != pShortUniName)
                        {
                            DSM_Free(pShortUniName);
                            pShortUniName = NULL;
                        }  
                        iResult = DSM_OEM2Uincode(szUseName, (UINT16)DSM_StrLen(szUseName), &pShortUniName, &iULen,NULL);
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            D((DL_FATWARNING, "fat_lookup_entry: DSM_OEM2Uincode() failed-2.")); 
                        }
                        DSM_TcStrCpy((PWSTR)filename,(PCWSTR)pShortUniName);  
                    }
                    else
                    {                            
                        if(0 == (iFlag & FAT_LOOK_EXISTED))
                        {                            
                            continue;
                        }                                                            
                    }                            
                    *decount = (UINT16)(iLCount + 1);
                    iBegClus = iCurClus;
                    iRet = _ERR_FAT_SUCCESS;
                    goto end_lookup;
                }   
                break;
            case FNM_LONG_LAST:
            case FNM_LONG_MID:
            case FNM_LONG_FIRST:
            case FNM_LONG_MACHING:
            case FNM_LONG_MACHING_SHORT:
            case FNM_LONG_NOT_MACHING:
            case FNM_LONG_NOT_MACHING_SHORT:
            case FNM_NOT_MACHING_ENTRY:
            case FNM_MACHING_END:
            case FNM_NOT_MACHING_END:
            default:
                break;
        }
        iEOff += DEFAULT_DIRENTRYSIZE;
    }


}
#endif

//********************************************************************************
// Function:
//   This function test a  fat  is last condition.
// Parameter:
//   sb_info[in],  Potint to the FAT_SB_INFO struct.
//   fatent[in], The fat.
// Return value:
//      TRUE: The value of fat  is last.
//      FALSE: The value of fat is't last.
//      
//*******************************************************************************
BOOL fat_is_last_cluster(FAT_SB_INFO *sb_info, UINT32 fatent) 
{
    BOOL IsEOF = FALSE;

    if(sb_info->iFATn == FAT12)
    {
        if(fatent >= 0xff8 )
        {
            IsEOF = TRUE;
        }
    }
    else if(sb_info->iFATn == FAT16)
    {
        if(fatent >= 0xfff8)
        {
            IsEOF = TRUE;
        }
    }
    else //FAT32
    {
        if(fatent >= 0x0ffffff8)
        {
            IsEOF = TRUE;
        }
    }

    return IsEOF;
}

INT32 fat_stat_free_cluster(FAT_SB_INFO *sb_info, UINT32 *piFreeCluNum)
{
    UINT32  iFreeCluNum = 0;
    UINT32 FATOffset;
    UINT32 ThisFATSecNum;
    UINT32 ThisFATEntOffset = 0, OldThisFATSecNum = 0;
    UINT32 FATCluEntry = 0;
    UINT32 iClusCount;
    UINT32 curclu;
    UINT8*  secbuf = NULL;
    INT32  lErrCode = _ERR_FAT_SUCCESS;
    UINT8 cheat = 1;
    UINT32 old_r_cache_size = 0;

    if (NULL == sb_info || NULL == piFreeCluNum)
    {
        return _ERR_FAT_PARAM_ERROR;
    }

    if (TRUE == sb_info->iFreeCnterInitFlag)
    {
        *piFreeCluNum = sb_info->iFreeCluCnt;
        return _ERR_FAT_SUCCESS;
    }
    
    iClusCount = sb_info->iDataSec/sb_info->iSecPerClus;
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        D( ( DL_FATERROR, "In fat_stat_free_cluster malloc secbuf failed.") );
        DSM_ASSERT(0,"In fat_stat_free_cluster malloc secbuf failed.");
        return _ERR_FAT_MALLOC_FAILED;        
    }    
    
    curclu = 2;
    
    if(sb_info->iFATn == FAT12)
    {
        do
        {
            FATOffset = curclu + (curclu / 2);// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iFATStartSec + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;

            if(OldThisFATSecNum < ThisFATSecNum)
            {
                DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);
                DSM_MemSet(secbuf+DEFAULT_SECSIZE, 0x00, DEFAULT_SECSIZE);

                //iResult = fat_sector_read(ThisFATSecNum, secbuf);  
                lErrCode = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum, secbuf );
                if(_ERR_FAT_SUCCESS != lErrCode)
                {
                    D( ( DL_FATERROR, "In fat_stat_free_cluster DRV_BLOCK_READ_0 failed lErrCode = %d\n",lErrCode));
                    goto end;
                }

                //iResult = fat_sector_read(ThisFATSecNum+1, secbuf+DEFAULT_SECSIZE); 
                lErrCode = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum+1, secbuf+DEFAULT_SECSIZE );
                if(_ERR_FAT_SUCCESS != lErrCode)
                {

                    D( ( DL_FATERROR, "In fat_stat_free_cluster DRV_BLOCK_READ_1 failed lErrCode = %d\n",lErrCode));
                    goto end;
                }
                
                OldThisFATSecNum = ThisFATSecNum;
            }

            FATCluEntry = MAKEFATUINT16(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1]);
            if(curclu & 0x0001)
            {
                FATCluEntry = ((FATCluEntry>>4) & 0x0fff);
            }
            else
            {
                FATCluEntry = FATCluEntry & 0x0fff;
            }

            if(0 == FATCluEntry)
            {
                iFreeCluNum++;
            }

            curclu++;
            if(curclu >= iClusCount + 2)
            {
                break;
            }
            
        }while(cheat);
    }
    else if(sb_info->iFATn == FAT16)
    {
        old_r_cache_size = DRV_GET_RCACHE_SIZE(sb_info->sb->s_dev);
        DRV_SET_RCACHE_SIZE(sb_info->sb->s_dev,DRV_DEF_CACHE_BLOCK_CNT*DEFAULT_SECSIZE*2);
        do
        {
            FATOffset = curclu*2;// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iFATStartSec + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;
            
            if(OldThisFATSecNum < ThisFATSecNum)
            {
                DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);

                lErrCode = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum, secbuf );
                if(_ERR_FAT_SUCCESS != lErrCode)
                {
                    D( ( DL_FATERROR, "In fat_stat_free_cluster DRV_BLOCK_READ_2 failed lErrCode = %d\n",lErrCode));
                    goto end;
                }
                
                OldThisFATSecNum = ThisFATSecNum;
            }

            FATCluEntry = MAKEFATUINT16(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1]);

            if(0 == FATCluEntry)
            {
                iFreeCluNum++;
            }
            
            curclu++;
            if(curclu >= iClusCount + 2)
            {
                break;
            }
            
        }while(cheat);
    }
    else  // FAT32
    {
        old_r_cache_size = DRV_GET_RCACHE_SIZE(sb_info->sb->s_dev);
        DRV_SET_RCACHE_SIZE(sb_info->sb->s_dev,DRV_DEF_CACHE_BLOCK_CNT*DEFAULT_SECSIZE*8);
        do
        {
            FATOffset = curclu * 4;// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iFATStartSec + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;
            
            if(OldThisFATSecNum < ThisFATSecNum)
            {
                //DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);
                lErrCode = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum, secbuf );
                if(_ERR_FAT_SUCCESS != lErrCode)
                {
                    D( ( DL_FATERROR, "In fat_stat_free_cluster DRV_BLOCK_READ_2 failed lErrCode = %d\n",lErrCode));
                    goto end;
                }
                
                OldThisFATSecNum = ThisFATSecNum;
            }

            FATCluEntry = MAKEFATUINT32(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1], 
                                                                secbuf[ThisFATEntOffset+2], secbuf[ThisFATEntOffset+3]);

            if(0 == FATCluEntry)
            {
                iFreeCluNum++;
            }
            
            curclu++;
            if(curclu >= iClusCount + 2)
            {
                break;
            }            
        }while(cheat);
    }

end:
    // resume r_cache size.
    if(old_r_cache_size > 0 && old_r_cache_size != (UINT32)DRV_GET_RCACHE_SIZE(sb_info->sb->s_dev))
    {
         DRV_SET_RCACHE_SIZE(sb_info->sb->s_dev,old_r_cache_size);
     }
    
     if(NULL != secbuf)
     {
        FAT_SECT_BUF_FREE((SECT_BUF*)secbuf);
     }

    if (lErrCode != _ERR_FAT_SUCCESS)
    {
        return lErrCode;
    }
    
    *piFreeCluNum = iFreeCluNum;

    sb_info->iFreeCnterInitFlag = TRUE;
    sb_info->iFreeCluCnt = iFreeCluNum;
    return _ERR_FAT_SUCCESS;
    
}

CLUSTER_FRAG * fat_new_frag(UINT32 iBegClu)
{
    CLUSTER_FRAG *pTmpFrag;
    pTmpFrag = (CLUSTER_FRAG*)DSM_MAlloc(sizeof(CLUSTER_FRAG));
    if(NULL == pTmpFrag)
    {
        DSM_ASSERT(0,"fat_new_frag: 1.malloc(0x%x) failed.",sizeof(CLUSTER_FRAG));
        return (CLUSTER_FRAG *)NULL;
    }

    pTmpFrag->iBegClus = iBegClu;
    pTmpFrag->iNum = 1;
    pTmpFrag->next = NULL;

    return pTmpFrag;
}

INT32 fat_free_fatentries(CLUSTER_CHAIN *pChain)
{
    CLUSTER_FRAG *pCurFrag = pChain->pFragChain;
    CLUSTER_FRAG *pPreFrag;
    
    while(pCurFrag)
    {
        pPreFrag = pCurFrag;
        pChain->pFragChain = pCurFrag->next;
        pCurFrag = pChain->pFragChain;
        DSM_Free((VOID*)pPreFrag);

    }

    pChain->iFragNum = 0;
    pChain->pFragChain = NULL;

    return _ERR_FAT_SUCCESS;
}


INT32 fat_append_fatentries_chain(CLUSTER_CHAIN *pChain, UINT32 iClu)
{
    INT32  lErrCode = _ERR_FAT_SUCCESS;
    CLUSTER_FRAG *pCurFrag;
    CLUSTER_FRAG *pTmpFrag;
	
    if(NULL == pChain)
    {
        lErrCode = _ERR_FAT_PARAM_ERROR;
    }
    
    DSM_ASSERT(iClu >= 2,"iClu = %d.",iClu);
    
    //D((DL_FATWARNING, "fat_append_fatentries_chain, cluster = %d.\n", iClu));
    // 如果节点中的碎片链未初始化，则申请新的碎片。
    if(NULL == pChain->pFragChain)
    {
        pTmpFrag = (CLUSTER_FRAG *)fat_new_frag(iClu);
        if(NULL == pTmpFrag)
        {
            lErrCode = _ERR_FAT_MALLOC_FAILED;
            goto end;
        }
        
        pChain->pFragChain = pTmpFrag;
        pChain->pLastFrag = pTmpFrag;
        pChain->iFragNum++; // 碎片个数加1。
       /*
       CSW_TRACE(BASE_DSM_TS_ID,
                "fat_append_fatentries_chain:1  begclus = 0x%x, clu = 0x%x,num = 0x%x.",
                 pTmpFrag->iBegClus,
 	   	  iClu,
		  pTmpFrag->iNum);
      */	   
    }
    else
    {
		pTmpFrag = pChain->pFragChain;
		while(pTmpFrag)
		{
			if(pTmpFrag->iBegClus <= iClu && iClu < pTmpFrag->iBegClus + pTmpFrag->iNum)
			{
			       D((DL_FATERROR, "fat_append_fatentries_chain: begclus = 0x%x, clu = 0x%x,num = 0x%x.", 
				   	pTmpFrag->iBegClus,
				   	iClu,
				   	pTmpFrag->iNum));				   
				lErrCode = _ERR_FAT_CLUSTER_NUMBER_CYC;
				goto end;
			}
			pTmpFrag = pTmpFrag->next;
		}
        
        // 如果已经存在碎片，则取最后一块碎片。
        pCurFrag = pChain->pLastFrag;
		
        // 判断当前要添加的cluster是否跟上次的cluster连续。
        if(iClu == pChain->pLastFrag->iBegClus + pChain->pLastFrag->iNum)
        {
            // 连续，计数加1即可。
            pCurFrag->iNum++;
	    /* CSW_TRACE(BASE_DSM_TS_ID,
                "fat_append_fatentries_chain: 2 begclus = 0x%x, clu = 0x%x,num = 0x%x.",
                 pCurFrag->iBegClus,
 	   	  iClu,
		  pCurFrag->iNum);			
	    */
        }
        else
        {
            // 不连续，申请一新碎片。
            pTmpFrag = (CLUSTER_FRAG *)fat_new_frag(iClu); 
            if(NULL == pTmpFrag)
            {
                lErrCode = _ERR_FAT_MALLOC_FAILED;
                goto end;
            }
            //D((DL_FATWARNING, "####fat_new_frag####.\n"));
            
            pCurFrag->next = pTmpFrag;
            pChain->pLastFrag = pCurFrag->next; 
            pChain->iFragNum++; // 碎片个数加1。
            /*CSW_TRACE(BASE_DSM_TS_ID,
                "fat_append_fatentries_chain: 3 begclus = 0x%x, clu = 0x%x,num = 0x%x.",
                 pTmpFrag->iBegClus,
 	   	  iClu,
		  pTmpFrag->iNum);	
		  */
        }
	  
    }
	
end:    
    return lErrCode;
}


INT32 fat_read_fatentries(FAT_SB_INFO *sb_info, UINT32 iStartClu, CLUSTER_CHAIN *pChain)
{
    UINT32 curclu;
    INT32  lErrCode = _ERR_FAT_SUCCESS;
    UINT8 cheat = 1;

    // 检查起始的FAT表项是否在合法范围。
    if (iStartClu > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || iStartClu < 2)
    {
        D((DL_FATERROR, "fat_read_fatentries: iStartClu = 0x%x, _ERR_FAT_PARAM_ERROR!\n", iStartClu));
        return _ERR_FAT_ERROR;
    }

    curclu = iStartClu;
   
    do
    {
       // CSW_TRACE(BASE_DSM_TS_ID, "fat_read_fatentries:curclu = 0x%x.\n", curclu);            
        // 将当前的cluster号添加到碎片链。
        lErrCode = fat_append_fatentries_chain(pChain, curclu);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            D((DL_FATERROR, "fat_read_fatentries: fat_append_fatentries_chain() return %d", lErrCode));
            return lErrCode;
        }
        lErrCode = fat_get_next_cluster(sb_info, curclu, &curclu);
        if (lErrCode != _ERR_FAT_SUCCESS)
        {
            D((DL_FATERROR, "fat_read_fatentries: fat_get_next_cluster() return %d", lErrCode));
            return lErrCode;
        }
	 
        if(fat_is_last_cluster(sb_info, curclu))
        {
            return _ERR_FAT_SUCCESS;
        }
        if (curclu > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || curclu < 2)
        {// 判断下一个cluster是否为合法的值。
            D((DL_FATERROR, "fat_read_fatentries: nextclu = 0x%x , Max Valid cluster is %d !!!\n", 
				curclu, (sb_info->iDataSec / sb_info->iSecPerClus) + 1));
             //return _ERR_FAT_ERROR;
		  return _ERR_FAT_SUCCESS;
        }
    }while(cheat);

    return lErrCode;

}


// 本函数根据传进来的inode及pos返回具体的cluster号，
// @pChain在文件打开时已经初始化，它用碎片的方式
// 记录了该文件使用的所有cluster。
// @pPreClu存放上一个簇，没有时值为0；
// pos没有对应的簇存时，@pPreClu的值为最后一个有效簇。
UINT32 fat_fpos2cluster(struct inode *inode, CLUSTER_CHAIN *pChain, UINT32 iPos, UINT32 *pPreClu)
{
    FAT_SB_INFO* sb = NULL;
    UINT32 clunum, clucnt;
    CLUSTER_FRAG *pCurFrag = NULL;
    CLUSTER_FRAG *pPreFrag = NULL;

    sb = &(inode->i_sb->u.fat_sb);
    if (0 == pChain->iFragNum || NULL == pChain->pFragChain)
    {// 文件为空。
        if (pPreClu != NULL)
        {
            *pPreClu = 0;
        }
        return 0;
    }
    clucnt = iPos / (sb->iSecPerClus * DEFAULT_SECSIZE);
    pCurFrag = pChain->pFragChain;
    
    while(clucnt >= pCurFrag->iNum)
    {
        clucnt -= MIN(clucnt, pCurFrag->iNum);

        if(NULL != pCurFrag->next)
        {
            pPreFrag = pCurFrag;
            pCurFrag = pCurFrag->next;
        }
        else
        {
            // 如果后面已没有碎片链，
            // 则pos已经超出文件的大小。
            // 把最后的有效cluster的编号放在@pLastCluster返回，
            // 直接则返回0表示要查找的pos已经超出文件大小。
            if (pPreClu != NULL)
            {
                *pPreClu = (pCurFrag->iBegClus + pCurFrag->iNum - 1);
            }
            return 0;   
        }
    }

    // 找到目标cluster，并将前一个cluster通过@pPreClu返回。
    clunum = pCurFrag->iBegClus + clucnt;
    if (pPreClu != NULL)
    {
        if (0 == clucnt)
        {
            if (NULL == pPreFrag)
            {
                *pPreClu = 0; // 0表示没有前趋的簇。
            }
            else
            {
                *pPreClu = pPreFrag->iBegClus + pPreFrag->iNum - 1;
            }
        }
        else
        {
            *pPreClu = clunum - 1;
        }
    }

    return clunum;    
}

//
//CSO - cluster, sector, offset
//
// 本函数根据传进来的inode及pos返回具体的cluster号、cluster中第几个sector及sector 中的偏移。
// 找到对应于pos的cluster时，同时通过@pPreClu返回前一个cluster，否则
// 直接返回_ERR_FAT_NOT_MATCH_CLUSTER，并通过@piClu返回0，@pPreClu返回最后一个有效的cluster.
INT32 fat_fpos2CSO(struct inode *inode, UINT32 iPos, UINT32 *piClu, UINT32 *piSec, UINT32 *piOff, UINT32 *pPreClu)
{
    FAT_SB_INFO* sb = NULL;
    UINT8 secperclu = 0;
    
    sb = &(inode->i_sb->u.fat_sb);
    secperclu = sb->iSecPerClus;

    // 得到pos所在的cluster、cluster中的第几个sector及sector中的offset。
    *piClu = fat_fpos2cluster(inode, &(inode->u.fat_i.CluChain), iPos, pPreClu); //(iPos / (secperclu * DEFAULT_SECSIZE));
    if (0 == *piClu)
    {
        return _ERR_FAT_NOT_MATCH_CLUSTER;
    }
    
    *piSec = (iPos / DEFAULT_SECSIZE) % secperclu;
    *piOff = iPos % DEFAULT_SECSIZE;
    return _ERR_FAT_SUCCESS;
}

//********************************************************************************
// Function:
//      Get the FAT[iPreClu] value, that is, the next cluster.
// Parameter:
//   sb_info[in], Potint to the FAT_SB_INFO struct.
//   iPreClu[in], the pre-cluster number.
//   pNexClu[out], Receives the next cluster.
// Return value:
//      Its return _ERR_FAT_SUCCESS if next cluster has taken.
//      And unsuccessful operation is denoted by the err code as following
//      _ERR_FAT_READ_SEC_FAILED: Read the sector is failed.      
//*******************************************************************************
INT32 fat_get_next_cluster(FAT_SB_INFO *sb_info, UINT32 iPreClu, UINT32 *pNexClu)
{
    UINT32 FATOffset;
    UINT32 ThisFATSecNum;
    UINT32 ThisFATEntOffset;
    UINT32 FATCluEntry = 0;
    FAT_CACHE *psFatCache = NULL;
    INT32 iResult;
    INT32 iRet;

    if (iPreClu > ((sb_info->iDataSec / sb_info->iSecPerClus) + 1) || iPreClu < 2)
    {
        D((DL_FATERROR, "fat_get_next_cluster: iPreClu = 0x%x, _ERR_FAT_PARAM_ERROR!\n", iPreClu));
        return _ERR_FAT_ERROR;
    }

    psFatCache = fat_get_fat_cache(sb_info->sb->s_dev);
    if(NULL == psFatCache)
    {
        D((DL_FATERROR, "fat_get_free_cluster: fat cache is null,s_dev = 0x%x.\n", sb_info->sb->s_dev));
        return _ERR_FAT_CACHE_IS_NULL;
    }

    iRet = _ERR_FAT_SUCCESS;
    
    if(sb_info->iFATn == FAT12)
    {
        FATOffset = iPreClu + (iPreClu / 2); // every FAT item occupies 1.5 byte
    }
    else if(sb_info->iFATn == FAT16)
    {
        FATOffset = iPreClu*2; // every FAT item occupies 2 byte
    }
    else //FAT32
    {
        FATOffset = iPreClu*4; // every FAT item occupies 4 byte
    }

    ThisFATSecNum = sb_info->iFATStartSec + (FATOffset / sb_info->iBytesPerSec);
    ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;

    if (psFatCache->iSecNum != ThisFATSecNum)
    {
        // 切换掉旧的FAT缓存，将其写回磁盘。
        if (psFatCache->iSecNum != 0)
        {
            iResult = fat_synch_fat_cache(sb_info, psFatCache);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                D( ( DL_FATERROR, "in fat_get_next_cluster,fat_synch_fat_cache() return = %d.\n",iResult) );
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
        }
        
        iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum, psFatCache->szSecBuf);
        if(_ERR_FAT_SUCCESS != iResult)
        {            
            psFatCache->iSecNum = 0;
            iRet = _ERR_FAT_READ_SEC_FAILED;
            D((DL_FATERROR, "fat_get_next_cluster(), _ERR_FAT_READ_SEC_FAILED1!iResult = %d\n", iResult));
            goto end;
        }
        
        if(sb_info->iFATn == FAT12)
        {
            // FAT12存在一个FAT表项跨两个扇区的问题。
            iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + 1, 
                                                                psFatCache->szSecBuf + DEFAULT_SECSIZE);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                //psFatCache->dev = 0;
                //psFatCache->FATSecNum = 0;
                iRet = _ERR_FAT_READ_SEC_FAILED;
                D((DL_FATERROR, "fat_get_next_cluster(), _ERR_FAT_READ_SEC_FAILED2!iResult = %d\n", iResult));
                goto end;
            }
        }
        
        psFatCache->iSecNum = ThisFATSecNum;
        psFatCache->iDirty = 0;
    }

    if(sb_info->iFATn == FAT12)
    {
        FATCluEntry = MAKEFATUINT16(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1]);
        if(iPreClu & 0x0001)
        {
            FATCluEntry = (UINT16)((FATCluEntry>>4) & 0x0fff);
        }
        else
        {
            FATCluEntry = (UINT16)(FATCluEntry & 0x0fff);
        }
    }
    else if(sb_info->iFATn == FAT16)
    {
        FATCluEntry = MAKEFATUINT16(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1]);
    }
    else //FAT32
    {
        FATCluEntry = MAKEFATUINT32(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1], 
                                                             psFatCache->szSecBuf[ThisFATEntOffset+2], psFatCache->szSecBuf[ThisFATEntOffset+3]);
        FATCluEntry &= 0x0FFFFFFF; // Found:FAT32的表项实际上只用了28位。
    }

    if (FATCluEntry == iPreClu)
    {
        D((DL_ERROR, "fat_get_next_cluster: next-cluster(0x%x) == current-cluster(0x%x)", FATCluEntry, iPreClu));
       
       if(sb_info->iFATn == FAT12)
       {
           FATCluEntry = 0xFFF;
       }
       else if(sb_info->iFATn == FAT16)
       {
           FATCluEntry = 0xFFFF;
       }
       else
       {
           FATCluEntry = 0xFFFFFFFF;
       }
    }
    
    *pNexClu = FATCluEntry;
    iRet = _ERR_FAT_SUCCESS;

end:    

    return iRet;
}

//
// The function gets the FAT entry value according to the position 
// in the FAT array
//
// iPreClu is the postion in FAT array
//

//********************************************************************************
// Function:
//   This function gets the FAT entry value according to the position 
//   in the FAT array. 
// Parameter:
//   sb_info[in],  Potint to the FAT_SB_INFO struct.
//   iPreClu[in], the pre-cluster number.
//   fCleanClu[in], whether to clean data of the new cluster.
//   pFreeClu[out], Receives the free cluster.
// Return value:
//      Its return _ERR_FAT_SUCCESS when free cluster found. 
//      And unsuccessful operation is denoted by the err code as following
//      _ERR_FAT_READ_SEC_FAILED: Read the sector is failed.      
//      _ERR_FAT_DISK_FULL: Disk space is full.
//*******************************************************************************
INT32 fat_get_free_cluster(FAT_SB_INFO *sb_info, UINT32 iPreClu, BOOL fCleanClu, UINT32 *pFreeClu)
{
    UINT32 FATOffset;
    UINT32 ThisFATSecNum;
    UINT32 ThisFATEntOffset = 0;
    //引入变量OldThisFATSecNum的目的是保证处理完一个sector中的诸entry后
    //才读取下一个sector。应该改写之。或者两重循环，或者用布尔变量，更直观。
    UINT32 OldThisFATSecNum = 0;
    UINT32 FATCluEntry = 0;
    UINT8*  secbuf = NULL;
    UINT32 iCurClu;
    INT32 iResult = _ERR_FAT_SUCCESS;
    INT32 iRet = _ERR_FAT_SUCCESS;
    UINT8 i;
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    UINT8 cheat = 1;
    FAT_CACHE *psFatCache = NULL; 
    UINT32 iClusCount; // cluster count. 

    D( ( DL_FATDETAIL, "********* Enter in fat_get_free_cluster....\n") );

    psFatCache = fat_get_fat_cache(sb_info->sb->s_dev);
    if(NULL == psFatCache)
    {
        D((DL_FATERROR, "fat_get_free_cluster: fat cache is null,s_dev = 0x%x.\n", sb_info->sb->s_dev));
        return _ERR_FAT_CACHE_IS_NULL;
    }
	
     //   MTK Fat Symbol parameter is zero,jump the cheking for fat entry 0 and fat entry 1.
     if (FAT16 == sb_info->iFATn || FAT12 == sb_info->iFATn)
     {
        iPreClu = iPreClu < 2 ? 2 : iPreClu;
     }   
     else if (FAT32 == sb_info->iFATn)
    {
        iPreClu = iPreClu < 4 ? 4 : iPreClu;
    }
    else
    {
        iPreClu = iPreClu;       
    }
    iCurClu = iPreClu; 
    
    iClusCount = sb_info->iDataSec/sb_info->iSecPerClus;
    if (iPreClu > iClusCount + 1)
    {
        D((DL_FATERROR, "fat_get_free_cluster: iPreClu = 0x%x, _ERR_FAT_PARAM_ERROR1!\n", iPreClu));
        return _ERR_FAT_PARAM_ERROR;
    }
    
    D((DL_FATDETAIL, 
       "fat_get_free_cluster(),iPreClu = %d,iCulsCount = %d,iMode = %d,\n",
       iPreClu,
       iClusCount,
       fCleanClu));
    
    // 根据cluster number算出它在FAT中的字节偏移量
    if(FAT12 == sb_info->iFATn)
    {
        FATOffset = iCurClu + (iCurClu / 2);
    }
    else if (FAT16 == sb_info->iFATn)
    {
        FATOffset = iCurClu*2;
    }
    else if (FAT32 == sb_info->iFATn)
    {
        FATOffset = iCurClu * 4;
    }
    else
    {
        D((DL_FATERROR, "fat_get_free_cluster: iPreClu = 0x%x, Unkow file system type: "
            "sb_info->iFATn = %d\n", sb_info->iFATn));
        return _ERR_FAT_ERROR;
    }
    
    ThisFATSecNum = sb_info->iFATStartSec + (FATOffset / sb_info->iBytesPerSec);
    ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;

    // 检查当前要读的FAT的sector是否已经保存在缓存中，否则更新FAT缓存。
    // TODO:  考虑到多进程共用FAT缓存，可能要上锁
    if (psFatCache->iSecNum != ThisFATSecNum)
    {
        // 切换掉旧的FAT缓存，将其写回磁盘。
        if (psFatCache->iSecNum != 0)
        {
            iResult = fat_synch_fat_cache(sb_info, psFatCache);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                D( ( DL_FATERROR, "in fat_get_free_cluster,fat_synch_fat_cache()1 return = %d.\n",iResult) );
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
        }
        
        iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum , psFatCache->szSecBuf);
        if(_ERR_FAT_SUCCESS != iResult)
        {
            D((DL_FATERROR, "DRV_BLOCK_READ failed1, dev = %d, FATSecNum = %d, sb_info->iFATStartSec = %d, FATOffset = %d, iResult = %d, goto end.\n", 
                                         sb_info->sb->s_dev, ThisFATSecNum, sb_info->iFATStartSec, FATOffset,iResult));
            psFatCache->iSecNum = 0;
            iRet = _ERR_FAT_READ_SEC_FAILED;
            goto end;
        }

        if (FAT12 == sb_info->iFATn)
        {// FAT12存在一个FAT表项跨两个扇区的问题。目前的简单处理是将下个扇区也读出。
            iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + 1, 
                                                                psFatCache->szSecBuf + DEFAULT_SECSIZE);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                D((DL_FATERROR, "DRV_BLOCK_READ failed2, dev = %d, FATSecNum = %d, iResult = %d, goto end.\n",
                   sb_info->sb->s_dev, ThisFATSecNum + 1, iResult));
              
                psFatCache->iSecNum = 0;
                iRet = _ERR_FAT_READ_SEC_FAILED;              
                goto end;
            }
        }
        psFatCache->iSecNum = ThisFATSecNum;
        psFatCache->iDirty = 0;
    }
    
    OldThisFATSecNum = ThisFATSecNum;    
     
    do
    {
        // 根据cluster number算出它在FAT中的字节偏移量
        if(FAT12 == sb_info->iFATn)
        {
            FATOffset = iCurClu + (iCurClu / 2);
        }
        else if (FAT16 == sb_info->iFATn)
        {
            FATOffset = iCurClu*2;
        }
        else if (FAT32 == sb_info->iFATn)
        {
            FATOffset = iCurClu * 4;
        }
        ThisFATSecNum = sb_info->iFATStartSec  + (FATOffset / sb_info->iBytesPerSec);
        ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;     
       
        if(OldThisFATSecNum != ThisFATSecNum) 
        {
            if(OldThisFATSecNum != 0)
            {
                iResult = fat_synch_fat_cache(sb_info, psFatCache);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    D( ( DL_FATERROR, "in fat_get_free_cluster,fat_synch_fat_cache()2 return = %d.\n",iResult) );
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
            }
            iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum, psFatCache->szSecBuf);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                D((DL_FATERROR, "DRV_BLOCK_READ failed3, dev = %d, FATSecNum = %d, iResult = %d, goto end.\n",
                   sb_info->sb->s_dev, ThisFATSecNum, iResult));
                psFatCache->iSecNum = 0;
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
            if (FAT12 == sb_info->iFATn)
            {// FAT12存在一个FAT表项跨两个扇区的问题。目前的简单处理是将下个扇区也读出。
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + 1, 
                                                                    psFatCache->szSecBuf + DEFAULT_SECSIZE);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    D((DL_FATERROR, "DRV_BLOCK_READ failed4, dev = %d, FATSecNum = %d, iResult = %d, goto end.\n",
                       sb_info->sb->s_dev, ThisFATSecNum + 1, iResult));
                    psFatCache->iSecNum = 0;
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
            }

            psFatCache->iSecNum = ThisFATSecNum;
            psFatCache->iDirty = 0;
            OldThisFATSecNum = ThisFATSecNum;
        }

        if (FAT12 == sb_info->iFATn)
        {
            FATCluEntry = MAKEFATUINT16(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1]);
            if(iCurClu & 0x0001)
            {
                FATCluEntry = ((FATCluEntry>>4) & 0x0fff);
            }
            else
            {
                FATCluEntry = FATCluEntry & 0x0fff;
            }
            
            if(0 == FATCluEntry) // Found
            {
                iRet = iCurClu;
                break;
            }
        }
        else if (FAT16 == sb_info->iFATn)
        {
            FATCluEntry = MAKEFATUINT16(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1]);
            if(0 == FATCluEntry) // Found
            {
                D((DL_FATDETAIL,"0 == FATCluEntry,break.\n"));
                iRet = iCurClu;
                break;
            }
        }
        else // FAT32
        {
            FATCluEntry = MAKEFATUINT32(psFatCache->szSecBuf[ThisFATEntOffset], psFatCache->szSecBuf[ThisFATEntOffset+1], 
                                                                 psFatCache->szSecBuf[ThisFATEntOffset+2], psFatCache->szSecBuf[ThisFATEntOffset+3]);
            if(0 == (FATCluEntry &= 0x0FFFFFFF)) // Found:FAT32的表项实际上只用了28位
            {
                D((DL_FATDETAIL,"0 == FATCluEntry,break.\n"));
                iRet = iCurClu;
                break;
            }
        }
        if(iCurClu < iClusCount + 2)
        {            
            iCurClu++; // 连续扫描后面所有的cluster
        }
        else
        {
            iCurClu = 2;
        }
        if(iCurClu == iPreClu)
        {//将FAT表看作一个环形链表，已经到链表尾
            D((DL_FATDETAIL,"iCurClus(%d) = iPreClu(%d) ,break.\n",iCurClu,iPreClu));
            break;
        }
       /*
        // FAT使用的簇号从2开始，0及1号簇实际不存在。
        if(iCurClu >= iClusCount + 2)
        {
            D((DL_FATDETAIL,"iCurClus = iPreClu,break.\n"));
            iCurClu = 2;                
            if (2 == iPreClu) // 当从0号cluster开始查找时，再回到0号cluster已经空间满。
            {
                break;
            }
        }
        */
    }while(cheat);

    if(iCurClu == iPreClu && 0 != FATCluEntry)
    {
        iRet = _ERR_FAT_DISK_FULL;
        goto end;
    }

    // set the new cluster space ZERO
    if(TRUE == fCleanClu && 2 <= iRet )
    {
        secbuf = FAT_SECT_BUF_ALLOC();
        if(NULL == secbuf)
        {
            D( ( DL_FATERROR, "In fat_get_free_cluster malloc secbuf failed.") );
            DSM_ASSERT(0,"In fat_get_free_cluster malloc secbuf failed.");
            return _ERR_FAT_MALLOC_FAILED;        
        }
        DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);
        for(i=0; i<sb_info->iSecPerClus; i++)
        {
            lErrCode = fat_write_cluster(sb_info, iRet, i, secbuf);
            if(_ERR_FAT_SUCCESS != lErrCode)
            {
                iRet = lErrCode;
                break;
            }
        }
        FAT_SECT_BUF_FREE((SECT_BUF*)secbuf);      
    }       
    if(2 <= iRet )
    {
        *pFreeClu = (UINT32)iRet;
         iRet = _ERR_FAT_SUCCESS;
    }    
        
end:    
 //   D( ( DL_FATDETAIL, "********* End of fat_get_free_cluster.\n") );
    DSM_ASSERT(!((iRet == _ERR_FAT_SUCCESS) && (*pFreeClu < 2)),"pFreeClu = %d.",pFreeClu);
    return iRet;
}


INT32 fat_get_long_entry(FAT_SB_INFO *psSBInfo,
                         UINT32 iStartClu,
                         WCHAR* pszFileName,
                         FAT_LONG_DIR_ENTRY* psLDe,
                         UINT32* pCount)
{
    INT32 iResult;
    INT32 iRet = _ERR_FAT_SUCCESS;
    UINT32 iLossy = FAT_INTACT_CONVERSION;
    UINT8 szSName[FAT_MAX_USER_NAME_LEN + 4];
    UINT8 szSName_83[FAT_MAX_NAME_LEN + 1];  
    UINT8 szPName[16];
    UINT32 iPLen = 0;    
    UINT8 szEName[8];
    UINT32 iELen = 0;
    UINT32 iSize = 0;
    UINT32 iLCount = 0;
    UINT8 iSumCheck = 0;
    UINT32 iIndex = 0;
    UINT32 iOffset = 0;
    FAT_DIR_ENTRY* psDe = NULL;
    FAT_DIR_ENTRY sDe;
    FAT_LONG_DIR_ENTRY sLDe;
    UINT32 iBClu = 0;
    UINT16 iSec = 0;
    UINT16 iEOff = 0;
    UINT16 iLDeCount = 0;
    INT32 i = 0;
    BOOL bLoop = TRUE;
    UINT32 iTry = 1;

    UINT32 iLen = 0;
    UINT8 szBuff[32];
    BOOL bIsEnd = FALSE;

    // Strip all leading and embedded spaces from the name.
    fat_Trim(pszFileName);    
     
    // the name is long.
    // 为了支持多国语言，文件名一律当成长文件名来处理。2008/1/24
    if(FALSE == fat_is_long_name_valid(pszFileName))
    {
        D((DL_FATERROR, "fat_get_long_entry: fat_is_long_name_valid() return FALSE"));
        return _ERR_FAT_PARAM_ERROR;
    }

    // Get the base short name.
    iResult = fat_long_to_short(pszFileName,szPName,&iPLen,szEName,&iELen,&iLossy);  
    if(_ERR_FAT_SUCCESS != iResult)
    {
        D((DL_FATERROR, "fat_get_long_entry: fat_is_long_name_valid() return %d", iResult));
        return _ERR_FAT_PARAM_ERROR;
    }

    szPName[6] = 0;
          
    //   DSM_StrPrint((INT8*)szSName,"%s~1.%s",szPName,szEName);
    // 用试探法确定短文件名后缀数字。
    while(TRUE == bLoop)
    {
        if(5 < iTry)
        {
            szPName[2] = 0;
        }
        if(99999 < iTry)
        {
            D((DL_FATERROR, "fat_get_long_entry: _ERR_FAT_FILE_TOO_MORE"));
            iRet = _ERR_FAT_FILE_TOO_MORE;
            goto func_end;
        }
        
        
        if(0 < iELen) // has extend name.
        {
            DSM_StrPrint((INT8*)szSName,"%s~%d.%s",szPName,iTry,szEName);
        }
        else 
        {
            DSM_StrPrint((INT8*)szSName,"%s~%d",szPName,iTry);
        }            
        
        
        // 在当前目录下查找短文件名，如果存在，将短文件名的数字后缀加1后重新查找。
        iBClu = 0;
        iSec = 0;
        iEOff = 0;
        iResult = fat_lookup_entry(psSBInfo,
            iStartClu,
            (WCHAR*)szSName,
            &sDe,
            &iBClu,
            &iSec,
            &iEOff,
            &iLDeCount,
            FAT_LOOK_EXISTED,
            FAT_STR_TYPE_OEM
            );
        if(_ERR_FAT_SUCCESS == iResult)
        {
             iTry++;   
        }
        // not find.
        else if(_ERR_FAT_FILE_NOT_EXISTED == iResult)
        {   
            iRet = _ERR_FAT_SUCCESS;
            break;
        }
        else 
        {
            iRet = iResult;
            goto func_end;
        }           
    }       


    // OEM change to Unicode.
    iSize = (UINT16)(DSM_TcStrLen(pszFileName) * SIZEOF(WCHAR));
            
    // A long dir entry placed 26 byte.
    iLCount = (iSize + 25)/26; 

    // short name change to FAT8-3  
    iResult = NameUser2Fat(szSName,szSName_83);
    if(_ERR_FAT_SUCCESS == iResult)
    {    
        DSM_MemSet(&sDe,0,SIZEOF(FAT_DIR_ENTRY));
        psDe = (FAT_DIR_ENTRY*)(psLDe + iLCount);
       // DSM_MemCpy(sDe.DIR_Name,szSName_83,FAT_MAX_NAME_LEN);
        DSM_MemCpy((UINT8*)psDe,szSName_83,FAT_MAX_NAME_LEN);
       // FDE2Buf(&sDe,(UINT8*)psDe);
        *pCount = iLCount + 1;
    }
    else
    {
        iRet = _ERR_FAT_PARAM_ERROR;
        D((DL_FATERROR, "fat_get_long_entry: NameUser2Fat() failed!"));
    }
    iSumCheck = fat_ChkSum(szSName_83);

    // add in the long name to long dir entry 
    DSM_MemSet(psLDe,0,(UINT16)(SIZEOF(FAT_LONG_DIR_ENTRY)*iLCount));
    iIndex = iLCount;
    iOffset = 0;

    for(i = iLCount - 1; i >= 0 ; i--)
    {
        DSM_MemSet(&sLDe,0,FLDE_LONG_ENTRY_SIZE);   
        
        if(0 == i) // Last ORD
        {
            sLDe.LDIR_Ord = (UINT8)((iLCount - i) | 0x40);                
        }
        else       
        {
            sLDe.LDIR_Ord = (UINT8)(iLCount - i);
        }
        
        // Set Attr,Type,SumCheck
        sLDe.LDIR_Attr = FAT_ATTR_LONG_NAME;
        sLDe.LDIR_Type = 0; 
        sLDe.LDIR_Chksum = iSumCheck;
        
        // Set NAME1
        iLen = (iOffset  + FLDE_LDIR_NAME1_SIZE) >= iSize ? (iSize - iOffset) : FLDE_LDIR_NAME1_SIZE;
        DSM_MemSet(szBuff,0xff,FLDE_LDIR_NAME1_SIZE);
        if(iLen > 0)
        {            
            DSM_MemCpy(szBuff, (UINT8*)pszFileName + iOffset, iLen);
            szBuff[iLen] = 0x0;
            szBuff[iLen + 1] = 0x0;
            if(iLen < FLDE_LDIR_NAME1_SIZE)
            {
                bIsEnd = TRUE;
            }
        }            
        DSM_MemCpy(sLDe.LDIR_Name1,szBuff,FLDE_LDIR_NAME1_SIZE);            
        iOffset = iOffset + iLen > iSize ? iSize : iOffset + iLen; 
        
        // Set NAME2
        iLen = (iOffset  + FLDE_LDIR_NAME2_SIZE) >= iSize ? (iSize - iOffset) : FLDE_LDIR_NAME2_SIZE;
        DSM_MemSet(szBuff,0xff,FLDE_LDIR_NAME2_SIZE);

        if(FALSE == bIsEnd && iOffset == iSize)
        {
            szBuff[0] = 0x0;
            szBuff[1] = 0x0;
            bIsEnd = TRUE;
        }
        if(iLen > 0)
        {            
            DSM_MemCpy(szBuff, ((UINT8*)pszFileName + iOffset), iLen);
            szBuff[iLen] = 0x0;
            szBuff[iLen + 1] = 0x0;
            if(iLen < FLDE_LDIR_NAME2_SIZE)
            {
                bIsEnd = TRUE;
            }
        }             
        DSM_MemCpy(sLDe.LDIR_Name2,szBuff,FLDE_LDIR_NAME2_SIZE);            
        iOffset = iOffset + iLen > iSize ? iSize : iOffset + iLen; 

        // Set NAME3
        iLen = (iOffset  + FLDE_LDIR_NAME3_SIZE) >= iSize ? (iSize - iOffset) : FLDE_LDIR_NAME3_SIZE;
        DSM_MemSet(szBuff,0xff,FLDE_LDIR_NAME3_SIZE);
        if(iLen > 0)
        {            
            DSM_MemCpy(szBuff, ((UINT8*)pszFileName + iOffset),iLen);
            szBuff[iLen] = 0x0;
            szBuff[iLen + 1] = 0x0;
            if(iLen < FLDE_LDIR_NAME3_SIZE)
            {
                bIsEnd = TRUE;
            }
        }
        // Set terminated character
        if(FALSE == bIsEnd && iOffset == iSize)
        {
            szBuff[0] = 0x0;
            szBuff[1] = 0x0;
            bIsEnd = TRUE;
        }
        DSM_MemCpy(sLDe.LDIR_Name3,szBuff,FLDE_LDIR_NAME3_SIZE);
        
        iOffset = iOffset + iLen > iSize ? iSize : iOffset + iLen; 

        FLDE2Buf(&sLDe,(UINT8*)&psLDe[i]);
    }        
       
func_end:
    
    return iRet;
}


// Trim the '.',' ','\t'
VOID fat_Trim(WCHAR* pBuff)
{
    WCHAR* p;
    WCHAR* q;
    UINT32 iLen;
   
    
    iLen = DSM_TcStrLen(pBuff);
    if(0 == iLen)
    {
        return;
    }
    p = pBuff;
    q = pBuff + iLen - 1;    

    if(DSM_TcStrCmp((PCWSTR)p, (PCWSTR)_L_DSM(".")) == 0 ||
       DSM_TcStrCmp((PCWSTR)p, (PCWSTR)_L_DSM("..")) == 0)
    {
        return;
    }

    // Trim left '.',' ','\t'
    while((0x20 == *p ||
          0x09 == *p ||
          0x2e == *p) &&
          p < q)
    {
        p++;
    }
    // Trim right.
    while((0x20 == *q ||
          0x09 == *q ||
          0x2e == *q) &&
          q >= p)
    {
        *q = '\0';
        q --;
    }
    if(p > pBuff)
    {
        DSM_TcStrCpy(pBuff,p);
    }
    return;
}



// Trim the '.',' ','\t'
VOID fat_TrimOem(UINT8* pBuff)
{
    UINT8* p;
    UINT8* q;
    UINT32 iLen;
   
    
    iLen = DSM_StrLen(pBuff);
    if(0 == iLen)
    {
        return;
    }
    p = pBuff;
    q = pBuff + iLen - 1;    

    if(DSM_StrCmp(p, _T_DSM(".")) == 0 ||
       DSM_StrCmp(p, _T_DSM("..")) == 0)
    {
        return;
    }

    // Trim left '.',' ','\t'
    while((' '  == *p ||
           '\t' == *p ||
           '.'  == *p) &&
          p < q)
    {
        p++;
    }
    // Trim right.
    while((' '  == *q ||
           '\t' == *q ||
           '.'  == *q) &&
          q >= p)
    {
        *q = '\0';
        q --;
    }
    if(p > pBuff)
    {
        DSM_StrCpy(pBuff,p);
    }
    return;
}

//-----------------------------------------------------------------------------
// fat_ChkSum()
// Returns an unsigned byte checksum computed on an unsigned byte
// array. The array must be 11 bytes long and is assumed to contain
// a name stored in the format of a MS-DOS directory entry.
// Passed: pFcbName Pointer to an unsigned byte array assumed to be
// 11 bytes long.
// Returns: Sum An 8-bit unsigned checksum of the array pointed
// to by pFcbName.
//------------------------------------------------------------------------------
UINT8 fat_ChkSum (UINT8 *pFcbName)
{
    UINT32 FcbNameLen;
    UINT8 Sum;

    Sum = 0;
    for (FcbNameLen=11; FcbNameLen!=0; FcbNameLen--) 
    {
        // NOTE: The operation is an unsigned char rotate right
        Sum = (UINT8) (((Sum & 1) ? 0x80 : 0) + (Sum >> 1) + *pFcbName++);
    }
    return (Sum);
}

BOOL fat_is_long_name_valid(WCHAR* pszFileName)
{
    BOOL bRet = TRUE;
    UINT32 iLen;
    WCHAR iCh;
    UINT32 i;

    iLen = DSM_TcStrLen(pszFileName);

    if(iLen > FAT_MAX_LONG_NAME_LEN || 0 == iLen)
    {
        return FALSE;
    }

    for(i = 0 ; i < iLen; i++)
    {
        iCh = pszFileName[i];

        // The following characters are not legal in any bytes of DIR_Name:
        // Values less than 0x20 except for the special case of 0x05 in DIR_Name[0] described above.
        if(iCh < 0x20 && iCh != 0x05)
        {
            bRet = FALSE;
            break;
        }           

        // 0x22, 0x2A, 0x2F, 0x3A, 0x3C,  0x3E, 0x3F,0x5C,0x7C.
        if(iCh == 0x3a || iCh == 0x22 ||
           iCh == 0x2a || iCh == 0x3c || 
           iCh == 0x3e || iCh == 0x3f || 
           iCh == 0x2f || iCh == 0x5c || 
           iCh == 0x7c
           )
        {
            bRet = FALSE;
            break;
        }
    }
        
    return bRet;
}


BOOL fat_is_short_name_valid(UINT8* pszFileName)
{
    BOOL   bRet = TRUE;
    UINT32 iLen;
    UINT32 iDotCount = 0;
    UINT8  iCh;
    UINT32 i;

    iLen = DSM_StrLen(pszFileName);

    if(iLen > FAT_MAX_USER_NAME_LEN || 0 == iLen)
    {
        return FALSE;
    }

    for(i = 0 ; i < iLen && bRet == TRUE; i++)
    {
        iCh = pszFileName[i];

        // The following characters are not legal in any bytes of DIR_Name:
        // Values less than 0x20 except for the special case of 0x05 in DIR_Name[0] described above.
        if(iCh <= 0x20 && iCh != 0x05)
        {
            bRet = FALSE;
        }
        else
        {
            switch(iCh)
            {
            case '\"': case '*' :
            case '+' : case ',' :
            case '/' : case ':' :
            case ';' : case '<' :
            case '=' : case '>' :
            case '?' : case '[' :
            case '\\': case ']' :
            case '|' :
                bRet = FALSE;
                break;
            case '.' :
                //  limit  '.' have once.
                if(iDotCount >= 1)
                {
                    bRet = FALSE;
                }
                ++iDotCount;
                break;
            }
        }
    }
    return bRet;
}



UINT16 fat_get_long_name(FAT_LONG_DIR_ENTRY* pLDe,UINT8* pszName)
{
    UINT16  iLen   = 0;
    UINT32  iOrd   = 0;
    UINT8*  p      = NULL;
    UINT16* pBegin = NULL;

    iOrd   = (pLDe->LDIR_Ord & 0x3f);
    p      = pszName + (iOrd - 1) * FAT_LONG_PER_ENTRY_NAME_LEN;
    pBegin = (UINT16*)p;

    //DSM_MemSet(p,0,FAT_LONG_PER_ENTRY_NAME_LEN);
    DSM_MemCpy(p,pLDe->LDIR_Name1,FLDE_LDIR_NAME1_SIZE);
    p += FLDE_LDIR_NAME1_SIZE;
    DSM_MemCpy(p,pLDe->LDIR_Name2,FLDE_LDIR_NAME2_SIZE);
    p += FLDE_LDIR_NAME2_SIZE;
    DSM_MemCpy(p,pLDe->LDIR_Name3,FLDE_LDIR_NAME3_SIZE);

    if(pLDe->LDIR_Ord & FAT_LONG_ENTRY_LAST_FLAG)
    {
        for(iLen = 0;pBegin[iLen] != 0x0 && iLen < FAT_LONG_PER_ENTRY_NAME_LEN/2;++iLen);
        --iLen;
        iLen *= sizeof(UINT16);
    }
    else
    {
        iLen = FAT_LONG_PER_ENTRY_NAME_LEN;
    }
    return iLen;
}


INT32 fat_long_to_short(WCHAR *pszLongName,
                        PSTR pszPName,
                        UINT32* piPLen,
                        PSTR pszEName,
                        UINT32* piELen,
                        UINT32* pLossy)
{
    
#if 1
	UINT16 iTime;

	if(NULL == pszLongName ||
	NULL == pszPName || 
	NULL == pszEName ||       
	NULL == piPLen ||
	NULL == piELen ||
	NULL == pLossy)
	{
		return _ERR_FAT_PARAM_ERROR;
	}
	
	DSM_MemSet(pszPName,0,9);
	DSM_MemSet(pszEName,0,4);
	iTime = DSM_GetTime();
	
    DSM_StrPrint(pszPName,"%x%x%x%x",
		(iTime & 0xf000)/0x1000,
		(iTime & 0xf00)/0x100,
        (iTime & 0xf0)/0x10,
        (iTime & 0xf)
		);
	//DSM_StrCpy(pszPName, "FOL0");
	*piPLen = 4;
	iTime = DSM_GetTime();
	iTime = (iTime & 0x0fff);

	DSM_StrPrint(pszEName,"%x%x%x",
		(iTime & 0xf00)/0x100,
		(iTime & 0xf0)/0x10,
        (iTime & 0xf)		
		);
	//DSM_StrCpy(pszEName,"XET");
	*piELen = 3;
	*pLossy = FAT_LOSSY_CONVERSION;
	
	return _ERR_FAT_SUCCESS;
#else
     typedef enum  
    {
        STATE_INIT,              // Init state.
        STATE_LEADIING_DOT,      // Leading dot.
        STATE_PRIMARY_PORTION,   // primary portion. 
        STATE_PRIMARY_OVERFLOW,  // primary overflow.
        STATE_EMBEDED_DOT,       // embeded dot.
        STATE_EXTED_PORTION,     // extension portion.
        STATE_EXTED_OVERFLOW,    // exted overflow. 
        STATE_END                // end state.
    }FAT_CONV_STATE;
    UINT32 iRet = _ERR_FAT_SUCCESS;
    UINT8 iCh;                  // tmp character
    UINT32 iLen;                // long name length.
    UINT32 iPLen = 0;           // primary portion length.
    UINT32 iELen = 0;           // extension portion length. 
    UINT32 i;     
    UINT32 iLossy = FAT_INTACT_CONVERSION;    // lossy conversion flag. if lossy ,output FAT_LOSSY_CONVERSION, else output FAT_INTACT_CONVERSION
    FAT_CONV_STATE iState = STATE_INIT;    
    BOOL bLoop = TRUE;
    UINT32 iDotCount = 0;
    UINT8 *pOemName = NULL;

	
    if(NULL == pszLongName ||
       NULL == pszPName || 
       NULL == pszEName ||       
       NULL == piPLen ||
       NULL == piELen ||
       NULL == pLossy)
    {
        return _ERR_FAT_PARAM_ERROR;
    }
    DSM_MemSet(pszPName,0,9);
    DSM_MemSet(pszEName,0,4);
    // ...1.The UNICODE name passed to the file system.
    // the step 1 in the caller function to do.

    // converted short file name.
    // ...2. The UNICODE name is converted to OEM.

    
    
    DSM_TcStrUpr(pszLongName);
    if (_ERR_FAT_SUCCESS != DSM_Unicode2OEM((UINT8*)pszLongName, DSM_UnicodeLen((UINT8*)pszLongName), &pOemName, &iLen,NULL))
    {
        D((DL_FATWARNING, "fat_long_to_short: ML_Unicode2LocalLanguage() failed!"));
        DSM_MemSet(pszPName, '_', *piPLen = 6);
        *piELen = 0;
        return _ERR_FAT_SUCCESS;
    }
    
    for(i = 0; (i < iLen) && (TRUE == bLoop); i++)
    {
        iCh = pOemName[i];
        // if the glyph is invalide in an 8.3 format, 
        // Replace the glyph to an OEM '_' (underscore) character.
        // Set a "lossy conversion" flag.
        if(iCh == 0x3a || iCh == 0x22 ||
           iCh == 0x2a || iCh == 0x3c || 
           iCh == 0x3e || iCh == 0x3f || 
           iCh == 0x2f || iCh == 0x5c || 
           iCh == 0x7c || iCh == 0x2b || 
           iCh == 0x2c || iCh == 0x3b || 
           iCh == 0x3d || iCh == 0x5b || 
           iCh == 0x5d 
           )
       {
           
            iCh = '_';                      
            iLossy = FAT_LOSSY_CONVERSION;
        }
        // Strip all leading and embedded spaces from the long name.
        if(0x20 == iCh)
        {
            continue;
        }

        switch(iState)
        {
            case STATE_INIT:             
                if('.' == iCh)
                {
                    iState = STATE_LEADIING_DOT;               
                }
                else if(0x20 == iCh)
                {
                    iLossy = FAT_LOSSY_CONVERSION;                    
                }

                else
                {
                    pszPName[iPLen] = iCh;
                    iPLen ++;
                    iState = STATE_PRIMARY_PORTION;
                }
                break;
            case STATE_LEADIING_DOT:     
                if('.' == iCh)
                {
                    // Do nothing.
                    // Strip all leading periods from the long name.
                }
                else if(0x20 == iCh)
                {
                    iLossy = FAT_LOSSY_CONVERSION;                    
                }
                else
                {
                    pszPName[iPLen] = iCh;
                    iPLen ++;
                    iPLen ++;
                    iState = STATE_PRIMARY_PORTION;
                }
                break;
            
            case STATE_PRIMARY_PORTION:  
                if('.' == iCh)
                {
                    iState = STATE_EMBEDED_DOT;
                    iDotCount ++;
                }
                else if(0x20 == iCh)
                {
                    iLossy = FAT_LOSSY_CONVERSION;                    
                }
                else 
                {
                    if(8 > iPLen)
                    {
                        pszPName[iPLen] = iCh;
                        iPLen ++;
                    }
                    else
                    {
                        iLossy = FAT_LOSSY_CONVERSION;
                        iState = STATE_PRIMARY_OVERFLOW;
                    }
                }
                break;
            case STATE_PRIMARY_OVERFLOW:  
                if('.' == iCh)
                {
                   if(iDotCount >= 1)
                   {
                        iLossy = FAT_LOSSY_CONVERSION;
                   }
                   iELen = 0;
                   iDotCount ++;
                   iState = STATE_EMBEDED_DOT;
                }                
                iLossy = FAT_LOSSY_CONVERSION;
                break;
            case STATE_EMBEDED_DOT:       
                if('.' == iCh)
                {
                   if(iDotCount >= 1)
                   {
                        iLossy = FAT_LOSSY_CONVERSION;
                   }
                   iDotCount ++;
                   iELen = 0;
                }
                else if(0x20 == iCh)
                {
                    iLossy = FAT_LOSSY_CONVERSION;                    
                }
                else 
                {                   
                    pszEName[iELen] = iCh;
                    iELen ++;
                    iState = STATE_EXTED_PORTION;                   
                }
                break;
            case STATE_EXTED_PORTION: 
                if('.' == iCh)
                {
                    if(8 > iPLen)
                    {
                        DSM_StrNCat(pszPName,pszEName,8-iPLen);
                        iPLen = DSM_StrLen(pszPName) > 8 ? 8: DSM_StrLen(pszPName);
                        iState = iPLen == 8 ? STATE_PRIMARY_OVERFLOW : STATE_PRIMARY_PORTION;    
                        
                    }
                    else
                    {                   
                        iState = STATE_PRIMARY_OVERFLOW;
                    }
                    iLossy = FAT_LOSSY_CONVERSION;
                    iELen = 0;
                    
                }
                else if(0x20 == iCh)
                {
                    iLossy = FAT_LOSSY_CONVERSION;                    
                }
                else
                {
                    if(3 == iELen)
                    {
                       iLossy = FAT_LOSSY_CONVERSION;
                       iState = STATE_EXTED_OVERFLOW;
                    }
                    else 
                    {                    
                        pszEName[iELen] = iCh;
                        iELen ++;                 
                    }
                }
                break;
            case STATE_EXTED_OVERFLOW:  
                if('.' == iCh)
                {
                    if(8 > iPLen)
                    {
                        DSM_StrNCat(pszPName,pszEName,8-iPLen);
                        iPLen = DSM_StrLen(pszPName) > 8 ? 8: DSM_StrLen(pszPName);
                        iState = iPLen == 8 ? STATE_PRIMARY_OVERFLOW : STATE_PRIMARY_PORTION;    
                        
                    }
                    else
                    {                   
                        iState = STATE_EXTED_PORTION;
                    }
                    iLossy = FAT_LOSSY_CONVERSION;
                    iELen = 0;                    
                }
                iLossy = FAT_LOSSY_CONVERSION;
                break;
            case STATE_END:
            default:
                bLoop = FALSE;
                break;
        }
    }    
    pszEName[iELen] = 0;
    pszPName[iPLen] = 0;
    *piPLen = iPLen;
    *piELen = iELen;        
    *pLossy = iLossy;  

    if(NULL != pOemName)
    {
      DSM_Free(pOemName);
    }
    return iRet;
    #endif
}


// change 0xff,0xff to 0x00 0x00 in the file name of long entry.
VOID fat_name_change(UINT8* pBuff,UINT32 iLen)
{
    UINT16 ch = 0; 
    UINT32 i; 
    
    for(i = 0; i < iLen; ++i) 
    { 
        if (i & 0x1) 
        { 
            ch |= pBuff[i]; 
            if(ch == 0xFFFF) 
            { 
                pBuff[i] = 0x0; 
                pBuff[i-1] = 0x0; 
            } 
        } 
        else 
        { 
            ch = (pBuff[i]<<8); 
        } 
    } 
}



BOOL fat_is_long_name(WCHAR* pszFileName)
{
    BOOL bRet = FALSE;
    UINT32 iLen;
    UINT32 iDotCount = 0;
    UINT32 iPrimLen = 0;
    UINT32 iExtedLen = 0;
    WCHAR iCh;
    UINT32 i;

    iLen = DSM_StrLen(pszFileName);

    if( FAT_MAX_USER_NAME_LEN < iLen )
    {
        return TRUE;
    }
  
    for(i = 0 ; i < iLen; i++)
    {        
        iCh = pszFileName[i];

        // extended ascii code.
        if(iCh >= 0x80)
        {
            bRet = TRUE;
            break;
        }
        
       // '+' ',' ';' '=' '[' ']'

        if(iCh == 0x2b || iCh == 0x2c ||
           iCh == 0x3b || iCh == 0x3d ||
           iCh == 0x5b || iCh == 0x5d)
        {
            bRet = TRUE;
            break;
        }
        //if " " 
        if(iCh == 0x20)
        {
            bRet = TRUE;
            break;
        }
        //  limit  '.' have once.
        if(iCh == 0x2e)
        {
            if(1 <= iDotCount)
            {
                bRet = TRUE;
                break;
            }
            iDotCount ++;
        }
        else if(0 == iDotCount)
        {
            iPrimLen++;
        }
        else
        {
            iExtedLen++;
        }
        
        if(8 < iPrimLen || 3 < iExtedLen)
        {
            bRet = TRUE;
            break;
        }
        
        
    }
        
    return bRet;
}



INT32 fat_get_name_for_entrys(FAT_SB_INFO* sb_info, WCHAR* filename, UINT32 bclu, UINT16 bsec, UINT16 eoff)
{
          
    INT32 iResult;                    // call function result.
    INT32 iRet = _ERR_FAT_FILE_NOT_EXISTED;    // Retrun value.    
    UINT32 iMaxSecCount;              // max sector count to read.
    UINT8* pSecBuf = NULL;                       // Sector buffer to read.
    UINT8  szFatName[FAT_MAX_NAME_LEN + 1];      // Fat name buffer.
    UINT8  szUseName[FAT_MAX_USER_NAME_LEN + 1]; // Use name buffer.   
    UINT32 iULen = 0;                            // Unicode name length. 
    FAT_DIR_ENTRY  sDe;                          // to storage the entry(long or short).
    FAT_LONG_DIR_ENTRY* psLDe = NULL;            // Point to a long dir entry struct.
    UINT32 iCurClus = 0;              // Current cluster.
    UINT16 iCurSec = 0;               // Current sector.
    UINT16 iCurEOff = 0;              // Current entry offset.    
    UINT8 iCheckSum = 0;                         // check sum number. 
    BOOL bLMatch = FALSE;                        // long name is matched.
    UINT16 iLCount = 0;                          // long dir entry count.
    UINT32 iLIndex = 0;                          // long dir entry ord index.
    UINT32 i = 0;   
    UINT8* pUniName = NULL;

    psLDe = (FAT_LONG_DIR_ENTRY*)&sDe;
    iCurClus = bclu;
    iCurSec = bsec;
    iCurEOff = eoff;
    
    // Check input paramert.
    if( bclu > 0 && bclu < 2)
    {
        D( ( DL_FATERROR, "In fat_get_name_for_entrys paramert error.bclu  == %d\n", bclu) );
        return _ERR_FAT_PARAM_ERROR;
    }

    // malloc sector buffer.
    pSecBuf = FAT_SECT_BUF_ALLOC();
    if(NULL == pSecBuf)
    {
        D( ( DL_FATERROR, "In fat_get_name_for_entrys malloc error \n") );
        DSM_ASSERT(0,"In fat_get_name_for_entrys malloc error \n");
        return _ERR_FAT_MALLOC_FAILED;        
    }

    if(FAT32 == sb_info->iFATn || 2 <= bclu) // FAT12/FAT16 sub dir or FAT32 dir.
    {
        iMaxSecCount = sb_info->iSecPerClus;  
    }
    else // FAT12/FAT16 root dir.
    {
        if(sb_info->iBytesPerSec == 0)
        {
             D( ( DL_FATERROR, "in fat_lookup_entry() sb_info->iBytesPerSec = %d.\n", sb_info->iBytesPerSec)); 
             if(NULL != pSecBuf)
             {
                FAT_SECT_BUF_FREE((SECT_BUF*)pSecBuf);
             }
             return _ERR_FAT_PARAM_ERROR;
        }
        iMaxSecCount = sb_info->iRootDirStartSec + sb_info->iRootDirSecCnt;//((sb_info->iRootEntCnt * DEFAULT_DIRENTRYSIZE)/sb_info->iBytesPerSec);
        iCurSec = (UINT16)(sb_info->iRootDirStartSec + bsec); 
    }

    do
    {   
        // loop read sector,if root dir, loop count = 32 else loop count = secPerClus.
        for(; iCurSec < iMaxSecCount; iCurSec++)
        {
            DSM_MemSet(pSecBuf, 0x00, DEFAULT_SECSIZE);

            if(FAT32 == sb_info->iFATn || 2 <= iCurClus) // FAT12/FAT16 sub dir or FAT32 dir.
            {
                iResult = fat_read_cluster(sb_info, iCurClus, iCurSec, pSecBuf);
            }
            else // FAT12/FAT16 root dir.
            {
                //cluster是就fat的File And Directory region而言的。对Root Directory Region
                //没有cluster的概念
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, iCurSec, pSecBuf);
            }
            if(_ERR_FAT_SUCCESS != iResult)
            {                
                D( ( DL_FATERROR, "In fat_get_name_for_entrys DRV_BLOCK_READ error ErrorCode = %d \n",iResult) );
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto fat_get_name_for_entrys;
            }
            
            // 取当前偏移位置的entry及其后面相关联的entry(如果是长目录项).
            for(i = iCurEOff; i < DEFAULT_SECSIZE; i += DEFAULT_DIRENTRYSIZE)
            {
                if(FAT_IS_SHORT_ENTRY(pSecBuf+i))   // short entry  
                {
                    if(FAT_IS_JAP_DELETE_ENTRY(pSecBuf + i)) // Japonic delete entry continue
                    {
                        D( ( DL_FATBRIEF, "In fat_get_name_for_entrys FAT_IS_FREE_ENTRY : i = 0x%x\n",i) );
                        iRet = _ERR_FAT_ERROR;
                        goto fat_get_name_for_entrys;
                    }
                    Buf2FDE((UINT8*)(pSecBuf+i), &sDe);
                          
                    DSM_MemCpy(szFatName, sDe.DIR_Name, FAT_MAX_NAME_LEN);
                    szFatName[FAT_MAX_NAME_LEN] = '\0';                     

                    // Check the chksum. check pass,output the long name.
                    if(iCheckSum != fat_ChkSum(szFatName))
                    {
                        D( ( DL_FATBRIEF, "fat_get_name_for_entrys: iCheckSum != fat_ChkSum(szFatName)") );
                        iRet = _ERR_FAT_ERROR;
                        goto fat_get_name_for_entrys;
                    }                            
                    // Long name matching
                    if(TRUE == bLMatch)
                    {
                        iRet = _ERR_FAT_SUCCESS;
                        goto fat_get_name_for_entrys;
                    }

                    if(_ERR_FAT_SUCCESS != NameFat2User(szFatName,szUseName,*(pSecBuf+i + 0x0c)))
                    {
                        D( ( DL_FATBRIEF, "In fat_get_name_for_entrys NameFat2User() failed!\n") );
                        iRet = _ERR_FAT_ERROR;
                        goto fat_get_name_for_entrys;
                    }
                    if(NULL != pUniName)
                    {
                        DSM_Free(pUniName);
                        pUniName = NULL;
                    }
                    iResult = DSM_OEM2Uincode(szUseName, (UINT16)DSM_StrLen(szUseName), &pUniName, &iULen,NULL);
                    
                    if(ERR_SUCCESS == iResult)
                    {
                        DSM_MemCpy(filename,pUniName,iULen);
                        iRet = ERR_SUCCESS;
                    }
                    else
                    {
                        D( ( DL_FATERROR, "In DSM_OEM2Uincode() failed,err code = %d.\n",iRet) );                        
                    }
                    goto fat_get_name_for_entrys;                        
                }
                else if(FAT_IS_FIRST_LONG_ENTRY(pSecBuf+i) || FAT_IS_MIDDLE_LONG_ENTRY(pSecBuf+i) || FAT_IS_LAST_LONG_ENTRY(pSecBuf+i))
                {
                    // 如果系统不支持长文件名，跳过长目录项。
                    if(FAT_NOT_SUPPORT_LONG_NAME == sb_info->iSuppLName)
                    {
                        continue;
                    } 
                    // 将BUF转换为目录项结构
                    Buf2FLDE((UINT8*)(pSecBuf+i), psLDe);
                    if(iLCount == 0)
                    {
                        if(FAT_IS_LAST_LONG_ENTRY(pSecBuf+i))
                        {
                            iLCount = (UINT16)(psLDe->LDIR_Ord & 0x3f);   
                            if(0 == iLCount)
                            {
                                continue;
                            }
                            
                            DSM_MemSet(filename, 0, (FAT_MAX_LONG_NAME_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
                            
                            // 按照长目录项ord从大到小的次序组装长文件名.
                            iULen = fat_get_long_name(psLDe, (UINT8*)filename);  
                            iLIndex = (psLDe->LDIR_Ord & 0x3f);                            
                            iLIndex --;
                            bLMatch = FALSE;
                            iCheckSum = psLDe->LDIR_Chksum;
                        }
                        else
                        {
                            D( ( DL_FATBRIEF, "In fat_get_name_for_entrys:iLCount == 0 but doesnot last entry") );
                            iRet = _ERR_FAT_ERROR;
                            goto fat_get_name_for_entrys;
                        }
                    }
                    else
                    {
                        if(psLDe->LDIR_Ord != iLIndex || 
                           psLDe->LDIR_Ord == 0)
                        {
                            D( ( DL_FATBRIEF, "In fat_get_name_for_entrys:psLDe->LDIR_Ord != iLIndex || psLDe->LDIR_Ord == 0") );
                            iRet = _ERR_FAT_ERROR;
                            goto fat_get_name_for_entrys;
                        }
                        if(iCheckSum != psLDe->LDIR_Chksum)
                        {
                            D( ( DL_FATBRIEF, "In fat_get_name_for_entrys:iCheckSum != psLDe->LDIR_Chksum") );
                            iRet = _ERR_FAT_ERROR;
                            goto fat_get_name_for_entrys;
                        }
                        
                        iULen = (UINT16)(iULen + fat_get_long_name(psLDe, (UINT8*)filename));  
                        iLIndex--;                        
                    }

                    // parse the first long entry,to match long name.                 
                    if(0 == iLIndex)
                    {
                        bLMatch = TRUE;                            
                    } 
                }
                else
                {
                    D( ( DL_FATBRIEF, "In fat_get_name_for_entrys FAT_IS_INVALID_ENTRY : i = 0x%x\n", i) );
                    iRet = _ERR_FAT_ERROR;
                    goto fat_get_name_for_entrys;
                }
            }
            iCurEOff = 0;
        }  

        if(FAT32 == sb_info->iFATn || 2 <= bclu) // FAT12/FAT16 sub dir or FAT32 dir.
	  {
  	       iRet = fat_get_next_cluster(sb_info, iCurClus, &iCurClus);
		  if (iRet != _ERR_FAT_SUCCESS)
		  {
  	           iRet = _ERR_FAT_FILE_NOT_EXISTED;
		      D((DL_FATERROR, "fat_get_name_for_entrys: Can't find the entry,next cluster = 0x%x.", iRet));
		      goto fat_get_name_for_entrys;
		  }
		  iCurSec = 0;
		  iCurEOff = 0;
	  }
	  else // FAT12/FAT16 root dir.
	  {
	      iRet = _ERR_FAT_FILE_NOT_EXISTED;
  	      D((DL_FATERROR, "fat_get_name_for_entrys: can't find the entry."));
		goto fat_get_name_for_entrys;
	  } 
    }while(!fat_is_last_cluster(sb_info, iCurClus) );
    
fat_get_name_for_entrys:
    if(NULL != pSecBuf)
    {
        FAT_SECT_BUF_FREE((SECT_BUF*)pSecBuf);
    }
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }    
    return iRet;    
}


// 提供给fat_format()用来标记坏簇。该函数跟fat_set_fat_entry_value()相似。
INT32 fat_format_set_fat_entry(UINT32 uDevNo, UINT32 iFatType, FAT_BOOT_RECORD *psFbr, UINT32 fatent, UINT32 value, UINT8* secBuf)
{
    UINT32 FATOffset = 0;
    UINT32 ThisFATSecNum = 0;
    UINT32 ThisFATEntOffset = 0;
    UINT16 bufvalue = 0;    
    INT32 iResult = ERR_SUCCESS;

    if(iFatType == FAT12)
    {
        FATOffset = fatent + (fatent / 2);
    }
    else if(iFatType == FAT16)
    {
        FATOffset = fatent * 2;
		
    }
    else // FAT32
    {
        FATOffset = fatent * 4;
    }

    ThisFATSecNum = psFbr->BPB_RsvdSecCnt + (FATOffset / psFbr->BPB_BytesPerSec);
    ThisFATEntOffset = FATOffset % psFbr->BPB_BytesPerSec;
    
    iResult = DRV_BLOCK_READ( uDevNo, ThisFATSecNum + psFbr->BPB_HiddSec, secBuf );
    if(_ERR_FAT_SUCCESS != iResult)
    {
        D( ( DL_FATERROR, "in fat_format_set_fat_entry, DRV_BLOCK_READ_0 ErrorCode = %d.\n",iResult) );
        return _ERR_FAT_READ_SEC_FAILED;
    }
    if(iFatType == FAT12)
    {
        iResult = DRV_BLOCK_READ( iFatType, ThisFATSecNum + 1 + psFbr->BPB_HiddSec, secBuf + DEFAULT_SECSIZE );
        if(_ERR_FAT_SUCCESS != iResult)
        {
            D( ( DL_FATERROR, "in fat_format_set_fat_entry, DRV_BLOCK_READ_1 ErrorCode = %d.\n",iResult) );
            return _ERR_FAT_READ_SEC_FAILED;
        }
    }

    if(iFatType == FAT12)
    {
        if(fatent & 0x0001) // cluster number is ODD
        {
            value = ((value << 4) & 0xfff0);
            bufvalue = MAKEFATUINT16(secBuf[ThisFATEntOffset], secBuf[ThisFATEntOffset+1]);
            bufvalue &= 0x000f;
        }
        else // cluster number is EVEN
        {
            value = value &0x0fff;
            bufvalue = MAKEFATUINT16(secBuf[ThisFATEntOffset], secBuf[ThisFATEntOffset+1]);
            bufvalue &= 0xf000;
        }
        
        bufvalue |= (UINT16)value;
        DSM_MemCpy(secBuf + ThisFATEntOffset, &bufvalue, sizeof(UINT16));
    }
    else if(iFatType == FAT16)
    {
        bufvalue = (UINT16)value;
        DSM_MemCpy(secBuf + ThisFATEntOffset, &bufvalue, sizeof(UINT16));
    }
    else
    {
        UINT32 bufvalue32;
        
        bufvalue32 = (UINT32)(value & 0x0FFFFFFF);
        *(UINT32*)(secBuf + ThisFATEntOffset) = \
            *(UINT32*)(secBuf + ThisFATEntOffset) & 0xF0000000;
        *(UINT32*)(secBuf + ThisFATEntOffset) = \
            (*(UINT32*)(secBuf + ThisFATEntOffset)) | bufvalue32;
    }      

    iResult = DRV_BLOCK_WRITE( uDevNo, ThisFATSecNum + psFbr->BPB_HiddSec, secBuf );
    if(_ERR_FAT_SUCCESS != iResult)
    {
        D( ( DL_FATERROR, "in fat_format_set_fat_entry, DRV_BLOCK_WRITE_0 ErrorCode = %d.\n", iResult) );
        return _ERR_FAT_WRITE_SEC_FAILED;
    } 
    
    if(iFatType == FAT12)
    {
        iResult = DRV_BLOCK_WRITE( uDevNo, ThisFATSecNum + 1 + psFbr->BPB_HiddSec, secBuf + DEFAULT_SECSIZE );
        if(_ERR_FAT_SUCCESS != iResult)
        {
            D( ( DL_FATERROR, "in fat_format_set_fat_entry, DRV_BLOCK_WRITE_1 ErrorCode = %d.\n", iResult) );
            return _ERR_FAT_WRITE_SEC_FAILED;
        }
    }
    
    return _ERR_FAT_SUCCESS;
}


FAT_CACHE* fat_get_fat_cache(UINT32 iDevNo)
{
	UINT32 i;
    
	for(i = 0 ; i < g_FatCahcheNum; i++)
	{  	
		if((g_FatCache + i)->iDevNo == iDevNo)
		{
			return (FAT_CACHE*)(g_FatCache + i);
		}
	}
	return NULL;
}

