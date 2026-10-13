#include "platform.h"

int8_t pattern_ccdet_dut0()
{
	int8_t ret;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 2, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 3, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 11, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 12, 1);

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
		const ccdet_calib_config_t cc1_config_table[] = {
				{200, 0x44, 0x00, 12 , 3},
				{400, 0x88, 0x00, 8 , 3},
				{800, 0xCC, 0x00, 4 , 3},
				{1440, 0x11, 0x00, 0 , 4},
				{2430, 0x33, 0x01, 12 , 4},
		};

		const ccdet_calib_config_t cc2_config_table[] = {
				{200, 0x44, 0x02, 12 , 6},
				{400, 0x88, 0x02, 8 , 6},
				{800, 0xCC, 0x02, 4 , 6},
				{1440, 0x11, 0x02, 0 , 7},
				{2430, 0x33, 0x03, 12 , 7},
		};

		//enable CC_DET
		ret = i2c_write_phy(0x04, 0x00, 0xC0, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = ccdet_calib_dut0(cc1_config_table, 5, 1);
		if(ret != 0x00)
		{
			print("dut0 pattern_ccdet cc1 fail!\r\n\r\n");
			return -1;
		}

		ret = ccdet_calib_dut0(cc2_config_table, 5, 2);
		if(ret != 0x00)
		{
			print("dut0 pattern_ccdet cc2 fail!\r\n\r\n");
			return -1;
		}

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num, DUT0_PATTERN_TYPEC);
		print("dut0 pattern_ccdet pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
