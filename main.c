/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "platform.h"

void axi_tmr0_intrq_handle();
void xgpio_intrq_handle_dut0();

DUT dut0;
uint8_t g_led_tg = 0;
uint8_t g_timer_flag = 0;

uint8_t g_s0_hub1_p1_para[20] = {0};
uint8_t g_s0_hub1_p2_para[20] = {0};
uint8_t g_s0_hub1_p3_para[20] = {0};

efuse_data_t write_data;
efuse_data_t read_data;

int main()
{
	int8_t ret;

    init_platform();

    print("FT2 start!\r\nFPGA Version:RTS5468_FT_merge_first_release_2026092801\r\n");

    pattern_initial_state_dut0();

	ret = pd_smbus_read(DUT0_IIC_0_ADDR, RL6613_ADDR, 0x81, 16, g_s0_hub1_p1_para);
	if(ret != 0)	return -1;
	for(uint8_t i=0;i<20;i++)
	{
		xil_printf("g_s0_hub1_p1_para[%02d] = %02x\r\n", i, g_s0_hub1_p1_para[i]);
	}
	print("\n");

	ret = pd_smbus_read(DUT0_IIC_0_ADDR, RL6613_ADDR, 0x82, 16, g_s0_hub1_p2_para);
	if(ret != 0)	return -1;
	for(uint8_t i=0;i<20;i++)
	{
		xil_printf("g_s0_hub1_p2_para[%02d] = %02x\r\n", i, g_s0_hub1_p2_para[i]);
	}
	print("\n");

	ret = pd_smbus_read(DUT0_IIC_0_ADDR, RL6613_ADDR, 0x83, 16, g_s0_hub1_p3_para);
	if(ret != 0)	return -1;
	for(uint8_t i=0;i<20;i++)
	{
		xil_printf("g_s0_hub1_p3_para[%02d] = %02x\r\n", i, g_s0_hub1_p3_para[i]);
	}
	print("\n");

	//DSP1(SSRX1)
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR1_L, HUB_DSP1_ADDR1_H, 0x10, g_s0_hub1_p1_para[12]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR2_L, HUB_DSP1_ADDR2_H, 0x08, g_s0_hub1_p1_para[13]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR3_L, HUB_DSP1_ADDR3_H, 0x00, g_s0_hub1_p1_para[14]);
	if(ret != 0)	return -1;

	//DSP2(SSRX2)
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR1_L, HUB_DSP2_ADDR1_H, 0x10, g_s0_hub1_p2_para[12]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR2_L, HUB_DSP2_ADDR2_H, 0x08, g_s0_hub1_p2_para[13]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR3_L, HUB_DSP2_ADDR3_H, 0x00, g_s0_hub1_p2_para[14]);
	if(ret != 0)	return -1;

	//DSP3(SSTX)
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR1_L, HUB_DSP3_ADDR1_H, 0x10, g_s0_hub1_p3_para[12]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR2_L, HUB_DSP3_ADDR2_H, 0x08, g_s0_hub1_p3_para[13]);
	if(ret != 0)	return -1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR3_L, HUB_DSP3_ADDR3_H, 0x00, g_s0_hub1_p3_para[14]);
	if(ret != 0)	return -1;

    //HUB TX measure
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 4, 1);	//GPIO28
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 2, 1);	//GPIO18
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 2, 1);	//GPIO10
    delay_ms(500);

    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 5, 1);	//GPIO29
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 1, 1);	//GPIO17
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 3, 1);	//GPIO11
    delay_ms(500);

    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 4, 0);	//GPIO28
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 2, 0);	//GPIO18
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 2, 0);	//GPIO10
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 5, 0);	//GPIO29
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 1, 0);	//GPIO17
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 3, 0);	//GPIO11

    //HUB TX measure
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 4, 1);	//GPIO28
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 2, 1);	//GPIO18
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 2, 1);	//GPIO10
    delay_ms(500);

    //DSP1 corr+, SSRX1->SSRX
    dut0.g_ssrx1_ssrx_600mv_in = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, SSRX_DC_GAIN_ADC1_CH, AVG_NUM);
    xil_printf("dut0 ssrx1_ssrx_600mv_in = %d\r\n", dut0.g_ssrx1_ssrx_600mv_in);

    if((dut0.g_ssrx1_ssrx_600mv_in < 3000) || (dut0.g_ssrx1_ssrx_600mv_in > 4000))	return -1;

    //DSP3 corr+, SSTX->SSTX1
    dut0.g_sstx_sstx1_600mv_in = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, SSTX1_DC_GAIN_ADC1_CH, AVG_NUM);
    xil_printf("dut0 sstx_sstx1_600mv_in = %d\r\n", dut0.g_sstx_sstx1_600mv_in);

    if((dut0.g_sstx_sstx1_600mv_in < 3000) || (dut0.g_sstx_sstx1_600mv_in > 4000))	return -1;

    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 5, 1);	//GPIO29
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 1, 1);	//GPIO17
    i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 3, 1);	//GPIO11
    delay_ms(500);

    //DSP2 corr+, SSRX2->SSRX
    dut0.g_ssrx2_ssrx_600mv_in = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, SSRX_DC_GAIN_ADC1_CH, AVG_NUM);
    xil_printf("dut0 ssrx2_ssrx_600mv_in = %d\r\n", dut0.g_ssrx2_ssrx_600mv_in);

    if((dut0.g_ssrx2_ssrx_600mv_in < 3000) || (dut0.g_ssrx2_ssrx_600mv_in > 4000))	return -1;

    //DSP3 corr-, SSTX->SSTX2
    dut0.g_sstx_sstx2_600mv_in = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, SSTX2_DC_GAIN_ADC1_CH, AVG_NUM);
    xil_printf("dut0 sstx_sstx2_600mv_in = %d\r\n", dut0.g_sstx_sstx2_600mv_in);

    if((dut0.g_sstx_sstx2_600mv_in < 3000) || (dut0.g_sstx_sstx2_600mv_in > 4000))	return -1;

    /* AXI GPIO configuration */
    //enable GPIO_1 global interrupt
    XGpio_WriteReg(DUT0_GPIO_1_ADDR, XGPIO_GIE_OFFSET, 0x80000000);
    //enable GPIO_1 channel 1 interrupt
    XGpio_WriteReg(DUT0_GPIO_1_ADDR, XGPIO_IER_OFFSET, XGPIO_IR_CH1_MASK);

    /* AXI timer configuration */
    //config TCSR0 as down counter, auto reload
    XTmrCtr_SetControlStatusReg(DUT0_TIMER_0_ADDR, 0, XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_DOWN_COUNT_MASK);
    //enable TCSR0 interrupt
    XTmrCtr_EnableIntr(DUT0_TIMER_0_ADDR, 0);
    //load TCSR0, 50,000,000 / 1000 - 2
    XTmrCtr_SetLoadReg(DUT0_TIMER_0_ADDR, 0, 49998);

    //register GPIO_1 ISR
    XIntc_RegisterHandler(DUT0_INTC_ADDR,
    					  XPAR_MB_DUT0_MICROBLAZE_0_AXI_INTC_MB_DUT0_AXI_GPIO_1_IP2INTC_IRPT_INTR,
                          (XInterruptHandler)xgpio_intrq_handle_dut0,
                          (void*)NULL);

    //register TCSR0 ISR
    XIntc_RegisterHandler(DUT0_INTC_ADDR,
    					  XPAR_MB_DUT0_MICROBLAZE_0_AXI_INTC_MB_DUT0_AXI_TIMER_0_INTERRUPT_INTR,
                          (XInterruptHandler)axi_tmr0_intrq_handle,
                          (void*)NULL);

    //enable INTC
    XIntc_MasterEnable(DUT0_INTC_ADDR);
    //enable each interrupt in INTC
    XIntc_EnableIntr(DUT0_INTC_ADDR, XPAR_MB_DUT0_AXI_GPIO_1_IP2INTC_IRPT_MASK | XPAR_MB_DUT0_AXI_TIMER_0_INTERRUPT_MASK);

    //start TCSR0 counting
    XTmrCtr_Enable(DUT0_TIMER_0_ADDR, 0);

    microblaze_enable_interrupts();

    ret = hub_slb_initial();

    if(ret != 0)
    {
    	print("HUB DSP3 SLB config fail, don't enter FT2 test!\r\n\r\n");
		return -1;
    }
    else
    {
    	print("HUB DSP3 SLB config success, wait for FT2 start!\r\n\r\n");
    }

    while(1)
    {
		if(dut0.g_ft2_start_ready == 0x11)
		{
			print("\r\nFT start signal start!\r\n\r\n");
			platform_initial();
		}

		if(dut0.g_ft2_start_ready == 1)
		{
//			print("dut0 redriver test~\r\n\r\n");

			ret = 0;

			switch(dut0.g_lrd_pattern_num)
			{
				case 0x00:
				{
					ret = pattern_setup_dut0();
					break;
				}
				case 0x01:
				{
					ret = pattern_rterm_dut0();
					break;
				}
				case 0x02:
				{
					ret = pattern_offset_dut0();
					break;
				}
				case 0x03:
				{
					ret = pattern_vcom_dut0();
					break;
				}
				case 0x04:
				{
					ret = pattern_dc_gain_dut0();
					break;
				}
				case 0x05:
				{
					ret = pattern_offset_2_dut0();
					break;
				}
				case 0x06:
				{
					ret = pattern_lfps_oobs_dut0();
					break;
				}
				case 0x07:
				{
					ret = pattern_sig_oobs_dut0();
					break;
				}
				case 0x08:
				{
					ret = pattern_slb_test_dut0();
					break;
				}
				case 0x09:
				{
					ret = pattern_efuse_dut0();
					break;
				}
				default:
					break;
			}
		}

		if((dut0.g_ft2_start_ready == 1) && (ret == 0) && (dut0.g_lrd_pattern_num > 0))
		{
//			print("dut0 type-c test~\r\n\r\n");

			switch(dut0.g_type_c_pattern_num)
			{
				case 0x00:
				{
					ret = pattern_rc_clock_dut0();
					break;
				}
				case 0x01:
				{
					ret = pattern_adc_dut0();
					break;
				}
				case 0x02:
				{
					ret = pattern_vmon_resistor_dut0();
					break;
				}
				case 0x03:
				{
					ret = pattern_rp_dut0();
					break;
				}
				case 0x04:
				{
					ret = pattern_ccdet_dut0();
					break;
				}
				case 0x05:
				{
					ret = pattern_vcon_sw_otp_dut0();
					break;
				}
				case 0x06:
				{
					ret = pattern_vcon_sw_ovp_dut0();
					break;
				}
				case 0x07:
				{
					ret = pattern_vcon_sw_uvlo_dut0();
					break;
				}
				case 0x08:
				{
					ret = pattern_vcon_sw_ocp_dut0();
					break;
				}
				case 0x09:
				{
					ret = pattern_vcon_sw_scp_dut0();
					break;
				}
				case 0x0a:
				{
					ret = pattern_eu2_test_dut0();
					break;
				}
				case 0x0b:
				{
					dut0.g_type_c_test_done = 1;
				}
				default:
					break;
			}
		}

		if(ret != 0)
		{
			xil_printf("dut0 lrd_pattern_num = %x, lrd_pattern_step = %x\r\n", dut0.g_lrd_pattern_num, dut0.g_lrd_pattern_step);
			xil_printf("dut0 type_c_pattern_num = %x, type_c_pattern_step = %x\r\n\r\n", dut0.g_type_c_pattern_num, dut0.g_type_c_pattern_step);

			ret = 0;
			dut0.g_result_fail = 1;

			pattern_safe_state_dut0();
		}

		if((dut0.g_ft2_test_done == 1) || (dut0.g_ft2_start_ready == 0))
		{
			if(g_timer_flag)
			{
				xil_printf("FT2 test time: %d.%03d s\r\n\r\n", dut0.g_axitmr_cnt1 / 1000, dut0.g_axitmr_cnt1 % 1000);
				dut0.g_axitmr_cnt1 = 0;

				g_timer_flag = 0;
			}
		}
    }

    cleanup_platform();
    return 0;
}
