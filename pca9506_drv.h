#ifndef PCA9506_DRV_H_
#define PCA9506_DRV_H_

#define	PCA9506_ADDR	0x26

void i2c_pca9506_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num, uint8_t ch_num, uint8_t ch_val);
void i2c_pca9506_output_writebank(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num, uint8_t ch_val);
uint8_t i2c_pca9506_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num);

#endif
