/*
 * RL7110 LRD DIC Register Table
 * Auto-generated from:
 *   https://wiki.realtek.com/display/PCHUB/RL7110+LRD+DIC+Reg+Table
 *
 * Address note: Addr (rf_addr[9:7] = 3'b001), 10-bit register address.
 * Each macro maps a register NAME to its base address.
 */
#ifndef RL7110_LRD_DIC_REG_H
#define RL7110_LRD_DIC_REG_H

#define USB_EQ1_2_A                            0x80
#define USB_EQ3_A                              0x81
#define USB_EQ1_B                              0x82
#define USB_EQ2_B                              0x83
#define USB_EQ4_A                              0x84
#define USB_RD1_2_3_A                          0x85
#define USB_RD1_2_B                            0x86
#define USB_RD3_4_AB                           0x87
#define USB_GAIN_BASIC                         0x88
#define LFPS_OOBS_OFFSET_U0_LEVEL1             0x89
#define LFPS_OOBS_OFFSET_U0_LEVEL2             0x8A
#define LFPS_OOBS_LEVEL_OFFSET_U2              0x8B
#define U0_ADD_DC                              0x8C
#define SW_OFFSET_LUT10_A                      0x8F
#define SW_OFFSET_LUT32_A                      0x90
#define SW_OFFSET_LUT10_B                      0x91
#define SW_OFFSET_LUT32_B                      0x92
#define DRVOUT_OFFSET_LUT10_A                  0x93
#define DRVOUT_OFFSET_LUT32_A                  0x94
#define DRVOUT_OFFSET_LUT10_B                  0x95
#define DRVOUT_OFFSET_LUT32_B                  0x96
#define PINMODE_LEVEL_DC                       0x97
#define DUMMY03                                0x98
#define DUMMY04                                0x99
#define DUMMY05                                0x9A
#define CFG_AUTO_EQ_EN_LEVEL                   0x9B
#define PINMODE_LEVEL_EQ1LV                    0x9C
#define PINMODE_LEVEL_CONFIG1                  0x9D
#define PINMODE_LEVEL_CONFIG2                  0x9E
#define CFG_U0_IDLE_INACT_TIME                 0x9F
#define PINMODE_C_LV0_H4_H2                    0xA0
#define PINMODE_C_LV1_H4_H2                    0xA1
#define PINMODE_C_LV2_H4_H2                    0xA2
#define PINMODE_C_LV3_H4_H2                    0xA3
#define PINMODE_C_LV4_H4_H2                    0xA4
#define PINMODE_C_LV5_H4_H2                    0xA5
#define PINMODE_C_LV6_H4_H2                    0xA6
#define PINMODE_C_LV7_H4_H2                    0xA7
#define PINMODE_C_LV8_H4_H2                    0xA8
#define PINMODE_C_LV9_H4_H2                    0xA9
#define PINMODE_C_LV10_MH                      0xAA
#define PINMODE_C_LV32_MH                      0xAB
#define PINMODE_C_LV54_MH                      0xAC
#define PINMODE_C_LV76_MH                      0xAD
#define PINMODE_C_LV98_MH                      0xAE
#define PINMODE_LV0_RD                         0xAF
#define PINMODE_LV1_RD                         0xB0
#define PINMODE_LV2_RD                         0xB1
#define PINMODE_LV3_RD                         0xB2
#define PINMODE_LV4_RD                         0xB3
#define PINMODE_LV5_RD                         0xB4
#define PINMODE_LV6_RD                         0xB5
#define PINMODE_LV7_RD                         0xB6
#define PINMODE_LV8_RD                         0xB7
#define PINMODE_LV9_RD                         0xB8
#define PINMODE_LV_ENHANCE                     0xB9
#define PIN_MODE_SETTING_A                     0xBA
#define PIN_MODE_SETTING_B                     0xBB
#define PINMODE_LV_CONFIG                      0xBC
#define PINMODE_LV_BOOST1234                   0xBD
#define PINMODE_LV_BOOST_TX                    0xBE
#define CFG_HS_ON_OFF_DBC                      0xBF
#define USB_LV_BOOST_LOAD                      0xC0
#define PINMODE_LV01_U0_ADD                    0xC1
#define PINMODE_LV23_U0_ADD                    0xC2
#define PINMODE_LV45_U0_ADD                    0xC3
#define PINMODE_LV67_U0_ADD                    0xC4
#define CFG_MODE_USB                           0xC5
#define PINMODE_LV89_U0_ADD                    0xC6
#define CFG_PIN_MODE                           0xC7
#define DC_OFFSET_A2                           0xC8		//SSTX -> SSTX2 DC offset
#define DC_OFFSET_B2                           0xC9		//SSRX2 -> SSRX DC offset
#define PINMODE_LV01_DC_OFFSET                 0xCA
#define PINMODE_LV23_DC_OFFSET                 0xCB
#define PINMODE_LV45_DC_OFFSET                 0xCC
#define PINMODE_LV67_DC_OFFSET                 0xCD
#define PINMODE_LV89_DC_OFFSET                 0xCE
#define PINMODE_LEVEL_EQ2LV                    0xD0
#define SW_LEVEL_SET                           0xD1
#define DUMMY06                                0xD3
#define DUMMY07                                0xD4
#define I2C_SETTING                            0xD5
#define I2C_ADDRESS                            0xD6
#define POWER_SAVING                           0xD7
#define CFG_RXD_INIT_TIME                      0xD8
#define CFG_RXD_PERIOD                         0xD9
#define CFG_U2_RXD_TIME                        0xDA
#define USB_SUB_STATE                          0xDB
#define CFG_AUTO_EQ_EN_LEVEL1                  0xDC
#define CFG_INACT_TIME2                        0xDD
#define CFG_IDLE_INACT_TIME1                   0xDE
#define CFG_U2_IDLE_TIME                       0xDF
#define CFG_P3P1_P2P1_TIME                     0xE0
#define CFG_LFPS_DEBOUNCE_RXDET_CNT_U2		   0xE1
#define CFG_LDO_ON_TIME                        0xE2
#define CFG_EQ_ON_OFF_DBC                      0xE3
#define RXD_FAST_EXIT_SIG_OOBS_DEGLITCH  	   0xE4
#define WRESET_CNT_MAX_MIN                     0xE5
#define AC_GAIN_MH_K                           0xE6
#define AC_GAIN_H2_K                           0xE7
#define AC_GAIN_H4_K                           0xE8
#define CFG_FAST_SIM_RXDSEL_VREF_SEL           0xEC
#define LUT_EQ_OFFSET1                         0xF0
#define LUT_EQ_OFFSET2                         0xF1
#define DC_OFFSET_A                            0xF2		//SSTX -> SSTX1 DC offset
#define DC_OFFSET_B                            0xF3		//SSRX1 -> SSRX DC offset
#define LFPS_OOBS_LUT_A                        0xF4		//SSTX LFPS_OOBS
#define LFPS_OOBS_LUT_B                        0xF5		//SSRX1/SSRX2 LFPS_OOBS
#define CFG_OOBS_QUIT_ON_TIME                  0xF9
#define DEBUG_REG                              0xFA
#define DEBUG_REG01                            0xFB
#define DEBUG_REG23                            0xFC
#define DEBUG_REG45                            0xFD
#define CFG_CONFIG                             0xFE
#define CFG_RXIDLE_L_HIGH_LOW_DEGLITCH_TIME    0xFF

#endif /* RL7110_LRD_DIC_REG_H */
