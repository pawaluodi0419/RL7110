#include "platform.h"

int8_t pattern_sig_oobs_dut0()
{
	int8_t ret;

	uint8_t reg_val;
	uint8_t usb_lnk_state,usb_sub_state;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		//DSP1(SSRX1)
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR1_L, HUB_DSP1_ADDR1_H, 0x10, g_s0_hub1_p1_para[9]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR2_L, HUB_DSP1_ADDR2_H, 0x08, g_s0_hub1_p1_para[10]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR3_L, HUB_DSP1_ADDR3_H, 0x00, g_s0_hub1_p1_para[11]);
		if(ret != 0)	return -1;

		//DSP2(SSRX2)
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR1_L, HUB_DSP2_ADDR1_H, 0x10, g_s0_hub1_p2_para[9]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR2_L, HUB_DSP2_ADDR2_H, 0x08, g_s0_hub1_p2_para[10]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR3_L, HUB_DSP2_ADDR3_H, 0x00, g_s0_hub1_p2_para[11]);
		if(ret != 0)	return -1;

		//DSP3(SSTX)
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR1_L, HUB_DSP3_ADDR1_H, 0x10, g_s0_hub1_p3_para[9]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR2_L, HUB_DSP3_ADDR2_H, 0x08, g_s0_hub1_p3_para[10]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR3_L, HUB_DSP3_ADDR3_H, 0x00, g_s0_hub1_p3_para[11]);
		if(ret != 0)	return -1;

		delay_ms(1);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x01:
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

	case 0x02:
	{
		//SSRX2 -> SSRX
		ret = sig_oobs_usb_calib_dut0(OOBS_TESTEN_VCM_SEN_RX2_B, 0);

		if(ret == 1)
		{
			xil_printf("SSRX2 SIG_OOBS offset: %d\r\n\r\n", dut0.g_sig_oobs_offset);
			dut0.g_ssrx2_sig_oobs_offset = dut0.g_sig_oobs_offset;

			dut0.g_lrd_pattern_step++;
		}
		else if(ret == -1)
		{
			return -1;
		}

		break;
	}

	case 0x03:
	{
		if(dut0.g_delay_control_bit == 0)
		{
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

	case 0x04:
	{
		//SSRX1 -> SSRX
		ret = sig_oobs_usb_calib_dut0(OOBS_TESTEN_VCM_SEN_RX1_B, 1);

		if(ret == 1)
		{
			xil_printf("SSRX1 SIG_OOBS offset: %d\r\n\r\n", dut0.g_sig_oobs_offset);
			dut0.g_ssrx1_sig_oobs_offset = dut0.g_sig_oobs_offset;

			dut0.g_lrd_pattern_step++;
		}
		else if(ret == -1)
		{
			return -1;
		}

		break;
	}

	case 0x05:
	{
		//SSTX -> SSTX1
		ret = sig_oobs_usb_calib_dut0(OOBS_TESTEN_VCM_SEN_A, 1);

		if(ret == 1)
		{
			xil_printf("SSTX SIG_OOBS offset: %d\r\n\r\n", dut0.g_sig_oobs_offset);
			dut0.g_sstx_sig_oobs_offset = dut0.g_sig_oobs_offset;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~0x03;
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_A);
			mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_A, (reg_val & 0xF0) | dut0.g_sstx_sig_oobs_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX1_B);
			mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX1_B, (reg_val & 0xF0) | dut0.g_ssrx1_sig_oobs_offset);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX2_B);
			mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX2_B, (reg_val & 0xF0) | dut0.g_ssrx2_sig_oobs_offset);

			dut0.g_lrd_pattern_step++;
		}
		else if(ret == -1)
		{
			return -1;
		}

		break;
	}

	case 0x06:
	{
		//DSP1(SSRX1)
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR1_L, HUB_DSP1_ADDR1_H, 0x10, g_s0_hub1_p1_para[12]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR2_L, HUB_DSP1_ADDR2_H, 0x08, g_s0_hub1_p1_para[13]);
		if(ret != 0)	return -1;
		ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR3_L, HUB_DSP1_ADDR3_H, 0x00, g_s0_hub1_p1_para[14]);
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

	case 0x07:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val |= (1 << 1);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			mdio_master_write(DUT0_MDIO_BASEADDR, OFFSET_CODE_RX_A, dut0.g_sstx_sstx1_pn_offset);
			mdio_master_write(DUT0_MDIO_BASEADDR, ITX_DRV_B, dut0.g_ssrx1_ssrx_vcom_offset);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
			usb_lnk_state = reg_val & 0x03;
			usb_sub_state = (reg_val & 0x70) >> 4;
//			xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

			if((usb_lnk_state == 1) && (usb_sub_state == 2))
			{
				print("dut0: check enter U0.active pass!\r\n");
				dut0.g_lrd_pattern_step++;
			}
			else
			{
				print("dut0: check enter U0.active fail!\r\n");
				return -1;
			}
		}

		break;
	}

	case 0x08:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//DSP1(SSRX1)
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR1_L, HUB_DSP1_ADDR1_H, 0x10, g_s0_hub1_p1_para[3]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR2_L, HUB_DSP1_ADDR2_H, 0x08, g_s0_hub1_p1_para[4]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP1_ADDR3_L, HUB_DSP1_ADDR3_H, 0x00, g_s0_hub1_p1_para[5]);
			if(ret != 0)	return -1;

			//DSP3(SSTX)
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR1_L, HUB_DSP3_ADDR1_H, 0x10, g_s0_hub1_p3_para[3]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR2_L, HUB_DSP3_ADDR2_H, 0x08, g_s0_hub1_p3_para[4]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR3_L, HUB_DSP3_ADDR3_H, 0x00, g_s0_hub1_p3_para[5]);
			if(ret != 0)	return -1;

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
			usb_lnk_state = reg_val & 0x03;
			usb_sub_state = (reg_val & 0x70) >> 4;
//			xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

			if(usb_lnk_state == 2)
			{
				print("dut0: check enter U2 pass!\r\n");
				dut0.g_lrd_pattern_step++;
			}
			else
			{
				print("dut0: check enter U2 fail!\r\n");
				return -1;
			}
		}

		break;
	}

	case 0x09:
	{
		if(dut0.g_delay_control_bit == 0)
		{
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

	case 0x0a:
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
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
			usb_lnk_state = reg_val & 0x03;
			usb_sub_state = (reg_val & 0x70) >> 4;
//			xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

			if((usb_lnk_state == 1) && (usb_sub_state == 2))
			{
				print("dut0: check enter U0.active pass!\r\n");
				dut0.g_lrd_pattern_step++;
			}
			else
			{
				print("dut0: check enter U0.active fail!\r\n");
				return -1;
			}
		}

		break;
	}

	case 0x0b:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//DSP2(SSRX2)
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR1_L, HUB_DSP2_ADDR1_H, 0x10, g_s0_hub1_p2_para[3]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR2_L, HUB_DSP2_ADDR2_H, 0x08, g_s0_hub1_p2_para[4]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP2_ADDR3_L, HUB_DSP2_ADDR3_H, 0x00, g_s0_hub1_p2_para[5]);
			if(ret != 0)	return -1;

			//DSP3(SSTX)
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR1_L, HUB_DSP3_ADDR1_H, 0x10, g_s0_hub1_p3_para[3]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR2_L, HUB_DSP3_ADDR2_H, 0x08, g_s0_hub1_p3_para[4]);
			if(ret != 0)	return -1;
			ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB1_ADDR, 0x1B, HUB_DSP3_ADDR3_L, HUB_DSP3_ADDR3_H, 0x00, g_s0_hub1_p3_para[5]);
			if(ret != 0)	return -1;

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
			usb_lnk_state = reg_val & 0x03;
			usb_sub_state = (reg_val & 0x70) >> 4;
//			xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

			if(usb_lnk_state == 2)
			{
				print("dut0: check enter U2 pass!\r\n");
				dut0.g_lrd_pattern_step++;
			}
			else
			{
				print("dut0: check enter U2 fail!\r\n");
				return -1;
			}
		}

		break;
	}

	case 0x0c:
	{
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
		reg_val &= ~0x03;
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);
		print("dut0_pattern_sig_oobs_pass!\r\n\r\n\r\n");
	}

	default:
		break;
	}

	return 0;
}
