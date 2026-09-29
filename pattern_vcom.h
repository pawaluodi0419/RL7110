#ifndef _PATTERN_VCOM_DUT_H_
#define _PATTERN_VCOM_DUT_H_

#define	VCOM_H	800
#define	VCOM_L	750

int8_t pattern_vcom_dut0();

int8_t output_vcom_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch1, uint8_t adc_ch2, uint16_t reg_addr, uint8_t shift_bit);

#endif
