#include "platform.h"

int8_t pattern_rc_clock_dut0()
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	uint8_t timeout;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 4, 1);

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
		//debug port(16M)
		ret = i2c_write_mem(0x18, 0xDF, 0x09);
		if(ret != 0)	return -1;
		delay_ms(1);

		//select channel, gate_cnt use default(100001)
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 4, 0);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x02:
	{
		uint32_t min_diff_coarse = 0xFFFFFFFF;
		uint16_t base_val;
		uint16_t reg_val;

		for(uint8_t i=0;i<4;i++)
		{
			ret = i2c_read_phy(0x12, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(3 << 3);
			reg_val = base_val | (i << 3);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x12, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(1);

			//start measure
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 1);

			//polling measure done
			for(timeout=0;timeout<50;timeout++)
			{
				if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) >> 31))
				{
					uint32_t rc16m_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) & 0xFFFFF;
//					xil_printf("dut0 rc16m_freq_cnt = %d\r\n", rc16m_freq_cnt);

					uint32_t rc16m_freq = rc16m_freq_cnt / 2;	//Hz
					xil_printf("i = %02d, dut0 rc16m_freq = %d\r\n", i, rc16m_freq);

					uint32_t diff;
					if(rc16m_freq > RC16M_TARGET)
					{
						diff = rc16m_freq - RC16M_TARGET;
					}
					else
					{
						diff = RC16M_TARGET - rc16m_freq;
					}

					if(diff < min_diff_coarse)
					{
						min_diff_coarse = diff;
						dut0.g_rc16m_coarse_offset = i;
					}

					//clear all
					FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);
					break;
				}
				delay_ms(1);
			}

			if(timeout == 50)
			{
				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);

				print("dut0 RC16M measure timeout!\r\n\r\n");
				return -1;
			}
		}
		xil_printf("dut0 rc16m_coarse_offset = %d\r\n\r\n", dut0.g_rc16m_coarse_offset);

		//write best coarse offset
		ret = i2c_read_phy(0x12, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(3 << 3);
		reg_val = base_val | (dut0.g_rc16m_coarse_offset << 3);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x12, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		uint8_t low = 0;
		uint8_t high = 127;
		uint8_t mid = 0;
		uint32_t min_diff_fine = 0xFFFFFFFF;
		uint16_t base_val;
		uint16_t reg_val;

		while(low <= high)
		{
			mid = (low + high) / 2;

			ret = i2c_read_phy(0x13, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0x7F << 9);
			reg_val = base_val | (mid << 9);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x13, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(1);

			//start measure
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 1);

			//polling measure done
			for(timeout=0;timeout<50;timeout++)
			{
				if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) >> 31))
				{
					uint32_t rc16m_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) & 0xFFFFF;
//					xil_printf("dut0 rc16m_freq_cnt = %d\r\n", rc16m_freq_cnt);

					uint32_t rc16m_freq = rc16m_freq_cnt / 2;	//Hz
					xil_printf("i = %02d, dut0 rc16m_freq = %d\r\n", mid, rc16m_freq);

					uint32_t diff;
					if(rc16m_freq > RC16M_TARGET)
					{
						diff = rc16m_freq - RC16M_TARGET;
					}
					else
					{
						diff = RC16M_TARGET - rc16m_freq;
					}

					if(diff < min_diff_fine)
					{
						min_diff_fine = diff;
						dut0.g_rc16m_fine_offset = mid;
					}

					if(rc16m_freq == RC16M_TARGET)
					{
						low = high + 1; 	//finish
					}
					else if(rc16m_freq < RC16M_TARGET)
					{
						low = mid + 1;
					}
					else
					{
						if(mid == 0)	break;
						high = mid - 1;
					}

					//clear all
					FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);
					break;
				}
				delay_ms(1);
			}

			if(timeout == 50)
			{
				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);

				print("dut0 RC16M measure timeout!\r\n\r\n");
				return -1;
			}
		}
		xil_printf("dut0 rc16m_fine_offset = %d\r\n\r\n", dut0.g_rc16m_fine_offset);

		//write best fine offset
		ret = i2c_read_phy(0x13, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(0x7F << 9);
		reg_val = base_val | (dut0.g_rc16m_fine_offset << 9);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x13, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x04:
	{
		//debug port(50M)
		ret = i2c_write_mem(0x18, 0xDF, 0x0D);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x05:
	{
		uint32_t min_diff = 0xFFFFFFFF;
		uint16_t base_val;
		uint16_t reg_val;

		for(uint8_t i=0;i<4;i++)
		{
			ret = i2c_read_phy(0x13, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(3 << 5);
			reg_val = base_val | (i << 5);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x13, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(1);

			//start measure
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 1);

			//polling measure done
			for(timeout=0;timeout<50;timeout++)
			{
				if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) >> 31))
				{
					uint32_t rc50m_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_0_ADDR, 12) & 0xFFFFF;
//					xil_printf("dut0 rc50m_freq_cnt = %d\r\n", rc50m_freq_cnt);

					uint32_t rc50m_freq = rc50m_freq_cnt / 2;	//Hz
					xil_printf("i = %02d, dut0 rc50m_freq = %d\r\n", i, rc50m_freq);

					uint32_t diff;
					if(rc50m_freq > RC50M_TARGET)
					{
						diff = rc50m_freq - RC50M_TARGET;
					}
					else
					{
						diff = RC50M_TARGET - rc50m_freq;
					}

					if(diff < min_diff)
					{
						min_diff = diff;
						dut0.g_rc50m_offset = i;
					}

					//clear all
					FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);
					break;
				}
				delay_ms(1);
			}

			if(timeout == 50)
			{
				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_0_ADDR, 0, 0);

				print("dut0 RC50M measure timeout!\r\n\r\n");
				return -1;
			}
		}
		xil_printf("dut0 rc50m_offset = %d\r\n\r\n", dut0.g_rc50m_offset);

		//write best 50M offset
		ret = i2c_read_phy(0x13, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(3 << 5);
		reg_val = base_val | (dut0.g_rc50m_offset << 5);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x13, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x06:
	{
		ret = i2c_write_mem(0x18, 0xDF, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_rc_clock pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
