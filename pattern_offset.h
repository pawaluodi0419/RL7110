#ifndef _PATTERN_OFFSET_H_
#define _PATTERN_OFFSET_H_

#define	PN_OFFSET_MIN	4

int8_t pattern_offset_dut0();
int8_t pattern_offset_2_dut0();

int16_t output_offset_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch1, uint8_t adc_ch2, uint16_t reg_addr);

#endif
