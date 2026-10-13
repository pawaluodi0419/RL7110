#include "platform.h"

int8_t pattern_vcom_dut0()
{
	int8_t ret;

	uint8_t reg_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		//SSRX2 -> SSRX
		ret = output_vcom_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_VCOM_ADC3_CH, SSRX_N_VCOM_ADC3_CH, ITX_DRV_B, 0);
		if(ret != -1)
		{
			dut0.g_ssrx2_ssrx_vcom_offset = ret;
			xil_printf("SSRX2->SSRX VCOM offset: %d\r\n\r\n", dut0.g_ssrx2_ssrx_vcom_offset);
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x01:
	{
		//SSTX -> SSTX2
		ret = output_vcom_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX2_P_VCOM_ADC1_CH, SSTX2_N_VCOM_ADC1_CH, ITX_DRV_A, 0);
		if(ret != -1)
		{
			dut0.g_sstx_sstx2_vcom_offset = ret;
			xil_printf("SSTX->SSTX2 VCOM offset: %d\r\n\r\n", dut0.g_sstx_sstx2_vcom_offset);
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x02:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val |= (1 << 3);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			mdio_master_write(DUT0_MDIO_BASEADDR, OFFSET_CODE_RX_A, dut0.g_sstx_sstx1_pn_offset);

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

	case 0x03:
	{
		//SSRX1 -> SSRX
		ret = output_vcom_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_VCOM_ADC3_CH, SSRX_N_VCOM_ADC3_CH, ITX_DRV_B, 0);
		if(ret != -1)
		{
			dut0.g_ssrx1_ssrx_vcom_offset = ret;
			xil_printf("SSRX1->SSRX VCOM offset: %d\r\n\r\n", dut0.g_ssrx1_ssrx_vcom_offset);
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x04:
	{
		//SSTX -> SSTX1
		ret = output_vcom_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX1_P_VCOM_ADC1_CH, SSTX1_N_VCOM_ADC1_CH, ITX_DRV_A, 1);
		if(ret != -1)
		{
			dut0.g_sstx_sstx1_vcom_offset = ret;
			xil_printf("SSTX->SSTX1 VCOM offset: %d\r\n\r\n", dut0.g_sstx_sstx1_vcom_offset);
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x05:
	{
		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num, DUT0_PATTERN_LRD);
		print("dut0_pattern_vcom_pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
