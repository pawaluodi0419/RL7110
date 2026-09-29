#ifndef	PATTERN_RP_H_
#define	PATTERN_RP_H_

#define RP_80UA_REF		(80.0 * 1.00 * 5.10)
#define RP_80UA_MIN		(80.0 * 0.90 * 5.10)
#define RP_80UA_MAX		(80.0 * 1.10 * 5.10)

#define RP_180UA_REF	(180.0 * 1.00 * 5.10)
#define RP_180UA_MIN	(180.0 * 0.95 * 5.10)
#define RP_180UA_MAX	(180.0 * 1.05 * 5.10)

#define RP_330UA_REF	(330.0 * 1.00 * 5.10)
#define RP_330UA_MIN	(330.0 * 0.95 * 5.10)
#define RP_330UA_MAX	(330.0 * 1.05 * 5.10)

typedef struct{
    uint8_t phy_addr;
    uint8_t field_shift;
    uint8_t field_mask;
    uint8_t init_gear;
    uint8_t adc_ch;
    uint8_t cc_id;
    uint8_t cur_type;   //0: 80uA, 1: 180uA, 2: 330uA
    uint16_t ref;
    uint16_t min;
    uint16_t max;
}rp_calib_config_t;

int8_t rp_calib_dut0(const rp_calib_config_t *cfg);
int8_t pattern_rp_dut0();

#endif /* PATTERN_RP_H_ */
