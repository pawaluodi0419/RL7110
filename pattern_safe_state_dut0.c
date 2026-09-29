#include "platform.h"

void pattern_safe_state_dut0()
{
	//DAC control
	for(uint8_t i=0;i<8;i++)
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, i, 0);
	}

	for(uint8_t i=0;i<8;i++)
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, i, 0);
	}

	for(uint8_t i=0;i<4;i++)
	{
		i2c_to_spi_ad5504_output(DUT0_IIC_1_ADDR, Bridge_ADDR, i, 0);
	}

	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 0, 0xEF);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 1, 0xF3);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 0x59);
	i2c_pca9506_output_writebank(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 0x00);

	dut0.g_gpio_0_shadow_state[0] = 0x00080a05;
	dut0.g_gpio_1_shadow_state[0] = 0x00010007 | (dut0.g_lrd_pattern_num << 10);

	xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 0);

	//write EOT high
	xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 15, 1);

	dut0.g_ft2_start_ready = 0;
	dut0.g_ft2_test_done = 1;
	dut0.g_pattern_timer = 0x5fff;
	dut0.g_lrd_pattern_step = 0;
	dut0.g_lrd_pattern_substep = 0;
	dut0.g_type_c_pattern_step = 0;
	dut0.g_type_c_test_timer = 100;
	dut0.g_type_c_test_done = 0;

	dut0.g_dut_addr = 0x41;

	dut0.g_delay_control_bit = 0;
	dut0.g_delay_done_bit = 0;
	dut0.g_delay2_control_bit = 0;
	dut0.g_delay2_done_bit = 0;

	dut0.g_pn_offset_test_cnt = 1;

	g_timer_flag = 1;

//	for(uint8_t i=32;i<=40;i=i+4)
//	{
//		xil_printf("dut0_pattern_code_capture_reg[%02d] = 0x%x\r\n", i, INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, i));
//	}
//
//	xil_printf("dut0_alarm_reg = 0x%x\r\n", INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 64));
//
//	for(uint8_t i=0;i<=10;i++)
//	{
//		INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 72, i);
//		xil_printf("dut0_pattern_counter_reg[%02d] = %d\r\n", i, INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 76));
//	}
//
//	xil_printf("dut0_end_counter_reg = %d\r\n\r\n", INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 80));
}
