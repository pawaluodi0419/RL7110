#include "platform.h"

int8_t pattern_slb_test_dut0()
{
	uint8_t rx_buf[10];

	int8_t ret;

	uint8_t reg_val;
	uint32_t dsp3_err_cnt;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//ITX_A=1, IRX_A=1, ITX_B=1, IRX_B=1
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, ADJ_IRX_A);
			reg_val &= ~0x3F;
			reg_val |= (1 << 4) | (1 << 2) | (1 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, ADJ_IRX_A, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, ADJ_IRX_B);
			reg_val &= ~0x3F;
			reg_val |= (1 << 4) | (1 << 2) | (1 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, ADJ_IRX_B, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, ADJ_ITX_A);
			reg_val &= ~0x3F;
			reg_val |= (1 << 4) | (1 << 2) | (1 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, ADJ_ITX_A, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, ADJ_ITX_B);
			reg_val &= ~0x3F;
			reg_val |= (1 << 4) | (1 << 2) | (1 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, ADJ_ITX_B, reg_val);

			//DRV_VOUT_A=7, DRV_VOUT_B=7
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, DRV_OUT_A);
			reg_val &= ~((7 << 4) | (7 << 0));
			reg_val |= (7 << 4) | (7 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, DRV_OUT_A, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, DRV_OUT_B);
			reg_val &= ~(7 << 0);
			reg_val |= (7 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, DRV_OUT_B, reg_val);

			//R1=3, R2=0, R3=0
			reg_val = 0;
			reg_val |= (3 << 6) | (0 << 4) | (0 << 2) | (0 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_RD1_2_3_A, reg_val);

			reg_val = 0;
			reg_val |= (3 << 6) | (3 << 4) | (0 << 2) | (0 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_RD1_2_B, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_RD3_4_AB);
			reg_val &= ~(3 << 6);
			reg_val |= (0 << 6);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_RD3_4_AB, reg_val);

			//C1=4, C2=6
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ1_2_A);
			reg_val &= ~((7 << 4) | (7 << 0));
			reg_val |= (4 << 4) | (6 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ1_2_A, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ1_B);
			reg_val &= ~((7 << 4) | (7 << 0));
			reg_val |= (4 << 4) | (6 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ1_B, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ2_B);
			reg_val &= ~((7 << 4) | (7 << 0));
			reg_val |= (4 << 4) | (6 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ2_B, reg_val);

			//boostTX off, boost123 off
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, NYQUIST_BOOST1_2);
			reg_val &= ~((7 << 4) | (7 << 0));
			reg_val |= (0 << 6) | (0 << 5) | (0 << 4) | (0 << 2) | (0 << 1) | (0 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, NYQUIST_BOOST1_2, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, NYQUIST_BOOST3_4);
			reg_val &= ~(7 << 4);
			reg_val |= (0 << 6) | (0 << 5) | (0 << 4);
			mdio_master_write(DUT0_MDIO_BASEADDR, NYQUIST_BOOST3_4, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, NYQUIST_BOOST_TX);
			reg_val &= ~(7 << 0);
			reg_val |= (0 << 2) | (0 << 1) | (0 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, NYQUIST_BOOST_TX, reg_val);

			//enhance on
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, EN_KOFFSET_RXIN_SW);
			reg_val &= ~(7 << 0);
			reg_val |= (1 << 2) | (1 << 1) | (1 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, EN_KOFFSET_RXIN_SW, reg_val);

			//force U0
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~((1 << 3) | (3 << 0));
			reg_val |= (1 << 3) | (3 << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			mdio_master_write(DUT0_MDIO_BASEADDR, OFFSET_CODE_RX_A, dut0.g_sstx_sstx1_pn_offset);
			mdio_master_write(DUT0_MDIO_BASEADDR, ITX_DRV_B, dut0.g_ssrx1_ssrx_vcom_offset);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(10);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			ret = hub_smbus_read(DUT0_IIC_0_ADDR, HUB2_ADDR, READ_MEM, 0x44, 0xE3, 1, rx_buf);
			if(ret != 0x00)	return -1;
			xil_printf("hub2 reg_0xE344 = %x\r\n", rx_buf[1]);

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x01:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 0, 0);	//GPIO0, SSRX1
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 6, 0);	//GPIO6, SSTX1

			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 1, 0);	//GPIO1, SSRX2
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 7, 0);	//GPIO7, SSTX2

			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 5, 0);	//GPIO13, SSTX
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 6, 0);	//GPIO14, SSRX

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(500);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			ret = hub_smbus_read(DUT0_IIC_0_ADDR, HUB2_ADDR, READ_MEM, 0x44, 0xE3, 1, rx_buf);
			if(ret != 0x00)	return -1;
			xil_printf("hub2 reg_0xE344 = %x\r\n", rx_buf[1]);

			print("dut0 lrd mux_sel = 1, hub2 run dsp3 slb!\r\n");
			hub_slb_dsp3_start();

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x02:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(200);

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
		hub_slb_dsp3_stop();
		hub_slb_dsp3_read_err_cnt(&dsp3_err_cnt);
		hub_slb_dsp3_reset_err_cnt();

		xil_printf("hub2 dsp3_err_cnt = %d\r\n", dsp3_err_cnt);
		if(dsp3_err_cnt > SLB_ERR_CNT)
		{
			print("hub2 dsp3 slb error count > spec!\r\n\r\n");
			return -1;
		}

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

			print("dut0 lrd mux_sel = 0, hub2 run dsp3 slb!\r\n");
			hub_slb_dsp3_start();

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x05:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(200);

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
		hub_slb_dsp3_stop();
		hub_slb_dsp3_read_err_cnt(&dsp3_err_cnt);
		hub_slb_dsp3_reset_err_cnt();

		xil_printf("hub2 dsp3_err_cnt = %d\r\n", dsp3_err_cnt);
		if(dsp3_err_cnt > SLB_ERR_CNT)
		{
			print("hub2 dsp3 slb error count > spec!\r\n\r\n");
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x07:
	{
		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num, DUT0_PATTERN_LRD);
		print("dut0 pattern_slb_test pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
