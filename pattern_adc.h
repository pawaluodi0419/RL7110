#ifndef	PATTERN_ADC_H_
#define	PATTERN_ADC_H_

#define ADC_5V_DIV_REF	(5000 * 2 / 7)
#define ADC_1V_DIV_REF	(1000 * 2 / 7)
#define ADC_0V5_REF		500

#define ADC_A_MAX		8500
#define ADC_A_MIN		7500

#define ADC_B_MAX		32
#define ADC_B_MIN		32

uint16_t adc_dout_read(uint8_t adc_ch);
int8_t pattern_adc_dut0();

#endif /* PATTERN_ADC_H_ */
