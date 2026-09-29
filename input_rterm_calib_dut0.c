#include "platform.h"

int8_t input_rterm_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr)
{
	uint8_t	rterm_signbit;
	uint8_t rterm_offset = 12;

	uint8_t reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);
	mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | rterm_offset);
	delay_ms(1);

	uint16_t adc_data = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
	uint16_t rterm = (adc_data * EXT_RP) / (1100 - adc_data);
	xil_printf("adc_data: %dmV, rterm default: %d\r\n", adc_data, rterm);

	if(rterm == TARGET_RTERM)	return rterm_offset;

	rterm_signbit = (rterm > TARGET_RTERM) ? 1 : 0;

	for(uint8_t i=0;i<16;i++)
	{
		if(rterm_signbit == 1)
		{
			if(rterm_offset < 15)	rterm_offset++;
			else					return -1;
		}
		else
		{
			if(rterm_offset > 0)	rterm_offset--;
			else					return -1;
		}

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | rterm_offset);
		delay_ms(1);

		adc_data = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch, AVG_NUM);
		rterm = (adc_data * EXT_RP) / (1100 - adc_data);
		xil_printf("rterm_offset: %02d, adc_data: %dmV, rterm: %d\r\n", rterm_offset, adc_data, rterm);

		if(rterm < TARGET_RTERM)
		{
			if(rterm_signbit == 1)	return rterm_offset;
		}
		else
		{
			if(rterm_signbit == 0)	return rterm_offset + 1;
		}
	}

	return -1;
}
