#include "platform.h"

void pattern_initial_state_dut0()
{
	uint8_t xgpio_0_data[8];
	uint8_t xgpio_1_data[8];

	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 0xEF);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 0xF3);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 0x59);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 0x00);

	xgpio_0_data[0] = 0x05;
	xgpio_0_data[1] = 0x00;
	xgpio_0_data[2] = 0x0a;
	xgpio_0_data[3] = 0x00;
	xgpio_0_data[4] = 0x08;
	xgpio_0_data[5] = 0x00;
	xgpio_0_data[6] = 0x00;
	xgpio_0_data[7] = 0x60;
	xgpio_write_word(DUT0_GPIO_0_ADDR, 0, xgpio_0_data);

	xgpio_1_data[0] = 0x07;
	xgpio_1_data[1] = 0x00;
	xgpio_1_data[2] = 0x00;
	xgpio_1_data[3] = 0x03;
	xgpio_1_data[4] = 0x01;
	xgpio_1_data[5] = 0x02;
	xgpio_1_data[6] = 0x00;
	xgpio_1_data[7] = 0x00;
	xgpio_write_word(DUT0_GPIO_1_ADDR, 0, xgpio_1_data);

	dut0.g_ft2_start_ready = 0;
	dut0.g_ft2_test_done = 1;
	dut0.g_result_fail = 0;
	dut0.g_lrd_pattern_num = 0x1f;
	dut0.g_lrd_pattern_step = 0;
	dut0.g_lrd_pattern_substep = 0;
	dut0.g_type_c_pattern_num = 0;
	dut0.g_type_c_pattern_step = 0;
	dut0.g_type_c_test_timer = 100;
	dut0.g_type_c_test_done = 0;
	dut0.g_pattern_timer = 0x5fff;
	dut0.g_axitmr_cnt0 = 0;
	dut0.g_axitmr_cnt1 = 0;
	dut0.g_test_cnt = 0;
	dut0.g_pass_cnt = 0;
	dut0.g_eqa_test_cnt = 0;
	dut0.g_eqa_pass_cnt = 0;
	dut0.g_gpio_0_shadow_state[0] = 0x00080a05;
	dut0.g_gpio_0_shadow_state[1] = 0;
	dut0.g_gpio_1_shadow_state[0] = 0x00010007;
	dut0.g_gpio_1_shadow_state[1] = 0;

	dut0.g_dut_addr = 0x41;

	dut0.g_delay_control_bit = 0;
	dut0.g_delay_done_bit = 0;
	dut0.g_delay2_control_bit = 0;
	dut0.g_delay2_done_bit = 0;

	dut0.g_pn_offset_test_cnt = 1;

	//FW version
	result_output_for_v50(DUT0_GPIO_1_ADDR, FT_FW_VER, DUT0_PATTERN_LRD);

	GPI_DIS_WRITE_REG(DUT0_GPI_DIS_ADDR, 0 , 0x60000000);
	GPI_DIS_WRITE_REG(DUT0_GPI_DIS_ADDR, 4 , 0x00020300);
}
