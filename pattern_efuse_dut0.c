#include "platform.h"

int8_t pattern_efuse_dut0()
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	uint16_t reg0000_val;
	uint16_t reg0001_val;
	uint16_t reg0002_val;
	uint16_t reg0003_val;

	switch(dut0.g_lrd_pattern_step)
	{
	case 0x00:
	{
		if(dut0.g_type_c_test_done == 1)
		{
			dut0.g_lrd_pattern_step++;
			dut0.g_lrd_pattern_step++;
		}
		else
		{
			if(dut0.g_type_c_test_timer > 0)
			{
				print("dut0 wait type-c test done...\r\n\r\n");
				dut0.g_type_c_test_timer--;
				dut0.g_lrd_pattern_step++;
			}
			else
			{
				print("dut0 type-c test timeout!\r\n\r\n");
				return -1;
			}
		}

		break;
	}

	case 0x01:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(50);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_step--;
		}

		break;
	}

	case 0x02:
	{
		write_data.bits.ft_lrd_version   		= 1;																	//Byte 1 [3:0]
		write_data.bits.ft_type_c_version      	= 1;																	//Byte 1 [7:4]

		write_data.bits.efuse_data_valid        = 1;																	//Byte 2 [0]
		write_data.bits.rc16m_valid  			= 1;																	//Byte 2 [1]
		write_data.bits.rc100k_valid			= 0;																	//Byte 2 [2]
		write_data.bits.rc100m_valid   			= 1;																	//Byte 2 [3]
		write_data.bits.adc_valid       		= 1;																	//Byte 2 [4]
		write_data.bits.vmon_r_valid          	= 1;																	//Byte 2 [5]
		write_data.bits.otp_valid          		= 1;																	//Byte 2 [6]
		write_data.bits.rp_valid          		= 1;																	//Byte 2 [7]

		write_data.bits.ra_valid        		= 0;																	//Byte 3 [0]
		write_data.bits.cc_det_valid 			= 1;																	//Byte 3 [1]
		write_data.bits.vcon_sw_ocp_valid       = 1;																	//Byte 3 [2]
		write_data.bits.vcon_sw_scp_valid       = 1;																	//Byte 3 [3]
		write_data.bits.vcon_sw_ovp_valid       = 1;																	//Byte 3 [4]
		write_data.bits.vcon_sw_uvlo_valid      = 1;																	//Byte 3 [5]
		write_data.bits.lrd_rterm_valid      	= 1;																	//Byte 3 [6]

		if(read_data.bytes[0] != 0)
		{
			write_data.bits.cp_lrd_ldo_valid    = 1;																	//Byte 3 [7]
		}
		else
		{
			write_data.bits.cp_lrd_ldo_valid    = 0;
		}

		write_data.bits.lrd_ra_valid            = 0;																	//Byte 4 [0]
		write_data.bits.flash_protect_valid     = 1;																	//Byte 4 [1]
		write_data.bits.boot_mode     			= 0;																	//Byte 4 [3:2]

		if(dut0.g_dut_ver == 0)
		{
			write_data.bits.efuse_patch_valid   = 1;																	//Byte 4 [4]
		}
		else
		{
			write_data.bits.efuse_patch_valid   = 0;
		}

		write_data.bits.erase_valid     		= 0;																	//Byte 4 [5]
		write_data.bits.not_rst_eu2     		= 0;																	//Byte 4 [6]
		write_data.bits.efuse_rw_test_result    = 0;																	//Byte 4 [7]

		write_data.bits.rc100k          		= 0;																	//Byte 5 [1:0]
		write_data.bits.rc100m   				= dut0.g_rc50m_offset;													//Byte 5 [3:2]
		write_data.bits.rc16m_coarse   			= dut0.g_rc16m_coarse_offset;											//Byte 5 [5:4]
		write_data.bits.reserved_46_47  		= 0;																	//Byte 5 [7:6]

		write_data.bits.rc16m_fine       		= dut0.g_rc16m_fine_offset;												//Byte 6 [6:0]
		write_data.bits.reserved_55       		= 0;																	//Byte 6 [7]

		write_data.bits.adc_b       			= (dut0.g_adc_b_signbit << 7) | dut0.g_adc_b_offset;					//Byte 7

		write_data.bits.adc_a       			= dut0.g_adc_a / 4;														//Byte 8~9

		write_data.bits.vmon_r       			= dut0.g_vmon_resistor;													//Byte 10

		write_data.bits.otp_vtl     			= dut0.g_vcon_sw_otp_offset;											//Byte 11 [3:0]
		write_data.bits.otp_vth          		= dut0.g_vcon_sw_otp_offset;											//Byte 11 [7:4]

		write_data.bits.rp_330ua_cc1       		= dut0.g_cc1_rp330ua_offset;											//Byte 12 [4:0]
		write_data.bits.ra_cc1          		= 0;																	//Byte 12 [7:5]

		write_data.bits.rp_330ua_cc2       		= dut0.g_cc2_rp330ua_offset;											//Byte 13 [4:0]
		write_data.bits.ra_cc2          		= 0;																	//Byte 13 [7:5]

		write_data.bits.rp_180ua_cc1            = dut0.g_cc1_rp180ua_offset;											//Byte 14 [4:0]
		write_data.bits.vcon_sw_uvlo        	= dut0.g_vcon_sw_uvlo_offset;											//Byte 14 [7:5]

		write_data.bits.rp_180ua_cc2      		= dut0.g_cc2_rp180ua_offset;											//Byte 15 [4:0]
		write_data.bits.reserved_125_127       	= 0;																	//Byte 15 [7:5]

		write_data.bits.rp_80ua_cc1  			= dut0.g_cc1_rp80ua_offset;												//Byte 16 [3:0]
		write_data.bits.rp_80ua_cc2        		= dut0.g_cc2_rp80ua_offset;												//Byte 16 [7:4]

		ret = i2c_read_phy(0x00, 0x00, rx_buf);
		if(ret != 0)	return -1;
		reg0000_val = (rx_buf[1] << 8) | rx_buf[0];

		ret = i2c_read_phy(0x01, 0x00, rx_buf);
		if(ret != 0)	return -1;
		reg0001_val = (rx_buf[1] << 8) | rx_buf[0];

		ret = i2c_read_phy(0x02, 0x00, rx_buf);
		if(ret != 0)	return -1;
		reg0002_val = (rx_buf[1] << 8) | rx_buf[0];

		ret = i2c_read_phy(0x03, 0x00, rx_buf);
		if(ret != 0)	return -1;
		reg0003_val = (rx_buf[1] << 8) | rx_buf[0];

		write_data.bits.cc_det_0p2v_cc1      	= (reg0000_val >> 12) & 0x000F;											//Byte 17 [3:0]
		write_data.bits.cc_det_0p2v_cc2      	= (reg0002_val >> 12) & 0x000F;											//Byte 17 [7:4]

		write_data.bits.cc_det_0p4v_cc1      	= (reg0000_val >> 8) & 0x000F;											//Byte 18 [3:0]
		write_data.bits.cc_det_0p4v_cc2      	= (reg0002_val >> 8) & 0x000F;											//Byte 18 [7:4]

		write_data.bits.cc_det_0p8v_cc1      	= (reg0000_val >> 4) & 0x000F;											//Byte 19 [3:0]
		write_data.bits.cc_det_0p8v_cc2      	= (reg0002_val >> 4) & 0x000F;											//Byte 19 [7:4]

		write_data.bits.cc_det_1p44v_cc1      	= reg0000_val & 0x000F;													//Byte 20 [3:0]
		write_data.bits.cc_det_1p44v_cc2      	= reg0002_val & 0x000F;													//Byte 20 [7:4]

		write_data.bits.cc_det_2p43v_cc1      	= (reg0001_val >> 12) & 0x000F;											//Byte 21 [3:0]
		write_data.bits.cc_det_2p43v_cc2      	= (reg0003_val >> 12) & 0x000F;											//Byte 21 [7:4]

		write_data.bits.vcon_sw_ocp_500_cc1     = dut0.g_cc1_ocp_500ma_offset;											//Byte 22 [3:0]
		write_data.bits.vcon_sw_ocp_500_cc2     = dut0.g_cc2_ocp_500ma_offset;											//Byte 22 [7:4]

		write_data.bits.vcon_sw_ocp_kos_cc1     = dut0.g_cc1_ocp_kos_offset;											//Byte 23 [3:0]
		write_data.bits.vcon_sw_ocp_kos_cc2     = dut0.g_cc2_ocp_kos_offset;											//Byte 23 [7:4]

		write_data.bits.vcon_sw_scp_400_cc1     = dut0.g_cc1_scp_400ma_offset;											//Byte 24 [3:0]
		write_data.bits.vcon_sw_scp_400_cc2     = dut0.g_cc2_scp_400ma_offset;											//Byte 24 [7:4]

		write_data.bits.vcon_sw_ovp_cc1     	= dut0.g_cc1_ovp_offset;												//Byte 25 [3:0]
		write_data.bits.vcon_sw_ovp_cc2     	= dut0.g_cc2_ovp_offset;												//Byte 25 [7:4]

		write_data.bits.vcon_sw_scp_250_cc1     = dut0.g_cc1_scp_250ma_offset;											//Byte 26 [3:0]
		write_data.bits.vcon_sw_ocp_350_cc1     = dut0.g_cc1_ocp_350ma_offset;											//Byte 26 [7:4]

		write_data.bits.vcon_sw_scp_250_cc2     = dut0.g_cc2_scp_250ma_offset;											//Byte 27 [3:0]
		write_data.bits.vcon_sw_ocp_350_cc2     = dut0.g_cc2_ocp_350ma_offset;											//Byte 27 [7:4]

		write_data.bits.oobs_lfps2_rx_a         = dut0.g_sstx_lfps2_oobs_offset;										//Byte 28 [2:0]
		write_data.bits.reserved_227            = 0;																	//Byte 28 [3]
		write_data.bits.oobs_lfps2_rx1_b        = dut0.g_ssrx1_lfps2_oobs_offset;										//Byte 28 [6:4]
		write_data.bits.reserved_231            = 0;																	//Byte 28 [7]

		write_data.bits.oobs_lfps2_rx2_b        = dut0.g_ssrx2_lfps2_oobs_offset;										//Byte 29 [2:0]
		write_data.bits.reserved_235_239        = 0;																	//Byte 29 [7:3]

		write_data.bits.swing_0_1_a_f        	= (dut0.g_sstx_sstx2_vcom_offset << 4) | dut0.g_sstx_sstx2_vcom_offset;	//Byte 30

		write_data.bits.swing_2_3_a_f       	= (dut0.g_sstx_sstx2_vcom_offset << 4) | dut0.g_sstx_sstx2_vcom_offset;	//Byte 31

		write_data.bits.offset_rx_a_f        	= dut0.g_sstx_sstx2_pn_offset;											//Byte 32

		write_data.bits.tx_eq_lv_and_dc     	= 0;																	//Byte 33 [3:0]
		write_data.bits.rx_eq_lv_and_dc     	= 0;																	//Byte 33 [7:4]

		write_data.bits.dc_offset_a2     		= dut0.g_sstx_sstx2_dc_offset;											//Byte 34 [4:0]
		write_data.bits.reserved_277_279        = 0;																	//Byte 34 [7:5]

		write_data.bits.dc_offset_b2     		= dut0.g_ssrx2_ssrx_dc_offset;											//Byte 35 [4:0]
		write_data.bits.reserved_285_287        = 0;																	//Byte 35 [7:5]

		write_data.bits.dc_offset_a1     		= dut0.g_sstx_sstx1_dc_offset;											//Byte 36 [4:0]
		write_data.bits.reserved_293_295        = 0;																	//Byte 36 [7:5]

		write_data.bits.dc_offset_b1     		= dut0.g_ssrx1_ssrx_dc_offset;											//Byte 37 [4:0]
		write_data.bits.reserved_301_303        = 0;																	//Byte 37 [7:5]

		write_data.bits.hs_term_tx1_a     		= dut0.g_sstx1_rterm_offset;											//Byte 38 [3:0]
		write_data.bits.hs_term_tx2_a     		= dut0.g_sstx2_rterm_offset;											//Byte 38 [7:4]

		write_data.bits.hs_term_tx_b     		= dut0.g_ssrx_rterm_offset;												//Byte 39 [3:0]
		write_data.bits.hs_term_rx_a     		= dut0.g_sstx_rterm_offset;												//Byte 39 [7:4]

		write_data.bits.hs_term_rx1_b     		= dut0.g_ssrx1_rterm_offset;											//Byte 40 [3:0]
		write_data.bits.hs_term_rx2_b     		= dut0.g_ssrx2_rterm_offset;											//Byte 40 [7:4]

		write_data.bits.drvout_0_1_a     		= (7 << 4) | 7;															//Byte 41

		write_data.bits.drvout_2_3_a     		= (7 << 4) | 7;															//Byte 42

		write_data.bits.drvout_0_1_b     		= (7 << 4) | 7;															//Byte 43

		write_data.bits.drvout_2_3_b     		= (7 << 4) | 7;															//Byte 44

		write_data.bits.swing_0_1_a     		= (dut0.g_sstx_sstx1_vcom_offset << 4) | dut0.g_sstx_sstx1_vcom_offset;	//Byte 45

		write_data.bits.swing_2_3_a     		= (dut0.g_sstx_sstx1_vcom_offset << 4) | dut0.g_sstx_sstx1_vcom_offset; //Byte 46

		write_data.bits.swing_0_1_b     		= (dut0.g_ssrx1_ssrx_vcom_offset << 4) | dut0.g_ssrx1_ssrx_vcom_offset; //Byte 47

		write_data.bits.swing_2_3_b     		= (dut0.g_ssrx1_ssrx_vcom_offset << 4) | dut0.g_ssrx1_ssrx_vcom_offset; //Byte 48

		write_data.bits.offset_rx_a     		= dut0.g_sstx_sstx1_pn_offset;											//Byte 49

		write_data.bits.offset_rx1_b     		= dut0.g_ssrx1_ssrx_pn_offset;											//Byte 50

		write_data.bits.offset_rx2_b     		= dut0.g_ssrx2_ssrx_pn_offset;											//Byte 51

		write_data.bits.oobs_sen_rx1_b     		= dut0.g_ssrx1_sig_oobs_offset;											//Byte 52 [3:0]
		write_data.bits.oobs_sen_rx2_b     		= dut0.g_ssrx2_sig_oobs_offset;											//Byte 52 [7:4]

		write_data.bits.oobs_sen_rx_a    		= dut0.g_sstx_sig_oobs_offset;											//Byte 53 [3:0]
		write_data.bits.oobs_lfps_rx_a    		= dut0.g_sstx_lfps1_oobs_offset;										//Byte 53 [7:4]

		write_data.bits.oobs_lfps_rx1_b     	= dut0.g_ssrx1_lfps1_oobs_offset;										//Byte 54 [3:0]
		write_data.bits.oobs_lfps_rx2_b     	= dut0.g_ssrx2_lfps1_oobs_offset;										//Byte 54 [7:4]

		write_data.bits.oobs_lfps_lv_u0_2_3     = 0x30;																	//Byte 55

		write_data.bits.oobs_lfps_lv_u0_0_1     = 0x21;																	//Byte 56

		write_data.bits.oobs_lfps_lv_u2     	= 0x90;																	//Byte 57

		write_data.bits.pinmode_lv0_c_mh     	= LV0_C1;																//Byte 58 [2:0]
		write_data.bits.reserved_467        	= 0;																	//Byte 58 [3]
		write_data.bits.pinmode_lv0_c_h2     	= LV0_C2;																//Byte 58 [6:4]
		write_data.bits.reserved_471       		= 0;																	//Byte 58 [7]

		write_data.bits.pinmode_lv1_c_mh     	= LV1_C1;																//Byte 59 [2:0]
		write_data.bits.reserved_475        	= 0;																	//Byte 59 [3]
		write_data.bits.pinmode_lv1_c_h2     	= LV1_C2;																//Byte 59 [6:4]
		write_data.bits.reserved_479       		= 0;																	//Byte 59 [7]

		write_data.bits.pinmode_lv2_c_mh     	= LV2_C1;																//Byte 60 [2:0]
		write_data.bits.reserved_483        	= 0;																	//Byte 60 [3]
		write_data.bits.pinmode_lv2_c_h2     	= LV2_C2;																//Byte 60 [6:4]
		write_data.bits.reserved_487       		= 0;																	//Byte 60 [7]

		write_data.bits.pinmode_lv3_c_mh     	= LV3_C1;																//Byte 61 [2:0]
		write_data.bits.reserved_491        	= 0;																	//Byte 61 [3]
		write_data.bits.pinmode_lv3_c_h2     	= LV3_C2;																//Byte 61 [6:4]
		write_data.bits.reserved_495       		= 0;																	//Byte 61 [7]

		write_data.bits.pinmode_lv4_c_mh     	= LV4_C1;																//Byte 62 [2:0]
		write_data.bits.reserved_499        	= 0;																	//Byte 62 [3]
		write_data.bits.pinmode_lv4_c_h2     	= LV4_C2;																//Byte 62 [6:4]
		write_data.bits.reserved_503       		= 0;																	//Byte 62 [7]

		write_data.bits.pinmode_lv5_c_mh     	= LV5_C1;																//Byte 63 [2:0]
		write_data.bits.reserved_507        	= 0;																	//Byte 63 [3]
		write_data.bits.pinmode_lv5_c_h2     	= LV5_C2;																//Byte 63 [6:4]
		write_data.bits.reserved_511       		= 0;																	//Byte 63 [7]

		write_data.bits.pinmode_lv6_c_mh     	= LV6_C1;																//Byte 64 [2:0]
		write_data.bits.reserved_515        	= 0;																	//Byte 64 [3]
		write_data.bits.pinmode_lv6_c_h2     	= LV6_C2;																//Byte 64 [6:4]
		write_data.bits.reserved_519       		= 0;																	//Byte 64 [7]

		write_data.bits.pinmode_lv7_c_mh     	= LV7_C1;																//Byte 65 [2:0]
		write_data.bits.reserved_523        	= 0;																	//Byte 65 [3]
		write_data.bits.pinmode_lv7_c_h2     	= LV7_C2;																//Byte 65 [6:4]
		write_data.bits.reserved_527       		= 0;																	//Byte 65 [7]

		write_data.bits.pinmode_lv8_c_mh     	= LV8_C1;																//Byte 66 [2:0]
		write_data.bits.reserved_531        	= 0;																	//Byte 66 [3]
		write_data.bits.pinmode_lv8_c_h2     	= LV8_C2;																//Byte 66 [6:4]
		write_data.bits.reserved_535       		= 0;																	//Byte 66 [7]

		write_data.bits.pinmode_lv9_c_mh     	= LV9_C1;																//Byte 67 [2:0]
		write_data.bits.reserved_539        	= 0;																	//Byte 67 [3]
		write_data.bits.pinmode_lv9_c_h2     	= LV9_C2;																//Byte 67 [6:4]
		write_data.bits.reserved_543       		= 0;																	//Byte 67 [7]

		write_data.bits.pinmode_lv0_rd     		= (LV0_R1 << 6) | (LV0_R2 << 4) | (LV0_R3 << 2);						//Byte 68
		write_data.bits.pinmode_lv1_rd     		= (LV1_R1 << 6) | (LV1_R2 << 4) | (LV1_R3 << 2);						//Byte 69
		write_data.bits.pinmode_lv2_rd     		= (LV2_R1 << 6) | (LV2_R2 << 4) | (LV2_R3 << 2);						//Byte 70
		write_data.bits.pinmode_lv3_rd     		= (LV3_R1 << 6) | (LV3_R2 << 4) | (LV3_R3 << 2);						//Byte 71
		write_data.bits.pinmode_lv4_rd     		= (LV4_R1 << 6) | (LV4_R2 << 4) | (LV4_R3 << 2);						//Byte 72
		write_data.bits.pinmode_lv5_rd     		= (LV5_R1 << 6) | (LV5_R2 << 4) | (LV5_R3 << 2);						//Byte 73
		write_data.bits.pinmode_lv6_rd     		= (LV6_R1 << 6) | (LV6_R2 << 4) | (LV6_R3 << 2);						//Byte 74
		write_data.bits.pinmode_lv7_rd     		= (LV7_R1 << 6) | (LV7_R2 << 4) | (LV7_R3 << 2);						//Byte 75
		write_data.bits.pinmode_lv8_rd     		= (LV8_R1 << 6) | (LV8_R2 << 4) | (LV8_R3 << 2);						//Byte 76
		write_data.bits.pinmode_lv9_rd     		= (LV9_R1 << 6) | (LV9_R2 << 4) | (LV9_R3 << 2);						//Byte 77

		write_data.bits.pinmode_lv_boost_tx    	= (LV7_BOOST_TX << 7) | (LV6_BOOST_TX << 6) |							//Byte 78
		             	 	 	 	 	 	 	  (LV5_BOOST_TX << 5) | (LV4_BOOST_TX << 4) |
		             	 	 	 	 	 	 	  (LV3_BOOST_TX << 3) | (LV2_BOOST_TX << 2) |
												  (LV1_BOOST_TX << 1) | (LV0_BOOST_TX << 0);

		write_data.bits.pinmode_lv_boost1234    = (LV7_BOOST123 << 7) | (LV6_BOOST123 << 6) |							//Byte 79
												  (LV5_BOOST123 << 5) | (LV4_BOOST123 << 4) |
												  (LV3_BOOST123 << 3) | (LV2_BOOST123 << 2) |
												  (LV1_BOOST123 << 1) | (LV0_BOOST123 << 0);

		write_data.bits.pinmode_lv98_related   	= (LV9_ENHANCE << 7) | (LV8_ENHANCE << 6) |								//Byte 80
												  (LV9_BOOST123 << 3) | (LV8_BOOST123 << 2) |
												  (LV9_BOOST_TX << 1) | (LV8_BOOST_TX << 0);

		write_data.bits.pinmode_lv_ehance   	= (LV7_ENHANCE << 7) | (LV6_ENHANCE << 6) |								//Byte 81
												  (LV5_ENHANCE << 5) | (LV4_ENHANCE << 4) |
												  (LV3_ENHANCE << 3) | (LV2_ENHANCE << 2) |
												  (LV1_ENHANCE << 1) | (LV0_ENHANCE << 0);

		write_data.bits.pinmode_lv0_dc_offset   = LV0_DC_OFFSET;														//Byte 82 [3:0]
		write_data.bits.pinmode_lv1_dc_offset   = LV1_DC_OFFSET;														//Byte 82 [7:4]
		write_data.bits.pinmode_lv2_dc_offset   = LV2_DC_OFFSET;														//Byte 83 [3:0]
		write_data.bits.pinmode_lv3_dc_offset   = LV3_DC_OFFSET;														//Byte 83 [7:4]
		write_data.bits.pinmode_lv4_dc_offset   = LV4_DC_OFFSET;														//Byte 84 [3:0]
		write_data.bits.pinmode_lv5_dc_offset   = LV5_DC_OFFSET;														//Byte 84 [7:4]
		write_data.bits.pinmode_lv6_dc_offset   = LV6_DC_OFFSET;														//Byte 85 [3:0]
		write_data.bits.pinmode_lv7_dc_offset   = LV7_DC_OFFSET;														//Byte 85 [7:4]
		write_data.bits.pinmode_lv8_dc_offset   = LV8_DC_OFFSET;														//Byte 86 [3:0]
		write_data.bits.pinmode_lv9_dc_offset   = LV9_DC_OFFSET;														//Byte 86 [7:4]

		write_data.bits.pinmode_lv01_u0_add   	= (LV1_U0_ADD << 5) | (LV0_U0_ADD << 2);								//Byte 87
		write_data.bits.pinmode_lv23_u0_add   	= (LV3_U0_ADD << 5) | (LV2_U0_ADD << 2);								//Byte 88
		write_data.bits.pinmode_lv45_u0_add   	= (LV5_U0_ADD << 5) | (LV4_U0_ADD << 2);								//Byte 89
		write_data.bits.pinmode_lv67_u0_add   	= (LV7_U0_ADD << 5) | (LV6_U0_ADD << 2);								//Byte 90
		write_data.bits.pinmode_lv89_u0_add   	= (LV9_U0_ADD << 5) | (LV8_U0_ADD << 2);								//Byte 91

//		write_data.bits.patch_addrh_0   		= ;																		//Byte 92
//		write_data.bits.patch_addrl_0   		= ;																		//Byte 93
//		write_data.bits.patch_data_0   			= ;																		//Byte 94
//		write_data.bits.patch_addrh_1   		= ;																		//Byte 95
//		write_data.bits.patch_addrl_1   		= ;																		//Byte 96
//		write_data.bits.patch_data_1    		= ;																		//Byte 97
//		write_data.bits.patch_addrh_2   		= ;																		//Byte 98
//		write_data.bits.patch_addrl_2   		= ;																		//Byte 99
//		write_data.bits.patch_data_2    		= ;																		//Byte 100
//		write_data.bits.patch_addrh_3   		= ;																		//Byte 101
//		write_data.bits.patch_addrl_3   		= ;																		//Byte 102
//		write_data.bits.patch_data_3    		= ;																		//Byte 103
//		write_data.bits.patch_addrh_4   		= ;																		//Byte 104
//		write_data.bits.patch_addrl_4   		= ;																		//Byte 105
//		write_data.bits.patch_data_4    		= ;																		//Byte 106
//		write_data.bits.patch_addrh_5   		= ;																		//Byte 107
//		write_data.bits.patch_addrl_5   		= ;																		//Byte 108
//		write_data.bits.patch_data_5    		= ;																		//Byte 109
//		write_data.bits.patch_addrh_6   		= ;																		//Byte 110
//		write_data.bits.patch_addrl_6   		= ;																		//Byte 111
//		write_data.bits.patch_data_6    		= ;																		//Byte 112
//		write_data.bits.patch_addrh_7   		= ;																		//Byte 113
//		write_data.bits.patch_addrl_7   		= ;																		//Byte 114
//		write_data.bits.patch_data_7    		= ;																		//Byte 115
//		write_data.bits.patch_addrh_8   		= ;																		//Byte 116
//		write_data.bits.patch_addrl_8   		= ;																		//Byte 117
//		write_data.bits.patch_data_8    		= ;																		//Byte 118
//		write_data.bits.patch_addrh_9   		= ;																		//Byte 119
//		write_data.bits.patch_addrl_9   		= ;																		//Byte 120
//		write_data.bits.patch_data_9    		= ;																		//Byte 121
//		write_data.bits.patch_addrh_10  		= ;																		//Byte 122
//		write_data.bits.patch_addrl_10  		= ;																		//Byte 123
//		write_data.bits.patch_data_10   		= ;																		//Byte 124
//		write_data.bits.patch_addrh_11  		= ;																		//Byte 125
//		write_data.bits.patch_addrl_11  		= ;																		//Byte 126
//		write_data.bits.patch_data_11   		= ;																		//Byte 127

		if(dut0.g_ft2_retest == 0)
		{
			print("dut0 go to write efuse!\r\n\r\n");
			dut0.g_lrd_pattern_step++;
		}
		else
		{
			print("dut0 go to judge tolerance!\r\n\r\n");
			dut0.g_lrd_pattern_step++;
			dut0.g_lrd_pattern_step++;
		}

		break;
	}

	case 0x03:
	{
		//enable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)	return -1;
		delay_ms(1);

		//write efuse
		for(uint8_t i=1;i<92;i++)
		{
			ret = i2c_write_efuse(i, write_data.bytes);
			if(ret != 0)	return -1;
			delay_ms(1);
		}

		//read efuse
		for(uint8_t i=0;i<128;i++)
		{
			ret = i2c_read_efuse(i, rx_buf);
			if(ret != 0)	return -1;
			read_data.bytes[i] = rx_buf[0];
			xil_printf("dut0 efuse_read_data[%03d]: %02x\r\n", i, read_data.bytes[i]);
		}
		print("\r\n");

		//disable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		if(memcmp(write_data.bytes, read_data.bytes, sizeof(write_data.bytes)) == 0)
		{
			print("dut0 efuse write/read compare pass!\r\n\r\n");
			write_data.bits.efuse_rw_test_result = 1;

			ret = i2c_write_efuse(4, write_data.bytes);
			if(ret != 0)	return -1;
			delay_ms(1);
		}
		else
		{
			print("dut0 efuse write/read compare fail!\r\n\r\n");
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x04:
	{
		//enable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7F);
		if(ret != 0)	return -1;
		delay_ms(1);

		//read efuse
		for(uint8_t i=0;i<128;i++)
		{
			ret = i2c_read_efuse(i, rx_buf);
			if(ret != 0)	return -1;
			read_data.bytes[i] = rx_buf[0];
			xil_printf("dut0 efuse_read_data[%03d]: %02x\r\n", i, read_data.bytes[i]);
		}
		print("\r\n");

		//disable efuse clock
		ret = i2c_write_mem(0x0C, 0xDF, 0x7E);
		if(ret != 0)	return -1;
		delay_ms(1);

		//judge tolerance: Rterm
		if((abs((int)write_data.bits.hs_term_tx1_a - (int)read_data.bits.hs_term_tx1_a) > RTERM_TOLERANCE) ||
		   (abs((int)write_data.bits.hs_term_tx2_a - (int)read_data.bits.hs_term_tx2_a) > RTERM_TOLERANCE) ||
		   (abs((int)write_data.bits.hs_term_tx_b - (int)read_data.bits.hs_term_tx_b) > RTERM_TOLERANCE) ||
		   (abs((int)write_data.bits.hs_term_rx_a - (int)read_data.bits.hs_term_rx_a) > RTERM_TOLERANCE) ||
		   (abs((int)write_data.bits.hs_term_rx1_b - (int)read_data.bits.hs_term_rx1_b) > RTERM_TOLERANCE) ||
		   (abs((int)write_data.bits.hs_term_rx2_b - (int)read_data.bits.hs_term_rx2_b) > RTERM_TOLERANCE))
		{
			print("dut0 Rterm tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: p/n offset
		uint8_t offset_rx_a_write_conv;
		uint8_t offset_rx_a_read_conv;
		uint8_t offset_rx_a_f_write_conv;
		uint8_t offset_rx_a_f_read_conv;
		uint8_t offset_rx1_b_write_conv;
		uint8_t offset_rx1_b_read_conv;
		uint8_t offset_rx2_b_write_conv;
		uint8_t offset_rx2_b_read_conv;

		if(write_data.bits.offset_rx_a < 128)	offset_rx_a_write_conv = write_data.bits.offset_rx_a | 0x80;
		else									offset_rx_a_write_conv = 0x7f - (write_data.bits.offset_rx_a & 0x7f);

		if(read_data.bits.offset_rx_a < 128)	offset_rx_a_read_conv = read_data.bits.offset_rx_a | 0x80;
		else									offset_rx_a_read_conv = 0x7f - (read_data.bits.offset_rx_a & 0x7f);

		if(write_data.bits.offset_rx_a_f < 128)	offset_rx_a_f_write_conv = write_data.bits.offset_rx_a_f | 0x80;
		else									offset_rx_a_f_write_conv = 0x7f - (write_data.bits.offset_rx_a_f & 0x7f);

		if(read_data.bits.offset_rx_a_f < 128)	offset_rx_a_f_read_conv = read_data.bits.offset_rx_a_f | 0x80;
		else									offset_rx_a_f_read_conv = 0x7f - (read_data.bits.offset_rx_a_f & 0x7f);

		if(write_data.bits.offset_rx1_b < 128)	offset_rx1_b_write_conv = write_data.bits.offset_rx1_b | 0x80;
		else									offset_rx1_b_write_conv = 0x7f - (write_data.bits.offset_rx1_b & 0x7f);

		if(read_data.bits.offset_rx1_b < 128)	offset_rx1_b_read_conv = read_data.bits.offset_rx1_b | 0x80;
		else									offset_rx1_b_read_conv = 0x7f - (read_data.bits.offset_rx1_b & 0x7f);

		if(write_data.bits.offset_rx2_b < 128)	offset_rx2_b_write_conv = write_data.bits.offset_rx2_b | 0x80;
		else									offset_rx2_b_write_conv = 0x7f - (write_data.bits.offset_rx2_b & 0x7f);

		if(read_data.bits.offset_rx2_b < 128)	offset_rx2_b_read_conv = read_data.bits.offset_rx2_b | 0x80;
		else									offset_rx2_b_read_conv = 0x7f - (read_data.bits.offset_rx2_b & 0x7f);

		if((abs((int)offset_rx_a_write_conv - (int)offset_rx_a_read_conv) > OFFSET_TOLERANCE) ||
		   (abs((int)offset_rx_a_f_write_conv - (int)offset_rx_a_f_read_conv) > OFFSET_TOLERANCE) ||
		   (abs((int)offset_rx1_b_write_conv - (int)offset_rx1_b_read_conv) > OFFSET_TOLERANCE) ||
		   (abs((int)offset_rx2_b_write_conv - (int)offset_rx2_b_read_conv) > OFFSET_TOLERANCE))
		{
			print("dut0 p/n offset tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: Vcom
		if((abs((int)(write_data.bits.swing_0_1_a & 0x0F) - (int)(read_data.bits.swing_0_1_a & 0x0F)) > VCOM_TOLERANCE) ||
		   (abs((int)(write_data.bits.swing_0_1_a_f & 0x0F) - (int)(read_data.bits.swing_0_1_a_f & 0x0F)) > VCOM_TOLERANCE) ||
		   (abs((int)(write_data.bits.swing_0_1_b & 0x0F) - (int)(read_data.bits.swing_0_1_b & 0x0F)) > VCOM_TOLERANCE))
		{
			print("dut0 Vcom tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: DC
		if((abs((int)write_data.bits.dc_offset_a2 - (int)read_data.bits.dc_offset_a2) > DC_TOLERANCE) ||
		   (abs((int)write_data.bits.dc_offset_b2 - (int)read_data.bits.dc_offset_b2) > DC_TOLERANCE) ||
		   (abs((int)write_data.bits.dc_offset_a1 - (int)read_data.bits.dc_offset_a1) > DC_TOLERANCE) ||
		   (abs((int)write_data.bits.dc_offset_b1 - (int)read_data.bits.dc_offset_b1) > DC_TOLERANCE))
		{
			print("dut0 DC tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: LFPS1_OOBS
		if((abs((int)write_data.bits.oobs_lfps_rx_a - (int)read_data.bits.oobs_lfps_rx_a) > LFPS_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_lfps_rx1_b - (int)read_data.bits.oobs_lfps_rx1_b) > LFPS_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_lfps_rx2_b - (int)read_data.bits.oobs_lfps_rx2_b) > LFPS_OOBS_TOLERANCE))
		{
			print("dut0 LFPS1_OOBS tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: LFPS2_OOBS
		if((abs((int)write_data.bits.oobs_lfps2_rx_a - (int)read_data.bits.oobs_lfps2_rx_a) > LFPS_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_lfps2_rx1_b - (int)read_data.bits.oobs_lfps2_rx1_b) > LFPS_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_lfps2_rx2_b - (int)read_data.bits.oobs_lfps2_rx2_b) > LFPS_OOBS_TOLERANCE))
		{
			print("dut0 LFPS2_OOBS tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: SIG_OOBS
		if((abs((int)write_data.bits.oobs_sen_rx1_b - (int)read_data.bits.oobs_sen_rx1_b) > SIG_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_sen_rx2_b - (int)read_data.bits.oobs_sen_rx2_b) > SIG_OOBS_TOLERANCE) ||
		   (abs((int)write_data.bits.oobs_sen_rx_a - (int)read_data.bits.oobs_sen_rx_a) > SIG_OOBS_TOLERANCE))
		{
			print("dut0 SIG_OOBS tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: RC16M
		if((abs((int)write_data.bits.rc16m_coarse - (int)read_data.bits.rc16m_coarse) > RC16M_COARSE_TOLERANCE) ||
		   (abs((int)write_data.bits.rc16m_fine - (int)read_data.bits.rc16m_fine) > RC16M_FINE_TOLERANCE))
		{
			print("dut0 RC16M tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: RC50M
		if(abs((int)write_data.bits.rc100m - (int)read_data.bits.rc100m) > RC50M_TOLERANCE)
		{
			print("dut0 RC50M tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: ADC_a
		if(abs((int)write_data.bits.adc_a - (int)read_data.bits.adc_a) > ADC_A_TOLERANCE)
		{
			print("dut0 ADC_a tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: ADC_b
		int8_t adc_b_write_conv;
		int8_t adc_b_read_conv;

		if((write_data.bits.adc_b >> 7) == 0)	adc_b_write_conv = 	write_data.bits.adc_b & 0x7f;
		else									adc_b_write_conv = 	-(write_data.bits.adc_b & 0x7f);

		if((read_data.bits.adc_b >> 7) == 0)	adc_b_read_conv = 	read_data.bits.adc_b & 0x7f;
		else									adc_b_read_conv = 	-(read_data.bits.adc_b & 0x7f);

		if(abs((int)adc_b_write_conv - (int)adc_b_read_conv) > ADC_B_TOLERANCE)
		{
			print("dut0 ADC_b tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VMON resistor
		if(abs((int)write_data.bits.vmon_r - (int)read_data.bits.vmon_r) > VMON_RES_TOLERANCE)
		{
			print("dut0 VMON resistor tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: Rp
		if((abs((int)write_data.bits.rp_330ua_cc1 - (int)read_data.bits.rp_330ua_cc1) > RP_TOLERANCE) ||
		   (abs((int)write_data.bits.rp_330ua_cc2 - (int)read_data.bits.rp_330ua_cc2) > RP_TOLERANCE) ||
		   (abs((int)write_data.bits.rp_180ua_cc1 - (int)read_data.bits.rp_180ua_cc1) > RP_TOLERANCE) ||
		   (abs((int)write_data.bits.rp_180ua_cc2 - (int)read_data.bits.rp_180ua_cc2) > RP_TOLERANCE) ||
		   (abs((int)write_data.bits.rp_80ua_cc1 - (int)read_data.bits.rp_80ua_cc1) > RP_TOLERANCE) ||
		   (abs((int)write_data.bits.rp_80ua_cc2 - (int)read_data.bits.rp_80ua_cc2) > RP_TOLERANCE))
		{
			print("dut0 Rp tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: CCDET
		if((abs((int)write_data.bits.cc_det_0p2v_cc1 - (int)read_data.bits.cc_det_0p2v_cc1) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_0p2v_cc2 - (int)read_data.bits.cc_det_0p2v_cc2) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_0p4v_cc1 - (int)read_data.bits.cc_det_0p4v_cc1) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_0p4v_cc2 - (int)read_data.bits.cc_det_0p4v_cc2) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_0p8v_cc1 - (int)read_data.bits.cc_det_0p8v_cc1) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_0p8v_cc2 - (int)read_data.bits.cc_det_0p8v_cc2) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_1p44v_cc1 - (int)read_data.bits.cc_det_1p44v_cc1) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_1p44v_cc2 - (int)read_data.bits.cc_det_1p44v_cc2) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_2p43v_cc1 - (int)read_data.bits.cc_det_2p43v_cc1) > CCDET_TOLERANCE) ||
		   (abs((int)write_data.bits.cc_det_2p43v_cc2 - (int)read_data.bits.cc_det_2p43v_cc2) > CCDET_TOLERANCE))
		{
			print("dut0 CCDET tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VCON_SW OTP
		if(abs((int)write_data.bits.otp_vth - (int)read_data.bits.otp_vth) > VCON_SW_OTP_TOLERANCE)
		{
			print("dut0 VCON_SW OTP tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VCON_SW OVP
		if((abs((int)write_data.bits.vcon_sw_ovp_cc1 - (int)read_data.bits.vcon_sw_ovp_cc1) > VCON_SW_OVP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ovp_cc2 - (int)read_data.bits.vcon_sw_ovp_cc2) > VCON_SW_OVP_TOLERANCE))
		{
			print("dut0 VCON_SW OVP tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VCON_SW UVLO
		if(abs((int)write_data.bits.vcon_sw_uvlo - (int)read_data.bits.vcon_sw_uvlo) > VCON_SW_UVLO_TOLERANCE)
		{
			print("dut0 VCON_SW UVLO tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VCON_SW OCP
		if((abs((int)write_data.bits.vcon_sw_ocp_kos_cc1 - (int)read_data.bits.vcon_sw_ocp_kos_cc1) > VCON_SW_OCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ocp_kos_cc2 - (int)read_data.bits.vcon_sw_ocp_kos_cc2) > VCON_SW_OCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ocp_500_cc1 - (int)read_data.bits.vcon_sw_ocp_500_cc1) > VCON_SW_OCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ocp_500_cc2 - (int)read_data.bits.vcon_sw_ocp_500_cc2) > VCON_SW_OCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ocp_350_cc1 - (int)read_data.bits.vcon_sw_ocp_350_cc1) > VCON_SW_OCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_ocp_350_cc2 - (int)read_data.bits.vcon_sw_ocp_350_cc2) > VCON_SW_OCP_TOLERANCE))
		{
			print("dut0 VCON_SW OCP tolerance judge fail!\r\n\r\n");
			return -1;
		}

		//judge tolerance: VCON_SW SCP
		if((abs((int)write_data.bits.vcon_sw_scp_400_cc1 - (int)read_data.bits.vcon_sw_scp_400_cc1) > VCON_SW_SCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_scp_400_cc2 - (int)read_data.bits.vcon_sw_scp_400_cc2) > VCON_SW_SCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_scp_250_cc1 - (int)read_data.bits.vcon_sw_scp_250_cc1) > VCON_SW_SCP_TOLERANCE) ||
		   (abs((int)write_data.bits.vcon_sw_scp_250_cc2 - (int)read_data.bits.vcon_sw_scp_250_cc2) > VCON_SW_SCP_TOLERANCE))
		{
			print("dut0 VCON_SW SCP tolerance judge fail!\r\n\r\n");
			return -1;
		}

		dut0.g_lrd_pattern_step++;
		break;
	}

	case 0x05:
	{
		dut0.g_lrd_pattern_num = 0x0c;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_lrd_pattern_num);

		dut0.g_pass_cnt++;
		xil_printf("dut0.g_test_cnt = %d, dut0.g_pass_cnt = %d\r\n\r\n", dut0.g_test_cnt, dut0.g_pass_cnt);

		pattern_safe_state_dut0();
	}

	default:
		break;
	}

	return 0;
}
