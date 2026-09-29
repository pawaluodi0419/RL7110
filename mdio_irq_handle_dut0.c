#include "platform.h"

void mdio_dut0_intrq_handle()
{
	if((MDIO_READ_REG(DUT0_MDIO_BASEADDR, 60))>>1)
	{
		xil_printf("dut0_mdio_data = 0x%x\r\n\r\n", MDIO_READ_REG(DUT0_MDIO_BASEADDR, 8));

		//clear mdio int
		MDIO_WRITE_REG(DUT0_MDIO_BASEADDR, 56, 0);
		MDIO_WRITE_REG(DUT0_MDIO_BASEADDR, 56, 1);
	}
}
