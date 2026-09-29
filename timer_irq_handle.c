#include "platform.h"

//1ms
void axi_tmr0_intrq_handle()
{
//	dut0.g_axitmr_cnt0++;
//
//	if(dut0.g_axitmr_cnt0 == 1000)
//	{
//		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, LED, g_led_tg);
//		g_led_tg ^= 1;
//
//		dut0.g_axitmr_cnt0 = 0;
//	}

	if(dut0.g_delay_control_bit == 1)
	{
		dut0.g_delay_count++;
	}

	if(dut0.g_delay2_control_bit == 1)
	{
		dut0.g_delay2_count++;
	}

	if(dut0.g_ft2_start_ready == 1)
	{
		dut0.g_axitmr_cnt1++;

//		if(dut0.g_pattern_timer == 0)
//		{
//			uint32_t csr = XTmrCtr_GetControlStatusReg(DUT0_TIMER_0_ADDR, 0);
//			XTmrCtr_SetControlStatusReg(DUT0_TIMER_0_ADDR, 0, csr | XTC_CSR_INT_OCCURED_MASK);
//
//			XTmrCtr_DisableIntr(DUT0_TIMER_0_ADDR, 0);
//
//			microblaze_softreset();
//		}
//		else
//		{
//			dut0.g_pattern_timer--;
//		}
	}

	//clear timer0 interrupt
    uint32_t csr = XTmrCtr_GetControlStatusReg(DUT0_TIMER_0_ADDR, 0);
    XTmrCtr_SetControlStatusReg(DUT0_TIMER_0_ADDR, 0, csr | XTC_CSR_INT_OCCURED_MASK);
}
