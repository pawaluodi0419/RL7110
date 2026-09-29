#ifndef	MDIO_DRV_H_
#define	MDIO_DRV_H_

void mdio_master_write(uint32_t mdio_baseaddr, uint16_t reg_addr, uint16_t reg_data);
uint8_t mdio_master_read(uint32_t mdio_baseaddr, uint16_t reg_addr);

#endif /* MDIO_DRV_H_ */
