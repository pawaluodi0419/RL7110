#include "platform.h"

void mdio_master_write(uint32_t mdio_baseaddr, uint16_t reg_addr, uint16_t reg_data)
{
	//write phy address+register address
    MDIO_WRITE_REG(mdio_baseaddr, 0, reg_addr);
    //write data
    MDIO_WRITE_REG(mdio_baseaddr, 4, reg_data);
	
    //bit1: 0->write, 1->read; bit0: 1->start
    MDIO_WRITE_REG(mdio_baseaddr, 32, 0x00);
    MDIO_WRITE_REG(mdio_baseaddr, 32, 0x01);

    while(MDIO_READ_REG(mdio_baseaddr, 60))
    {
//    	xil_printf("dut0_mdio_status = 0x%x\r\n", MDIO_READ_REG(mdio_baseaddr, 60));
    };

//    xil_printf("dut0_mdio_status = 0x%x\r\n\r\n", MDIO_READ_REG(mdio_baseaddr, 60));
}

uint8_t mdio_master_read(uint32_t mdio_baseaddr, uint16_t reg_addr)
{
	uint16_t read_data = 0;
	
	//write phy address+register address
	MDIO_WRITE_REG(mdio_baseaddr, 0, reg_addr);
	
	//bit1: 0->write, 1->read; bit0: 1->start
	MDIO_WRITE_REG(mdio_baseaddr, 32, 0x02);
	MDIO_WRITE_REG(mdio_baseaddr, 32, 0x03);

	while(MDIO_READ_REG(mdio_baseaddr, 60))
	{
//		xil_printf("dut0_mdio_status = 0x%x\r\n", MDIO_READ_REG(mdio_baseaddr, 60));
	};

//	xil_printf("dut0_mdio_status = 0x%x\r\n\r\n", MDIO_READ_REG(mdio_baseaddr, 60));
//	xil_printf("dut0_mdio_data = 0x%x\r\n\r\n", MDIO_READ_REG(mdio_baseaddr, 8));
	
	read_data = MDIO_READ_REG(mdio_baseaddr, 8);
	
	return read_data;
}
