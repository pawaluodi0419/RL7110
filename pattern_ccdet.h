#ifndef	PATTERN_CCDET_H_
#define	PATTERN_CCDET_H_

#define VREF_MASK		0xF

#define ABS_DIFF(a, b)	((a) > (b) ? ((a) - (b)) : ((b) - (a)))

typedef struct{
	uint16_t dac_data;
	uint8_t vref_sel;
	uint16_t vref_reg;
	uint8_t vref_shift;
	uint8_t check_bit;
}ccdet_calib_config_t;

extern const ccdet_calib_config_t cc1_config_table[5];
extern const ccdet_calib_config_t cc2_config_table[5];

int8_t ccdet_calib_dut0(const ccdet_calib_config_t *table, uint8_t table_size, uint8_t cc_id);
int8_t pattern_ccdet_dut0();

uint16_t ccdet_meas_by_dac_dut0(uint8_t gear, const ccdet_calib_config_t *table, uint8_t cc_id);

#endif /* PATTERN_CCDET_H_ */
