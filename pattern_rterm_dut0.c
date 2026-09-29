#include "platform.h"

int8_t pattern_rterm_dut0()
{
	int8_t ret;

	uint8_t reg_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		//SSRX1
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX1_P_TERM_ADC2_CH, TERM_TEST1_RX1_B);
		if(ret != -1)
		{
			dut0.g_ssrx1_rterm_offset = ret;
			xil_printf("SSRX1 rterm offset: %d\r\n\r\n", dut0.g_ssrx1_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX1_B);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_ssrx1_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX1_B, reg_val);
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
		//SSRX2
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX2_P_TERM_ADC2_CH, TERM_TEST1_RX2_B);
		if(ret != -1)
		{
			dut0.g_ssrx2_rterm_offset = ret;
			xil_printf("SSRX2 rterm offset: %d\r\n\r\n", dut0.g_ssrx2_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX2_B);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_ssrx2_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX2_B, reg_val);
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
		//SSTX1
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX1_P_TERM_ADC1_CH, TERM_TEST1_TX1_A);
		if(ret != -1)
		{
			dut0.g_sstx1_rterm_offset = ret;
			xil_printf("SSTX1 rterm offset: %d\r\n\r\n", dut0.g_sstx1_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX1_A);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_sstx1_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX1_A, reg_val);
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
		//SSTX2
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX2_P_TERM_ADC1_CH, TERM_TEST1_TX2_A);
		if(ret != -1)
		{
			dut0.g_sstx2_rterm_offset = ret;
			xil_printf("SSTX2 rterm offset: %d\r\n\r\n", dut0.g_sstx2_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX2_A);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_sstx2_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX2_A, reg_val);
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
		//SSTX
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSTX_P_TERM_ADC3_CH, TERM_TEST1_RX_A);
		if(ret != -1)
		{
			dut0.g_sstx_rterm_offset = ret;
			xil_printf("SSTX rterm offset: %d\r\n\r\n", dut0.g_sstx_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX_A);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_sstx_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX_A, reg_val);
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
		//SSRX
		ret = input_rterm_calib_dut0(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_TERM_ADC3_CH, TERM_TEST1_TX_B);
		if(ret != -1)
		{
			dut0.g_ssrx_rterm_offset = ret;
			xil_printf("SSRX rterm offset: %d\r\n\r\n", dut0.g_ssrx_rterm_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX_B);
			reg_val &= ~0x1F;
			reg_val |= dut0.g_ssrx_rterm_offset;
			mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX_B, reg_val);
		}
		else
		{
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x06:
	{
//		//HUB TX measure
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 4, 0);	//GPIO28
//
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 0, 1);	//GPIO0
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 2, 0);	//GPIO2
//
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 1, 1);	//GPIO1
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 3, 0);	//GPIO3
//
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 5, 1);	//GPIO13
//		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 7, 0);	//GPIO15

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);
		print("dut0_pattern_rterm_pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
