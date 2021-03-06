////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://svn.coolsand-tech.com/svn/developing1/Sources/chip/branches/gallite441/bcpu/spal/src/bcpu_rom.S $ //
//    $Author: huazeng $                                                        // 
//    $Date: 2010-10-11 14:24:51 +0800 (Mon, 11 Oct 2010) $                     //   
//    $Revision: 2715 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file bcpu_rom.S                                                          //
/// That file provides the boot code                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "regdef.h"
#include "cp0.h"

#include "global_macros.h"
#include "tcu_asm.h"
#include "mem_bridge_asm.h"
#include "sys_ctrl_asm.h"
#include "timer_asm.h"
#include "debug_port_asm.h"
#include "bb_sram_asm.h"

#include "tester_chip_asm.h"

    
        
///////////////////////////////////////////////////////////////////////////////
// MACROS
///////////////////////////////////////////////////////////////////////////////

    

.globl bcpu_main_entry

.text
.set at
//############################################
//# BCPU BOOT at 0x1000
//############################################
.globl  __bcpu_start
__bcpu_start:
    nop
    // ------------------------------------
    // Initialize caches
    // Flush both caches
    // ------------------------------------
    cache 0,0
    nop

    // ------------------------------------
    //  initialize stack pointer and GP
    // ------------------------------------
    .globl bcpu_stack_base
    lw  sp, bcpu_stack_base


    la    v0, __bcpu_init
    jr    v0



// ##################################################################
// # Initialization function - Must be put after Uhandler definition
// ##################################################################
.globl  __bcpu_init
__bcpu_init:
    // barely nothing to do the xcpu does the setup of the system

//  Enable ITs with handler in ram and enable CP2 
    li        t0,    Status_BEV | Status_IntMASK | Status_IEc | 0x40000000
    mtc0    zero, Cause
    nop
    mtc0    t0, Status

    lw      ra, __bb_int_handler //hal_ErrorHandler// if return from main... error
    lw      v0, bcpu_main_entry // jump to the bcpu_main
    j       v0







