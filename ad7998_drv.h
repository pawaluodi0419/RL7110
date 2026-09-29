#ifndef	AD7998_DRV_H_
#define AD7998_DRV_H_

#define	ADC1_ADDR	0x20	//AS NC
#define	ADC2_ADDR	0x21	//AS GND
#define	ADC3_ADDR	0x22	//AS VDD

//IIC_0_ADDR
#define	SSTX1_P_TERM_ADC1_CH	1
#define	SSTX1_P_VCOM_ADC1_CH	2
#define	SSTX1_N_TERM_ADC1_CH	3
#define	SSTX1_N_VCOM_ADC1_CH	4
#define	SSTX2_P_TERM_ADC1_CH	5
#define	SSTX2_P_VCOM_ADC1_CH	6
#define	SSTX2_N_TERM_ADC1_CH	7
#define	SSTX2_N_VCOM_ADC1_CH	8

//IIC_0_ADDR
#define	SSRX1_N_TERM_ADC2_CH	1
#define	SSRX1_N_VCOM_ADC2_CH	2
#define	SSRX1_P_TERM_ADC2_CH	3
#define	SSRX1_P_VCOM_ADC2_CH	4
#define	SSRX2_N_TERM_ADC2_CH	5
#define	SSRX2_N_VCOM_ADC2_CH	6
#define	SSRX2_P_TERM_ADC2_CH	7
#define	SSRX2_P_VCOM_ADC2_CH	8

//IIC_0_ADDR
#define	SSTX_P_TERM_ADC3_CH		1
#define	SSTX_P_VCOM_ADC3_CH		2
#define	SSTX_N_TERM_ADC3_CH		3
#define	SSTX_N_VCOM_ADC3_CH		4
#define	SSRX_P_TERM_ADC3_CH		5
#define	SSRX_P_VCOM_ADC3_CH		6
#define	SSRX_N_TERM_ADC3_CH		7
#define	SSRX_N_VCOM_ADC3_CH		8

//IIC_1_ADDR
#define	SSTX1_DC_GAIN_ADC1_CH	1
#define	DP_ADC1_CH				2
#define	SSTX2_DC_GAIN_ADC1_CH	3
#define	DM_ADC1_CH				4
#define	SSRX_DC_GAIN_ADC1_CH	5
#define	eDP_ADC1_CH				6
#define	NC1_ADC1_CH				7
#define	eDM_ADC1_CH				8

//IIC_1_ADDR
#define	POWER_3V3_ADC2_CH		1
#define	CC1_ADC2_CH				2
#define	POWER_1V8_ADC2_CH		3
#define	CC2_ADC2_CH				4
#define	VMON_RES_ADC2_CH		5
#define	SCP_CC1_ADC2_CH			6
#define	NC1_ADC2_CH				7
#define	SCP_CC2_ADC2_CH			8

#define	AVG_NUM		10

uint16_t ad7998_readcvsdata(uint32_t i2c_fpga_baseaddr, uint8_t slave_addr, uint8_t ch_val, uint8_t avg_points);

#endif
