#ifndef	PATTERN_RC_CLOCK_H_
#define	PATTERN_RC_CLOCK_H_

#define	RC16M_TARGET	16000
#define	RC50M_TARGET	50000

#define	RC16M_MIN		16000.0 * 0.98
#define	RC16M_MAX		16000.0 * 1.02
#define	RC50M_MIN		50000.0 * 0.95
#define	RC50M_MAX		50000.0 * 1.05

int8_t pattern_rc_clock_dut0();

#endif /* PATTERN_RC_CLOCK_H_ */
