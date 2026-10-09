#include "platform.h"

int8_t pattern_eu2_test_dut0()
{
	uint8_t rx_buf[10];
    int8_t ret;

    uint8_t timeout;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		//RL7125 work around
		ret = i2c_write_mem(0xCA, 0xDF, 0x01);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0xCA, 0xDF, 0x02);
		if(ret != 0)	return -1;
		delay_ms(1);

		//bit1: efuse fsource switch
		ret = i2c_write_eu2(0x42, 0x02);
		if(ret != 0)	return -1;
		delay_ms(1);

		//bit0: efuse por switch
		ret = i2c_write_eu2(0x42, 0x03);
		if(ret != 0)	return -1;
		delay_ms(1);

		for(uint8_t i=0;i<40;i++)
		{
			//bit[5:0]: efuse address
			ret = i2c_write_eu2(0x21, i);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit7: efuse enable, bit6: efuse mode(0: read, 1: write)
			ret = i2c_write_eu2(0x20, 0x80);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit[7:0]: efuse read data
			ret = i2c_read_eu2(0x23, rx_buf);
			if(ret != 0)	return -1;
			xil_printf("dut0 eu2_efuse_read_data[%02d]: %02x\r\n", i, rx_buf[0]);

			if((dut0.g_ft2_retest == 0) && (rx_buf[0] != 0))
			{
				print("dut0 eu2 efuse is not empty!\r\n");
				return -1;
			}
		}
		print("\r\n");

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x01:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 31, 1);

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

	case 0x02:
	{
		//bit2: rc_cal 3.3V mode
		ret = i2c_write_eu2(0x54, 0x05);
		if(ret != 0)	return -1;
		delay_ms(1);

		//bit3: rc_cal enable
		ret = i2c_write_eu2(0x62, 0x08);
		if(ret != 0)	return -1;
		delay_ms(1);

		//bit[7:0]: cal_cycles_diff_l
		ret = i2c_write_eu2(0x6B, 0x0C);
		if(ret != 0)	return -1;
		delay_ms(1);

		//bit1: rc_cal start
		ret = i2c_write_eu2(0x61, 0x02);
		if(ret != 0)	return -1;
		delay_ms(1);

		for(timeout=0;timeout<50;timeout++)
		{
			//bit 0: rc_cal done
			ret = i2c_read_eu2(0x61, rx_buf);
			if(ret != 0)	return -1;

			if(rx_buf[0] & 0x01)
			{
				//bit 7: rc_cal pass
				ret = i2c_read_eu2(0x63, rx_buf);
				if(ret != 0)	return -1;

				if((rx_buf[0] >> 7) != 1)
				{
					print("dut0 eu2 rc_cal done but cal fail!\r\n\r\n");
					return -1;
				}

				dut0.g_eu2_cal_osc = rx_buf[0] & 0x7F;
				xil_printf("dut0 eu2_cal_osc = 0x%x\r\n", dut0.g_eu2_cal_osc);

				ret = i2c_read_eu2(0x66, rx_buf);
				if(ret != 0)	return -1;

				dut0.g_eu2_cap_osc = rx_buf[0] & 0x03;
				xil_printf("dut0 eu2_cap_osc = 0x%x\r\n\r\n", dut0.g_eu2_cap_osc);

				//bit[6:0] cal_osc efuse
				ret = i2c_write_eu2(0x64, dut0.g_eu2_cal_osc);
				if(ret != 0)	return -1;
				delay_ms(1);

				//bit[3:2] cap_osc efuse
				ret = i2c_write_eu2(0x67, (dut0.g_eu2_cap_osc << 2) | 1);
				if(ret != 0)	return -1;
				delay_ms(1);

				//bit0: rc_cal done w1c
				ret = i2c_write_eu2(0x61, 0x01);
				if(ret != 0)	return -1;
				delay_ms(1);

				dut0.g_type_c_pattern_step++;
				break;
			}
			delay_ms(10);
		}

		if(timeout == 50)
		{
			print("dut0 eu2 rc_cal timeout!\r\n\r\n");
			return -1;
		}

		break;
	}

	case 0x03:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 3, 1);	//GPIO27
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 7, 1);	//GPIO31, eDP/eDM NC

			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 4, 0);	//GPIO20
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 0, 1);	//GPIO24, DP/DM measure

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

	case 0x04:
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VREF1_LV_DAC1_CH, 1650);
		delay_ms(10);

		//eUSB PHY TX drive signal and repeater to USB
		ret = i2c_write_eu2(0x04, 0x09);
		if(ret != 0)	return -1;
		delay_ms(1);

		//FS repeater mode
		ret = i2c_write_eu2(0x03, 0x08);
		if(ret != 0)	return -1;
		delay_ms(1);

		//force repeater FSM to REP mode
		ret = i2c_write_eu2(0x03, 0x09);
		if(ret != 0)	return -1;

		delay_ms(10);

		//select channel, gate_cnt use default(100001)
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 4, 0);

		//start measure
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 1);

		//polling measure done
		for(timeout=0;timeout<50;timeout++)
		{
			if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) >> 31))
			{
				uint32_t dp_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) & 0xFFFFF;
				xil_printf("dut0 eu2 dp_freq_cnt = %d\r\n", dp_freq_cnt);

				uint32_t dp_freq = dp_freq_cnt / 2;	//Hz
				xil_printf("dut0 eu2 dp_freq = %d\r\n\r\n", dp_freq);

				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

				if((dp_freq < RC6M_MIN) || (dp_freq > RC6M_MAX))	return -1;

				break;
			}
			delay_ms(1);
		}

		if(timeout == 50)
		{
			//clear all
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

			print("dut0 eu2 dp_freq measure timeout!\r\n\r\n");
			return -1;
		}

		//select channel, gate_cnt use default(100001)
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 4, 1);

		//start measure
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 1);

		//polling measure done
		for(timeout=0;timeout<50;timeout++)
		{
			if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) >> 31))
			{
				uint32_t dm_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) & 0xFFFFF;
				xil_printf("dut0 eu2 dm_freq_cnt = %d\r\n", dm_freq_cnt);

				uint32_t dm_freq = dm_freq_cnt / 2;	//Hz
				xil_printf("dut0 eu2 dm_freq = %d\r\n\r\n", dm_freq);

				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

				if((dm_freq < RC6M_MIN) || (dm_freq > RC6M_MAX))	return -1;

				break;
			}
			delay_ms(1);
		}

		if(timeout == 50)
		{
			//clear all
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

			print("dut0 eu2 dm_freq measure timeout!\r\n\r\n");
			return -1;
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x05:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 4, 1);	//GPIO20, DP/DM NC

			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 6, 0);	//GPIO22
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 1, 1);	//GPIO25, eDP/eDM measure

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

	case 0x06:
	{
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, VREF2_LV_DAC1_CH, 600);
		delay_ms(10);

		//USB PHY TX drive signal and repeater to eUSB
		ret = i2c_write_eu2(0x04, 0x05);
		if(ret != 0)	return -1;
		delay_ms(1);

		//FS repeater mode
		ret = i2c_write_eu2(0x03, 0x08);
		if(ret != 0)	return -1;
		delay_ms(1);

		//force repeater FSM to REP mode
		ret = i2c_write_eu2(0x03, 0x09);
		if(ret != 0)	return -1;

		delay_ms(10);

		//select channel, gate_cnt use default(100001)
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 4, 3);

		//start measure
		FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 1);

		//polling measure done
		for(timeout=0;timeout<50;timeout++)
		{
			if((FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) >> 31))
			{
				uint32_t edm_freq_cnt = FREQ_MEAS_READ_REG(DUT0_GATE_COUNTER_1_ADDR, 12) & 0xFFFFF;
				xil_printf("dut0 eu2 edm_freq_cnt = %d\r\n", edm_freq_cnt);

				uint32_t edm_freq = edm_freq_cnt / 2;	//Hz
				xil_printf("dut0 eu2 edm_freq = %d\r\n\r\n", edm_freq);

				//clear all
				FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

				if((edm_freq < RC6M_MIN) || (edm_freq > RC6M_MAX))	return -1;

				break;
			}
			delay_ms(1);
		}

		if(timeout == 50)
		{
			//clear all
			FREQ_MEAS_WRITE_REG(DUT0_GATE_COUNTER_1_ADDR, 0, 0);

			print("dut0 eu2 edm_freq measure timeout!\r\n\r\n");
			return -1;
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x07:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 3, 0);	//GPIO27
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 7, 0);	//GPIO31

			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 4, 1);	//GPIO20
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 6, 1);	//GPIO22

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

	case 0x08:
	{
		//U2 swing 0x00
		ret = i2c_write_eu2(0xDC, 0x00);
		if(ret != 0)	return -1;
		delay_ms(1);

		//U2 Z0 default
		ret = i2c_write_eu2(0xF0, 0x07);
		if(ret != 0)	return -1;
		delay_ms(1);

		//eU2 Rrcv off
		ret = i2c_write_eu2(0x0A, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		//eU2 Rsrc/Rrcv default
		ret = i2c_write_eu2(0xEB, 0x87);
		if(ret != 0)	return -1;
		delay_ms(1);

		//force mode: HS_J
		ret = eu2_force_mode_enter(0x06, 0x07);
		if(ret != 0)	return -1;

		uint16_t hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dp = %d\r\n", hs_j_u2_dp);

		uint16_t hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dm = %d\r\n", hs_j_u2_dm);

		dut0.g_hs_j_u2_base_diff = abs((int)hs_j_u2_dp - (int)hs_j_u2_dm);
		xil_printf("dut0 hs_j_u2_diff = %d\r\n", dut0.g_hs_j_u2_base_diff);

		uint16_t hs_j_eu2_edp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_edp = %d\r\n", hs_j_eu2_edp);

		uint16_t hs_j_eu2_edm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_edm = %d\r\n", hs_j_eu2_edm);

		dut0.g_hs_j_eu2_base_diff = abs((int)hs_j_eu2_edp - (int)hs_j_eu2_edm);
		xil_printf("dut0 hs_j_eu2_diff = %d\r\n\r\n", dut0.g_hs_j_eu2_base_diff);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x09:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 5, 1);	//GPIO21, U2 45 ohm
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 7, 1);	//GPIO23, eU2 80 ohm

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

	case 0x0a:
	{
	    //target is half of open voltage
	    uint16_t u2_target_diff = dut0.g_hs_j_u2_base_diff / 2;
	    xil_printf("dut0 u2 target_diff = %d\r\n", u2_target_diff);

		//U2 Z0 trim, 0xF0[3:0]
		uint16_t hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dp = %d\r\n", hs_j_u2_dp);

		uint16_t hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dm = %d\r\n", hs_j_u2_dm);

		uint16_t hs_j_u2_diff = abs((int)hs_j_u2_dp - (int)hs_j_u2_dm);
		xil_printf("dut0 hs_j_u2_diff = %d\r\n", hs_j_u2_diff);

		ret = i2c_read_eu2(0xF0, rx_buf);
		if(ret != 0)	return -1;

	    uint8_t base_val = rx_buf[0] & 0x0F;

	    int8_t start_gear, step;
	    uint8_t end_gear;

	    if(hs_j_u2_diff > u2_target_diff)
	    {
	        start_gear = base_val;
	        end_gear = 15;
	        step = 1;
	    }
	    else
	    {
	        start_gear = base_val;
	        end_gear = 0;
	        step = -1;
	    }

	    uint16_t min_diff = 0xFFFF;
	    uint16_t best_vol = 0;

	    for(int8_t i=start_gear;i!=(end_gear+step);i+=step)
	    {
			ret = i2c_write_eu2(0xF0, (rx_buf[0] & 0xF0) | i);
			if(ret != 0)	return -1;
			delay_ms(1);

			hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
			hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
			hs_j_u2_diff = abs((int)hs_j_u2_dp - (int)hs_j_u2_dm);
			xil_printf("i = %02d, dut0 hs_j_u2_dp = %d, hs_j_u2_dm = %d, diff = %d\r\n", i, hs_j_u2_dp, hs_j_u2_dm, hs_j_u2_diff);

			uint16_t diff = abs((int)hs_j_u2_diff - (int)u2_target_diff);

	        if(diff < min_diff)
	        {
	            min_diff = diff;
	            dut0.g_u2_z0_offset = i;
	            best_vol = hs_j_u2_diff;
	        }
	    }
	    xil_printf("dut0 u2_z0_offset = %x\r\n\r\n", dut0.g_u2_z0_offset);

//	    if((best_vol < (dut0.g_hs_j_u2_base_diff * 23 / 47)) || (best_vol > (dut0.g_hs_j_u2_base_diff * 22 / 43)))	return -1;

		ret = i2c_write_eu2(0xF0, (rx_buf[0] & 0xF0) | dut0.g_u2_z0_offset);
		if(ret != 0)	return -1;

	    //target is half of open voltage
	    uint16_t eu2_target_diff = dut0.g_hs_j_eu2_base_diff / 2;
	    xil_printf("dut0 eu2 target_diff = %d\r\n", eu2_target_diff);

		//eU2 Rsrc trim, 0xEB[7:4]
		uint16_t hs_j_eu2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_dp = %d\r\n", hs_j_eu2_dp);

		uint16_t hs_j_eu2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_dm = %d\r\n", hs_j_eu2_dm);

		uint16_t hs_j_eu2_diff = abs((int)hs_j_eu2_dp - (int)hs_j_eu2_dm);
		xil_printf("dut0 hs_j_eu2_diff = %d\r\n", hs_j_eu2_diff);

		ret = i2c_read_eu2(0xEB, rx_buf);
		if(ret != 0)	return -1;

	    base_val = rx_buf[0] >> 4;

	    if(hs_j_eu2_diff > eu2_target_diff)
	    {
	        start_gear = base_val;
	        end_gear = 0;
	        step = -1;
	    }
	    else
	    {
	        start_gear = base_val;
	        end_gear = 15;
	        step = 1;
	    }

	    min_diff = 0xFFFF;

	    for(int8_t i=start_gear;i!=(end_gear+step);i+=step)
	    {
			ret = i2c_write_eu2(0xEB, (rx_buf[0] & 0x0F) | (i << 4));
			if(ret != 0)	return -1;
			delay_ms(1);

			hs_j_eu2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
			hs_j_eu2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
			hs_j_eu2_diff = abs((int)hs_j_eu2_dp - (int)hs_j_eu2_dm);
			xil_printf("i = %02d, dut0 hs_j_eu2_dp = %d, hs_j_eu2_dm = %d, diff = %d\r\n", i, hs_j_eu2_dp, hs_j_eu2_dm, hs_j_eu2_diff);

			uint16_t diff = abs((int)hs_j_eu2_diff - (int)eu2_target_diff);

	        if(diff < min_diff)
	        {
	            min_diff = diff;
	            dut0.g_eu2_rsrc_offset = i;
	            dut0.g_hs_j_eu2_best_diff = hs_j_eu2_diff;
	        }
	    }
	    xil_printf("dut0 eu2_rsrc_offset = %x, eu2_best_diff = %d\r\n\r\n", dut0.g_eu2_rsrc_offset, dut0.g_hs_j_eu2_best_diff);

//	    if((dut0.g_hs_j_eu2_best_diff < (dut0.g_hs_j_eu2_base_diff * 80 / (42 + 80 + 42))) || (dut0.g_hs_j_eu2_best_diff > (dut0.g_hs_j_eu2_base_diff * 80 / (38 + 80 + 38))))	return -1;

		ret = i2c_write_eu2(0xEB, (rx_buf[0] & 0x0F) | (dut0.g_eu2_rsrc_offset << 4));
		if(ret != 0)	return -1;

		//U2 swing default
		ret = i2c_write_eu2(0xDC, 0x10);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0b:
	{
		//U2 swing trim, 0xDE[7:5]
		uint16_t hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dp = %d\r\n", hs_j_u2_dp);

		uint16_t hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dm = %d\r\n", hs_j_u2_dm);

		uint16_t hs_j_u2_diff = abs((int)hs_j_u2_dp - (int)hs_j_u2_dm);
		xil_printf("dut0 hs_j_u2_diff = %d\r\n", hs_j_u2_diff);

		ret = i2c_read_eu2(0xDE, rx_buf);
		if(ret != 0)	return -1;

	    uint8_t base_val = rx_buf[0] >> 5;

	    int8_t start_gear, step;
	    uint8_t end_gear;

	    if(hs_j_u2_diff > HS_J_U2_DIFF_REF)
	    {
	        start_gear = base_val;
	        end_gear = 0;
	        step = -1;
	    }
	    else
	    {
	        start_gear = base_val;
	        end_gear = 7;
	        step = 1;
	    }

	    uint16_t min_diff = 0xFFFF;
	    uint16_t best_vol;

	    for(int8_t i=start_gear;i!=(end_gear+step);i+=step)
	    {
			ret = i2c_write_eu2(0xDE, (rx_buf[0] & 0x1F) | (i << 5));
			if(ret != 0)	return -1;
			delay_ms(1);

			hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
			hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
			hs_j_u2_diff = abs((int)hs_j_u2_dp - (int)hs_j_u2_dm);
			xil_printf("i = %02d, dut0 hs_j_u2_dp = %d, hs_j_u2_dm = %d, diff = %d\r\n", i, hs_j_u2_dp, hs_j_u2_dm, hs_j_u2_diff);

			uint16_t diff = abs((int)hs_j_u2_diff - HS_J_U2_DIFF_REF);

	        if(diff < min_diff)
	        {
	            min_diff = diff;
	            dut0.g_u2_swing_offset = i;
	            best_vol = hs_j_u2_diff;
	        }
	    }
	    xil_printf("dut0 u2_swing_offset = %x\r\n\r\n", dut0.g_u2_swing_offset);

	    if((best_vol < HS_J_U2_DIFF_MIN) || (best_vol > HS_J_U2_DIFF_MAX))	return -1;

		ret = i2c_write_eu2(0xDE, (rx_buf[0] & 0x1F) | (dut0.g_u2_swing_offset << 5));
		if(ret != 0)	return -1;

		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 5, 0);	//GPIO21, remove U2 45 ohm
		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 7, 0);	//GPIO23, remove eU2 80 ohm

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0c:
	{
		//eU2 Rrcv on
		ret = i2c_write_eu2(0x0A, 0x7C);
		if(ret != 0)	return -1;
		delay_ms(1);

		//eU2 Rrcv trim, 0xEB[3:0]
		uint16_t hs_j_eu2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_dp = %d\r\n", hs_j_eu2_dp);

		uint16_t hs_j_eu2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_dm = %d\r\n", hs_j_eu2_dm);

		uint16_t hs_j_eu2_diff = abs((int)hs_j_eu2_dp - (int)hs_j_eu2_dm);
		xil_printf("dut0 hs_j_eu2_diff = %d\r\n", hs_j_eu2_diff);

		ret = i2c_read_eu2(0xEB, rx_buf);
		if(ret != 0)	return -1;

	    uint8_t base_val = rx_buf[0] & 0x0F;

	    int8_t start_gear, step;
	    uint8_t end_gear;

	    if(hs_j_eu2_diff > dut0.g_hs_j_eu2_best_diff)
	    {
	        start_gear = base_val;
	        end_gear = 15;
	        step = 1;
	    }
	    else
	    {
	        start_gear = base_val;
	        end_gear = 0;
	        step = -1;
	    }

	    uint16_t min_diff = 0xFFFF;
	    uint16_t best_vol = 0;

	    for(int8_t i=start_gear;i!=(end_gear+step);i+=step)
	    {
			ret = i2c_write_eu2(0xEB, (rx_buf[0] & 0xF0) | i);
			if(ret != 0)	return -1;
			delay_ms(1);

			hs_j_eu2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
			hs_j_eu2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
			hs_j_eu2_diff = abs((int)hs_j_eu2_dp - (int)hs_j_eu2_dm);
			xil_printf("i = %02d, dut0 hs_j_eu2_dp = %d, hs_j_eu2_dm = %d, diff = %d\r\n", i, hs_j_eu2_dp, hs_j_eu2_dm, hs_j_eu2_diff);

			uint16_t diff = abs((int)hs_j_eu2_diff - (int)dut0.g_hs_j_eu2_best_diff);

	        if(diff < min_diff)
	        {
	            min_diff = diff;
	            dut0.g_eu2_rrcv_offset = i;
	            best_vol = hs_j_eu2_diff;
	        }
	    }
	    xil_printf("dut0 eu2_rrcv_offset = %x\r\n\r\n", dut0.g_eu2_rrcv_offset);

//	    if((best_vol < (dut0.g_hs_j_eu2_base_diff * 78 / (40 + 78 + 40))) || (best_vol > (dut0.g_hs_j_eu2_base_diff * 82 / (40 + 82 + 40))))	return -1;

		ret = i2c_write_eu2(0xEB, (rx_buf[0] & 0xF0) | dut0.g_eu2_rrcv_offset);
		if(ret != 0)	return -1;

		ret = eu2_force_mode_reset();
		if(ret != 0) return -1;

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0d:
	{
		//force mode: FS_K
		ret = eu2_force_mode_enter(0x00, 0x01);
		if(ret != 0)	return -1;

		uint16_t fs_k_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_k_u2_dp = %d\r\n", fs_k_u2_dp);

		if(fs_k_u2_dp > FS_K_U2_DP_MAX)	return -1;

		uint16_t fs_k_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_k_u2_dm = %d\r\n", fs_k_u2_dm);

		if((fs_k_u2_dm < FS_K_U2_DM_MIN) ||(fs_k_u2_dm > FS_K_U2_DM_MAX))	return -1;

		uint16_t fs_k_eu2_edp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_k_eu2_edp = %d\r\n", fs_k_eu2_edp);

		if(fs_k_eu2_edp > FS_K_eU2_eDP_MAX)	return -1;

		uint16_t fs_k_eu2_edm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_k_eu2_edm = %d\r\n", fs_k_eu2_edm);

		if((fs_k_eu2_edm < FS_K_eU2_eDM_MIN) ||(fs_k_eu2_edm > FS_K_eU2_eDM_MAX))	return -1;

		//force mode: FS_J
		ret = eu2_force_mode_enter(0x02, 0x03);
		if(ret != 0)	return -1;

		uint16_t fs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_j_u2_dp = %d\r\n", fs_j_u2_dp);

		if((fs_j_u2_dp < FS_J_U2_DP_MIN) ||(fs_j_u2_dp > FS_J_U2_DP_MAX))	return -1;

		uint16_t fs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_j_u2_dm = %d\r\n", fs_j_u2_dm);

		if(fs_j_u2_dm > FS_J_U2_DM_MAX)	return -1;

		uint16_t fs_j_eu2_edp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_j_eu2_edp = %d\r\n", fs_j_eu2_edp);

		if((fs_j_eu2_edp < FS_J_eU2_eDP_MIN) ||(fs_j_eu2_edp > FS_J_eU2_eDP_MAX))	return -1;

		uint16_t fs_j_eu2_edm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 fs_j_eu2_edm = %d\r\n\r\n", fs_j_eu2_edm);

		if(fs_j_eu2_edm > FS_J_eU2_eDM_MAX)	return -1;

		ret = eu2_force_mode_reset();
		if(ret != 0) return -1;

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0e:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 5, 1);	//GPIO21, U2 45 ohm

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

	case 0x0f:
	{
		//force mode: HS_K
		ret = eu2_force_mode_enter(0x04, 0x05);
		if(ret != 0)	return -1;

		uint16_t hs_k_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_k_u2_dp = %d\r\n", hs_k_u2_dp);

		uint16_t hs_k_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_k_u2_dm = %d\r\n", hs_k_u2_dm);

		if((abs((int)hs_k_u2_dp - (int)hs_k_u2_dm) < HS_K_U2_DIFF_MIN) || (abs((int)hs_k_u2_dp - (int)hs_k_u2_dm) > HS_K_U2_DIFF_MAX))	return -1;

		uint16_t hs_k_eu2_edp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_k_eu2_edp = %d\r\n", hs_k_eu2_edp);

		uint16_t hs_k_eu2_edm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_k_eu2_edm = %d\r\n", hs_k_eu2_edm);

		if((abs((int)hs_k_eu2_edp - (int)hs_k_eu2_edm) < HS_K_eU2_DIFF_MIN) || (abs((int)hs_k_eu2_edp - (int)hs_k_eu2_edm) > HS_K_eU2_DIFF_MAX))	return -1;

		//force mode: HS_J
		ret = eu2_force_mode_enter(0x06, 0x07);
		if(ret != 0)	return -1;

		uint16_t hs_j_u2_dp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dp = %d\r\n", hs_j_u2_dp);

		uint16_t hs_j_u2_dm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, DM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_u2_dm = %d\r\n", hs_j_u2_dm);

		if((abs((int)hs_j_u2_dp - (int)hs_j_u2_dm) < HS_J_U2_DIFF_MIN) || (abs((int)hs_j_u2_dp - (int)hs_j_u2_dm) > HS_J_U2_DIFF_MAX))	return -1;

		uint16_t hs_j_eu2_edp = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDP_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_edp = %d\r\n", hs_j_eu2_edp);

		uint16_t hs_j_eu2_edm = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC1_ADDR, eDM_ADC1_CH, AVG_NUM);
		xil_printf("dut0 hs_j_eu2_edm = %d\r\n\r\n", hs_j_eu2_edm);

		if((abs((int)hs_j_eu2_edp - (int)hs_j_eu2_edm) < HS_J_eU2_DIFF_MIN) || (abs((int)hs_j_eu2_edp - (int)hs_j_eu2_edm) > HS_J_eU2_DIFF_MAX))	return -1;

		ret = eu2_force_mode_reset();
		if(ret != 0) return -1;

		i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 2, 5, 0);	//GPIO21, remove U2 45 ohm

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x10:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			i2c_pca9506_output(DUT0_IIC_0_ADDR, PCA9506_ADDR, 3, 3, 1);	//GPIO27, DAC

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

	case 0x11:
	{
		//U2_squelch_Vth trim(DP 98mV, DM 0mV): 0xF1 bit[3:0]
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, DP_LV_DAC2_CH, 98);
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, DM_LV_DAC2_CH, 0);
		delay_ms(10);

		//eU2_squelch_Vth set max
		ret = i2c_write_eu2(0xE9, 0xF7);
		if(ret != 0)	return -1;
		delay_ms(1);

		//U2_squelch_Vth/U2_disconnect_Vth set max
		ret = i2c_write_eu2(0xF1, 0xFF);
		if(ret != 0)	return -1;
		delay_ms(1);

		//force mode: HS repeater mode
		ret = eu2_force_mode_enter(0x0A, 0x0B);
		if(ret != 0)	return -1;

		ret = i2c_read_eu2(0x03, rx_buf);
		if(ret != 0)	return -1;

		if(rx_buf[0] != 0x0B)	return -1;

		uint8_t low = 0;
		uint8_t high = 15;
		uint8_t best_gear = 0;

		ret = i2c_read_eu2(0xF1, rx_buf);
		if(ret != 0)	return -1;

		uint8_t base_val = rx_buf[0] & 0xF0;

		while(low <= high)
		{
			uint8_t mid = low + (high - low) / 2;
			xil_printf("low = %x, high = %x, mid = %x\r\n", low, high, mid);

			ret = i2c_write_eu2(0xF1, base_val | mid);
			if(ret != 0)	return -1;
			delay_ms(1);

			uint8_t count = 0;

			for(uint8_t i=0;i<8;i++)
			{
				ret = i2c_read_eu2(0x0E, rx_buf);
				if(ret != 0)	return -1;

				xil_printf("reg_val = %x\r\n", rx_buf[0]);

				uint8_t bit_status = (rx_buf[0] >> 5) & 0x01;
				if(bit_status == 1)	count++;
			}
			print("\r\n");

			if(count >= 7)
			{
				best_gear = mid;
				low = mid + 1;
			}
			else
			{
				if(mid == 0)	break;
				high = mid - 1;
			}
		}
		xil_printf("dut0 u2_squelch_vth_offset = %x\r\n\r\n", best_gear);

		if(best_gear == 15 || best_gear == 0)	return -1;

		ret = i2c_write_eu2(0xF1, base_val | best_gear);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x12:
	{
		//U2_disconnect_Vth trim(DP 635mV, DM 0mV): 0xF1 bit[7:4]
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, DP_LV_DAC2_CH, 635);
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, DM_LV_DAC2_CH, 0);
		delay_ms(10);

		uint8_t low = 0;
		uint8_t high = 15;
		uint8_t best_gear = 0;

		ret = i2c_read_eu2(0xF1, rx_buf);
		if(ret != 0)	return -1;

		uint8_t base_val = rx_buf[0] & 0x0F;

		while(low <= high)
		{
			uint8_t mid = low + (high - low) / 2;
			xil_printf("low = %x, high = %x, mid = %x\r\n", low, high, mid);

			ret = i2c_write_eu2(0xF1, base_val | (mid << 4));
			if(ret != 0)	return -1;
			delay_ms(1);

			uint8_t count = 0;

			for(uint8_t i=0;i<8;i++)
			{
				ret = i2c_read_eu2(0x0E, rx_buf);
				if(ret != 0)	return -1;

				xil_printf("reg_val = %x\r\n", rx_buf[0]);

				uint8_t bit_status = (rx_buf[0] >> 6) & 0x01;
				if(bit_status == 1)	count++;
			}
			print("\r\n");

			if(count >= 7)
			{
				best_gear = mid;
				low = mid + 1;
			}
			else
			{
				if(mid == 0)	break;
				high = mid - 1;
			}
		}
		xil_printf("dut0 u2_disconnect_vth_offset = %x\r\n\r\n", best_gear);

		if(best_gear == 15 || best_gear == 0)	return -1;

		ret = i2c_write_eu2(0xF1, base_val | (best_gear << 4));
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x13:
	{
		//eU2_squelch_Vth trim(eDP 218mV, eDM 150mV): 0xE9 bit[7:4]
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, eDP_LV_DAC2_CH, 218);
		i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR2, eDM_LV_DAC2_CH, 150);
		delay_ms(10);

		uint8_t low = 0;
		uint8_t high = 15;
		uint8_t best_gear = 0;

		ret = i2c_read_eu2(0xE9, rx_buf);
		if(ret != 0)	return -1;

		uint8_t base_val = rx_buf[0] & 0x0F;

		while(low <= high)
		{
			uint8_t mid = low + (high - low) / 2;
			xil_printf("low = %x, high = %x, mid = %x\r\n", low, high, mid);

			ret = i2c_write_eu2(0xE9, base_val | (mid << 4));
			if(ret != 0)	return -1;
			delay_ms(1);

			uint8_t count = 0;

			for(uint8_t i=0;i<8;i++)
			{
				ret = i2c_read_eu2(0x0D, rx_buf);
				if(ret != 0)	return -1;

				xil_printf("reg_val = %x\r\n", rx_buf[0]);

				uint8_t bit_status = (rx_buf[0] >> 5) & 0x01;
				if(bit_status == 1)	count++;
			}
			print("\r\n");

			if(count >= 7)
			{
				best_gear = mid;
				low = mid + 1;
			}
			else
			{
				if(mid == 0)	break;
				high = mid - 1;
			}
		}
		xil_printf("dut0 eu2_squelch_vth_offset = %x\r\n\r\n", best_gear);

		if(best_gear == 15 || best_gear == 0)	return -1;

		ret = i2c_write_eu2(0xE9, base_val | (best_gear << 4));
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x14:
	{
		const uint8_t reg_data[] = {0x64, 0x67, 0xF1, 0xE9, 0xF0, 0xEB, 0xDE};

	    memset(dut0.g_eu2_efuse_w_data, 0, sizeof(dut0.g_eu2_efuse_w_data));
	    memset(dut0.g_eu2_efuse_r_data, 0, sizeof(dut0.g_eu2_efuse_r_data));

		for(uint8_t i=0;i<7;i++)
		{
			ret = i2c_read_eu2(reg_data[i], rx_buf);
			if(ret != 0)	return -1;

			dut0.g_eu2_efuse_w_data[2 * i] = rx_buf[0];
			dut0.g_eu2_efuse_w_data[2 * i + 1] = reg_data[i];
		}

		for(uint8_t i=0;i<14;i++)
		{
			xil_printf("dut0 eu2_efuse_w_data[%02d] = %02x\r\n", i, dut0.g_eu2_efuse_w_data[i]);
		}
		print("\r\n");

		if(dut0.g_ft2_retest == 0)
		{
			print("dut0 eu2 new, go to write efuse!\r\n\r\n");
			dut0.g_type_c_pattern_step++;

		}
		else
		{
			print("dut0 eu2 retest, go to judge tolerance!\r\n\r\n");
			dut0.g_type_c_pattern_step++;
			dut0.g_type_c_pattern_step++;
		}
		break;
	}

	case 0x15:
	{
		for(uint8_t i=0;i<14;i++)
		{
			//bit6: efuse mode(0: read, 1: write)
			ret = i2c_write_eu2(0x20, 0x40);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit[5:0]: efuse address
			ret = i2c_write_eu2(0x21, i);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit[7:0]: efuse write data
			ret = i2c_write_eu2(0x22, dut0.g_eu2_efuse_w_data[i]);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit7: efuse enable
			ret = i2c_write_eu2(0x20, 0xC0);
			if(ret != 0)	return -1;
			delay_ms(1);
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x16:
	{
		for(uint8_t i=0;i<40;i++)
		{
			//bit[5:0]: efuse address
			ret = i2c_write_eu2(0x21, i);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit7: efuse enable, bit6: efuse mode(0: read, 1: write)
			ret = i2c_write_eu2(0x20, 0x80);
			if(ret != 0)	return -1;
			delay_ms(1);

			//bit[7:0]: efuse read data
			ret = i2c_read_eu2(0x23, rx_buf);
			if(ret != 0)	return -1;

			dut0.g_eu2_efuse_r_data[i] = rx_buf[0];
			xil_printf("dut0 eu2_efuse_read_data[%02d] = %02x\r\n", i, dut0.g_eu2_efuse_r_data[i]);
		}
		print("\r\n");

		if(dut0.g_ft2_retest == 0)
		{
			if(memcmp(dut0.g_eu2_efuse_w_data, dut0.g_eu2_efuse_r_data, sizeof(dut0.g_eu2_efuse_w_data)) == 0)
			{
				print("dut0 eu2 efuse write/read compare pass!\r\n\r\n");
			}
			else
			{
				print("dut0 eu2 efuse write/read compare fail!\r\n\r\n");
				return -1;
			}

			dut0.g_type_c_pattern_step++;
			dut0.g_type_c_pattern_step++;
			break;
		}
		else
		{
			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x17:
	{
		//judge tolerance: cal_osc
		if(abs((int)(dut0.g_eu2_efuse_w_data[0] & 0x7F) - (int)(dut0.g_eu2_efuse_r_data[0] & 0x7F)) > CAL_OSC_TOLERANCE)
		{
			print("dut0 cal_osc tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: cap_osc
		if(abs((int)((dut0.g_eu2_efuse_w_data[2] >> 2) & 0x03) - (int)((dut0.g_eu2_efuse_r_data[2] >> 2) & 0x03)) > CAP_OSC_TOLERANCE)
		{
			print("dut0 cap_osc tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: u2_squelch_vth
		if(abs((int)(dut0.g_eu2_efuse_w_data[4] & 0x0F) - (int)(dut0.g_eu2_efuse_r_data[4] & 0x0F)) > U2_SQUELCH_VTH_TOLERANCE)
		{
			print("dut0 u2_squelch_vth tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: u2_disconnect_vth
		if(abs((int)(dut0.g_eu2_efuse_w_data[4] >> 4) - (int)(dut0.g_eu2_efuse_r_data[4] >> 4)) > U2_DISC_VTH_TOLERANCE)
		{
			print("dut0 u2_disconnect_vth tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: eu2_squelch_vth
		if(abs((int)(dut0.g_eu2_efuse_w_data[6] >> 4) - (int)(dut0.g_eu2_efuse_r_data[6] >> 4)) > EU2_SQUELCH_VTH_TOLERANCE)
		{
			print("dut0 eu2_squelch_vth tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: u2_z0
		if(abs((int)(dut0.g_eu2_efuse_w_data[8] & 0x0F) - (int)(dut0.g_eu2_efuse_r_data[8] & 0x0F)) > U2_Z0_TOLERANCE)
		{
			print("dut0 u2_z0 tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: eu2_rsrc
		if(abs((int)(dut0.g_eu2_efuse_w_data[10] >> 4) - (int)(dut0.g_eu2_efuse_r_data[10] >> 4)) > EU2_RSRC_TOLERANCE)
		{
			print("dut0 eu2_rsrc tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: eu2_rrcv
		if(abs((int)(dut0.g_eu2_efuse_w_data[10] & 0x0F) - (int)(dut0.g_eu2_efuse_r_data[10] & 0x0F)) > EU2_RRCV_TOLERANCE)
		{
			print("dut0 eu2_rrcv tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: u2_swing
		if(abs((int)(dut0.g_eu2_efuse_w_data[12] >> 5) - (int)(dut0.g_eu2_efuse_r_data[12] >> 5)) > U2_SWING_TOLERANCE)
		{
			print("dut0 u2_swing tolerance judge fail!\r\n\r\n");
			return -1;
		}

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x18:
	{

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_eu2_test pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
