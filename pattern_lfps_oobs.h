#ifndef _PATTERN_LFPS_OOBS_H_
#define _PATTERN_LFPS_OOBS_H_

int8_t pattern_lfps_oobs_dut0();

int8_t lfps1_oobs_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr, uint8_t en_bit);
int8_t lfps2_oobs_calib_dut0(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t adc_ch, uint16_t reg_addr, uint8_t en_bit);

#endif
