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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_board_config.h
/// That file describes the configuration of the board drivers for the specific
/// gallite g33 target.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _TGT_BOARD_CFG_H_
#define _TGT_BOARD_CFG_H_



// #############################################################################
// #                                                                           #
// #                      CHIP AND MANDATORY DRIVERS                           #
// #                                                                           #
// #############################################################################




// =============================================================================
// TGT_HAL_CONFIG
// =============================================================================
#ifndef TGT_HAL_CONFIG

// =============================================================================
// GPIO Assignments
// -----------------------------------------------------------------------------
// Only the free GPIOs must be used
// Beware of compatibility issues 
// ()
// This description is target specific, each GPIO can be assigned
// one of the following values:
/// pin used on it's alternative function
#define AS_ALT_FUNC     0
/// pin used as it's GPIO function
#define AS_GPIO         1
/// GPIO not connected, set to driving 0 for low power
#define NOT_CONNECTED   2
// -----------------------------------------------------------------------------
// GPIO(0)  // Free RDA1203=PENIRQ
#define TGT_HAL_GPIO_0_USED AS_GPIO
// GPIO(1)  // 
#define TGT_HAL_GPIO_1_USED AS_GPIO
// GPIO(2)  // HST_CTS
#define TGT_HAL_GPIO_2_USED AS_GPIO
// GPIO(3)  // HST_RTS
#define TGT_HAL_GPIO_3_USED AS_GPIO
// GPIO(4)  // LPSCO_1
#define TGT_HAL_GPIO_4_USED AS_GPIO
// GPIO(5)  // UART2_CTS, UART1_DSR
#define TGT_HAL_GPIO_5_USED AS_GPIO
// GPIO(6)  // UART2_TXD, UART1_CD
#define TGT_HAL_GPIO_6_USED AS_ALT_FUNC
// GPIO(7)  // UART2_RTS, UART1_RI
#define TGT_HAL_GPIO_7_USED AS_ALT_FUNC
// GPIO(8)  // UART2_RXD, UART1_DTR
#define TGT_HAL_GPIO_8_USED AS_ALT_FUNC
// GPIO(9)  // KEYOUT_5
#define TGT_HAL_GPIO_9_USED AS_GPIO
// GPIO(10) // KEYIN_5
#define TGT_HAL_GPIO_10_USED AS_ALT_FUNC
// GPIO(11) // I2S_DI1
#define TGT_HAL_GPIO_11_USED AS_GPIO
// GPIO(12) // SPI1_CLK
#define TGT_HAL_GPIO_12_USED AS_ALT_FUNC
// GPIO(13) // SPI1_CS0
#define TGT_HAL_GPIO_13_USED AS_ALT_FUNC
// GPIO(14) // SPI1_CS1, I2C3_SDA
#define TGT_HAL_GPIO_14_USED AS_ALT_FUNC
// GPIO(15) // SPI1_DIO
#define TGT_HAL_GPIO_15_USED AS_ALT_FUNC
// GPIO(16) // SPI1_DI, I2C3_SCL
#define TGT_HAL_GPIO_16_USED AS_ALT_FUNC
// GPIO(17) // I2S_BCK
#define TGT_HAL_GPIO_17_USED AS_ALT_FUNC
// GPIO(18) // LCD DATA
#define TGT_HAL_GPIO_18_USED AS_GPIO
// GPIO(19) // LCD DATA
#define TGT_HAL_GPIO_19_USED AS_GPIO
// GPIO(20) // UART1_CTS
#define TGT_HAL_GPIO_20_USED AS_ALT_FUNC
// GPIO(21) // UART1_RTS
#define TGT_HAL_GPIO_21_USED AS_ALT_FUNC
// GPIO(22) // M_CS2
#define TGT_HAL_GPIO_22_USED AS_ALT_FUNC
// GPIO(23) // M_CS3
#define TGT_HAL_GPIO_23_USED AS_ALT_FUNC
// GPIO(24) // I2C2 SCL, USB_RXD
#define TGT_HAL_GPIO_24_USED AS_ALT_FUNC
// GPIO(25) // I2C2 SDA
#define TGT_HAL_GPIO_25_USED AS_ALT_FUNC
// GPIO(26) // LCD DATA
#define TGT_HAL_GPIO_26_USED AS_GPIO
// GPIO(27) // LCD DATA
#define TGT_HAL_GPIO_27_USED AS_GPIO
// GPIO(28) // LCD DATA
#define TGT_HAL_GPIO_28_USED AS_GPIO
// GPIO(29) // LCD DATA
#define TGT_HAL_GPIO_29_USED AS_GPIO
// GPIO(30) // LCD DATA
#define TGT_HAL_GPIO_30_USED AS_GPIO
// GPIO(31) // LCD DATA
#define TGT_HAL_GPIO_31_USED AS_GPIO
// -----------------------------------------------------------------------------
// Each GPO can be assigned one of the following values:
// 0 : unused
// 1 : used
// -----------------------------------------------------------------------------
// GPO(0)   // GPO_0 KEYOUT_6, USB_SUSPEND
#define TGT_HAL_GPO_0_USED 1
// GPO(1)   // GPO_1 SPI2_DI0, USB_DIP
#define TGT_HAL_GPO_1_USED 0
// GPO(2)   // GPO_2 SPI2_CLK, USB_DIM
#define TGT_HAL_GPO_2_USED  1
// GPO(3)   // GPO_3 SPI2_CS_0, USB_OEN
#define TGT_HAL_GPO_3_USED 1
// GPO(4)   // GPO_4 SPI2_DI_1, USB_RX_DP
#define TGT_HAL_GPO_4_USED 1
// GPO(5)   // GPO_5 KEYOUT_7, USB_RX_DM
#define TGT_HAL_GPO_5_USED 1
// GPO(6)   // PWL_1, CLK_32K
#define TGT_HAL_GPO_6_USED 0
// GPO(7)   // TCO_1, DIGRF: CLK_RF_26M_EN
#define TGT_HAL_GPO_7_USED 0
// GPO(8)   // TCO_3
#define TGT_HAL_GPO_8_USED 0
// GPO(9)   // TCO_4
#define TGT_HAL_GPO_9_USED 0
// GPO(10)  // TCO_2
#define TGT_HAL_GPO_10_USED 0
// GPO(10)  // LCD_CS1
#define TGT_HAL_GPO_11_USED 1
// -----------------------------------------------------------------------------
// Each TCO can be assigned one of the following values:
// 0 : unused
// 1 : used
// -----------------------------------------------------------------------------
// TCO(0)   // SPI2_CS1, DIGRF_RXTX_DATA
#define TGT_HAL_TCO_0_USED 1
// TCO(1)   // GPO_7, DIGRF: CLK_RF_26M_EN
#define TGT_HAL_TCO_1_USED 1
// TCO(2)   // GPO_10
#define TGT_HAL_TCO_2_USED 1
// TCO(3)   // GPO_8
#define TGT_HAL_TCO_3_USED 1
// TCO(4)   // GPO_9
#define TGT_HAL_TCO_4_USED 1
// TCO(5)   // I2S_DO
#define TGT_HAL_TCO_5_USED 0
// TCO(6)   // 
#define TGT_HAL_TCO_6_USED 1
// TCO(7)   // 
#define TGT_HAL_TCO_7_USED 1
// TCO(8)   // 
#define TGT_HAL_TCO_8_USED 1
// TCO(9)   // 
#define TGT_HAL_TCO_9_USED 1
// TCO(10)   // 
#define TGT_HAL_TCO_10_USED 1
// TCO(11)   // 
#define TGT_HAL_TCO_11_USED 1
// =============================================================================


// =============================================================================
// Chip version
// =============================================================================
#define TGT_HAL_CHIP_VERSION 1


// =============================================================================
// RF CLK FREQUENCY 
// =============================================================================
#define TGT_HAL_RF_CLK_FREQ     HAL_SYS_FREQ_26M

// =============================================================================
// TGT_HAL_CAM_CFG
// -----------------------------------------------------------------------------
// This fills the structure HAL_CFG_CAM_T
// =============================================================================
#define TGT_HAL_CAM_CFG                                                 \
{                                                                       \
    .camUsed            = TRUE,                                         \
    .camRstActiveH      = FALSE,                                        \
    .camPdnActiveH      = TRUE,                                         \
    .camPdnRemap        = { .gpioId  = HAL_GPIO_NONE},                  \
    .camRstRemap        = { .gpioId  = HAL_GPIO_NONE},                  \
}

// =============================================================================
// TGT_HAL_PWM_CFG
// -----------------------------------------------------------------------------
/// This structure describes the PWM configuration for a given target. 
/// The first field identify which PWL is used for GLOW (if any).
/// The lasts fields tell wether the pin corresponding to PWM output 
/// is actually used as PWM output and not as something else (for 
/// instance as a GPIO). 
// =============================================================================
#define TGT_HAL_PWM_CFG                                                 \
{                                                                       \
    .pwlGlowPosition    = HAL_PWL_NONE,                                 \
    .pwl0Used           = TRUE,                                         \
    .pwl1Used           = TRUE,                                         \
    .pwtUsed            = FALSE,                                        \
    .lpgUsed            = FALSE                                         \
}

// =============================================================================
// HAL_CFG_I2C_T
// -----------------------------------------------------------------------------
/// This structure describes the I2C configuration for a given target. The
/// fields tell wether the corresponding I2C pins are actually used
/// for I2C and not as something else (for instance as a GPIO). 
// =============================================================================
#define TGT_HAL_I2C_CFG                                                 \
{                                                                       \
    .i2cUsed             = FALSE,                                        \
    .i2cBps              = HAL_I2C_BPS_100K,                            \
    .i2c2Used            = TRUE,                                       \
    .i2c2Bps             = HAL_I2C_BPS_100K,                            \
    .i2c3Used            = FALSE,                                       \
    .i2c3Bps             = HAL_I2C_BPS_100K,                            \
}

// =============================================================================
// TGT_HAL_I2S_CFG
// -----------------------------------------------------------------------------
/// This structure describes the I2S configuration for a given target. The
/// fields tell wether the corresponding I2S pin is actually used
/// for I2S and not as something else (for instance as a GPIO). 
// =============================================================================
#define TGT_HAL_I2S_CFG                                                 \
{                                                                       \
    .doUsed             = TRUE,                                         \
    .di0Used            = TRUE,                                         \
    .di1Used            = FALSE,                                        \
}

// =============================================================================
// TGT_HAL_EBC_CFG
// -----------------------------------------------------------------------------
/// This structure describes the EBC configuration for a given target. The first
/// fields tell wether the pin corresponding to chip select is actually used
/// as a chip select and not as something else (for instance as a GPIO). 
// =============================================================================
#define TGT_HAL_EBC_CFG                                                 \
{                                                                       \
    .cs2Used            = TRUE,                                         \
    .cs3Used            = TRUE,                                         \
    .cs4Used            = FALSE                                         \
}

// =============================================================================
// TGT_HAL_UART_CFG
// -----------------------------------------------------------------------------
/// Used to describes a configuration for used pin by an UART for a given target.
// =============================================================================
#define TGT_HAL_UART_CFG                                                \
{                                                                       \
    HAL_UART_CONFIG_FLOWCONTROL,                                        \
    HAL_UART_CONFIG_NONE                                                \
}

// =============================================================================
// TGT_HAL_SPI_CFG
// -----------------------------------------------------------------------------
/// This structure describes the SPI configuration for a given target. The first
/// fields tell wether the pin corresponding to chip select is actually used
/// as a chip select and not as something else (for instance as a GPIO). 
/// Then, the polarity of the Chip Select is given. It is only relevant
/// if the corresponding Chip Select is used as a Chip Select.
/// Finally which pin is used as input, Can be none, one or the other.
/// On most chip configuration the input 0 (di0) is on the output pin: SPI_DIO
// =============================================================================
#define TGT_HAL_SPI_CFG                                                 \
    {                                                                   \
        {                                                               \
        .cs0Used        = TRUE,                                         \
        .cs1Used        = TRUE,                                         \
        .cs2Used        = FALSE,                                        \
        .cs3Used        = FALSE,                                        \
        .cs0ActiveLow   = FALSE,                                        \
        .cs1ActiveLow   = FALSE,                                        \
        .cs2ActiveLow   = FALSE,                                        \
        .cs3ActiveLow   = FALSE,                                        \
        .di0Used        = TRUE,                                         \
        .di1Used        = TRUE                                          \
    },                                                                  \
    {                                                                   \
        .cs0Used        = FALSE,                                        \
        .cs1Used        = FALSE,                                        \
        .cs2Used        = FALSE,                                        \
        .cs3Used        = FALSE,                                        \
        .cs0ActiveLow   = FALSE,                                        \
        .cs1ActiveLow   = FALSE,                                        \
        .cs2ActiveLow   = FALSE,                                        \
        .cs3ActiveLow   = FALSE,                                        \
        .di0Used        = FALSE,                                        \
        .di1Used        = FALSE                                         \
        }                                                               \
    }

// =============================================================================
// TGT_HAL_GOUDA_CFG
// -----------------------------------------------------------------------------
/// This structure describes the GOUDA configuration for a given target. 
/// The first fields tell wether the pin corresponding to chip select is 
/// actually used as a chip select and not as something else (for instance
/// as a GPIO). If none are used, the GOUDA is considered unused.
// =============================================================================
#define TGT_HAL_GOUDA_CFG                                               \
{                                                                       \
    .cs0Used            = TRUE,                                         \
    .cs1Used            = FALSE,                                        \
    .lcdData16Bit       = FALSE,                                         \
    .lcdResetActiveLow  = TRUE,                                         \
    .lcdResetPin        = { .gpoId = HAL_GPO_11 },                      \
}


// =============================================================================
//  GPIO pins that can be GPIO but are not connected on the board
//  any pin described here will be driven low all the time for power optimization
//  It's actually computed from the TGT_HAL_GPIO_xx_USED macros above.
//  DO NOT MODIFY !
// =============================================================================
#define TGT_HAL_NO_CONNECT_GPIO         ( \
    ((TGT_HAL_GPIO_0_USED & 2) >> 1)    | \
    (TGT_HAL_GPIO_1_USED & 2)           | \
    ((TGT_HAL_GPIO_2_USED & 2) << 1)    | \
    ((TGT_HAL_GPIO_3_USED & 2) << 2)    | \
    ((TGT_HAL_GPIO_4_USED & 2) << 3)    | \
    ((TGT_HAL_GPIO_5_USED & 2) << 4)    | \
    ((TGT_HAL_GPIO_6_USED & 2) << 5)    | \
    ((TGT_HAL_GPIO_7_USED & 2) << 6)    | \
    ((TGT_HAL_GPIO_8_USED & 2) << 7)    | \
    ((TGT_HAL_GPIO_9_USED & 2) << 8)    | \
    ((TGT_HAL_GPIO_10_USED & 2) << 9)   | \
    ((TGT_HAL_GPIO_11_USED & 2) << 10)  | \
    ((TGT_HAL_GPIO_12_USED & 2) << 11)  | \
    ((TGT_HAL_GPIO_13_USED & 2) << 12)  | \
    ((TGT_HAL_GPIO_14_USED & 2) << 13)  | \
    ((TGT_HAL_GPIO_15_USED & 2) << 14)  | \
    ((TGT_HAL_GPIO_16_USED & 2) << 15)  | \
    ((TGT_HAL_GPIO_17_USED & 2) << 16)  | \
    ((TGT_HAL_GPIO_18_USED & 2) << 17)  | \
    ((TGT_HAL_GPIO_19_USED & 2) << 18)  | \
    ((TGT_HAL_GPIO_20_USED & 2) << 19)  | \
    ((TGT_HAL_GPIO_21_USED & 2) << 20)  | \
    ((TGT_HAL_GPIO_22_USED & 2) << 21)  | \
    ((TGT_HAL_GPIO_23_USED & 2) << 22)  | \
    ((TGT_HAL_GPIO_24_USED & 2) << 23)  | \
    ((TGT_HAL_GPIO_25_USED & 2) << 24)  | \
    ((TGT_HAL_GPIO_26_USED & 2) << 25)  | \
    ((TGT_HAL_GPIO_27_USED & 2) << 26)  | \
    ((TGT_HAL_GPIO_28_USED & 2) << 27)  | \
    ((TGT_HAL_GPIO_29_USED & 2) << 28)  | \
    ((TGT_HAL_GPIO_30_USED & 2) << 29)  | \
    ((TGT_HAL_GPIO_31_USED & 2) << 30)  )


// =============================================================================
//  GPIO pins that are actually used as GPIO
//  It's actually computed from the TGT_HAL_GPIO_xx_USED macros above.
//  DO NOT MODIFY !
// =============================================================================
#define TGT_HAL_USED_GPIO               ( \
    (TGT_HAL_GPIO_0_USED & 1)           | \
    ((TGT_HAL_GPIO_1_USED & 1) << 1)    | \
    ((TGT_HAL_GPIO_2_USED & 1) << 2)    | \
    ((TGT_HAL_GPIO_3_USED & 1) << 3)    | \
    ((TGT_HAL_GPIO_4_USED & 1) << 4)    | \
    ((TGT_HAL_GPIO_5_USED & 1) << 5)    | \
    ((TGT_HAL_GPIO_6_USED & 1) << 6)    | \
    ((TGT_HAL_GPIO_7_USED & 1) << 7)    | \
    ((TGT_HAL_GPIO_8_USED & 1) << 8)    | \
    ((TGT_HAL_GPIO_9_USED & 1) << 9)    | \
    ((TGT_HAL_GPIO_10_USED & 1) << 10)  | \
    ((TGT_HAL_GPIO_11_USED & 1) << 11)  | \
    ((TGT_HAL_GPIO_12_USED & 1) << 12)  | \
    ((TGT_HAL_GPIO_13_USED & 1) << 13)  | \
    ((TGT_HAL_GPIO_14_USED & 1) << 14)  | \
    ((TGT_HAL_GPIO_15_USED & 1) << 15)  | \
    ((TGT_HAL_GPIO_16_USED & 1) << 16)  | \
    ((TGT_HAL_GPIO_17_USED & 1) << 17)  | \
    ((TGT_HAL_GPIO_18_USED & 1) << 18)  | \
    ((TGT_HAL_GPIO_19_USED & 1) << 19)  | \
    ((TGT_HAL_GPIO_20_USED & 1) << 20)  | \
    ((TGT_HAL_GPIO_21_USED & 1) << 21)  | \
    ((TGT_HAL_GPIO_22_USED & 1) << 22)  | \
    ((TGT_HAL_GPIO_23_USED & 1) << 23)  | \
    ((TGT_HAL_GPIO_24_USED & 1) << 24)  | \
    ((TGT_HAL_GPIO_25_USED & 1) << 25)  | \
    ((TGT_HAL_GPIO_26_USED & 1) << 26)  | \
    ((TGT_HAL_GPIO_27_USED & 1) << 27)  | \
    ((TGT_HAL_GPIO_28_USED & 1) << 28)  | \
    ((TGT_HAL_GPIO_29_USED & 1) << 29)  | \
    ((TGT_HAL_GPIO_30_USED & 1) << 30)  | \
    ((TGT_HAL_GPIO_31_USED & 1) << 31)  )

// =============================================================================
//  GPO pins that are actually used as GPO
//  It's actually computed from the TGT_HAL_GPO_xx_USED macros above.
//  DO NOT MODIFY !
// =============================================================================
#define TGT_HAL_USED_GPO                ( \
    (TGT_HAL_GPO_0_USED & 1)            | \
    ((TGT_HAL_GPO_1_USED & 1) << 1)     | \
    ((TGT_HAL_GPO_2_USED & 1) << 2)     | \
    ((TGT_HAL_GPO_3_USED & 1) << 3)     | \
    ((TGT_HAL_GPO_4_USED & 1) << 4)     | \
    ((TGT_HAL_GPO_5_USED & 1) << 5)     | \
    ((TGT_HAL_GPO_6_USED & 1) << 6)     | \
    ((TGT_HAL_GPO_7_USED & 1) << 7)     | \
    ((TGT_HAL_GPO_8_USED & 1) << 8)     | \
    ((TGT_HAL_GPO_9_USED & 1) << 9)     | \
    ((TGT_HAL_GPO_10_USED & 1) << 10)    | \
    ((TGT_HAL_GPO_11_USED & 1) << 11)   )


// =============================================================================
//  TCO pins that are actually used as TCO
//  It's actually computed from the TGT_HAL_TCO_xx_USED macros above.
//  DO NOT MODIFY !
// =============================================================================
#define TGT_HAL_USED_TCO                ( \
    (TGT_HAL_TCO_0_USED & 1)            | \
    ((TGT_HAL_TCO_1_USED & 1) << 1)     | \
    ((TGT_HAL_TCO_2_USED & 1) << 2)     | \
    ((TGT_HAL_TCO_3_USED & 1) << 3)     | \
    ((TGT_HAL_TCO_4_USED & 1) << 4)     | \
    ((TGT_HAL_TCO_5_USED & 1) << 5)     | \
    ((TGT_HAL_TCO_6_USED & 1) << 6)     | \
    ((TGT_HAL_TCO_7_USED & 1) << 7)     | \
    ((TGT_HAL_TCO_8_USED & 1) << 8)     | \
    ((TGT_HAL_TCO_9_USED & 1) << 9)     | \
    ((TGT_HAL_TCO_10_USED & 1) << 10)   | \
    ((TGT_HAL_TCO_11_USED & 1) << 11)   )

// =============================================================================
// TGT_HAL_IO_DRIVE
// -----------------------------------------------------------------------------
/// This structure describes the IO Drive configuration for a given target. 
// =============================================================================
#define TGT_HAL_IO_DRIVE                                                \
{                                                                       \
    {                                                                   \
        .mData          = 4,                                            \
        .lsbMa          = 4,                                            \
        .msbMa          = 4,                                            \
        .ma23           = 4,                                            \
        .mCs0           = 4,                                            \
        .mCs1           = 4,                                            \
        .mCs2           = 4,                                            \
        .mCtrl          = 4,                                            \
        .mCtrlWe        = 4,                                            \
        .mClk           = 4,                                            \
        .serialLcd      = 4,                                            \
        .serialLcdSce   = 4,                                            \
        .xvrDomain      = 4,                                            \
    }                                                                   \
}


// =============================================================================
// TGT_HAL_CONFIG
// =============================================================================
#define TGT_HAL_CONFIG                                                  \
{                                                                       \
    .chipVersion        = TGT_HAL_CHIP_VERSION,                         \
    .rfClkFreq          = TGT_HAL_RF_CLK_FREQ,                          \
    .useLpsCo1          = FALSE,                                        \
    .keyInMask          = 0x1f,                                         \
    .keyOutMask         = 0x1f,                                         \
    .pwmCfg             = TGT_HAL_PWM_CFG,                              \
    .useUsbBackup       = FALSE,                                        \
    .useClk32k          = FALSE,                                        \
    .i2cCfg             = TGT_HAL_I2C_CFG,                              \
    .i2sCfg             = TGT_HAL_I2S_CFG,                              \
    .ebcCfg             = TGT_HAL_EBC_CFG,                              \
    .uartCfg            = TGT_HAL_UART_CFG,                             \
    .spiCfg             = TGT_HAL_SPI_CFG,                              \
    .spi2UseSdPins      = FALSE,                                        \
    .useSdmmc           = TRUE,                                         \
    .camCfg             = TGT_HAL_CAM_CFG,                              \
    .goudaCfg           = TGT_HAL_GOUDA_CFG,                            \
    .noConnectGpio      = TGT_HAL_NO_CONNECT_GPIO,                      \
    .usedGpio           = TGT_HAL_USED_GPIO,                            \
    .usedGpo            = TGT_HAL_USED_GPO,                             \
    .usedTco            = TGT_HAL_USED_TCO,                             \
    .ioDrive            = TGT_HAL_IO_DRIVE                              \
}

#endif // TGT_HAL_CONFIG


// =============================================================================
// KEY Mapping
// =============================================================================
#ifndef KEY_MAP

#define KEY_ROW_NUM 8
#define KEY_COL_NUM 8

#define TGT_KEY_NB (KEY_ROW_NUM * KEY_COL_NUM)

#define KEY_MAP                                                           \
{                                                                         \
    KP_POUND, KP_9, KP_6, KP_3, KP_SR , KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_0, KP_8, KP_5, KP_2, KP_DW, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_STAR,KP_7, KP_4, KP_1, KP_SND , KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_RT, KP_UP, KP_OK ,KP_LT ,KP_SL , KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
    KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, KP_UNMAPPED, \
}
#endif // KEY_MAP


// =============================================================================
// RFD config
// =============================================================================
#ifndef TGT_RFD_CONFIG
#define TGT_RFD_CONFIG

#include "hal_tcu.h"
#define UNUSED  -1
#define TGT_XCV_CONFIG    {.RST = UNUSED, .PDN  = TCO(11)              }
#define TGT_PA_CONFIG     {.ENA = UNUSED, .TXEN = UNUSED, .BS  = UNUSED }
#define TGT_SW_CONFIG     {.SW1 = TCO(2), .SW2  = TCO(3), .SW3 = TCO(4) }

// Note: Some XCV maybe have different control pin names, so someone who develop
//       the target configuration should explain the pin usage as below.
//       
// FIXME Fix that with proper knowledge !
// PA->ENA is VLOGIC pin for SKY77518, MODEN for TQM4002, MOD for RDA6216
// PA-TXEN is BIAS for RDA6216 ?
//
#endif // TGT_RFD_CONFIG


// =============================================================================
// PMD config
// -----------------------------------------------------------------------------
/// This fills the structure PMD_CONFIG_T
// =============================================================================
#ifndef TGT_PMD_CONFIG

#define TGT_PMD_CONFIG                                                  \
    {                                                                   \
        .power =                                                        \
        {                                                               \
            { /* PMD_POWER_MIC          : Micro bias enable */          \
                .ldo            = {  .opal = PMD_LDO_MIC},              \
                .polarity       = TRUE,                                 \
                .shared         = TRUE, /* with PMD_POWER_EARPIECE */   \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_CAMERA       : Camera LDO enable */          \
                .ldo            = { .opal = PMD_LDO_CAM|PMD_LDO_RF},    \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_ANALOG       : Analog LDO enable */          \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_STEREO_DAC   : Stereo DAC LDO enable */      \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_LOUD_SPEAKER : Loud Speaker enable */        \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_PA           : RF Power Amplifier */         \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_USB          : USB LDOs enable */            \
                .ldo            = { .opal = PMD_LDO_USB},               \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_SDMMC        : SD/MMC LDO enable */          \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_FM           : FM LDO enable */              \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_EARPIECE     : Ear Piece Micro bias enable */\
                .ldo            = { .opal = PMD_LDO_MIC},               \
                .polarity       = TRUE,                                 \
                .shared         = TRUE, /* with PMD_POWER_MIC */        \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_BT           : BlueTooth LDOs enable */      \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_CAMERA_FLASH : Camera Flash Light enable */  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_LCD          : (main) LCD LDO enable */      \
                .ldo            = { .opal = PMD_LDO_LCD},               \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
            { /* PMD_POWER_I2C          : I2C LDO enable */             \
                 /* Inside Gallite chip, PMD_LDO_CAM also supplies  */  \
                 /*   power to I2C1 I/O. It must be specified here  */  \
                 /*   if I2C1 is used on this board.                */  \
                 /* On this board, PMD_LDO_LCD also supplies power  */  \
                 /*   to I2C2 pull-up resistor.                     */  \
                .ldo            = { .opal = PMD_LDO_LCD},               \
                .polarity       = TRUE,                                 \
                .shared         = FALSE,                                \
                .powerOnState   = FALSE,                                \
            },                                                          \
        },                                                              \
        .level =                                                        \
        {                                                               \
            { /* PMD_LEVEL_SIM          : Sim class voltage */          \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_KEYPAD       : KeyPad Back Light level */    \
                .type           = PMD_LEVEL_TYPE_OPAL,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LCD          : (main) LCD Back Light level*/ \
                .type           = PMD_LEVEL_TYPE_BACKLIGHT,             \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_SUB_LCD      : Sub LCD Back Light level */   \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LED0         : LED0 Light level */           \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LED1         : LED1 Light level */           \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LED2         : LED2 Light level */           \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LED3         : LED3 Light level */           \
                .type           = PMD_LEVEL_TYPE_LDO,                  \
                .ldo            = { .opal = PMD_LDO_NONE},     \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_VIBRATOR     : Vibrator control level */     \
                .type           = PMD_LEVEL_TYPE_LDO,                   \
                .ldo            = { .opal = PMD_LDO_VIBR},              \
                .powerOnState   = 0,                                    \
            },                                                          \
            { /* PMD_LEVEL_LOUD_SPEAKER : loudspeaker gain */           \
                .type           = PMD_LEVEL_TYPE_NONE,                  \
                .ldo            = { .opal = PMD_LDO_NONE},              \
                .powerOnState   = 0,                                    \
            },                                                          \
        },                                                              \
        .lightLevelToRda1203 = {    0x8088, 0x8188, 0x8488, 0x8888,     \
                            0x8c88, 0x8d88, 0x8e88, 0x8f88},            \
        .lightLevelToPwm0 = {   0, 1<<3, 2<<3, 3<<3, 4<<3, 5<<3, 6<<3, 7<<3},  \
        .lightLevelToPwm1 = {   0,   0,   0,   0,   0, 224, 224, 224},  \
        /* Inside Gallite chip, PMD_LDO_MMC also supplies power to  */  \
        /*   TCU0-2, so if any of TCU0-2 is used, MMC LDO should be */  \
        /*   always on in active mode and cannot be controlled as a */  \
        /*   POWER or LEVEL LDO.                                    */  \
        /* On this board, PMD_LDO_LCD also supplies power to I2C2   */  \
        /*   pull-up resistor.                                      */  \
        .ldoEnableNormal = PMD_LDO_MMC,                     \
        .ldoEnableLowPower = 0,                                         \
        .ldoMicIs2_8 = TRUE,                                            \
        .ldoCamIs2_8 = TRUE,                                            \
        .ldoLcdIs2_8 = TRUE,                                            \
        .ldoMMCIs2_8 = TRUE,                                            \
        .ldoIbrIs2_8 = FALSE,                                           \
        .ldoRfIs2_8  = FALSE,                                           \
        .batteryGpadcChannel = HAL_ANA_GPADC_CHAN_3,                    \
        .batteryLevelFullMV          = 4200,                            \
        .batteryLevelPrechargeMV     = 3200,                            \
        .batteryChargeTimeout        = 4 HOURS,                         \
        .batteryPulseChargeEndRatio  = 4, /* 400mA / 50mA */            \
        .batteryOffsetHighActivityMV = 30,                              \
        .powerOnVoltageMV            = 3400,                            \
        .powerDownVoltageMV          = 3400,                            \
        .batteryChargeCurrent        = PMD_CHARGER_100MA,               \
        .batteryChargerTopingUp      = TRUE,                            \
        .earpieceDetectGpio          = HAL_GPIO_5, /* HAL_GPIO_NONE */  \
    }

#endif // TGT_PMD_CONFIG


// =============================================================================
// MEMD config
// =============================================================================
#include "memd_tgt_params_gallite.h"


// =============================================================================
// TGT_AUD_CONFIG
// -----------------------------------------------------------------------------
/// Audio interface configuration
// =============================================================================
#ifndef TGT_AUD_CONFIG
#define TGT_AUD_CONFIG

#define TGT_AUD_CONFIG_RECEIVER_DRIVER             CodecGallite
#define TGT_AUD_CONFIG_RECEIVER_PARAM              0
#define TGT_AUD_CONFIG_RECEIVER_OUTPUT_PATH        AUD_SPK_RECEIVER
#define TGT_AUD_CONFIG_RECEIVER_OUTPUT_TYPE        AUD_SPEAKER_STEREO
#define TGT_AUD_CONFIG_RECEIVER_INPUT_PATH         AUD_MIC_RECEIVER

#define TGT_AUD_CONFIG_EAR_PIECE_DRIVER            CodecGallite
#define TGT_AUD_CONFIG_EAR_PIECE_PARAM             0
#define TGT_AUD_CONFIG_EAR_PIECE_OUTPUT_PATH       AUD_SPK_EAR_PIECE
#define TGT_AUD_CONFIG_EAR_PIECE_OUTPUT_TYPE       AUD_SPEAKER_STEREO
#define TGT_AUD_CONFIG_EAR_PIECE_INPUT_PATH        AUD_MIC_EAR_PIECE

#define TGT_AUD_CONFIG_LOUD_SPEAKER_DRIVER         CodecGallite
#define TGT_AUD_CONFIG_LOUD_SPEAKER_PARAM          0
#define TGT_AUD_CONFIG_LOUD_SPEAKER_OUTPUT_PATH    AUD_SPK_LOUD_SPEAKER
#define TGT_AUD_CONFIG_LOUD_SPEAKER_OUTPUT_TYPE    AUD_SPEAKER_STEREO
#define TGT_AUD_CONFIG_LOUD_SPEAKER_INPUT_PATH     AUD_MIC_LOUD_SPEAKER

#define TGT_AUD_CONFIG_BT_DRIVER                   Bt
#define TGT_AUD_CONFIG_BT_PARAM                    0
#define TGT_AUD_CONFIG_BT_OUTPUT_PATH              AUD_SPK_EAR_PIECE
#define TGT_AUD_CONFIG_BT_OUTPUT_TYPE              AUD_SPEAKER_STEREO
#define TGT_AUD_CONFIG_BT_INPUT_PATH               AUD_MIC_RECEIVER

#define TGT_AUD_CONFIG_FM_DRIVER                   Fm
#define TGT_AUD_CONFIG_FM_PARAM                    0
#define TGT_AUD_CONFIG_FM_OUTPUT_PATH              AUD_SPK_EAR_PIECE
#define TGT_AUD_CONFIG_FM_OUTPUT_TYPE              AUD_SPEAKER_STEREO
#define TGT_AUD_CONFIG_FM_INPUT_PATH               AUD_MIC_RECEIVER

#endif // TGT_AUD_CONFIG



// #############################################################################
// #                                                                           #
// #                           OPTIONNAL DRIVERS                               #
// #                                                                           #
// #############################################################################



#ifdef TGT_WITH_MCD_IF_spi
// =============================================================================
// MCD config
// -----------------------------------------------------------------------------
/// @Elem HAL_SPI_CS0
/// @Elem HAL_GPIO_13 GPIO over CS0 used for MMC init process step to drive CS
/// to 1 during at least 74 clocks.
// =============================================================================
#ifndef TGT_MCD_CONFIG
#define TGT_MCD_CONFIG                                                  \
    {                                                                   \
        HAL_SPI_CS1,                                                    \
        HAL_GPIO_7                                                      \
    }
#endif // TGT_MCD_CONFIG
#endif
#ifdef TGT_WITH_MCD_IF_sdmmc
// =============================================================================
// MCD config
// -----------------------------------------------------------------------------
/// @elem cardDetectGpio
/// Gpio Connected to socket to detect card insertion/removal .
/// (set to #HAL_GPIO_NONE) if not available.
/// @elem gpioCardDetectHigh
/// Define the polarity of the above GPIO: \c TRUE when active high,
/// \c FALSE when active low.
/// @elem dat3HasPullDown
/// Wether or not the DAT3/CD line has a pull-down and can be used for 
/// minimalist new card detection (no insertion/removal interrupt.)
// =============================================================================
#ifndef TGT_MCD_CONFIG
#define TGT_MCD_CONFIG                                                  \
    {                                                                   \
        .cardDetectGpio     = HAL_GPIO_NONE,                            \
        .gpioCardDetectHigh = TRUE,                                     \
        .dat3HasPullDown    = FALSE                                     \
    }
#endif // TGT_MCD_CONFIG
#endif

#ifdef TGT_WITH_TS_MODEL_rda1203_gallite
// =============================================================================
// TSD config
// -----------------------------------------------------------------------------
/// This fills the TSD_CONFIG_T structure
// =============================================================================
#ifndef TGT_TSD_CONFIG
#define TGT_TSD_CONFIG                                                  \
    {                                                                   \
        .penGpio        = HAL_GPIO_0,                                   \
        .debounceTime   = 5*HAL_TICK1S/1000,                            \
        .downPeriod     = 3,                                            \
        .upPeriod       = 3,                                            \
        .maxError       = 0x50                                          \
    }

#endif // TGT_TSD_CONFIG
#endif // TGT_WITH_TS_MODEL_rda1203_gallite

#ifdef TGT_WITH_FM_MODEL_rda5802e_gallite 
// =============================================================================
// FMD config
// -----------------------------------------------------------------------------
/// This fills the FMD_CONFIG_T structure for RDA5802e_gallite
/// @Elem HAL_SPI Either SPI1 or SPI2
/// @Elem HAL_SPI_CS1 Chip select on the selected SPI ID
// =============================================================================
#ifndef TGT_FMD_CONFIG
#define TGT_FMD_CONFIG                                                  \
    {                                                                   \
        .powerOnTime        = 16384,                                    \
        .channelSpacing     = FMD_CHANNEL_SPACE_50,                     \
        .seekRSSIThreshold  = 0x10                                      \
    }

#endif // TGT_FMD_CONFIG
#endif // TGT_WITH_FM_MODEL_rda5802e_gallite

#ifdef TGT_WITH_GSENSOR_MODEL_mma7660fc
// =============================================================================
//  Gsensor config
// -----------------------------------------------------------------------------
/// This fills the Gsensor CONFIG_T structure for MMA7660FC
// =============================================================================
#ifndef TGT_GSENSOR_CONFIG
#define TGT_GSENSOR_CONFIG                                              \
    {                                                                   \
        .i2cBusId           = HAL_I2C_BUS_ID_2,                         \
        .gsensorGpio        = HAL_GPIO_3                                \
    }
#endif // TGT_GSENSOR_CONFIG
#endif // TGT_WITH_GSENSOR_MODEL_mma7660fc

#ifdef TGT_WITH_GSENSOR_MODEL_mma7660fc_gpio
// =============================================================================
//  GsensorGpio config
// -----------------------------------------------------------------------------
/// This fills the Gsensor CONFIG_T structure for MMA7660FC GPIO
// =============================================================================
#ifndef TGT_GSENSOR_CONFIG
#define TGT_GSENSOR_CONFIG                                              \
    {                                                                   \
        .i2cBusId           = HAL_I2C_BUS_ID_2,                         \
        .gsensorGpio        = HAL_GPIO_5,                               \
        .scl_i2c_gpio       = HAL_GPIO_NONE ,                           \
        .scl_i2c_gpo        = HAL_GPO_4 ,                               \
        .sda_i2c            = HAL_GPIO_3                                \
}
#endif // TGT_GSENSOR_CONFIG 
#endif // TGT_WITH_GSENSOR_MODEL_mma7660fc_gpio


#ifdef TGT_WITH_GPIOI2C_MODEL_i2c_gpio
// =============================================================================
//  GPIO I2C config
// -----------------------------------------------------------------------------
/// This fills the GPIOI2C CONFIG_T structure
// =============================================================================
#ifndef TGT_GPIOI2C_CONFIG
#define TGT_GPIOI2C_CONFIG                                              \
    {                                                                   \
        .i2c_gpio_Bps       = GPIO_I2C_BPS_80K ,                        \
        .scl_i2c_gpio       = HAL_GPIO_24 ,                             \
        .scl_i2c_gpo        = HAL_GPO_NONE ,                            \
        .sda_i2c            = HAL_GPIO_25                               \
}
#endif // TGT_GPIOI2C_CONFIG 
#endif // TGT_WITH_GPIOI2C_MODEL_i2c_gpio


// =============================================================================
// BTD config
// -----------------------------------------------------------------------------
/// This fills the BTD_CONFIG_T structure for RDA5868
// =============================================================================
#ifndef TGT_BTD_CONFIG
#define TGT_BTD_CONFIG                                                  \
    {                                                                   \
        .i2cBusId  = HAL_I2C_BUS_ID_2,                                  \
        .pinReset  = { .gpoId  = HAL_GPIO_9 },                          \
        .pinWakeUp = { .gpioId = HAL_GPIO_NONE },                       \
        .pinSleep  = { .gpioId = HAL_GPIO_1 }                           \
    }
#endif // TGT_BTD_CONFIG


// =============================================================================
// TGT_SIMD_CONFIG
// -----------------------------------------------------------------------------
/// Configuration for SIMD
// =============================================================================
#ifndef TGT_SIMD_CONFIG
#define TGT_SIMD_CONFIG                                                 \
{                                                                       \
}
#endif // TGT_SIMD_CONFIG


// =============================================================================
// DSIM config
// -----------------------------------------------------------------------------
/// This fills the dual sim configuration
// =============================================================================
#ifndef TGT_DUALSIM_CONFIG
#define TGT_DUALSIM_CONFIG                                              \
    {                                                                   \
        HAL_SPI,                                                        \
        HAL_SPI_CS1                                                     \
    }
#endif // TGT_DUALSIM_CONFIG


// =============================================================================
// TGT_CAMD_CONFIG
// -----------------------------------------------------------------------------
/// Configuration for CAMD
// =============================================================================
#ifndef TGT_CAMD_CONFIG
#define TGT_CAMD_CONFIG                                                 \
{                                                                       \
    .i2cBusId          = HAL_I2C_BUS_ID_2,                              \
}
#endif // TGT_CAMD_CONFIG


// =============================================================================
// TGT_CAMS_CONFIG
// -----------------------------------------------------------------------------
/// Configuration for CAMS
// =============================================================================
#ifndef TGT_CAMS_CONFIG
#define TGT_CAMS_CONFIG                                                 \
{                                                                       \
    .sensorOrientation = 1                                              \
}
#endif // TGT_CAMS_CONFIG


// =============================================================================
// ?d config
// -----------------------------------------------------------------------------
/// @todo add other driver configuration here if needed
// =============================================================================



// #############################################################################
// #                                                                           #
// #                                 SERVICES                                  #
// #                                                                           #
// #############################################################################



// =============================================================================
// ?s config
// -----------------------------------------------------------------------------
/// @todo add other service configuration here if needed
// =============================================================================




#endif //_TGT_BOARD_CFG_H_

