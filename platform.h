/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#ifndef SDT
#include "platform_config.h"
#include "xparameters.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"

#include "xgpio.h"
#include "xgpio_l.h"
#include "xiic_l.h"
#include "xintc_l.h"
#include "xtmrctr.h"
#include "xtmrctr_l.h"
#include "xuartlite_l.h"
#include "mb_interface.h"

#include "gpio_drv.h"
#include "iic_drv.h"
#include "timer_drv.h"
#include "mdio_drv.h"
#include "pca9506_drv.h"
#include "ad7998_drv.h"
#include "dac_max5825_drv.h"
#include "dac_ad5504_drv.h"
#include "rl7110_lrd_aic_reg.h"
#include "rl7110_lrd_dic_reg.h"
#include "hub_tx_parameter.h"

#include "pattern_setup.h"
#include "pattern_rterm.h"
#include "pattern_offset.h"
#include "pattern_vcom.h"
#include "pattern_dc_gain.h"
#include "pattern_lfps_oobs.h"
#include "pattern_sig_oobs.h"
#include "pattern_slb_test.h"
#include "pattern_efuse.h"

#include "pattern_rc_clock.h"
#include "pattern_adc.h"
#include "pattern_vmon_resistor.h"
#include "pattern_rp.h"
#include "pattern_ccdet.h"
#include "pattern_vcon_sw_otp.h"
#include "pattern_vcon_sw_ovp.h"
#include "pattern_vcon_sw_uvlo.h"
#include "pattern_vcon_sw_ocp.h"
#include "pattern_vcon_sw_scp.h"
#include "pattern_eu2_test.h"

#define	RESET_VECTOR_ADDRESS	0x00000000

typedef void (*reset_function_t)();

#define	DUT0_GPIO_0_ADDR			XPAR_MB_DUT0_AXI_GPIO_0_BASEADDR
#define	DUT0_GPIO_1_ADDR			XPAR_MB_DUT0_AXI_GPIO_1_BASEADDR
#define	DUT0_GPI_DIS_ADDR 			XPAR_MB_DUT0_GPI_DIS_0_S00_AXI_BASEADDR
#define	DUT0_IIC_0_ADDR				XPAR_MB_DUT0_AXI_IIC_0_BASEADDR
#define	DUT0_IIC_1_ADDR				XPAR_MB_DUT0_AXI_IIC_1_BASEADDR
#define	DUT0_IIC_2_ADDR				XPAR_MB_DUT0_AXI_IIC_2_BASEADDR
#define	DUT0_TIMER_0_ADDR			XPAR_MB_DUT0_AXI_TIMER_0_BASEADDR
#define	DUT0_INTC_ADDR				XPAR_MB_DUT0_MICROBLAZE_0_AXI_INTC_BASEADDR
#define	DUT0_MDIO_BASEADDR			XPAR_MB_DUT0_MDIO_MASTER_AXI_0_S00_AXI_BASEADDR
#define	DUT0_INF_CHECK_ADDR			XPAR_MB_DUT0_V50_INTERFACE_CHECK_0_S00_AXI_BASEADDR
#define	DUT0_GATE_COUNTER_0_ADDR	XPAR_MB_DUT0_GATE_COUNTER_0_S00_AXI_BASEADDR
#define	DUT0_GATE_COUNTER_1_ADDR	XPAR_MB_DUT0_GATE_COUNTER_1_S00_AXI_BASEADDR

#define GPI_DIS_WRITE_REG(base_addr, reg_offset, data) \
	Xil_Out32((base_addr) + (reg_offset), (uint32_t)(data))

#define GPI_DIS_READ_REG(base_addr, reg_offset) \
	Xil_In32((base_addr) + (reg_offset))

#define MDIO_WRITE_REG(base_addr, reg_offset, data) \
	Xil_Out32((base_addr) + (reg_offset), (uint32_t)(data))

#define MDIO_READ_REG(base_addr, reg_offset) \
	Xil_In32((base_addr) + (reg_offset))

#define INF_CHECK_WRITE_REG(base_addr, reg_offset, data) \
	Xil_Out32((base_addr) + (reg_offset), (uint32_t)(data))

#define INF_CHECK_READ_REG(base_addr, reg_offset) \
	Xil_In32((base_addr) + (reg_offset))

#define FREQ_MEAS_WRITE_REG(base_addr, reg_offset, data) \
	Xil_Out32((base_addr) + (reg_offset), (uint32_t)(data))

#define FREQ_MEAS_READ_REG(base_addr, reg_offset) \
	Xil_In32((base_addr) + (reg_offset))

#define	FT_FW_VER	1

typedef struct{
	uint8_t g_ft2_start_ready;
	uint8_t g_ft2_test_done;
	uint8_t g_ft2_retest;
	int8_t g_result_fail;
	uint8_t g_type_c_pattern_num;
	uint8_t g_type_c_pattern_step;
	uint8_t g_type_c_test_timer;
	uint8_t g_type_c_test_done;
	uint8_t g_lrd_pattern_num;
	uint8_t g_lrd_pattern_step;
	uint8_t g_lrd_pattern_substep;
	uint32_t g_pattern_timer;
	uint32_t g_axitmr_cnt0;
	uint32_t g_axitmr_cnt1;
	uint32_t g_test_cnt;
	uint32_t g_pass_cnt;
	uint32_t g_eqa_test_cnt;
	uint32_t g_eqa_pass_cnt;
	uint32_t g_gpio_0_shadow_state[2];
	uint32_t g_gpio_1_shadow_state[2];

	uint8_t g_delay_control_bit;
	uint8_t g_delay_done_bit;
	uint16_t g_delay_count;

	uint8_t g_delay2_control_bit;
	uint8_t g_delay2_done_bit;
	uint16_t g_delay2_count;

	uint8_t  g_dut_addr;
	uint8_t  g_dut_ver;

	uint16_t g_ssrx1_ssrx_600mv_in;
	uint16_t g_ssrx2_ssrx_600mv_in;
	uint16_t g_sstx_sstx1_600mv_in;
	uint16_t g_sstx_sstx2_600mv_in;

	uint8_t g_ssrx1_rterm_offset;
	uint8_t g_ssrx2_rterm_offset;
	uint8_t g_sstx1_rterm_offset;
	uint8_t g_sstx2_rterm_offset;
	uint8_t g_sstx_rterm_offset;
	uint8_t g_ssrx_rterm_offset;

	uint8_t g_pn_offset_test_cnt;
	uint8_t g_ssrx1_ssrx_pn_offset;
	uint8_t g_ssrx2_ssrx_pn_offset;
	uint8_t g_sstx_sstx1_pn_offset;
	uint8_t g_sstx_sstx2_pn_offset;

	uint8_t g_ssrx1_ssrx_vcom_offset;
	uint8_t g_ssrx2_ssrx_vcom_offset;
	uint8_t g_sstx_sstx1_vcom_offset;
	uint8_t g_sstx_sstx2_vcom_offset;

	uint8_t g_ssrx1_ssrx_dc_offset;
	uint8_t g_ssrx2_ssrx_dc_offset;
	uint8_t g_sstx_sstx1_dc_offset;
	uint8_t g_sstx_sstx2_dc_offset;

	uint16_t g_adc_data_eq_off;
	uint16_t g_adc_data_eq_on;
	uint8_t	g_lfps_oobs_signbit;
	uint8_t	g_lfps_oobs_offset;
	uint8_t g_ssrx1_lfps1_oobs_offset;
	uint8_t g_ssrx2_lfps1_oobs_offset;
	uint8_t g_sstx_lfps1_oobs_offset;
	uint8_t g_ssrx1_lfps2_oobs_offset;
	uint8_t g_ssrx2_lfps2_oobs_offset;
	uint8_t g_sstx_lfps2_oobs_offset;

	uint8_t g_sig_oobs_retry_bit;
	uint8_t	g_sig_oobs_offset;
	uint8_t g_ssrx1_sig_oobs_offset;
	uint8_t g_ssrx2_sig_oobs_offset;
	uint8_t g_sstx_sig_oobs_offset;

	uint8_t g_rc16m_coarse_offset;
	uint8_t g_rc16m_fine_offset;
	uint8_t g_rc50m_offset;

	uint16_t g_adc_a;
	uint8_t g_adc_b_signbit;
	uint8_t g_adc_b_offset;

	uint8_t g_vmon_resistor;

	uint8_t g_cc1_rp80ua_signbit;
	uint8_t g_cc1_rp80ua_offset;
	uint8_t g_cc2_rp80ua_signbit;
	uint8_t g_cc2_rp80ua_offset;
	uint8_t g_cc1_rp180ua_signbit;
	uint8_t g_cc1_rp180ua_offset;
	uint8_t g_cc2_rp180ua_signbit;
	uint8_t g_cc2_rp180ua_offset;
	uint8_t g_cc1_rp330ua_signbit;
	uint8_t g_cc1_rp330ua_offset;
	uint8_t g_cc2_rp330ua_signbit;
	uint8_t g_cc2_rp330ua_offset;

	uint8_t g_vcon_sw_otp_offset;
	uint8_t g_cc1_ovp_offset;
	uint8_t g_cc2_ovp_offset;
	uint8_t g_vcon_sw_uvlo_offset;
	uint8_t g_cc1_ocp_kos_offset;
	uint8_t g_cc2_ocp_kos_offset;
	uint8_t g_cc1_ocp_350ma_offset;
	uint8_t g_cc2_ocp_350ma_offset;
	uint8_t g_cc1_ocp_500ma_offset;
	uint8_t g_cc2_ocp_500ma_offset;
	uint8_t g_cc1_scp_250ma_offset;
	uint8_t g_cc2_scp_250ma_offset;
	uint8_t g_cc1_scp_400ma_offset;
	uint8_t g_cc2_scp_400ma_offset;

	uint8_t g_eu2_cal_osc;
	uint8_t g_eu2_cap_osc;
	uint16_t g_hs_j_u2_base_diff;
	uint16_t g_hs_j_eu2_base_diff;
	uint16_t g_hs_j_eu2_best_diff;
	uint8_t g_u2_z0_offset;
	uint8_t g_eu2_rsrc_offset;
	uint8_t g_eu2_rrcv_offset;
	uint8_t g_u2_swing_offset;
	uint8_t g_eu2_efuse_w_data[40];
	uint8_t g_eu2_efuse_r_data[40];
}DUT;

extern DUT dut0;
extern uint8_t g_led_tg;
extern uint8_t g_timer_flag;

extern uint8_t g_s0_hub1_p1_para[20];
extern uint8_t g_s0_hub1_p2_para[20];
extern uint8_t g_s0_hub1_p3_para[20];

extern efuse_data_t write_data;
extern efuse_data_t read_data;

void pattern_initial_state_dut0();
void pattern_safe_state_dut0();

void platform_initial();
void microblaze_softreset();

void init_platform();
void cleanup_platform();

#endif
