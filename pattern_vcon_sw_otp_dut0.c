#include "platform.h"

int8_t pattern_vcon_sw_otp_dut0()
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
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 20, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 19, 1);

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
		//enable OTP
		ret = i2c_read_phy(0x05, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 11);
		uint16_t reg_val = base_val | (1 << 11);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x05, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//enable OTP trim
		ret = i2c_read_phy(0x05, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 8);
		reg_val = base_val | (1 << 8);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x05, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//VMON 0.62V
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 620);
		delay_ms(10);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x02:
	{
		//start OTP trim
		for(i=0;i<16;i++)
		{
			ret = i2c_read_phy(0x07, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~0xFF;
			uint16_t reg_val = base_val | (i << 4) | i;
			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read flag
			ret = i2c_read_phy(0x05, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			xil_printf("dut0 status_val = %x\r\n", base_val);
			uint8_t otp_flag = (base_val >> 4) & 0x01;

			if(otp_flag == 1)
			{
				if(i == 0)	return -1;

				dut0.g_vcon_sw_otp_offset = i;
				break;
			}
		}
		if(i == 16)	return -1;

		xil_printf("dut0 vcon_sw_otp_offset = %x\r\n", dut0.g_vcon_sw_otp_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 800);
		delay_ms(10);

		//disable OTP, OTP trim
		ret = i2c_read_phy(0x05, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~((1 << 11) | (1 << 8));
		xil_printf("dut0 base_val = %x\r\n", base_val);

		tx_buf[0] = base_val & 0xFF;
		tx_buf[1] = base_val >> 8;
		ret = i2c_write_phy(0x05, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;

		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 0);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_vcon_sw_otp pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
