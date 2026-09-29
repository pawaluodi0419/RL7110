#include "platform.h"

int8_t dc_gain_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr, uint16_t target_vol)
{
	uint8_t dc_signbit;
	uint8_t dc_offset = 12;
	uint32_t adc_data, prev_adc_data;

	mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, dc_offset);
	delay_ms(1);

	adc_data = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
	xil_printf("dc_offset: %02d, input_data: %dmV, adc_data: %dmV\r\n", dc_offset, target_vol, adc_data);

	if(adc_data == target_vol)	return dc_offset;

	dc_signbit = (adc_data > target_vol) ? 1 : 0;

	for(uint8_t i=0;i<16;i++)
	{
		prev_adc_data = adc_data;

		if(dc_signbit == 0)
		{
			if(dc_offset < 17)	dc_offset++;
			else				return -1;
		}
		else
		{
			if(dc_offset > 0)	dc_offset--;
			else				return -1;
		}

		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, dc_offset);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
		xil_printf("input_data: %dmV, dc_offset: %02d, adc_data: %dmV\r\n", target_vol, dc_offset, adc_data);

		if(dc_signbit == 0)
		{
			if(adc_data >= target_vol)
			{
				if(adc_data < (target_vol + DC_DIFF))
				{
					return dc_offset;
				}
				else
				{
					xil_printf("dut0 DC K not in spec!\r\n");
					return -1;
				}
			}
		}
		else
		{
			if(adc_data < target_vol)
			{
				dc_offset++;
				mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, dc_offset);

				if(prev_adc_data < (target_vol + DC_DIFF))
				{
					return dc_offset;
				}
				else
				{
					xil_printf("dut0 DC K not in spec!\r\n");
					return -1;
				}
			}
		}
	}

	return -1;
}
