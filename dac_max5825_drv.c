#include "platform.h"

void i2c_max5825_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_num, uint32_t ch_val)
{
	uint8_t i2c_ref_databuf[3] = {0};
	uint8_t i2c_pow_databuf[3] = {0};
	uint8_t i2c_dacwrite_databuf[3] = {0};

	uint32_t ch_val_temp;

	i2c_ref_databuf[0] = 0x20;								//Command byte   -> REF mode: EXT
	i2c_ref_databuf[1] = 0;									//Data high byte -> Don't care
	i2c_ref_databuf[2] = 0;									//Data low byte  -> Don't care

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, i2c_ref_databuf, 3, XIIC_STOP);
	delay_us(10);

	i2c_pow_databuf[0] = 0x40;								//Command byte
	i2c_pow_databuf[1] = (1 << ch_num);						//Data high byte -> DAC channel selection
	i2c_pow_databuf[2] = 0;									//Data low byte  -> Power mode: normal

	XIic_Send(i2c_fpga_baseaddr, slave_addr, i2c_pow_databuf, 3, XIIC_STOP);
	delay_us(10);

	ch_val_temp = (ch_val * 4095) / 5000;

	i2c_dacwrite_databuf[0] = (0xB0 | ch_num);				//Command byte
	i2c_dacwrite_databuf[1] = (ch_val_temp >> 4);			//Data high byte -> code register data[11:4]
	i2c_dacwrite_databuf[2] = ((ch_val_temp & 0x0F) << 4);	//Data low byte  -> code register data[3:0]

	XIic_Send(i2c_fpga_baseaddr, slave_addr, i2c_dacwrite_databuf, 3, XIIC_STOP);
	delay_us(10);
}
