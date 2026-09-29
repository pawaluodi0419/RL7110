#ifndef	PATTERN_EU2_TEST_H_
#define	PATTERN_EU2_TEST_H_

#define	RC6M_MIN			6250.0 * 0.95
#define	RC6M_MAX			6250.0 * 1.05

#define	FS_K_U2_DP_MAX		100
#define	FS_K_U2_DM_MIN		3300.0 * 0.95
#define	FS_K_U2_DM_MAX		3300.0 * 1.05

#define	FS_J_U2_DP_MIN		3300.0 * 0.95
#define	FS_J_U2_DP_MAX		3300.0 * 1.05
#define	FS_J_U2_DM_MAX		100

#define	FS_K_eU2_eDP_MAX	100
#define	FS_K_eU2_eDM_MIN	1200.0 * 0.90
#define	FS_K_eU2_eDM_MAX	1200.0 * 1.10

#define	FS_J_eU2_eDP_MIN	1200.0 * 0.90
#define	FS_J_eU2_eDP_MAX	1200.0 * 1.10
#define	FS_J_eU2_eDM_MAX	100

#define	HS_K_U2_DIFF_MIN	420
#define	HS_K_U2_DIFF_MAX	480

#define	HS_K_eU2_DIFF_MIN	180
#define	HS_K_eU2_DIFF_MAX	240

#define	HS_J_U2_DIFF_MIN	420
#define	HS_J_U2_DIFF_REF	450
#define	HS_J_U2_DIFF_MAX	480

#define	HS_J_eU2_DIFF_MIN	180
#define	HS_J_eU2_DIFF_MAX	240

#define	CAL_OSC_TOLERANCE			2
#define	CAP_OSC_TOLERANCE			2
#define	U2_SQUELCH_VTH_TOLERANCE	2
#define	U2_DISC_VTH_TOLERANCE		2
#define	EU2_SQUELCH_VTH_TOLERANCE	2
#define	U2_Z0_TOLERANCE				2
#define	EU2_RSRC_TOLERANCE			2
#define	EU2_RRCV_TOLERANCE			2
#define	U2_SWING_TOLERANCE			2

int8_t pattern_eu2_test_dut0();

int8_t eu2_force_mode_enter(uint8_t test_mode, uint8_t test_en);
int8_t eu2_force_mode_reset();

#endif /* PATTERN_EU2_TEST_H_ */
