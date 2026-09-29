#ifndef IIC_DRV_H_
#define IIC_DRV_H_

//IIC_0_ADDR
#define	RL6613_ADDR	0x62
#define	HUB1_ADDR	0x6A
#define	HUB2_ADDR	0x6B

#define OP_MEM		0xC0
#define OP_PHY		0xC4
#define OP_eU2		0xC9
#define OP_eFUSE_1	0xD0
#define OP_eFUSE_2	0xDA
#define OP_eFUSE_3	0x0B
#define OP_eFUSE_4	0xAA

#define	SECOND_MEM_READCMD		0xC2
#define	SECOND_PHY_READCMD		0xC6
#define	SECOND_eU2_READCMD		0xCA
#define	FIRST_eFUSE_READCMD		0xCC
#define	SECOND_eFUSE_READCMD	0xCD

#define WRITE_MEM	0x18
#define WRITE_PHY	0x1B
#define READ_MEM	0x38
#define READ_PHY	0x3B

int8_t i2c_write_mem(uint8_t addr_l, uint8_t addr_h, uint8_t data);
int8_t i2c_write_phy(uint8_t addr_l, uint8_t addr_h, uint8_t data_l, uint8_t data_h);
int8_t i2c_write_eu2(uint8_t reg_addr, uint8_t reg_data);
int8_t i2c_write_efuse(uint8_t efuse_addr, uint8_t *write_data);

int8_t i2c_read_mem(uint8_t addr_l, uint8_t addr_h, uint8_t *read_data);
int8_t i2c_read_phy(uint8_t addr_l, uint8_t addr_h, uint8_t *read_data);
int8_t i2c_read_eu2(uint8_t reg_addr, uint8_t *read_data);
int8_t i2c_read_efuse(uint8_t efuse_addr, uint8_t *read_data);

uint8_t smbus_get_pinstatus(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr);

int8_t hub_smbus_write(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t cmd_type, uint8_t addr_l, uint8_t addr_h, uint8_t data_l, uint8_t data_h);
int8_t hub_smbus_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t cmd_type, uint8_t addr_l, uint8_t addr_h, uint8_t data_cnt, uint8_t *read_data);
int8_t pd_smbus_read(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t port_index, uint8_t data_cnt, uint8_t *read_data);

#endif
