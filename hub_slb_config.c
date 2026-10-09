#include "platform.h"

int8_t hub_slb_initial()
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	int8_t ret;

    /* HUB SLB configuration */
	tx_buf[0] = 0x00;	//H
	tx_buf[1] = 0x00;	//L
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x30, 0x05, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_data = 0x21;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_MEM, 0x05, 0xE3, tx_data, 0);	//DSP3
	if(ret != 0x00)	return -2;

	tx_buf[0] = 0x21;
	tx_buf[1] = 0xE1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x23, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -3;

	tx_buf[0] = 0x23;
	tx_buf[1] = 0xE1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x23, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -4;

	tx_buf[0] = 0x33;
	tx_buf[1] = 0xE1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x23, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -5;

	tx_buf[0] = 0x12;
	tx_buf[1] = 0xE1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x23, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -6;

	tx_data = 0x00;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_MEM, 0x36, 0xFA, tx_data, 0);	//DSP3
	if(ret != 0x00)	return -7;

	tx_buf[0] = 0x41;
	tx_buf[1] = 0x2C;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x19, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -8;

	tx_buf[0] = 0x41;
	tx_buf[1] = 0xF4;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x99, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -9;

	tx_buf[0] = 0x09;
	tx_buf[1] = 0x00;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x1A, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -9;

	tx_buf[0] = 0x09;
	tx_buf[1] = 0x00;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x9A, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -9;

	tx_buf[0] = 0x01;
	tx_buf[1] = 0x08;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x1B, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -9;

	tx_buf[0] = 0x02;
	tx_buf[1] = 0x08;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x9B, 0x04, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -9;

	return 0;
}

int8_t hub_slb_dsp3_start()
{
	uint8_t tx_buf[10];
	int8_t ret;

	//start error count
	tx_buf[0] = 0x10;
	tx_buf[1] = 0xA9;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_buf[0] = 0x10;
	tx_buf[1] = 0xAD;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_buf[0] = 0x00;
	tx_buf[1] = 0xAD;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	return 0;
}

int8_t hub_slb_dsp3_stop()
{
	uint8_t tx_buf[10];
	int8_t ret;

	//stop error count
	tx_buf[0] = 0x10;
	tx_buf[1] = 0xAD;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_buf[0] = 0x10;
	tx_buf[1] = 0xA9;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_buf[0] = 0x10;
	tx_buf[1] = 0xA1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	return 0;
}

int8_t hub_slb_dsp3_read_err_cnt(uint32_t *err_cnt)
{
	uint8_t rx_buf[10];
	int8_t ret;

	uint16_t err_cnt_low;
	uint16_t err_cnt_high;

	ret = hub_smbus_read(DUT0_IIC_0_ADDR, HUB2_ADDR, READ_PHY, 0x24, 0x06, 2, rx_buf);
	if(ret != 0x00)	return -1;
	err_cnt_high  = (rx_buf[1] << 8) | rx_buf[2];
//	xil_printf("err_cnt_high: rx_buf[1] = %02x, rx_buf[2] = %02x\r\n", rx_buf[1], rx_buf[2]);

	ret = hub_smbus_read(DUT0_IIC_0_ADDR, HUB2_ADDR, READ_PHY, 0x25, 0x06, 2, rx_buf);
	if(ret != 0x00)	return -1;
	err_cnt_low  = (rx_buf[1] << 8) | rx_buf[2];
//	xil_printf("err_cnt_low:  rx_buf[1] = %02x, rx_buf[2] = %02x\r\n", rx_buf[1], rx_buf[2]);

	*err_cnt = (err_cnt_high << 16) | err_cnt_low;

	return 0;
}

int8_t hub_slb_dsp3_reset_err_cnt()
{
	uint8_t tx_buf[10];
	int8_t ret;

	tx_buf[0] = 0x18;
	tx_buf[1] = 0xA1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	tx_buf[0] = 0x10;
	tx_buf[1] = 0xA1;
	ret = hub_smbus_write(DUT0_IIC_0_ADDR, HUB2_ADDR, WRITE_PHY, 0x20, 0x06, tx_buf[0], tx_buf[1]);
	if(ret != 0x00)	return -1;

	return 0;
}
