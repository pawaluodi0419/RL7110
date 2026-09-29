#include "platform.h"

void i2c_pca9506_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num, uint8_t ch_num, uint8_t ch_val)
{
	uint8_t io_read_data = 0;
	uint8_t io_ioc_databuf[2] = {0};
	uint8_t io_op_databuf[2] = {0};

	uint8_t ch_mask = ~(1 << ch_num);

	io_ioc_databuf[0] = (0x03 << 3) | bank_num;
	io_ioc_databuf[1] = 0x00;

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, io_ioc_databuf, 2, XIIC_STOP);
//	xil_printf("io_ioc_databuf = %x, %x\r\n", io_ioc_databuf[0], io_ioc_databuf[1]);

	io_read_data = i2c_pca9506_read(i2c_fpga_baseaddr, slave_addr, bank_num);

	io_op_databuf[0] = (0x01 << 3) | bank_num;
	io_op_databuf[1] = (io_read_data & ch_mask) | (ch_val << ch_num);

	XIic_Send(i2c_fpga_baseaddr, slave_addr, io_op_databuf, 2, XIIC_STOP);
//	xil_printf("io_op_databuf = %x, %x\r\n", io_op_databuf[0], io_op_databuf[1]);
}

void i2c_pca9506_output_writebank(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num, uint8_t ch_val)
{
	uint8_t io_ioc_databuf[2] = {0};
	uint8_t io_op_databuf[2] = {0};

	io_ioc_databuf[0] = (0x03 << 3) | bank_num;
	io_ioc_databuf[1] = 0x00;

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, io_ioc_databuf, 2, XIIC_STOP);
//	xil_printf("io_ioc_databuf =%x,%x\r\n",io_ioc_databuf[0],io_ioc_databuf[1]);

	io_op_databuf[0] = (0x01 << 3) | bank_num;
	io_op_databuf[1] = ch_val;

	XIic_Send(i2c_fpga_baseaddr, slave_addr, io_op_databuf, 2, XIIC_STOP);
//	xil_printf("io_op_databuf =%x,%x\r\n",io_op_databuf[0],io_op_databuf[1]);
}

uint8_t i2c_pca9506_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t bank_num)
{
	uint8_t io_read_databuf[2] = {0};
	uint8_t io_ip_databuf[2] = {0};

	io_ip_databuf[0] = (0x00 << 3) | bank_num;

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, io_ip_databuf, 1, XIIC_REPEATED_START);

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Recv(i2c_fpga_baseaddr, slave_addr, io_read_databuf, 1, XIIC_STOP);

	return io_read_databuf[0];
}
