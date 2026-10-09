#ifndef _PATTERN_SLB_TEST_H_
#define _PATTERN_SLB_TEST_H_

#define	SLB_ERR_CNT	0

int8_t hub_slb_initial();

int8_t hub_slb_dsp3_start();
int8_t hub_slb_dsp3_stop();
int8_t hub_slb_dsp3_read_err_cnt(uint32_t *err_cnt);
int8_t hub_slb_dsp3_reset_err_cnt();

int8_t pattern_slb_test_dut0();

#endif
