#ifndef	PATTERN_EFUSE_H_
#define	PATTERN_EFUSE_H_

#define	RTERM_TOLERANCE				2
#define	OFFSET_TOLERANCE			10
#define	VCOM_TOLERANCE				2
#define	DC_TOLERANCE				2
#define	LFPS_OOBS_TOLERANCE			1
#define	SIG_OOBS_TOLERANCE			2

#define	RC16M_COARSE_TOLERANCE		0
#define	RC16M_FINE_TOLERANCE		2
#define	RC50M_TOLERANCE				1
#define	ADC_A_TOLERANCE				100
#define	ADC_B_TOLERANCE				10
#define	VMON_RES_TOLERANCE			2
#define	RP_TOLERANCE				1
#define	CCDET_TOLERANCE				1
#define	VCON_SW_OTP_TOLERANCE		1
#define	VCON_SW_OVP_TOLERANCE		1
#define	VCON_SW_UVLO_TOLERANCE		1
#define	VCON_SW_OCP_TOLERANCE		1
#define	VCON_SW_SCP_TOLERANCE		1

#define	LV0_DC_OFFSET	0
#define	LV1_DC_OFFSET	0
#define	LV2_DC_OFFSET	0
#define	LV3_DC_OFFSET	0
#define	LV4_DC_OFFSET	3
#define	LV5_DC_OFFSET	3
#define	LV6_DC_OFFSET	12
#define	LV7_DC_OFFSET	12
#define	LV8_DC_OFFSET	13
#define	LV9_DC_OFFSET	13

#define	LV0_R1			3
#define	LV1_R1			3
#define	LV2_R1			3
#define	LV3_R1			3
#define	LV4_R1			1
#define	LV5_R1			1
#define	LV6_R1			3
#define	LV7_R1			3
#define	LV8_R1			3
#define	LV9_R1			3

#define	LV0_R2			0
#define	LV1_R2			0
#define	LV2_R2			0
#define	LV3_R2			0
#define	LV4_R2			0
#define	LV5_R2			0
#define	LV6_R2			2
#define	LV7_R2			2
#define	LV8_R2			2
#define	LV9_R2			2

#define	LV0_R3			0
#define	LV1_R3			0
#define	LV2_R3			0
#define	LV3_R3			0
#define	LV4_R3			0
#define	LV5_R3			0
#define	LV6_R3			0
#define	LV7_R3			0
#define	LV8_R3			1
#define	LV9_R3			1

#define	LV0_C1			1
#define	LV1_C1			2
#define	LV2_C1			4
#define	LV3_C1			4
#define	LV4_C1			3
#define	LV5_C1			5
#define	LV6_C1			3
#define	LV7_C1			5
#define	LV8_C1			2
#define	LV9_C1			4

#define	LV0_C2			2
#define	LV1_C2			4
#define	LV2_C2			6
#define	LV3_C2			6
#define	LV4_C2			4
#define	LV5_C2			7
#define	LV6_C2			5
#define	LV7_C2			7
#define	LV8_C2			4
#define	LV9_C2			6

#define	LV0_BOOST123	0
#define	LV1_BOOST123	0
#define	LV2_BOOST123	0
#define	LV3_BOOST123	1
#define	LV4_BOOST123	1
#define	LV5_BOOST123	1
#define	LV6_BOOST123	1
#define	LV7_BOOST123	1
#define	LV8_BOOST123	1
#define	LV9_BOOST123	1

#define	LV0_BOOST_TX	0
#define	LV1_BOOST_TX	0
#define	LV2_BOOST_TX	0
#define	LV3_BOOST_TX	1
#define	LV4_BOOST_TX	1
#define	LV5_BOOST_TX	1
#define	LV6_BOOST_TX	1
#define	LV7_BOOST_TX	1
#define	LV8_BOOST_TX	1
#define	LV9_BOOST_TX	1

#define	LV0_ENHANCE		1
#define	LV1_ENHANCE		1
#define	LV2_ENHANCE		1
#define	LV3_ENHANCE		1
#define	LV4_ENHANCE		1
#define	LV5_ENHANCE		1
#define	LV6_ENHANCE		1
#define	LV7_ENHANCE		1
#define	LV8_ENHANCE		1
#define	LV9_ENHANCE		1

#define	LV0_U0_ADD		0
#define	LV1_U0_ADD		0
#define	LV2_U0_ADD		0
#define	LV3_U0_ADD		0
#define	LV4_U0_ADD		0
#define	LV5_U0_ADD		0
#define	LV6_U0_ADD		4
#define	LV7_U0_ADD		4
#define	LV8_U0_ADD		4
#define	LV9_U0_ADD		4

#pragma pack(push, 1)

typedef union{
    uint8_t bytes[128];

    struct{
        //Byte 0
        uint8_t cp_lrd_ldo        		: 3;
        uint8_t reserved_3_7           	: 5;

        //Byte 1
        uint8_t ft_lrd_version   		: 4;
        uint8_t ft_type_c_version      	: 4;

        //Byte 2
        uint8_t efuse_data_valid        : 1;
        uint8_t rc16m_valid  			: 1;
        uint8_t rc100k_valid			: 1;
        uint8_t rc100m_valid   			: 1;
        uint8_t adc_valid       		: 1;
        uint8_t vmon_r_valid          	: 1;
        uint8_t otp_valid          		: 1;
        uint8_t rp_valid          		: 1;

        //Byte 3
        uint8_t ra_valid        		: 1;
        uint8_t cc_det_valid 			: 1;
        uint8_t vcon_sw_ocp_valid       : 1;
        uint8_t vcon_sw_scp_valid       : 1;
        uint8_t vcon_sw_ovp_valid       : 1;
        uint8_t vcon_sw_uvlo_valid      : 1;
        uint8_t lrd_rterm_valid      	: 1;
        uint8_t cp_lrd_ldo_valid      	: 1;

        //Byte 4
        uint8_t lrd_ra_valid            : 1;
        uint8_t flash_protect_valid     : 1;
        uint8_t boot_mode     			: 2;
        uint8_t efuse_patch_valid     	: 1;
        uint8_t erase_valid     		: 1;
        uint8_t not_rst_eu2     		: 1;
        uint8_t efuse_rw_test_result    : 1;

        //Byte 5
        uint8_t rc100k          		: 2;
        uint8_t rc100m   				: 2;
        uint8_t rc16m_coarse   			: 2;
        uint8_t reserved_46_47  		: 2;

        //Byte 6
        uint8_t rc16m_fine       		: 7;
        uint8_t reserved_55       		: 1;

        //Byte 7
        uint8_t adc_b       			: 8;

        //Byte 8~9
        uint16_t adc_a       			: 16;

        //Byte 10
        uint8_t vmon_r       			: 8;

        //Byte 11
        uint8_t otp_vtl     			: 4;
        uint8_t otp_vth          		: 4;

        //Byte 12
        uint8_t rp_330ua_cc1       		: 5;
        uint8_t ra_cc1          		: 3;

        //Byte 13
        uint8_t rp_330ua_cc2       		: 5;
        uint8_t ra_cc2          		: 3;

        //Byte 14
        uint8_t rp_180ua_cc1            : 5;
        uint8_t vcon_sw_uvlo        	: 3;

        //Byte 15
        uint8_t rp_180ua_cc2      		: 5;
        uint8_t reserved_125_127       	: 3;

        //Byte 16
        uint8_t rp_80ua_cc1  			: 4;
        uint8_t rp_80ua_cc2        		: 4;

        //Byte 17
        uint8_t cc_det_0p2v_cc1      	: 4;
        uint8_t cc_det_0p2v_cc2      	: 4;

        //Byte 18
        uint8_t cc_det_0p4v_cc1      	: 4;
        uint8_t cc_det_0p4v_cc2      	: 4;

        //Byte 19
        uint8_t cc_det_0p8v_cc1      	: 4;
        uint8_t cc_det_0p8v_cc2      	: 4;

        //Byte 20
        uint8_t cc_det_1p44v_cc1      	: 4;
        uint8_t cc_det_1p44v_cc2      	: 4;

        //Byte 21
        uint8_t cc_det_2p43v_cc1      	: 4;
        uint8_t cc_det_2p43v_cc2      	: 4;

        //Byte 22
        uint8_t vcon_sw_ocp_500_cc1    	: 4;
        uint8_t vcon_sw_ocp_500_cc2    	: 4;

        //Byte 23
        uint8_t vcon_sw_ocp_kos_cc1     : 4;
        uint8_t vcon_sw_ocp_kos_cc2     : 4;

        //Byte 24
        uint8_t vcon_sw_scp_400_cc1     : 4;
        uint8_t vcon_sw_scp_400_cc2     : 4;

        //Byte 25
        uint8_t vcon_sw_ovp_cc1     	: 4;
        uint8_t vcon_sw_ovp_cc2     	: 4;

        //Byte 26
        uint8_t vcon_sw_scp_250_cc1		: 4;
        uint8_t vcon_sw_ocp_350_cc1		: 4;

        //Byte 27
        uint8_t vcon_sw_scp_250_cc2		: 4;
        uint8_t vcon_sw_ocp_350_cc2		: 4;

        //Byte 28
        uint8_t oobs_lfps2_rx_a         : 3;
        uint8_t reserved_227         	: 1;
        uint8_t oobs_lfps2_rx1_b        : 3;
        uint8_t reserved_231         	: 1;

        //Byte 29
        uint8_t oobs_lfps2_rx2_b        : 3;
        uint8_t reserved_235_239        : 5;

        //Byte 30
        uint8_t swing_0_1_a_f    		: 8;

        //Byte 31
        uint8_t swing_2_3_a_f       	: 8;

        //Byte 32
        uint8_t offset_rx_a_f       	: 8;

        //Byte 33
        uint8_t tx_eq_lv_and_dc     	: 4;
        uint8_t rx_eq_lv_and_dc     	: 4;

        //Byte 34
        uint8_t dc_offset_a2     		: 5;
        uint8_t reserved_277_279        : 3;

        //Byte 35
        uint8_t dc_offset_b2     		: 5;
        uint8_t reserved_285_287        : 3;

        //Byte 36
        uint8_t dc_offset_a1     		: 5;
        uint8_t reserved_293_295        : 3;

        //Byte 37
        uint8_t dc_offset_b1     		: 5;
        uint8_t reserved_301_303        : 3;

        //Byte 38
        uint8_t hs_term_tx1_a     		: 4;
        uint8_t hs_term_tx2_a     		: 4;

        //Byte 39
        uint8_t hs_term_tx_b     		: 4;
        uint8_t hs_term_rx_a     		: 4;

        //Byte 40
        uint8_t hs_term_rx1_b     		: 4;
        uint8_t hs_term_rx2_b     		: 4;

        //Byte 41
        uint8_t drvout_0_1_a     		: 8;

        //Byte 42
        uint8_t drvout_2_3_a     		: 8;

        //Byte 43
        uint8_t drvout_0_1_b     		: 8;

        //Byte 44
        uint8_t drvout_2_3_b     		: 8;

        //Byte 45
        uint8_t swing_0_1_a     		: 8;

        //Byte 46
        uint8_t swing_2_3_a     		: 8;

        //Byte 47
        uint8_t swing_0_1_b     		: 8;

        //Byte 48
        uint8_t swing_2_3_b     		: 8;

        //Byte 49
        uint8_t offset_rx_a     		: 8;

        //Byte 50
        uint8_t offset_rx1_b     		: 8;

        //Byte 51
        uint8_t offset_rx2_b     		: 8;

        //Byte 52
        uint8_t oobs_sen_rx1_b     		: 4;
        uint8_t oobs_sen_rx2_b     		: 4;

        //Byte 53
        uint8_t oobs_sen_rx_a    		: 4;
        uint8_t oobs_lfps_rx_a    		: 4;

        //Byte 54
        uint8_t oobs_lfps_rx1_b     	: 4;
        uint8_t oobs_lfps_rx2_b     	: 4;

        //Byte 55
        uint8_t oobs_lfps_lv_u0_2_3     : 8;

        //Byte 56
        uint8_t oobs_lfps_lv_u0_0_1     : 8;

        //Byte 57
        uint8_t oobs_lfps_lv_u2     	: 8;

        //Byte 58
        uint8_t pinmode_lv0_c_mh     	: 3;
        uint8_t reserved_467        	: 1;
        uint8_t pinmode_lv0_c_h2     	: 3;
        uint8_t reserved_471       		: 1;

        //Byte 59
        uint8_t pinmode_lv1_c_mh     	: 3;
        uint8_t reserved_475        	: 1;
        uint8_t pinmode_lv1_c_h2     	: 3;
        uint8_t reserved_479       		: 1;

        //Byte 60
        uint8_t pinmode_lv2_c_mh     	: 3;
        uint8_t reserved_483        	: 1;
        uint8_t pinmode_lv2_c_h2     	: 3;
        uint8_t reserved_487       		: 1;

        //Byte 61
        uint8_t pinmode_lv3_c_mh     	: 3;
        uint8_t reserved_491        	: 1;
        uint8_t pinmode_lv3_c_h2     	: 3;
        uint8_t reserved_495       		: 1;

        //Byte 62
        uint8_t pinmode_lv4_c_mh     	: 3;
        uint8_t reserved_499        	: 1;
        uint8_t pinmode_lv4_c_h2     	: 3;
        uint8_t reserved_503       		: 1;

        //Byte 63
        uint8_t pinmode_lv5_c_mh     	: 3;
        uint8_t reserved_507        	: 1;
        uint8_t pinmode_lv5_c_h2     	: 3;
        uint8_t reserved_511       		: 1;

        //Byte 64
        uint8_t pinmode_lv6_c_mh     	: 3;
        uint8_t reserved_515        	: 1;
        uint8_t pinmode_lv6_c_h2     	: 3;
        uint8_t reserved_519       		: 1;

        //Byte 65
        uint8_t pinmode_lv7_c_mh     	: 3;
        uint8_t reserved_523        	: 1;
        uint8_t pinmode_lv7_c_h2     	: 3;
        uint8_t reserved_527       		: 1;

        //Byte 66
        uint8_t pinmode_lv8_c_mh     	: 3;
        uint8_t reserved_531        	: 1;
        uint8_t pinmode_lv8_c_h2     	: 3;
        uint8_t reserved_535       		: 1;

        //Byte 67
        uint8_t pinmode_lv9_c_mh     	: 3;
        uint8_t reserved_539        	: 1;
        uint8_t pinmode_lv9_c_h2     	: 3;
        uint8_t reserved_543       		: 1;

        //Byte 68
        uint8_t pinmode_lv0_rd     		: 8;

        //Byte 69
        uint8_t pinmode_lv1_rd     		: 8;

        //Byte 70
        uint8_t pinmode_lv2_rd     		: 8;

        //Byte 71
        uint8_t pinmode_lv3_rd     		: 8;

        //Byte 72
        uint8_t pinmode_lv4_rd     		: 8;

        //Byte 73
        uint8_t pinmode_lv5_rd     		: 8;

        //Byte 74
        uint8_t pinmode_lv6_rd     		: 8;

        //Byte 75
        uint8_t pinmode_lv7_rd     		: 8;

        //Byte 76
        uint8_t pinmode_lv8_rd     		: 8;

        //Byte 77
        uint8_t pinmode_lv9_rd     		: 8;

        //Byte 78
        uint8_t pinmode_lv_boost_tx    	: 8;

        //Byte 79
        uint8_t pinmode_lv_boost1234    : 8;

        //Byte 80
        uint8_t pinmode_lv98_related   	: 8;

        //Byte 81
        uint8_t pinmode_lv_ehance   	: 8;

        //Byte 82
        uint8_t pinmode_lv0_dc_offset   : 4;
        uint8_t pinmode_lv1_dc_offset   : 4;

        //Byte 83
        uint8_t pinmode_lv2_dc_offset   : 4;
        uint8_t pinmode_lv3_dc_offset   : 4;

        //Byte 84
        uint8_t pinmode_lv4_dc_offset   : 4;
        uint8_t pinmode_lv5_dc_offset   : 4;

        //Byte 85
        uint8_t pinmode_lv6_dc_offset   : 4;
        uint8_t pinmode_lv7_dc_offset   : 4;

        //Byte 86
        uint8_t pinmode_lv8_dc_offset   : 4;
        uint8_t pinmode_lv9_dc_offset   : 4;

        //Byte 87
        uint8_t pinmode_lv01_u0_add   	: 8;

        //Byte 88
        uint8_t pinmode_lv23_u0_add   	: 8;

        //Byte 89
        uint8_t pinmode_lv45_u0_add   	: 8;

        //Byte 90
        uint8_t pinmode_lv67_u0_add   	: 8;

        //Byte 91
        uint8_t pinmode_lv89_u0_add   	: 8;

        //Byte 92
        uint8_t patch_addrh_0   		: 8;

        //Byte 93
        uint8_t patch_addrl_0   		: 8;

        //Byte 94
        uint8_t patch_data_0   			: 8;

        //Byte 95
        uint8_t patch_addrh_1   		: 8;

        //Byte 96
        uint8_t patch_addrl_1   		: 8;

        //Byte 97
        uint8_t patch_data_1    		: 8;

        //Byte 98
        uint8_t patch_addrh_2   		: 8;

        //Byte 99
        uint8_t patch_addrl_2   		: 8;

        //Byte 100
        uint8_t patch_data_2    		: 8;

        //Byte 101
        uint8_t patch_addrh_3   		: 8;

        //Byte 102
        uint8_t patch_addrl_3   		: 8;

        //Byte 103
        uint8_t patch_data_3    		: 8;

        //Byte 104
        uint8_t patch_addrh_4   		: 8;

        //Byte 105
        uint8_t patch_addrl_4   		: 8;

        //Byte 106
        uint8_t patch_data_4    		: 8;

        //Byte 107
        uint8_t patch_addrh_5   		: 8;

        //Byte 108
        uint8_t patch_addrl_5   		: 8;

        //Byte 109
        uint8_t patch_data_5    		: 8;

        //Byte 110
        uint8_t patch_addrh_6   		: 8;

        //Byte 111
        uint8_t patch_addrl_6   		: 8;

        //Byte 112
        uint8_t patch_data_6    		: 8;

        //Byte 113
        uint8_t patch_addrh_7   		: 8;

        //Byte 114
        uint8_t patch_addrl_7   		: 8;

        //Byte 115
        uint8_t patch_data_7    		: 8;

        //Byte 116
        uint8_t patch_addrh_8   		: 8;

        //Byte 117
        uint8_t patch_addrl_8   		: 8;

        //Byte 118
        uint8_t patch_data_8    		: 8;

        //Byte 119
        uint8_t patch_addrh_9   		: 8;

        //Byte 120
        uint8_t patch_addrl_9   		: 8;

        //Byte 121
        uint8_t patch_data_9    		: 8;

        //Byte 122
        uint8_t patch_addrh_10  		: 8;

        //Byte 123
        uint8_t patch_addrl_10  		: 8;

        //Byte 124
        uint8_t patch_data_10   		: 8;

        //Byte 125
        uint8_t patch_addrh_11  		: 8;

        //Byte 126
        uint8_t patch_addrl_11  		: 8;

        //Byte 127
        uint8_t patch_data_11   		: 8;
    } bits;
}efuse_data_t;

#pragma pack(pop)

int8_t pattern_efuse_dut0();
int8_t pattern_efuse_check_dut0();

#endif /* PATTERN_EFUSE_H_ */
