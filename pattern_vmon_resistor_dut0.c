#include "platform.h"

int8_t pattern_vmon_resistor_dut0()
{
//	uint8_t tx_data;
//	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 19, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 20, 1);

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
		uint16_t vmon_div_vol = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC2_ADDR, VMON_RES_ADC2_CH, AVG_NUM);
		xil_printf("dut0 vmon_div_vol = %d\r\n", vmon_div_vol);

		if(vmon_div_vol >= 3300) return -1;

		dut0.g_vmon_resistor = vmon_div_vol * 175 / (3300 - vmon_div_vol);
		xil_printf("dut0 vmon_resistor = %d\r\n", dut0.g_vmon_resistor);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num, DUT0_PATTERN_TYPEC);
		print("dut0 pattern_vmon_resistor pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
