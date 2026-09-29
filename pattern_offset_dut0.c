#include "platform.h"

int8_t pattern_offset_dut0()
{
	int8_t ret;

	uint8_t reg_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 5, 1);	//SSRX1/SSRX2
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 4, 1);	//SSTX1/SSTX2
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 3, 1);	//SSTX/SSRX

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x01:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~((1 << 3) | (3 << 0));
			reg_val |= (1 << 3) | (3 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(1500);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x02:
	{
		//SSRX1 -> SSRX
		ret = output_offset_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_VCOM_ADC3_CH, SSRX_N_VCOM_ADC3_CH, OFFSET_CODE_RX1_B);

		dut0.g_ssrx1_ssrx_pn_offset = ret;
		xil_printf("SSRX1->SSRX P/N offset: %d\r\n\r\n", dut0.g_ssrx1_ssrx_pn_offset);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x03:
	{
		//SSTX -> SSTX1
		ret = output_offset_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX1_P_VCOM_ADC1_CH, SSTX1_N_VCOM_ADC1_CH, OFFSET_CODE_RX_A);

		dut0.g_sstx_sstx1_pn_offset = ret;
		xil_printf("SSTX->SSTX1 P/N offset: %d\r\n\r\n", dut0.g_sstx_sstx1_pn_offset);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x04:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~(1 << 3);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(10);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x05:
	{
		//SSRX2 -> SSRX
		ret = output_offset_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_VCOM_ADC3_CH, SSRX_N_VCOM_ADC3_CH, OFFSET_CODE_RX2_B);

		dut0.g_ssrx2_ssrx_pn_offset = ret;
		xil_printf("SSRX2->SSRX P/N offset: %d\r\n\r\n", dut0.g_ssrx2_ssrx_pn_offset);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x06:
	{
		//SSTX -> SSTX2
		ret = output_offset_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX2_P_VCOM_ADC1_CH, SSTX2_N_VCOM_ADC1_CH, OFFSET_CODE_RX_A);

		dut0.g_sstx_sstx2_pn_offset = ret;
		xil_printf("SSTX->SSTX2 P/N offset: %d\r\n\r\n", dut0.g_sstx_sstx2_pn_offset);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x07:
	{
		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);
		print("dut0_pattern_offset_pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
