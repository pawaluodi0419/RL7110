#include "platform.h"

int8_t lfps1_oobs_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr, uint8_t en_bit)
{
	uint8_t reg_val;
	uint16_t eq_diff;

	switch(dut0.g_lrd_pattern_substep)
	{
	//auto_eq off
	case 0x00:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_lfps_oobs_offset = 7;

			reg_val = 0;
			reg_val |= (dut0.g_lfps_oobs_offset << 4);
			mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, reg_val);

			//auto eq off
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1);
			reg_val &= ~0x03;
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(10);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_adc_data_eq_off = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
//			xil_printf("dut0.g_adc_data_eq_off: %dmV\r\n", dut0.g_adc_data_eq_off);

			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	//auto_eq on
	case 0x01:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			if(en_bit)
			{
				reg_val = 0;
				reg_val |= (1 << 2) | (1 << 1);
				mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);
			}
			else
			{
				reg_val = 0;
				reg_val |= (1 << 5) | (1 << 0);
				mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);
			}

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_adc_data_eq_on = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
//			xil_printf("dut0.g_adc_data_eq_on:  %dmV\r\n\r\n", dut0.g_adc_data_eq_on);

			eq_diff = abs((int)dut0.g_adc_data_eq_off - (int)dut0.g_adc_data_eq_on);

			if(eq_diff < 10)
			{
				dut0.g_lfps_oobs_signbit = 1;
			}
			else
			{
				dut0.g_lfps_oobs_signbit = 0;
			}

			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	case 0x02:
	{
		if(dut0.g_lfps_oobs_signbit == 0)
		{
			if((dut0.g_lfps_oobs_offset >= 7) && (dut0.g_lfps_oobs_offset < 13))
			{
				dut0.g_lfps_oobs_offset++;

				reg_val = 0;
				reg_val |= (dut0.g_lfps_oobs_offset << 4);
				mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, reg_val);

				dut0.g_lrd_pattern_substep++;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			if((dut0.g_lfps_oobs_offset > 0) && (dut0.g_lfps_oobs_offset <= 7))
			{
				dut0.g_lfps_oobs_offset--;

				reg_val = 0;
				reg_val |= (dut0.g_lfps_oobs_offset << 4);
				mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, reg_val);

				dut0.g_lrd_pattern_substep++;
			}
			else
			{
				return -1;
			}
		}
		break;
	}

	case 0x03:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(10);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	//auto_eq off
	case 0x04:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//auto eq off
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1);
			reg_val &= ~0x03;
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(10);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_adc_data_eq_off = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
//			xil_printf("dut0.g_adc_data_eq_off: %dmV\r\n", dut0.g_adc_data_eq_off);

			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	//auto_eq on
	case 0x05:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			if(en_bit)
			{
				reg_val = 0;
				reg_val |= (1 << 2) | (1 << 1);
				mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);
			}
			else
			{
				reg_val = 0;
				reg_val |= (1 << 5) | (1 << 0);
				mdio_master_write(DUT0_MDIO_BASEADDR, CFG_AUTO_EQ_EN_LEVEL1, reg_val);
			}

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_adc_data_eq_on = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
//			xil_printf("dut0.g_adc_data_eq_on:  %dmV\r\n\r\n", dut0.g_adc_data_eq_on);

			eq_diff = abs((int)dut0.g_adc_data_eq_off - (int)dut0.g_adc_data_eq_on);

			if(eq_diff < 10)
			{
				if(dut0.g_lfps_oobs_signbit == 0)
				{
					dut0.g_lfps_oobs_offset--;

					xil_printf("dut0.g_adc_data_eq_off: %dmV\r\n", dut0.g_adc_data_eq_off);
					xil_printf("dut0.g_adc_data_eq_on:  %dmV\r\n", dut0.g_adc_data_eq_on);

					dut0.g_lrd_pattern_substep = 0;
					return 1;
				}
				else
				{
					dut0.g_lrd_pattern_substep = 0x02;
				}
			}
			else
			{
				if(dut0.g_lfps_oobs_signbit == 1)
				{
					xil_printf("dut0.g_adc_data_eq_off: %dmV\r\n", dut0.g_adc_data_eq_off);
					xil_printf("dut0.g_adc_data_eq_on:  %dmV\r\n", dut0.g_adc_data_eq_on);

					dut0.g_lrd_pattern_substep = 0;
					return 1;
				}
				else
				{
					dut0.g_lrd_pattern_substep = 0x02;
				}
			}
		}
		break;
	}
	}

	return 0;
}
