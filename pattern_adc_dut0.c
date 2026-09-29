#include "platform.h"

int8_t pattern_adc_dut0()
{
//	uint8_t tx_data;
//	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	uint8_t adc_dout_0v;
	uint8_t adc_dout_5v_div;
	uint8_t adc_dout_1v_div;

	uint8_t adc_dout_0p5v;
	uint32_t adc_0p5v_vol;

//	uint32_t adc_fs_vol;

	int8_t ret;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 19, 1);
			xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 4, 0);

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
		//enable internal divider resistor
		ret = i2c_write_phy(0x10, 0x00, 0x00, 0x38);
		if(ret != 0)	return -1;
		delay_ms(1);

		//set adc idle time
		ret = i2c_write_mem(0x04, 0xDA, 0x0A);
		if(ret != 0)	return -1;
		delay_ms(1);

		//enable Vmon channel
		ret = i2c_write_mem(0x01, 0xDA, 0xF3);
		if(ret != 0)	return -1;
		delay_ms(1);

		//enable adc
		ret = i2c_write_mem(0x00, 0xDA, 0x77);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x02:
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 0);
		delay_ms(10);

		ret = i2c_read_mem(0x51, 0xDA, rx_buf);
		if(ret != 0)	return -1;
		adc_dout_0v = rx_buf[0];

		if(adc_dout_0v > 8)	return -1;

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		//DAC output 5V
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 5000);
		delay_ms(10);

		ret = i2c_read_mem(0x51, 0xDA, rx_buf);
		if(ret != 0)	return -1;
		adc_dout_5v_div = rx_buf[0];

		//DAC output 1V
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 1000);
		delay_ms(10);

		ret = i2c_read_mem(0x51, 0xDA, rx_buf);
		if(ret != 0)	return -1;
		adc_dout_1v_div = rx_buf[0];

		if(adc_dout_5v_div <= adc_dout_1v_div)	return -1;

		//calculate a/b
		uint32_t adc_ratiob_temp0 = ADC_1V_DIV_REF * adc_dout_5v_div;
		uint32_t adc_ratiob_temp1 = ADC_5V_DIV_REF * adc_dout_1v_div;

		dut0.g_adc_a = (ADC_5V_DIV_REF - ADC_1V_DIV_REF) * 1000 / (adc_dout_5v_div - adc_dout_1v_div);	//a * 1000
		xil_printf("dut0 adc_a = %d\r\n", dut0.g_adc_a);

		if((dut0.g_adc_a < ADC_A_MIN) || (dut0.g_adc_a > ADC_A_MAX))
		{
			print("dut0 adc_a out of range!\r\n\r\n");
			return -1;
		}

		if(adc_ratiob_temp0 >= adc_ratiob_temp1)
		{
			dut0.g_adc_b_offset = (adc_ratiob_temp0 - adc_ratiob_temp1) / (adc_dout_5v_div - adc_dout_1v_div);
			dut0.g_adc_b_signbit = 0;	//b: +
		}
		else
		{
			dut0.g_adc_b_offset = (adc_ratiob_temp1 - adc_ratiob_temp0) / (adc_dout_5v_div - adc_dout_1v_div);
			dut0.g_adc_b_signbit = 1;	//b: -
		}
		xil_printf("dut0 adc_b_signbit = %d, adc_b_offset = %d\r\n", dut0.g_adc_b_signbit, dut0.g_adc_b_offset);

		if((dut0.g_adc_b_signbit == 0) && (dut0.g_adc_b_offset > ADC_B_MAX))
		{
			print("dut0 adc_b out of range!\r\n\r\n");
			return -1;
		}

		if((dut0.g_adc_b_signbit == 1) && (dut0.g_adc_b_offset > ADC_B_MIN))
		{
			print("dut0 adc_b out of range!\r\n\r\n");
			return -1;
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x04:
	{
		//adc test
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, GPIO0_LV_DAC1_CH, 500);
		delay_ms(10);

		ret = i2c_read_mem(0x52, 0xDA, rx_buf);
		if(ret != 0)	return -1;
		adc_dout_0p5v = rx_buf[0];

		if(dut0.g_adc_b_signbit)
		{
			adc_0p5v_vol = dut0.g_adc_a * adc_dout_0p5v - dut0.g_adc_b_offset * 1000;
		}
		else
		{
			adc_0p5v_vol = dut0.g_adc_a * adc_dout_0p5v + dut0.g_adc_b_offset * 1000;
		}
		xil_printf("dut0 adc_0p5v_vol = %d\r\n", adc_0p5v_vol);

		if((adc_0p5v_vol > ADC_0V5_REF * 1020) || (adc_0p5v_vol < ADC_0V5_REF * 980))
		{
			print("dut0 adc ch2 test fail!\r\n\r\n");
			return -1;
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x05:
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VMON_LV_DAC1_CH, 0);
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, GPIO0_LV_DAC1_CH, 0);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_adc pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
