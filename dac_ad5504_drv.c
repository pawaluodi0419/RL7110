#include "platform.h"

void i2c_to_spi_ad5504_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_num, uint16_t ch_val)
{
	uint8_t i2c_control_reg_databuf[3] = {0};
	uint8_t i2c_dac_reg_databuf[3] = {0};

	uint32_t ch_val_temp;

	i2c_control_reg_databuf[0] = 0x01;
	i2c_control_reg_databuf[1] = 0x70;
	i2c_control_reg_databuf[2] = (1 << (ch_num + 1));

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, i2c_control_reg_databuf, 3, XIIC_STOP);
	delay_us(10);

	ch_val_temp = (ch_val * 4095) / 30000;

	i2c_dac_reg_databuf[0] = 0x01;
	i2c_dac_reg_databuf[1] = ((ch_num << 4) | (ch_val_temp >> 8));
	i2c_dac_reg_databuf[2] = (ch_val_temp & 0xFF);

	XIic_Send(i2c_fpga_baseaddr, slave_addr, i2c_dac_reg_databuf, 3, XIIC_STOP);
	delay_us(10);
}
