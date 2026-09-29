#ifndef	DAC_MAX5825_DRV_H_
#define DAC_MAX5825_DRV_H_

#define	DAC_MAX5825_ADDR1	0x10
#define	DAC_MAX5825_ADDR2	0x1C

//IIC_1_ADDR
#define	CC1_LV_DAC1_CH			0
#define	CC2_LV_DAC1_CH			1
#define	VMON_LV_DAC1_CH			2
#define	GPIO0_LV_DAC1_CH		3
#define	VCON_LV_DAC1_CH			4
#define	VREF1_LV_DAC1_CH		5
#define	VREF2_LV_DAC1_CH		6
#define	NC1_LV_DAC1_CH			7

//IIC_1_ADDR
#define	DP_LV_DAC2_CH			0
#define	DM_LV_DAC2_CH			1
#define	eDP_LV_DAC2_CH			2
#define	eDM_LV_DAC2_CH			3
#define	GPIO1_LV_DAC2_CH		4
#define	GPIO2_LV_DAC2_CH		5
#define	NC1_LV_DAC2_CH			6
#define	NC2_LV_DAC2_CH			7

void i2c_max5825_output(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_num, uint32_t ch_val);

#endif
