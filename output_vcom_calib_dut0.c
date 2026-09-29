#include "platform.h"

int8_t output_vcom_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch1, uint8_t adc_ch2, uint16_t reg_addr, uint8_t shift_bit)
{
	uint8_t vcom_offset = 10;
	uint32_t adc_data_p, adc_data_n;

	uint8_t reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);

	if(shift_bit)
	{
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0x0F) | (vcom_offset << 4));
	}
	else
	{
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | vcom_offset);
	}
	delay_ms(1);

	for(uint8_t i=0;i<16;i++)
	{
		adc_data_p = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch1, AVG_NUM);
		adc_data_n = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch2, AVG_NUM);

//		xil_printf("vcom_offset = %d\r\nadc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", vcom_offset, adc_data_p, adc_data_n);

		if((adc_data_p >= VCOM_L) && (adc_data_p <= VCOM_H) &&
		   (adc_data_n >= VCOM_L) && (adc_data_n <= VCOM_H))
		{
			xil_printf("adc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", adc_data_p, adc_data_n);

			return vcom_offset;
		}

		if((adc_data_p >= VCOM_H) || (adc_data_n >= VCOM_H))
		{
			if((vcom_offset >= 10) && (vcom_offset < 15))
			{
				vcom_offset++;
			}
			else
			{
				return -1;
			}
		}
		else if((adc_data_p <= VCOM_L) || (adc_data_n <= VCOM_L))
		{
			if((vcom_offset > 0) && (vcom_offset <= 10))
			{
				vcom_offset--;
			}
			else
			{
				return -1;
			}
		}

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);

		if(shift_bit)
		{
			mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0x0F) | (vcom_offset << 4));
		}
		else
		{
			mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | vcom_offset);
		}
		delay_ms(1);
	}

	return -1;
}
