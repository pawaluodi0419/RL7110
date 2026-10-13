#include "platform.h"

int8_t pattern_dc_gain_dut0()
{
	int8_t ret;

	uint8_t reg_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
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

		delay_ms(1);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x01:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 4, 0);	//GPIO28

			//SSRX1 to HUBTX
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 0, 1);	//GPIO0
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 2, 0);	//GPIO2
			//SSRX to ADL5906
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 6, 1);	//GPIO14
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 0, 0);	//GPIO16
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 2, 0);	//GPIO18

			//SSTX to HUBTX
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 5, 1);	//GPIO13
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 7, 0);	//GPIO15
			//SSTX1 to ADL5906
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 6, 1);	//GPIO6
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 0, 0);	//GPIO8
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 2, 0);	//GPIO10

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

	case 0x02:
	{
		//SSRX1 -> SSRX
		ret = dc_gain_calib_dut0(DUT0_IIC_1_ADDR, ADC1_ADDR, SSRX_DC_GAIN_ADC1_CH, DC_OFFSET_B, dut0.g_ssrx1_ssrx_600mv_in);

		if(ret != -1)
		{
			dut0.g_ssrx1_ssrx_dc_offset = ret;
			xil_printf("SSRX1->SSRX DC offset: %d\r\n\r\n", dut0.g_ssrx1_ssrx_dc_offset);

			if(dut0.g_ssrx1_ssrx_dc_offset >= 1)
			{
				mdio_master_write(DUT0_MDIO_BASEADDR, DC_OFFSET_B, dut0.g_ssrx1_ssrx_dc_offset - 1);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x03:
	{
		//SSTX -> SSTX1
		ret = dc_gain_calib_dut0(DUT0_IIC_1_ADDR, ADC1_ADDR, SSTX1_DC_GAIN_ADC1_CH, DC_OFFSET_A, dut0.g_sstx_sstx1_600mv_in);

		if(ret != -1)
		{
			dut0.g_sstx_sstx1_dc_offset = ret;
			xil_printf("SSTX->SSTX1 DC offset: %d\r\n\r\n", dut0.g_sstx_sstx1_dc_offset);

			if(dut0.g_sstx_sstx1_dc_offset >= 1)
			{
				mdio_master_write(DUT0_MDIO_BASEADDR, DC_OFFSET_A, dut0.g_sstx_sstx1_dc_offset - 1);
			}
			else
			{
				return -1;
			}
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
		if(dut0.g_delay_control_bit == 0)
		{
			//SSRX2 to HUBTX
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 1, 1);	//GPIO1
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 3, 0);	//GPIO3
			//SSRX to ADL5906
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 6, 1);	//GPIO14
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 0, 0);	//GPIO16
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 2, 0);	//GPIO18

			//SSTX to HUBTX
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 5, 1);	//GPIO13
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 7, 0);	//GPIO15
			//SSTX2 to ADL5906
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 7, 1);	//GPIO7
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 1, 0);	//GPIO9
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 3, 0);	//GPIO11

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

	case 0x05:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~(1 << 3);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			mdio_master_write(DUT0_MDIO_BASEADDR, OFFSET_CODE_RX_A, dut0.g_sstx_sstx2_pn_offset);
			mdio_master_write(DUT0_MDIO_BASEADDR, ITX_DRV_B, dut0.g_ssrx2_ssrx_vcom_offset);

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

	case 0x06:
	{
		//SSRX2 -> SSRX
		ret = dc_gain_calib_dut0(DUT0_IIC_1_ADDR, ADC1_ADDR, SSRX_DC_GAIN_ADC1_CH, DC_OFFSET_B2, dut0.g_ssrx2_ssrx_600mv_in);

		if(ret != -1)
		{
			dut0.g_ssrx2_ssrx_dc_offset = ret;
			xil_printf("SSRX2->SSRX DC offset: %d\r\n\r\n", dut0.g_ssrx2_ssrx_dc_offset);

			if(dut0.g_ssrx2_ssrx_dc_offset >= 1)
			{
				mdio_master_write(DUT0_MDIO_BASEADDR, DC_OFFSET_B2, dut0.g_ssrx2_ssrx_dc_offset - 1);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x07:
	{
		//SSTX -> SSTX2
		ret = dc_gain_calib_dut0(DUT0_IIC_1_ADDR, ADC1_ADDR, SSTX2_DC_GAIN_ADC1_CH, DC_OFFSET_A2, dut0.g_sstx_sstx2_600mv_in);

		if(ret != -1)
		{
			dut0.g_sstx_sstx2_dc_offset = ret;
			xil_printf("SSTX->SSTX2 DC offset: %d\r\n\r\n", dut0.g_sstx_sstx2_dc_offset);

			if(dut0.g_sstx_sstx2_dc_offset >= 1)
			{
				mdio_master_write(DUT0_MDIO_BASEADDR, DC_OFFSET_A2, dut0.g_sstx_sstx2_dc_offset - 1);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x08:
	{
		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num, DUT0_PATTERN_LRD);
		print("dut0_pattern_dc_gain_pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
