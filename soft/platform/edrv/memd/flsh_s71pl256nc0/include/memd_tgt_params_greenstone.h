// =============================================================================
// MEMD config
// =============================================================================
#ifndef TGT_MEMD_CONFIG
#define TGT_MEMD_CONFIG
// -------------------------------
// RAM SETTINGS
// -------------------------------

#define SRAM_TIME_SETTING {                                         \
    .rdLatHalfMode  = 0,                                            \
    .rdLat          = 4,                                            \
    .oedlyHalfMode  = 0,                                            \
    .oedly          = 0,                                            \
    .pageLat        = 0,                                            \
    .wrLatHalfMode  = 0,                                            \
    .wrLat          = 4,                                            \
    .wedlyHalfMode  = 0,                                            \
    .wedly          = 0,                                            \
    .relax          = 1,                                            \
    .forceRelax     = 0}
#define SRAM_MODE_SETTING {                                         \
    .csEn           = 1,                                            \
    .polarity       = 0,                                            \
    .admuxMode      = 0,                                            \
    .writeAllow     = 1,                                            \
    .wbeMode        = 0,                                            \
    .bedlyMode      = 0,                                            \
    .waitMode       = 0,                                            \
    .pageSize       = 0,                                            \
    .pageMode       = 0,                                            \
    .writePageMode  = 0,                                            \
    .burstMode      = 0,                                            \
    .writeBurstMode = 0,                                            \
    .advAsync       = 0,                                            \
    .advwePulse     = 0,                                            \
    .admuxDly       = 0,                                            \
    .writeSingle    = 0,                                            \
    .writeHold      = 0}


#define TGT_MEMD_RAM_CONFIG                                         \
    {                                                               \
        {SRAM_TIME_SETTING, SRAM_MODE_SETTING}                      \
    }


// -------------------------------
// FLASH SETTINGS
// -------------------------------

#define FLASH_TIME_SETTING {                                        \
    .rdLatHalfMode  = 1,                                            \
    .rdLat          = 5,                                            \
    .oedlyHalfMode  = 1,                                            \
    .oedly          = 1,                                            \
    .pageLat        = 1,                                            \
    .wrLatHalfMode  = 1,                                            \
    .wrLat          = 9,                                            \
    .wedlyHalfMode  = 1,                                            \
    .wedly          = 1,                                            \
    .relax          = 1,                                            \
    .forceRelax     = 0}
#define FLASH_MODE_SETTING {                                        \
    .csEn           = 1,                                            \
    .polarity       = 0,                                            \
    .admuxMode      = 0,                                            \
    .writeAllow     = 1,                                            \
    .wbeMode        = 0,                                            \
    .bedlyMode      = 0,                                            \
    .waitMode       = 0,                                            \
    .pageSize       = 0,                                            \
    .pageMode       = 0,                                            \
    .writePageMode  = 0,                                            \
    .burstMode      = 0,                                            \
    .writeBurstMode = 0,                                            \
    .advAsync       = 0,                                            \
    .advwePulse     = 0,                                            \
    .admuxDly       = 0,                                            \
    .writeSingle    = 0,                                            \
    .writeHold      = 0}


#define TGT_MEMD_FLASH_CONFIG                                       \
    {                                                               \
        {FLASH_TIME_SETTING, FLASH_MODE_SETTING}                    \
    }

#endif // TGT_MEMD_CONFIG
