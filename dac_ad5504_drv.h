#ifndef	DAC_AD5504_DRV_H_
#define DAC_AD5504_DRV_H_

#define	Bridge_ADDR		0x28

//IIC_1_ADDR
#define	VMON_HV_DAC_CH			1
#define	CC1_HV_DAC_CH			2
#define	CC2_HV_DAC_CH			3
#define	NC1_HV_DAC_CH			4

void i2c_to_spi_ad5504_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_num, uint16_t ch_val);

#endif
