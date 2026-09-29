#include "platform.h"

int8_t pattern_setup_dut0()
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	uint8_t reg_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//relay control
//			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 1); 	//DUT 5V on
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 25, 1);	//DUT 3V3 on
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 27, 1);	//DUT 1V8 on

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(100);

		if(dut0.g_delay_done_bit == 1)
		{
			result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);

			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x01:
	{
	    memset(write_data.bytes, 0, sizeof(write_data.bytes));
	    memset(read_data.bytes, 0, sizeof(read_data.bytes));

		//enable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)
		{
			dut0.g_dut_addr = 0x43;
			ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
			if(ret != 0)	return -1;
		}
		delay_ms(1);

		//read efuse
		for(uint8_t i=0;i<128;i++)
		{
			ret = i2c_read_efuse(i, rx_buf);
			if(ret != 0)	return -1;
			read_data.bytes[i] = rx_buf[0];
			xil_printf("dut0 efuse_read_data[%03d]: %02x\r\n", i, read_data.bytes[i]);
		}
		print("\r\n");

		//disable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		write_data.bits.cp_lrd_ldo = read_data.bytes[0];

		if(read_data.bytes[1] == 0)
		{
		    print("dut0 is new IC!\r\n\r\n");
		    dut0.g_ft2_retest = 0;
		}
		else
		{
		    print("dut0 is retest IC!\r\n\r\n");
		    dut0.g_ft2_retest = 1;
		}

		//confirm ic version
		ret = i2c_read_mem(0xFF, 0xDF, rx_buf);
		if(ret != 0)	return -1;
		dut0.g_dut_ver = rx_buf[0];
		xil_printf("dut0 ic version: 0x%x\r\n\r\n", dut0.g_dut_ver);

		if((dut0.g_dut_ver == 0) && (read_data.bytes[1] == 0))
		{
		    dut0.g_lrd_pattern_step++;
		}
		else
		{
		    dut0.g_lrd_pattern_step = 0x06;
		}

		break;
	}

	case 0x02:
	{
		//patch
		write_data.bits.efuse_data_valid = 1;
		write_data.bits.efuse_patch_valid = 1;

		write_data.bits.patch_addrh_0 = 0xDF;
		write_data.bits.patch_addrl_0 = 0x0C;
		write_data.bits.patch_data_0 = 0x7E;

		write_data.bits.patch_addrh_1 = 0xDF;
		write_data.bits.patch_addrl_1 = 0x00;
		write_data.bits.patch_data_1 = 0x20;

		write_data.bits.patch_addrh_2 = 0xDF;
		write_data.bits.patch_addrl_2 = 0x09;
		write_data.bits.patch_data_2 = 0xA0;

		//enable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)	return -1;
		delay_ms(1);

		//write efuse
		ret = i2c_write_efuse(2, write_data.bytes);
		if(ret != 0)	return -1;
		delay_ms(1);
		ret = i2c_write_efuse(4, write_data.bytes);
		if(ret != 0)	return -1;
		delay_ms(1);

		for(uint8_t i=92;i<101;i++)
		{
			ret = i2c_write_efuse(i, write_data.bytes);
			if(ret != 0)	return -1;
			delay_ms(1);
		}

		//disable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x03:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//power cycle
//			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 0); 	//DUT 5V off
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 25, 0);	//DUT 3V3 off
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 27, 0);	//DUT 1V8 off

			xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 3, 1);		//dischage

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(100);

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
		if(dut0.g_delay_control_bit == 0)
		{
			xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 3, 0);

//			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 25, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 27, 1);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(100);

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
		//change dut address
		dut0.g_dut_addr = 0x43;

		//enable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)	return -1;
		delay_ms(1);

		//read efuse
		for(uint8_t i=0;i<128;i++)
		{
			ret = i2c_read_efuse(i, rx_buf);
			if(ret != 0)	return -1;
			read_data.bytes[i] = rx_buf[0];
			xil_printf("dut0 efuse_read_data[%03d]: %02x\r\n", i, read_data.bytes[i]);
		}
		print("\r\n");

		//disable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		if(memcmp(write_data.bytes, read_data.bytes, sizeof(write_data.bytes)) == 0)
		{
			print("dut0 efuse write/read compare pass!\r\n\r\n");
		}
		else
		{
			print("dut0 efuse write/read compare fail!\r\n\r\n");
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x06:
	{
		ret = i2c_write_mem(0x20, 0xD0, 0x90);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x25, 0xD0, 0x03);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x21, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x22, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x23, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x24, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x26, 0xD0, 0x02);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x27, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x28, 0xD0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0x00, 0xD0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_read_mem(0x00, 0xD1, rx_buf);
		if(ret != 0)	return -1;
		xil_printf("dut0 read flash manufacturer ID: 0x%x\r\n", rx_buf[0]);

		ret = i2c_read_mem(0x01, 0xD1, rx_buf);
		if(ret != 0)	return -1;
		xil_printf("dut0 read flash device ID: 0x%x\r\n\r\n", rx_buf[0]);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x07:
	{
		ret = i2c_read_mem(0x00, 0xDF, rx_buf);
		if(ret != 0)	return -1;
		rx_buf[0] |= (1 << 4);

		ret = i2c_write_mem(0x00, 0xDF, rx_buf[0]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//confirm mdio status
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, SW_OFFSET_LUT10_A);
		if(reg_val != 0x66)	return -1;

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x08:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 5, 0);	//SSRX1/SSRX2 45.3 pull 1.1V, GPIO5
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 4, 0);	//SSTX1/SSTX2 45.3 pull 1.1V, GPIO12
			 i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 3, 0);	//SSTX/SSRX 45.3 pull 1.1V, GPIO19

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

	case 0x09:
	{
		//SSRX1
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX1_B);
		reg_val |= (1 << 4);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX1_B, reg_val);
		delay_ms(1);

		uint16_t adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX1_P_TERM_ADC2_CH, AVG_NUM);
		uint16_t ssrx1_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX1_P rterm default: %d\r\n", adc_data, ssrx1_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX1_N_TERM_ADC2_CH, AVG_NUM);
		uint16_t ssrx1_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX1_N rterm default: %d\r\n", adc_data, ssrx1_n_rterm);

		if((abs((int)ssrx1_p_rterm - (int)ssrx1_n_rterm) > 20) || (ssrx1_p_rterm > 600) || (ssrx1_n_rterm > 600) || (ssrx1_p_rterm < 300) || (ssrx1_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//SSRX2
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX2_B);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX2_B, reg_val | 0x10);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX2_P_TERM_ADC2_CH, AVG_NUM);
		uint16_t ssrx2_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX2_P rterm default: %d\r\n", adc_data, ssrx2_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC2_ADDR, SSRX2_N_TERM_ADC2_CH, AVG_NUM);
		uint16_t ssrx2_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX2_N rterm default: %d\r\n", adc_data, ssrx2_n_rterm);

		if((abs((int)ssrx2_p_rterm - (int)ssrx2_n_rterm) > 20) || (ssrx2_p_rterm > 600) || (ssrx2_n_rterm > 600) || (ssrx2_p_rterm < 300) || (ssrx2_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//SSTX1
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX1_A);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX1_A, reg_val | 0x10);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX1_P_TERM_ADC1_CH, AVG_NUM);
		uint16_t sstx1_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX1_P rterm default: %d\r\n", adc_data, sstx1_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX1_N_TERM_ADC1_CH, AVG_NUM);
		uint16_t sstx1_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX1_N rterm default: %d\r\n", adc_data, sstx1_n_rterm);

		if((abs((int)sstx1_p_rterm - (int)sstx1_n_rterm) > 20) || (sstx1_p_rterm > 600) || (sstx1_n_rterm > 600) || (sstx1_p_rterm < 300) || (sstx1_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//SSTX2
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX2_A);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX2_A, reg_val | 0x10);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX2_P_TERM_ADC1_CH, AVG_NUM);
		uint16_t sstx2_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX2_P rterm default: %d\r\n", adc_data, sstx2_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC1_ADDR, SSTX2_N_TERM_ADC1_CH, AVG_NUM);
		uint16_t sstx2_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX2_N rterm default: %d\r\n", adc_data, sstx2_n_rterm);

		if((abs((int)sstx2_p_rterm - (int)sstx2_n_rterm) > 20) || (sstx2_p_rterm > 600) || (sstx2_n_rterm > 600) || (sstx2_p_rterm < 300) || (sstx2_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//SSTX
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_RX_A);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_RX_A, reg_val | 0x10);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC3_ADDR, SSTX_P_TERM_ADC3_CH, AVG_NUM);
		uint16_t sstx_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX_P rterm default: %d\r\n", adc_data, sstx_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC3_ADDR, SSTX_N_TERM_ADC3_CH, AVG_NUM);
		uint16_t sstx_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSTX_N rterm default: %d\r\n", adc_data, sstx_n_rterm);

		if((abs((int)sstx_p_rterm - (int)sstx_n_rterm) > 20) || (sstx_p_rterm > 600) || (sstx_n_rterm > 600) || (sstx_p_rterm < 300) || (sstx_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//SSRX
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, TERM_TEST1_TX_B);
		mdio_master_write(DUT0_MDIO_BASEADDR, TERM_TEST1_TX_B, reg_val | 0x10);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_P_TERM_ADC3_CH, AVG_NUM);
		uint16_t ssrx_p_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX_P rterm default: %d\r\n", adc_data, ssrx_p_rterm);

		adc_data = ad7998_readcvsdata(DUT0_IIC_0_ADDR, ADC3_ADDR, SSRX_N_TERM_ADC3_CH, AVG_NUM);
		uint16_t ssrx_n_rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("adc_data: %dmV, SSRX_N rterm default: %d\r\n", adc_data, ssrx_n_rterm);

		if((abs((int)ssrx_p_rterm - (int)ssrx_n_rterm) > 20) || (ssrx_p_rterm > 600) || (ssrx_n_rterm > 600) || (ssrx_p_rterm < 300) || (ssrx_n_rterm < 300))
		{
			print("dut0 contact issue, please take out IC and place again!!!\r\n\r\n");
			return -1;
		}

		//LRD_LDO
		if(read_data.bytes[0] != 0)
		{
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, 0x11);
			reg_val &= ~((7 << 8) | (7 << 0));
			reg_val |= (read_data.bytes[0] << 8) | (read_data.bytes[0] << 0);
			mdio_master_write(DUT0_MDIO_BASEADDR, 0x11, reg_val);
			print("dut0 change lrd_ldo!\r\n\r\n");
		}

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

		//C1=0, C2=0
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ1_2_A);
		reg_val &= ~((7 << 4) | (7 << 0));
		reg_val |= (0 << 4) | (0 << 0);
		mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ1_2_A, reg_val);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ1_B);
		reg_val &= ~((7 << 4) | (7 << 0));
		reg_val |= (0 << 4) | (0 << 0);
		mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ1_B, reg_val);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_EQ2_B);
		reg_val &= ~((7 << 4) | (7 << 0));
		reg_val |= (0 << 4) | (0 << 0);
		mdio_master_write(DUT0_MDIO_BASEADDR, USB_EQ2_B, reg_val);

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

		//enhance on
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, EN_KOFFSET_RXIN_SW);
		reg_val &= ~(7 << 0);
		reg_val |= (1 << 2) | (1 << 1) | (1 << 0);
		mdio_master_write(DUT0_MDIO_BASEADDR, EN_KOFFSET_RXIN_SW, reg_val);

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

		//auto eq off
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1);
		reg_val &= ~(3 << 0);
		reg_val |= (0 << 1) | (0 << 0);
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);

		//USB mode off
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
		reg_val &= ~(1 << 1);
		reg_val |= (0 << 1);
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

		//force i2c mode on
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, I2C_SETTING);
		reg_val &= ~(1 << 5);
		reg_val |= (1 << 5);
		mdio_master_write(DUT0_MDIO_BASEADDR, I2C_SETTING, reg_val);

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x0a:
	{
		dut0.g_pattern_timer = 0x5fff;
		dut0.g_lrd_pattern_step = 0;
		dut0.g_lrd_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);
		print("dut0 pattern_setup pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
