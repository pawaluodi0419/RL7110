#include "platform.h"

int16_t output_offset_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch1, uint8_t adc_ch2, uint16_t reg_addr)
{
	uint8_t pn_signbit;
	uint16_t pn_offset;
	uint32_t adc_data_p, adc_data_n;
	uint32_t pn_diff;

	int16_t low, high, mid;

	if(dut0.g_pn_offset_test_cnt == 1)
	{
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, 128);
		delay_ms(1);
	}

	adc_data_p = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch1, AVG_NUM);
	adc_data_n = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch2, AVG_NUM);
	pn_diff = abs((int)adc_data_p - (int)adc_data_n);

//	xil_printf("adc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", adc_data_p, adc_data_n);

	if(pn_diff < PN_OFFSET_MIN)
	{
		pn_offset =  mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);

		xil_printf("adc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", adc_data_p, adc_data_n);
		xil_printf("PN_DIFF[%03d]: %dmV\r\n", pn_offset, pn_diff);

		return pn_offset;
	}

	if(dut0.g_pn_offset_test_cnt == 2)
	{
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, 128);
		delay_ms(1);

		adc_data_p = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch1, AVG_NUM);
		adc_data_n = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch2, AVG_NUM);
		pn_diff = abs((int)adc_data_p - (int)adc_data_n);
	}

	if(adc_data_p > adc_data_n)
	{
		pn_signbit = 1;
		low = 128;
		high = 255;
	}
	else
	{
		pn_signbit = 0;
		low = 0;
		high = 128;
	}

	while(low <= high)
	{
		mid = low + (high - low) / 2;
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, mid);
		delay_ms(1);

		adc_data_p = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch1, AVG_NUM);
		adc_data_n = ad7998_readcvsdata(i2c_fpga_baseaddr, slave_addr, adc_ch2, AVG_NUM);
		pn_diff = abs((int)adc_data_p - (int)adc_data_n);

//		xil_printf("adc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", adc_data_p, adc_data_n);
//		xil_printf("PN_DIFF[%03d]: %dmV\r\n", mid, pn_diff);

		if(adc_data_p > adc_data_n)
		{
			if(pn_signbit == 1)	low = mid + 1;
			else                high = mid - 1;
		}
		else
		{
			if(pn_signbit == 1) high = mid - 1;
			else                low = mid + 1;
		}
	}

	pn_offset =  mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);

	xil_printf("adc_data_p: %dmV\r\nadc_data_n: %dmV\r\n", adc_data_p, adc_data_n);
	xil_printf("PN_DIFF[%03d]: %dmV\r\n", pn_offset, pn_diff);

	return pn_offset;
}
