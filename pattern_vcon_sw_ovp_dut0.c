#include "platform.h"

int8_t pattern_vcon_sw_ovp_dut0()
{
	uint8_t i;

//	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 3, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 8, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 12, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 17, 1);

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x01:
	{
		//enable CC1 OVP
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 1);
		uint16_t reg_val = base_val | (1 << 1);
		xil_printf("dut0 reg_val = %x\r\n", reg_val);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//CC1 6V
		i2c_to_spi_ad5504_output(DUT0_IIC_1_ADDR, Bridge_ADDR, CC1_HV_DAC_CH, 6000);
		delay_ms(10);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x02:
	{
		//start CC1 OVP trim
		for(i=0;i<16;i++)
		{
			ret = i2c_read_phy(0x2C, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 4);
			uint16_t reg_val = base_val | (i << 4);
			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x2C, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read flag
			ret = i2c_read_phy(0x04, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			xil_printf("dut0 status_val = %x\r\n", base_val);
			uint8_t ovp_flag = (base_val >> 1) & 0x01;

			if(ovp_flag == 0)
			{
				if(i == 0)	return -1;

				dut0.g_cc1_ovp_offset = i;
				break;
			}
		}
		if(i == 16)	return -1;

		xil_printf("dut0 g_cc1_ovp_offset = %x\r\n", dut0.g_cc1_ovp_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		i2c_to_spi_ad5504_output(DUT0_IIC_1_ADDR, Bridge_ADDR, CC1_HV_DAC_CH, 0);
		delay_ms(10);

		//enable CC2 OVP
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~((1 << 1) | (1 << 0));
		uint16_t reg_val = base_val | (1 << 0);
		xil_printf("dut0 reg_val = %x\r\n", reg_val);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//CC2 6V
		i2c_to_spi_ad5504_output(DUT0_IIC_1_ADDR, Bridge_ADDR, CC2_HV_DAC_CH, 6000);
		delay_ms(10);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x04:
	{
		//start CC2 OVP trim
		for(i=0;i<16;i++)
		{
			ret = i2c_read_phy(0x2C, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 0);
			uint16_t reg_val = base_val | (i << 0);
			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x2C, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read flag
			ret = i2c_read_phy(0x04, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			xil_printf("dut0 status_val = %x\r\n", base_val);
			uint8_t ovp_flag = (base_val >> 0) & 0x01;

			if(ovp_flag == 0)
			{
				if(i == 0)	return -1;

				dut0.g_cc2_ovp_offset = i;
				break;
			}
		}
		if(i == 16)	return -1;

		xil_printf("dut0 g_cc2_ovp_offset = %x\r\n", dut0.g_cc2_ovp_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x05:
	{
		i2c_to_spi_ad5504_output(DUT0_IIC_1_ADDR, Bridge_ADDR, CC2_HV_DAC_CH, 0);
		delay_ms(10);

		//disable OVP
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~((1 << 1) | (1 << 0));
		xil_printf("dut0 base_val = %x\r\n", base_val);

		tx_buf[0] = base_val & 0xFF;
		tx_buf[1] = base_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_vcon_sw_ovp pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
