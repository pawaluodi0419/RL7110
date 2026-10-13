#include "platform.h"

int8_t pattern_efuse_check_dut0()
{
	uint8_t rx_buf[10];

	int8_t ret;

	uint8_t efuse_all_zero = 1;

	//relay control
//	xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 1); 	//DUT 5V on
	xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 25, 1);	//DUT 3V3 on
	xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 27, 1);	//DUT 1V8 on
	delay_ms(100);

	result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num, DUT0_PATTERN_LRD);

	//enable efuse clock
	ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
	if(ret != 0)
	{
		dut0.g_dut_addr = 0x43;
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)	return -1;
	}
	delay_ms(1);

	//read efuse
	for(uint8_t i=0;i<128;i++)
	{
		ret = i2c_read_efuse(i, rx_buf);
		if(ret != 0)	return -1;

		if(rx_buf[0] != 0x00)
		{
			efuse_all_zero = 0;
		}
		xil_printf("dut0 efuse_read_data[%03d]: %02x\r\n", i, rx_buf[0]);
	}
	print("\r\n");

	//disable efuse clock
	ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
	if(ret != 0)	return -1;
	delay_ms(1);

	if(efuse_all_zero)
	{
		print("dut0 efuse is all zero!\r\n\r\n");
		return -1;
	}

	print("dut0 efuse check pass!\r\n\r\n\r\n");

	dut0.g_lrd_pattern_num = 0x1d;
	result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num, DUT0_PATTERN_LRD);

	dut0.g_eqa_pass_cnt++;
	xil_printf("dut0.g_eqa_test_cnt = %d, dut0.g_eqa_pass_cnt = %d\r\n\r\n", dut0.g_eqa_test_cnt, dut0.g_eqa_pass_cnt);

	pattern_safe_state_dut0();

	return 0;
}
