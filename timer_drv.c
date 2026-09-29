#include "platform.h"

void delay_ms(uint16_t time_ms)
{
	uint16_t time_cnt;
	uint16_t i;

	for(time_cnt=0;time_cnt<time_ms;time_cnt++){
		for(i=0;i<4166;i++);
	}
}

void delay_us(uint16_t time_us)
{
	uint16_t time_cnt;
	uint16_t i;

	for(time_cnt=0;time_cnt<time_us;time_cnt++){
		for(i=0;i<4;i++);
	}
}

void tmr0delay_dut0(uint16_t time_val)
{
	if(dut0.g_delay_count >= time_val)
	{
		dut0.g_delay_done_bit = 1;
	}
}

void tmr0delay2_dut0(uint16_t time_val)
{
	if(dut0.g_delay2_count >= time_val)
	{
		dut0.g_delay2_done_bit = 1;
	}
}
