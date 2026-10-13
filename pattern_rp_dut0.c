#include "platform.h"

int8_t pattern_rp_dut0()
{
    int8_t ret;

    switch(dut0.g_type_c_pattern_step)
    {
    case 0x00:
    {
        //enable CC1 80uA Rp(36K)
        ret = i2c_write_mem(0xA2, 0xE0, 0x24);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0B, .field_shift = 6, .field_mask = 0x0F,
            .init_gear = 7, .adc_ch = CC1_ADC2_CH,
            .cc_id = 1, .cur_type = 0,
            .ref = RP_80UA_REF, .min = RP_80UA_MIN, .max = RP_80UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 pattern_cc1_rp80ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x01:
    {
        //enable CC2 80uA Rp(36K)
        ret = i2c_write_mem(0xA3, 0xE0, 0x24);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0B, .field_shift = 2, .field_mask = 0x0F,
            .init_gear = 7, .adc_ch = CC2_ADC2_CH,
            .cc_id = 2, .cur_type = 0,
            .ref = RP_80UA_REF, .min = RP_80UA_MIN, .max = RP_80UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 pattern_cc2_rp80ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x02:
    {
        //enable CC1 180uA Rp(12K)
        ret = i2c_write_mem(0xA2, 0xE0, 0x64);
        if(ret != 0)	return -1;
        ret = i2c_write_mem(0xA2, 0xE0, 0x44);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0A, .field_shift = 11, .field_mask = 0x1F,
            .init_gear = 15, .adc_ch = CC1_ADC2_CH,
            .cc_id = 1, .cur_type = 1,
            .ref = RP_180UA_REF, .min = RP_180UA_MIN, .max = RP_180UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 pattern_cc1_rp180ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x03:
    {
        //enable CC2 180uA Rp(12K)
        ret = i2c_write_mem(0xA3, 0xE0, 0x64);
        if(ret != 0)	return -1;
        ret = i2c_write_mem(0xA3, 0xE0, 0x44);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0A, .field_shift = 6, .field_mask = 0x1F,
            .init_gear = 15, .adc_ch = CC2_ADC2_CH,
            .cc_id = 2, .cur_type = 1,
            .ref = RP_180UA_REF, .min = RP_180UA_MIN, .max = RP_180UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 pattern_cc2_rp180ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x04:
    {
        //enable CC1 330uA Rp(4.7K)
        ret = i2c_write_mem(0xA2, 0xE0, 0x54);
        if(ret != 0)	return -1;
        ret = i2c_write_mem(0xA2, 0xE0, 0x14);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0C, .field_shift = 11, .field_mask = 0x1F,
            .init_gear = 15, .adc_ch = CC1_ADC2_CH,
            .cc_id = 1, .cur_type = 2,
            .ref = RP_330UA_REF, .min = RP_330UA_MIN, .max = RP_330UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 cc1_pattern_rp330ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x05:
    {
        //enable CC2 330uA Rp(4.7K)
        ret = i2c_write_mem(0xA3, 0xE0, 0x54);
        if(ret != 0)	return -1;
        ret = i2c_write_mem(0xA3, 0xE0, 0x14);
        if(ret != 0)	return -1;
        delay_ms(1);

        const rp_calib_config_t cfg = {
            .phy_addr = 0x0C, .field_shift = 6, .field_mask = 0x1F,
            .init_gear = 15, .adc_ch = CC2_ADC2_CH,
            .cc_id = 2, .cur_type = 2,
            .ref = RP_330UA_REF, .min = RP_330UA_MIN, .max = RP_330UA_MAX,
        };

        if(rp_calib_dut0(&cfg) != 0)
        {
            print("dut0 cc2_pattern_rp330ua fail!\r\n\r\n");
            return -1;
        }

        dut0.g_type_c_pattern_step++;
        break;
    }

    case 0x06:
    {
        //CC open
        ret = i2c_write_mem(0xA2, 0xE0, 0x04);
        if(ret != 0)	return -1;
        ret = i2c_write_mem(0xA3, 0xE0, 0x04);
        if(ret != 0)	return -1;

        dut0.g_pattern_timer = 0x5fff;
        dut0.g_type_c_pattern_step = 0;
        dut0.g_type_c_pattern_num++;
        result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num, DUT0_PATTERN_TYPEC);
        print("dut0 pattern_rp all pass!\r\n\r\n\r\n");

        break;
    }

    default:
        break;
    }

    return 0;
}
