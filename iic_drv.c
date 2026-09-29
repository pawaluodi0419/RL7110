#include "platform.h"

int8_t i2c_write_mem(uint8_t addr_l, uint8_t addr_h, uint8_t data)
{
	uint8_t write_databuf[10] = {0};
	unsigned byte_count;

	write_databuf[0] = OP_MEM;
	write_databuf[1] = addr_l;
	write_databuf[2] = addr_h;
	write_databuf[3] = data;
	write_databuf[4] = 0x80;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 5, XIIC_STOP);
	if(byte_count != 5)
	{
		xil_printf("Step1 Fail! Expected 5, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_write_phy(uint8_t addr_l, uint8_t addr_h, uint8_t data_l, uint8_t data_h)
{
	uint8_t write_databuf[10] = {0};
	unsigned byte_count;

	write_databuf[0] = OP_PHY;
	write_databuf[1] = addr_l;
	write_databuf[2] = addr_h;
	write_databuf[3] = data_l;
	write_databuf[4] = data_h;
	write_databuf[5] = 0x80;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 6, XIIC_STOP);
	if(byte_count != 6)
	{
		xil_printf("Step1 Fail! Expected 6, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_write_eu2(uint8_t reg_addr, uint8_t reg_data)
{
	uint8_t write_databuf[10] = {0};
	unsigned byte_count;

	write_databuf[0] = OP_eU2;
	write_databuf[1] = reg_addr;
	write_databuf[2] = reg_data;
	write_databuf[3] = 0x80;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 4, XIIC_STOP);
	if(byte_count != 4)
	{
		xil_printf("Step1 Fail! Expected 4, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_write_efuse(uint8_t efuse_addr, uint8_t *write_data)
{
	uint8_t write_databuf[150] = {0};
	unsigned byte_count;

	write_databuf[0] = OP_eFUSE_1;
	write_databuf[1] = OP_eFUSE_2;
	write_databuf[2] = OP_eFUSE_3;
	write_databuf[3] = OP_eFUSE_4;
	write_databuf[4] = efuse_addr;
	write_databuf[5] = 0x01;
	write_databuf[6] = write_data[efuse_addr];

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 7, XIIC_STOP);
	if(byte_count != 7)
	{
		xil_printf("Step1 Fail! Expected 7, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_read_mem(uint8_t addr_l, uint8_t addr_h, uint8_t *read_data)
{
	uint8_t write_databuf[10] = {0};
	uint8_t second_readcmd = SECOND_MEM_READCMD;
	unsigned byte_count;

	write_databuf[0] = OP_MEM;
	write_databuf[1] = addr_l;
	write_databuf[2] = addr_h;
	write_databuf[3] = 0x00;
	write_databuf[4] = 0x40;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 5, XIIC_STOP);
	if(byte_count != 5)
	{
		xil_printf("Step1 Fail! Expected 5, Sent %d\r\n", byte_count);
		return -1;
	}

	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, &second_readcmd, 1, XIIC_REPEATED_START);
	if(byte_count != 1)
	{
		xil_printf("Step2 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Recv(DUT0_IIC_2_ADDR, dut0.g_dut_addr, read_data, 1, XIIC_STOP);
	if(byte_count != 1)
	{
		xil_printf("Step3 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_read_phy(uint8_t addr_l, uint8_t addr_h, uint8_t *read_data)
{
	uint8_t write_databuf[10] = {0};
	uint8_t second_readcmd = SECOND_PHY_READCMD;
	unsigned byte_count;

	write_databuf[0] = OP_PHY;
	write_databuf[1] = addr_l;
	write_databuf[2] = addr_h;
	write_databuf[3] = 0x00;
	write_databuf[4] = 0x00;
	write_databuf[5] = 0x40;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 6, XIIC_STOP);
	if(byte_count != 6)
	{
		xil_printf("Step1 Fail! Expected 6, Sent %d\r\n", byte_count);
		return -1;
	}

	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, &second_readcmd, 1, XIIC_REPEATED_START);
	if(byte_count != 1)
	{
		xil_printf("Step2 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Recv(DUT0_IIC_2_ADDR, dut0.g_dut_addr, read_data, 2, XIIC_STOP);
	if(byte_count != 2)
	{
		xil_printf("Step3 Fail! Expected 2, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_read_eu2(uint8_t reg_addr, uint8_t *read_data)
{
	uint8_t write_databuf[10] = {0};
	uint8_t second_readcmd = SECOND_eU2_READCMD;
	unsigned byte_count;

	write_databuf[0] = OP_eU2;
	write_databuf[1] = reg_addr;
	write_databuf[2] = 0x00;
	write_databuf[3] = 0x40;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 4, XIIC_STOP);
	if(byte_count != 4)
	{
		xil_printf("Step1 Fail! Expected 4, Sent %d\r\n", byte_count);
		return -1;
	}

	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, &second_readcmd, 1, XIIC_REPEATED_START);
	if(byte_count != 1)
	{
		xil_printf("Step2 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Recv(DUT0_IIC_2_ADDR, dut0.g_dut_addr, read_data, 1, XIIC_STOP);
	if(byte_count != 1)
	{
		xil_printf("Step3 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

int8_t i2c_read_efuse(uint8_t efuse_addr, uint8_t *read_data)
{
	uint8_t write_databuf[10] = {0};
	uint8_t second_readcmd = SECOND_eFUSE_READCMD;
	unsigned byte_count;

	write_databuf[0] = FIRST_eFUSE_READCMD;
	write_databuf[1] = efuse_addr;
	write_databuf[2] = 0x00;
	write_databuf[3] = 0x40;

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, write_databuf, 4, XIIC_STOP);
	if(byte_count != 4)
	{
		xil_printf("Step1 Fail! Expected 4, Sent %d\r\n", byte_count);
		return -1;
	}

	byte_count = XIic_Send(DUT0_IIC_2_ADDR, dut0.g_dut_addr, &second_readcmd, 1, XIIC_REPEATED_START);
	if(byte_count != 1)
	{
		xil_printf("Step2 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	XIic_DynInit(DUT0_IIC_2_ADDR);
	delay_us(10);
	byte_count = XIic_Recv(DUT0_IIC_2_ADDR, dut0.g_dut_addr, read_data, 1, XIIC_STOP);
	if(byte_count != 1)
	{
		xil_printf("Step3 Fail! Expected 1, Sent %d\r\n", byte_count);
		return -1;
	}

	return 0;
}

uint8_t smbus_get_pinstatus(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr)
{
	uint8_t pinstatus_databuf[1] = {0};

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Recv(i2c_fpga_baseaddr, slave_addr, pinstatus_databuf, 1, XIIC_STOP);

	return pinstatus_databuf[0];
}

int8_t hub_smbus_write(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t cmd_type, uint8_t addr_l, uint8_t addr_h, uint8_t data_l, uint8_t data_h)
{
	uint8_t i;
	uint8_t pinstatus;
	uint8_t write_databuf[10] = {0};
	uint8_t send_len = 0;

	if(cmd_type == WRITE_PHY)
	{
		write_databuf[0] = cmd_type;
		write_databuf[1] = 0x05;
		write_databuf[2] = addr_l;
		write_databuf[3] = addr_h;
		write_databuf[4] = 0x02;
		write_databuf[5] = data_l;
		write_databuf[6] = data_h;

		send_len = 7;
	}
	else if(cmd_type == WRITE_MEM)
	{
		write_databuf[0] = cmd_type;
		write_databuf[1] = 0x04;
		write_databuf[2] = addr_l;
		write_databuf[3] = addr_h;
		write_databuf[4] = 0x01;
		write_databuf[5] = data_l;

		send_len = 6;
	}
	else
	{
		return -1;
	}

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, write_databuf, send_len, XIIC_STOP);
	delay_us(10);

	for(i=0;i<100;i++)
	{
		pinstatus = smbus_get_pinstatus(i2c_fpga_baseaddr, slave_addr);

		if(pinstatus)
		{
			return 0;
		}
		delay_ms(1);
	}

	return -1;
}

int8_t hub_smbus_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t cmd_type, uint8_t addr_l, uint8_t addr_h, uint8_t data_cnt, uint8_t *read_data)
{
	uint8_t i;
	uint8_t pinstatus;
	uint8_t write_databuf[10] = {0};
	uint8_t write_second_databuf[1] = {0};

	if(cmd_type == READ_PHY)
	{
		write_databuf[0] = cmd_type;
		write_databuf[1] = 0x03;
		write_databuf[2] = addr_l;
		write_databuf[3] = addr_h;
		write_databuf[4] = 0x02;
	}
	else if(cmd_type == READ_MEM)
	{
		write_databuf[0] = cmd_type;
		write_databuf[1] = 0x03;
		write_databuf[2] = addr_l;
		write_databuf[3] = addr_h;
		write_databuf[4] = 0x01;
	}

	write_second_databuf[0] = 0x80;

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, write_databuf, 5, XIIC_STOP);
	delay_us(10);

	for(i=0;i<100;i++)
	{
		pinstatus = smbus_get_pinstatus(i2c_fpga_baseaddr, slave_addr);

		if(pinstatus)
		{
			XIic_DynInit(i2c_fpga_baseaddr);
			delay_us(10);
			XIic_Send(i2c_fpga_baseaddr, slave_addr, write_second_databuf, 1, XIIC_REPEATED_START);
			delay_us(10);

		    XIic_DynInit(i2c_fpga_baseaddr);
		    delay_us(10);
			XIic_Recv(i2c_fpga_baseaddr, slave_addr, read_data, data_cnt+1, XIIC_STOP);

			return 0;
		}
		delay_ms(1);
	}

	return -1;
}

int8_t pd_smbus_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t port_index, uint8_t data_cnt, uint8_t *read_data)
{
	uint8_t i;
	uint8_t pinstatus;
	uint8_t write_databuf[10] = {0};
	uint8_t write_second_databuf[1] = {0};

	write_databuf[0] = 0x5B;
	write_databuf[1] = 0x01;
	write_databuf[2] = port_index;

	write_second_databuf[0] = 0x80;

	XIic_DynInit(i2c_fpga_baseaddr);
	delay_us(10);
	XIic_Send(i2c_fpga_baseaddr, slave_addr, write_databuf, 3, XIIC_STOP);
	delay_us(10);

	for(i=0;i<1000;i++)
	{
		pinstatus = smbus_get_pinstatus(i2c_fpga_baseaddr, slave_addr);

		if(pinstatus)
		{
			XIic_DynInit(i2c_fpga_baseaddr);
			delay_us(10);
			XIic_Send(i2c_fpga_baseaddr, slave_addr, write_second_databuf, 1, XIIC_REPEATED_START);
			delay_us(10);

		    XIic_DynInit(i2c_fpga_baseaddr);
		    delay_us(10);
			XIic_Recv(i2c_fpga_baseaddr, slave_addr, read_data, data_cnt+3, XIIC_STOP);

			return 0;
		}
		delay_ms(1);
	}

	return -1;
}
