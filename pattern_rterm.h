#ifndef _PATTERN_RTERM_H_
#define _PATTERN_RTERM_H_

#define	TARGET_RTERM	450

int8_t pattern_rterm_dut0();

int8_t input_rterm_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr);

#endif
