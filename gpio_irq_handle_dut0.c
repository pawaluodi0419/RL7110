#include "platform.h"

void xgpio_intrq_handle_dut0()
{
	uint8_t ft2_start_hsk = 1;

	for(uint8_t i=0;i<10;i++)
	{
		if(xgpio_read_bit(DUT0_GPIO_1_ADDR, 0, 8) == 0)
		{
			ft2_start_hsk = 0;
			break;
		}
	}

	if(ft2_start_hsk == 1)
	{
		uint8_t current_pattern = dut0.g_lrd_pattern_num;

		if((current_pattern == 0x1f) || (current_pattern == 0x0c) || (current_pattern == 0x0e))
		{
			dut0.g_ft2_start_ready = 0x11;
		}
		else if(current_pattern <= 0x09)
		{
			if(dut0.g_result_fail == 1)
			{
				print("dut0 ft2 pattern fail, start next test!\r\n\r\n");
				dut0.g_ft2_start_ready = 0x11;
				dut0.g_result_fail = 0;
			}
		}
	}

	//clear GPIO_0 interrupt
	XGpio_WriteReg(DUT0_GPIO_1_ADDR, XGPIO_ISR_OFFSET, XGPIO_IR_CH1_MASK);
}
