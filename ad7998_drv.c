#include "platform.h"

//use mode 2(command mode, don't need to toggle CONVST pin)
uint16_t ad7998_readcvsdata(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_val, uint8_t avg_points)
{
	uint8_t i;
	uint8_t write_databuf[5] = {0};
	uint8_t read_databuf[5] = {0};
	uint16_t adc_cvsdata = 0;

	write_databuf[0] = (0x80 | ((ch_val - 1) << 4));	//Address Pointer Byte

	//read and discard the first conversion data
	XIic_Send(i2c_fpga_baseaddr, slave_addr, write_databuf, 1, XIIC_REPEATED_START);
	delay_us(10);

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Recv(i2c_fpga_baseaddr, slave_addr, read_databuf, 2, XIIC_STOP);

	for(i=0;i<avg_points;i++)
	{
		XIic_Send(i2c_fpga_baseaddr, slave_addr, write_databuf, 1, XIIC_REPEATED_START);
		delay_us(10);

		XIic_DynInit(i2c_fpga_baseaddr);
		delay_us(10);
		XIic_Recv(i2c_fpga_baseaddr, slave_addr, read_databuf, 2, XIIC_STOP);
//		xil_printf("read_databuf[0] = %x, read_databuf[1] = %x\r\n", read_databuf[0], read_databuf[1]);

		adc_cvsdata += (((read_databuf[0] & 0x0f) << 8) | read_databuf[1]);
		delay_us(10);
	}

	return adc_cvsdata / avg_points;
}
