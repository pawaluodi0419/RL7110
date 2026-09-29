#include "platform.h"

int8_t pattern_efuse_check_dut0()
{
	uint8_t tx_data;
//	uint8_t tx_buf[10];
	uint8_t rx_buf[50];

	int8_t ret;


	uint8_t efuse_all_zero = 1;

	for(uint8_t i=0;i<27;i++)
	{
		if(rx_buf[i+1] != 0x00)
		{
			efuse_all_zero = 0;
		}
		xil_printf("dut0 efuse_read_data[%02d]: %02x\r\n", i, rx_buf[i+1]);
	}
	print("\r\n");

	if(efuse_all_zero)
	{
		print("dut0 efuse is all zero!\r\n\r\n");
		return -1;
	}

	print("dut0 efuse check pass!\r\n\r\n\r\n");

	dut0.g_type_c_pattern_num = 0x0e;
	result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);

	dut0.g_eqa_pass_cnt++;
	xil_printf("dut0.g_eqa_test_cnt = %d, dut0.g_eqa_pass_cnt = %d\r\n\r\n", dut0.g_eqa_test_cnt, dut0.g_eqa_pass_cnt);

	pattern_safe_state_dut0();

	return 0;
}
