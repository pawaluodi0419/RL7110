#ifndef _PATTERN_DC_GAIN_H_
#define _PATTERN_DC_GAIN_H_

#define	DC_DIFF		50

int8_t pattern_dc_gain_dut0();

int8_t dc_gain_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr, uint16_t target_vol);

#endif
