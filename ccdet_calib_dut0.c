#include "platform.h"

int8_t ccdet_calib_dut0(const ccdet_calib_config_t *table, uint8_t table_size, uint8_t cc_id)
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	for(uint8_t i=0;i<table_size;i++)
	{
		tx_data = table[i].vref_sel;
		ret = i2c_write_mem(0xA0, 0xE0, tx_data);
		if(ret != 0)	return -1;

		if(cc_id == 1)
		{
			i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, CC1_LV_DAC1_CH, table[i].dac_data);
		}
		else
		{
			i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, CC2_LV_DAC1_CH, table[i].dac_data);
		}
		delay_ms(10);

		ret = i2c_read_phy((table[i].vref_reg) & 0xFF, (table[i].vref_reg) >> 8, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val  = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(VREF_MASK << table[i].vref_shift);

		uint8_t low = 0;
		uint8_t high = 7;
		uint8_t best_gear = 8;

		while(low <= high)
		{
			uint8_t mid = low + (high - low) / 2;
			uint16_t reg_val = base_val | (mid << table[i].vref_shift);
//			xil_printf("low = %x, high = %x, mid = %x\r\n", low, high, mid);
//			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
	        tx_buf[1] = reg_val >> 8;
	    	ret = i2c_write_phy((table[i].vref_reg) & 0xFF, (table[i].vref_reg) >> 8, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
	    	delay_ms(1);

	    	ret = i2c_read_mem(0xA9, 0xE0, rx_buf);
			if(ret != 0)	return -1;
//	    	xil_printf("dut0 rx_buf[0] = %x\r\n", rx_buf[0]);

			uint8_t bit_status = (rx_buf[0] >> table[i].check_bit) & 0x01;
			if(bit_status == 0)
			{
				best_gear = mid;

				if(mid == 0)	break;

				high = mid - 1;
			}
			else
			{
				low = mid + 1;
			}
		}

		if(best_gear == 8 || best_gear == 0)	return -1;

		uint8_t final_gear = best_gear;
		uint8_t prev_gear = best_gear - 1;
		uint16_t target_dac_val = table[i].dac_data;
//		xil_printf("dut0 final_gear = %x, prev_gear = %x\r\n", final_gear, prev_gear);

		uint16_t actual_dac_0 = ccdet_meas_by_dac_dut0(best_gear, &table[i], cc_id);
	    xil_printf("dut0 actual_dac_0 = %d\r\n", actual_dac_0);
		uint16_t actual_dac_1 = ccdet_meas_by_dac_dut0(prev_gear, &table[i], cc_id);
	    xil_printf("dut0 actual_dac_1 = %d\r\n", actual_dac_1);

		uint16_t diff_0 = ABS_DIFF(actual_dac_0, target_dac_val);
		uint16_t diff_1 = ABS_DIFF(actual_dac_1, target_dac_val);

		if(diff_1 < diff_0)
		{
		    final_gear = prev_gear;
		}
		xil_printf("dut0 cc_id = %x, ccdet_best_offset = %x\r\n", cc_id, final_gear);

		uint16_t final_reg_val = base_val | (final_gear << table[i].vref_shift);

		tx_buf[0] = final_reg_val & 0xFF;
		tx_buf[1] = final_reg_val >> 8;
		ret = i2c_write_phy((table[i].vref_reg) & 0xFF, (table[i].vref_reg) >> 8, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
	}

	return 0;
}
