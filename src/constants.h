#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "x32ctrl.h"

//########################################################
//
//               X32 Generic Stuff
//
//########################################################

// LED Brightness
#define X32_BRIGHTNESS_1 0x0
#define X32_BRIGHTNESS_2 0x40
#define X32_BRIGHTNESS_3 0x80
#define X32_BRIGHTNESS_4 0xC0

//########################################################
//
//               X32 Full
//
//########################################################
#ifdef X32_MODEL_FULL

#define X32_BOARD_MAIN 1
#define X32_BOARD_L 4
#define X32_BOARD_M 5
#define X32_BOARD_R 8

#define X32_BTN_TALK_A 302
#define X32_BTN_TALK_B 303
#define X32_BTN_MONITOR_DIM 300
#define X32_BTN_VIEW_MONITOR 301

#define X32_BTN_VIEW_USB 305

#define X32_BTN_48V 257
#define X32_BTN_PHASE_INVERT 258
#define X32_BTN_LOW_CUT 259

#define X32_BTN_ENCODER1 279
#define X32_BTN_ENCODER2 280
#define X32_BTN_ENCODER3 281
#define X32_BTN_ENCODER4 282
#define X32_BTN_ENCODER5 283
#define X32_BTN_ENCODER6 284

#define X32_BTN_HOME 290
#define X32_BTN_METERS 291
#define X32_BTN_ROUTING 292
#define X32_BTN_SETUP 293
#define X32_BTN_LIBRARY 294
#define X32_BTN_EFFECTS 295
#define X32_BTN_MUTE_GRP 296
#define X32_BTN_UTILITY 297

#define X32_BTN_UP 285
#define X32_BTN_DOWN 288
#define X32_BTN_LEFT 286
#define X32_BTN_RIGHT 287

#endif

//########################################################
//
//               X32 Compact
//
//    tested 2025 by code@schulzalex.de - Firmware 4.13
//
//########################################################
#ifdef X32_MODEL_COMPACT

#define X32_BOARD_MAIN 0x01
#define X32_BOARD_L 0x04
#define X32_BOARD_R 0x08

// Button and LED combined

// Board Main

#define X32_BTN_TALK_A 0x0100
#define X32_BTN_TALK_B 0x0101
#define X32_BTN_MONITOR_DIM 0x0102
#define X32_BTN_VIEW_MONITOR 0x0103

#define X32_BTN_VIEW_USB 0x0104

#define X32_BTN_48V 0x0105
#define X32_BTN_PHASE_INVERT 0x0106
#define X32_BTN_LOW_CUT 0x0107
#define X32_BTN_VIEW_CONFIG 0x0108

#define X32_BTN_GATE 0x0109
#define X32_BTN_VIEW_GATE 0x010A

#define X32_BTN_COMPRESSOR 0x010B
#define X32_BTN_VIEW_COMPRESSOR 0x010C

#define X32_BTN_EQ 0x010D
#define X32_BTN_EQ_MODE 0x010E
#define X32_BTN_EQ_HIGH 0x010F
#define X32_BTN_EQ_HIGH_MID 0x0110
#define X32_BTN_EQ_LOW_MID 0x0111
#define X32_BTN_EQ_LOW 0x0112
#define X32_BTN_VIEW_EQ 0x0113

#define X32_BTN_VIEW_MIX_BUS_SENDS 0x0114
#define X32_BTN_MONO_BUS 0x0115
#define X32_BTN_MAIN_LR_BUS 0x0116
#define X32_BTN_VIEW_BUS_MIXES 0x0117

#define X32_BTN_ENCODER1 0x0118
#define X32_BTN_ENCODER2 0x0119
#define X32_BTN_ENCODER3 0x011A
#define X32_BTN_ENCODER4 0x011B
#define X32_BTN_ENCODER5 0x011C
#define X32_BTN_ENCODER6 0x011D

#define X32_BTN_HOME 0x011E
#define X32_BTN_METERS 0x011F
#define X32_BTN_ROUTING 0x0120
#define X32_BTN_SETUP 0x0121
#define X32_BTN_LIBRARY 0x0122
#define X32_BTN_EFFECTS 0x0123
#define X32_BTN_MUTE_GRP 0x0124
#define X32_BTN_UTILITY 0x0125

#define X32_BTN_UP 0x0126
#define X32_BTN_DOWN 0x0127
#define X32_BTN_LEFT 0x0128
#define X32_BTN_RIGHT 0x0129

// Board L

#define X32_BTN_CH_1_8 0x400
#define X32_BTN_CH_9_16 0x401
#define X32_BTN_CH_17_24 0x402
#define X32_BTN_CH_25_32 0x403
#define X32_BTN_AUX_IN_1_6_USB_REC 0x404
#define X32_BTN_EFFECTS_RETURNS 0x405
#define X32_BTN_BUS_1_8_MASTER 0x406
#define X32_BTN_BUS_9_16_MASTER 0x407

#define X32_BTN_BOARD_L_CH_1_SELECT 0x420
#define X32_BTN_BOARD_L_CH_2_SELECT 0x421
#define X32_BTN_BOARD_L_CH_3_SELECT 0x422
#define X32_BTN_BOARD_L_CH_4_SELECT 0x423
#define X32_BTN_BOARD_L_CH_5_SELECT 0x424
#define X32_BTN_BOARD_L_CH_6_SELECT 0x425
#define X32_BTN_BOARD_L_CH_7_SELECT 0x426
#define X32_BTN_BOARD_L_CH_8_SELECT 0x427

#define X32_BTN_BOARD_L_CH_1_SOLO 0x430
#define X32_BTN_BOARD_L_CH_2_SOLO 0x431
#define X32_BTN_BOARD_L_CH_3_SOLO 0x432
#define X32_BTN_BOARD_L_CH_4_SOLO 0x433
#define X32_BTN_BOARD_L_CH_5_SOLO 0x434
#define X32_BTN_BOARD_L_CH_6_SOLO 0x435
#define X32_BTN_BOARD_L_CH_7_SOLO 0x436
#define X32_BTN_BOARD_L_CH_8_SOLO 0x437

#define X32_BTN_BOARD_L_CH_1_MUTE 0x440
#define X32_BTN_BOARD_L_CH_2_MUTE 0x441
#define X32_BTN_BOARD_L_CH_3_MUTE 0x442
#define X32_BTN_BOARD_L_CH_4_MUTE 0x443
#define X32_BTN_BOARD_L_CH_5_MUTE 0x444
#define X32_BTN_BOARD_L_CH_6_MUTE 0x445
#define X32_BTN_BOARD_L_CH_7_MUTE 0x446
#define X32_BTN_BOARD_L_CH_8_MUTE 0x447

// Board R

#define X32_BTN_DAW_REMOTE 0x800
#define X32_BTN_SEND_ON_FADERS 0x801
#define X32_BTN_GROUP_DCA_1_8 0x802
#define X32_BTN_BUS_1_4 0x803
#define X32_BTN_BUS_9_16 0x804
#define X32_BTN_MATRIX_MAIN_C 0x805
#define X32_BTN_CLEAR_SOLO 0x806

#define X32_BTN_SCENES_UNDO 0x07
#define X32_BTN_SCENES_GO 0x08
#define X32_BTN_SCENES_PREV 0x09
#define X32_BTN_SCENES_NEXT 0x0A
#define X32_BTN_VIEW_SCENES 0x0B

#define X32_BTN_ASSIGN_1 0x0C
#define X32_BTN_ASSIGN_2 0x0D
#define X32_BTN_ASSIGN_3 0x0E
#define X32_BTN_ASSIGN_4 0x0F
#define X32_BTN_ASSIGN_5 0x10
#define X32_BTN_ASSIGN_6 0x11
#define X32_BTN_ASSIGN_7 0x12
#define X32_BTN_ASSIGN_8 0x13
#define X32_BTN_VIEW_ASSIGN 0x14

#define X32_BTN_MUTE_GROUP_1 0x815
#define X32_BTN_MUTE_GROUP_2 0x816
#define X32_BTN_MUTE_GROUP_3 0x817
#define X32_BTN_MUTE_GROUP_4 0x818
#define X32_BTN_MUTE_GROUP_5 0x819
#define X32_BTN_MUTE_GROUP_6 0x81A

#define X32_BTN_BOARD_R_CH_1_SELECT 0x820
#define X32_BTN_BOARD_R_CH_2_SELECT 0x821
#define X32_BTN_BOARD_R_CH_3_SELECT 0x822
#define X32_BTN_BOARD_R_CH_4_SELECT 0x823
#define X32_BTN_BOARD_R_CH_5_SELECT 0x824
#define X32_BTN_BOARD_R_CH_6_SELECT 0x825
#define X32_BTN_BOARD_R_CH_7_SELECT 0x826
#define X32_BTN_BOARD_R_CH_8_SELECT 0x827
#define X32_BTN_MAIN_SELECT 0x828

#define X32_BTN_BOARD_R_CH_1_SOLO 0x830
#define X32_BTN_BOARD_R_CH_2_SOLO 0x831
#define X32_BTN_BOARD_R_CH_3_SOLO 0x832
#define X32_BTN_BOARD_R_CH_4_SOLO 0x833
#define X32_BTN_BOARD_R_CH_5_SOLO 0x834
#define X32_BTN_BOARD_R_CH_6_SOLO 0x835
#define X32_BTN_BOARD_R_CH_7_SOLO 0x836
#define X32_BTN_BOARD_R_CH_8_SOLO 0x837
#define X32_BTN_MAIN_SOLO 0x838

#define X32_BTN_BOARD_R_CH_1_MUTE 0x840
#define X32_BTN_BOARD_R_CH_2_MUTE 0x841
#define X32_BTN_BOARD_R_CH_3_MUTE 0x842
#define X32_BTN_BOARD_R_CH_4_MUTE 0x843
#define X32_BTN_BOARD_R_CH_5_MUTE 0x844
#define X32_BTN_BOARD_R_CH_6_MUTE 0x845
#define X32_BTN_BOARD_R_CH_7_MUTE 0x846
#define X32_BTN_BOARD_R_CH_8_MUTE 0x847
#define X32_BTN_MAIN_MUTE 0x848


// LED only

#define X32_LED_USB_ACCESS 0x0126

#define X32_LED_BACKLIGHT_CONFIG_GAIN 0x012A
#define X32_LED_BACKLIGHT_CONFIG_FREQ 0x012B
#define X32_LED_BACKLIGHT_GATE_THRESHOLD 0x012C
#define X32_LED_BACKLIGHT_DYNAMICS_THRESHOLD 0x012D
#define X32_LED_BACKLIGHT_EQ_Q 0x012E
#define X32_LED_BACKLIGHT_EQ_FREQ 0x012F
#define X32_LED_BACKLIGHT_EQ_GAIN 0x0130
#define X32_LED_BACKLIGHT_BUS_MIXES_LEVEL 0x0131
#define X32_LED_BACKLIGHT_BUS_MIXES_PAN 0x0132


#endif

//########################################################
//
//               X32 Producer - not tested
//
//########################################################
#ifdef X32_MODEL_PRODUCER

#define X32_BOARD_MAIN 1
#define X32_BOARD_L 4
#define X32_BOARD_R 8

#endif

//########################################################
//
//               X32 Rack - not tested
//
//########################################################
#ifdef X32_MODEL_RACK


#endif



#endif
