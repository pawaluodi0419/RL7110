#include "platform.h"

void platform_initial()
{
	if(dut0.g_ft2_start_ready == 0x11)
	{
		xgpio_write_bit(DUT0_GPIO_1_ADDR, 0, 15, 0);									//write EOT low

		if(xgpio_read_bit(DUT0_GPIO_1_ADDR, 0, 9) == 0)
		{
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 0, 0x02010000 | FT_FW_VER);	//reg0: write pattern code
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 4, 0x06050403);				//reg1: write pattern code
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 8, 0x000c0807);				//reg2: write pattern code
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 124, 0x0000000c);				//reg31: write pass code/fail code

			dut0.g_lrd_pattern_num = 0;
			dut0.g_type_c_pattern_num = 0;

			result_output_for_v50(DUT0_GPIO_1_ADDR, FT_FW_VER);						//write ready code

			dut0.g_ft2_start_ready = 1;
			dut0.g_ft2_test_done = 0;
			dut0.g_test_cnt++;
			xil_printf("---------- dut0.g_test_cnt ---------- = %d\r\n\r\n", dut0.g_test_cnt);
		}
		else
		{
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 0, 0x000e0000 | FT_FW_VER);	//reg0: write pattern code
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 124, 0x0000000e);				//reg31: write pass code/fail code

			dut0.g_lrd_pattern_num = 0;
			dut0.g_type_c_pattern_num = 0;

			result_output_for_v50(DUT0_GPIO_1_ADDR, FT_FW_VER);						//write ready code

			dut0.g_ft2_start_ready = 1;
			dut0.g_ft2_test_done = 0;
			dut0.g_eqa_test_cnt++;
			xil_printf("---------- dut0.g_eqa_test_cnt ---------- = %d\r\n\r\n", dut0.g_eqa_test_cnt);
		}

//		uint32_t alarm_reg_val = INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 64);
//
//		if(alarm_reg_val >> 16)
//		{
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 64, alarm_reg_val | (1 << 0));
//			xil_printf("dut0_alarm_reg = 0x%x\r\n\r\n", INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 64));
//
//			INF_CHECK_WRITE_REG(DUT0_INF_CHECK_ADDR, 64, alarm_reg_val & ~(1 << 0));
//			xil_printf("dut0_alarm_reg = 0x%x\r\n\r\n", INF_CHECK_READ_REG(DUT0_INF_CHECK_ADDR, 64));
//		}
	}
}

void microblaze_softreset()
{
    microblaze_disable_interrupts();

    reset_function_t reset_vector = (reset_function_t)RESET_VECTOR_ADDRESS;
    reset_vector();
}
