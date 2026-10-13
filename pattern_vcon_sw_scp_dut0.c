#include "platform.h"

int8_t pattern_vcon_sw_scp_dut0()
{
	int8_t i;

//	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	/* trim: CC1/CC2 scan i desc(15->0), write i to reg0x06[11:8]/[7:4],
	 * read cc adc, record i with min |adc - target|, target 500->250mA offset / 800->400mA offset, offset = i;
	 * no fail in scan, always pick closest within 15..0 */
	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		//SCP normal mode, with current limit
		ret = i2c_read_phy(0x06, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 13);

		tx_buf[0] = base_val & 0xFF;
		tx_buf[1] = base_val >> 8;
		ret = i2c_write_phy(0x06, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x01:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 7, 1);		//CC1 2ohm short

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

	case 0x02:
	{
		//CC1 VCON_SW on
		ret = i2c_write_mem(0xA2, 0xE0, 0x0C);
		if(ret != 0)	return -1;

		delay_ms(10);

		//read SCP flag
		ret = i2c_read_phy(0x06, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		uint8_t scp_flag = (base_val >> 3) & 0x01;
		xil_printf("dut0 scp_flag = %x\r\n", scp_flag);

		if(scp_flag != 1)	return -1;

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		uint16_t min_diff_500 = 0xFFFF;
		uint16_t val_at_500 = 0;

		uint16_t min_diff_800 = 0xFFFF;
		uint16_t val_at_800 = 0;

		//start CC1 SCP trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 8);
			uint16_t reg_val = base_val | (i << 8);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x06, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			uint16_t cc1_adc_data = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC2_ADDR, SCP_CC1_ADC2_CH, AVG_NUM);
			xil_printf("i = %02d, dut0 cc1_adc_data = %d\r\n", i, cc1_adc_data);

			uint16_t diff_500 = (cc1_adc_data > 500) ? (cc1_adc_data - 500) : (500 - cc1_adc_data);
			uint16_t diff_800 = (cc1_adc_data > 800) ? (cc1_adc_data - 800) : (800 - cc1_adc_data);

			if (diff_500 < min_diff_500)
			{
				min_diff_500 = diff_500;
				dut0.g_cc1_scp_250ma_offset = i;
				val_at_500 = cc1_adc_data;
			}

			if (diff_800 < min_diff_800)
			{
				min_diff_800 = diff_800;
				dut0.g_cc1_scp_400ma_offset = i;
				val_at_800 = cc1_adc_data;
			}
		}
		xil_printf("Closest to 500: i = %02d (ADC = %d, diff = %d)\r\n", dut0.g_cc1_scp_250ma_offset, val_at_500, min_diff_500);
		xil_printf("Closest to 800: i = %02d (ADC = %d, diff = %d)\r\n", dut0.g_cc1_scp_400ma_offset, val_at_800, min_diff_800);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x04:
	{
		//CC1 VCON_SW off
		ret = i2c_write_mem(0xA2, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		if(dut0.g_delay2_control_bit == 0)
		{
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 7, 0);		//CC1 floating
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 16, 1);	//CC2 2ohm short

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

	case 0x05:
	{
		//CC2 VCON_SW on
		ret = i2c_write_mem(0xA3, 0xE0, 0x0C);
		if(ret != 0)	return -1;

		delay_ms(10);

		//read SCP flag
		ret = i2c_read_phy(0x06, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		uint8_t scp_flag = (base_val >> 2) & 0x01;
		xil_printf("dut0 scp_flag = %x\r\n", scp_flag);

		if(scp_flag != 1)	return -1;

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x06:
	{
		uint16_t min_diff_500 = 0xFFFF;
		uint16_t val_at_500 = 0;

		uint16_t min_diff_800 = 0xFFFF;
		uint16_t val_at_800 = 0;

		//start CC2 SCP trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 4);
			uint16_t reg_val = base_val | (i << 4);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x06, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			uint16_t cc2_adc_data = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC2_ADDR, SCP_CC2_ADC2_CH, AVG_NUM);
			xil_printf("i = %02d, dut0 cc2_adc_data = %d\r\n", i, cc2_adc_data);

			uint16_t diff_500 = (cc2_adc_data > 500) ? (cc2_adc_data - 500) : (500 - cc2_adc_data);
			uint16_t diff_800 = (cc2_adc_data > 800) ? (cc2_adc_data - 800) : (800 - cc2_adc_data);

			if (diff_500 < min_diff_500)
			{
				min_diff_500 = diff_500;
				dut0.g_cc2_scp_250ma_offset = i;
				val_at_500 = cc2_adc_data;
			}

			if (diff_800 < min_diff_800)
			{
				min_diff_800 = diff_800;
				dut0.g_cc2_scp_400ma_offset = i;
				val_at_800 = cc2_adc_data;
			}
		}
		xil_printf("Closest to 500: i = %02d (ADC = %d, diff = %d)\r\n", dut0.g_cc2_scp_250ma_offset, val_at_500, min_diff_500);
		xil_printf("Closest to 800: i = %02d (ADC = %d, diff = %d)\r\n", dut0.g_cc2_scp_400ma_offset, val_at_800, min_diff_800);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x07:
	{
		//CC2 VCON_SW off
		ret = i2c_write_mem(0xA3, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 16, 0);		//CC2 floating

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num, DUT0_PATTERN_TYPEC);
		print("dut0 pattern_vcon_sw_scp pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
