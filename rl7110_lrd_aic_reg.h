/*
 * RL7110 LRD AIC Register Table
 * Auto-generated from:
 *   https://wiki.realtek.com/display/PCHUB/RL7110+LRD+AIC+Reg+Table
 *
 * Address note: Addr (rf_addr[9:7] = 3'b010), 10-bit register address.
 * Each macro maps a register NAME to its base address.
 */
#ifndef	RL7110_LRD_AIC_REG_H
#define RL7110_LRD_AIC_REG_H

#define	ACGAIN1_2_RX_A                 0x100
#define ACGAIN4_TX_A                   0x101
#define ACGAIN1_RX_B                   0x102
#define ACGAIN2_RX_B                   0x103
#define ACGAIN4_TX_B                   0x104
#define ITX_DRV_A                      0x105		//SSTX1/SSTX2 VCOM
#define ITX_DRV_B                      0x106		//SSRX VCOM
#define ADJ_IRX_A                      0x107
#define ADJ_IRX_B                      0x108
#define ADJ_ITX_A                      0x109
#define ADJ_ITX_B                      0x10A
#define DRV_OUT_A                      0x10B
#define DRV_OUT_B                      0x10C
#define RXDET_A1_1                     0x10D
#define RXDET_A2_1                     0x10E
#define RXDET_B_1                      0x10F
#define RXDET_A1_2                     0x110
#define RXDET_A2_2                     0x111
#define RXDET_B_2                      0x112
#define TERM_TEST1_TX1_A               0x113		//SSTX1 RTERM
#define TERM_TEST1_TX2_A               0x114		//SSTX2 RTERM
#define TERM_TEST1_TX_B                0x115		//SSRX RTERM
#define TERM_TEST1_RX_A                0x116		//SSTX RTERM
#define TERM_TEST1_RX1_B               0x117		//SSRX1 RTERM
#define TERM_TEST1_RX2_B               0x118		//SSRX2 RTERM
#define LFPS_AC_OFF_1_2                0x119
#define LFPS_AC_OFF_3_4                0x11A
#define NYQUIST_BOOST1_2               0x11B
#define NYQUIST_BOOST3_4               0x11C
#define NYQUIST_BOOST_TX               0x11D
#define OOBS_TESTEN_VCM_SEN_A          0x11E		//SSTX SIG_OOBS
#define OOBS_TESTEN_VCM_SEN_RX1_B      0x11F		//SSRX1 SIG_OOBS
#define OOBS_TESTEN_VCM_SEN_RX2_B      0x120		//SSRX2 SIG_OOBS
#define OOBS_LFPS_SW_RX_A              0x121
#define OOBS_LFPS_SW_RX1_B             0x122
#define OOBS_LFPS_SW_RX2_B             0x123
#define OOBS_CALI_DEBEN_LATCH_RX_A     0x124
#define OOBS_CALI_DEBEN_LATCH_RX1_B    0x125
#define OOBS_CALI_DEBEN_LATCH_RX2_B    0x126
#define OOBS_PASS_IDLE_NSQDLY_RX_A     0x127
#define OOBS_PASS_IDLE_NSQDLY_RX1_B    0x128
#define OOBS_PASS_IDLE_NSQDLY_RX2_B    0x129
#define RESERVED_RX_A                  0x12A
#define RESERVED_RX1_B                 0x12B
#define RESERVED_RX2_B                 0x12C
#define RESERVED_TX1_A                 0x12D
#define RESERVED_TX2_A                 0x12E
#define RESERVED_TX_B                  0x12F
#define TERM_TEST_MODE                 0x130
#define EN_KOFFSET_RXIN_SW             0x131
#define R_SEL1                         0x132
#define R_SEL2                         0x133
#define R_SEL3                         0x134
#define R_SEL4                         0x135
#define OFFSET_RANGE                   0x136
#define OFFSET_CODE_RX_A               0x137		//SSTX1/SSTX2 P/N offset
#define OFFSET_CODE_RX1_B              0x138		//SSRX P/N offset
#define OFFSET_CODE_RX2_B              0x139		//SSRX P/N offset
#define TX_DCGAIN_CM_TX1_A             0x13A
#define TX_DCGAIN_CM_TX2_A             0x13B
#define TX_DCGAIN_CM_TX_B              0x13C

#endif /* RL7110_LRD_AIC_REG_H */
