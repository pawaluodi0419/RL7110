#include "platform.h"

int8_t rp_calib_dut0(const rp_calib_config_t *cfg)
{
	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

    int8_t ret, step;
    uint8_t start_gear, end_gear;

    uint8_t *signbit;
    uint8_t *offset;

    if(cfg->cc_id == 1)
    {
        if(cfg->cur_type == 0)
        {
            signbit = &dut0.g_cc1_rp80ua_signbit;
            offset  = &dut0.g_cc1_rp80ua_offset;
        }
        else if(cfg->cur_type == 1)
        {
            signbit = &dut0.g_cc1_rp180ua_signbit;
            offset  = &dut0.g_cc1_rp180ua_offset;
        }
        else
        {
            signbit = &dut0.g_cc1_rp330ua_signbit;
            offset  = &dut0.g_cc1_rp330ua_offset;
        }
    }
    else
    {
        if(cfg->cur_type == 0)
        {
            signbit = &dut0.g_cc2_rp80ua_signbit;
            offset  = &dut0.g_cc2_rp80ua_offset;
        }
        else if(cfg->cur_type == 1)
        {
            signbit = &dut0.g_cc2_rp180ua_signbit;
            offset  = &dut0.g_cc2_rp180ua_offset;
        }
        else
        {
            signbit = &dut0.g_cc2_rp330ua_signbit;
            offset  = &dut0.g_cc2_rp330ua_offset;
        }
    }

    ret = i2c_read_phy(cfg->phy_addr, 0x00, rx_buf);
    if(ret != 0)	return -1;

    uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
    base_val &= ~((uint16_t)cfg->field_mask << cfg->field_shift);
    uint16_t reg_val = base_val | ((uint16_t)cfg->init_gear << cfg->field_shift);

    tx_buf[0] = reg_val & 0xFF;
    tx_buf[1] = reg_val >> 8;
    ret = i2c_write_phy(cfg->phy_addr, 0x00, tx_buf[0], tx_buf[1]);
    if(ret != 0)	return -1;
    delay_ms(1);

    uint16_t adc_data = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC2_ADDR, cfg->adc_ch, AVG_NUM);
    xil_printf("dut0 cc_adc_data = %d\r\n", adc_data);

    if(adc_data > cfg->ref)
    {
        *signbit = 1;
        start_gear = cfg->init_gear;
        end_gear = 0;
        step = -1;
    }
    else
    {
        *signbit = 0;
        start_gear = cfg->init_gear;
        end_gear = cfg->field_mask;
        step = 1;
    }

    uint16_t best_vol = 0;
    uint16_t min_diff = 0xFFFF;

    for(int8_t i=start_gear;i!=(end_gear+step);i+=step)
    {
        reg_val = base_val | ((uint16_t)i << cfg->field_shift);

        tx_buf[0] = reg_val & 0xFF;
        tx_buf[1] = reg_val >> 8;
        ret = i2c_write_phy(cfg->phy_addr, 0x00, tx_buf[0], tx_buf[1]);
        if(ret != 0)	return -1;
        delay_ms(1);

        adc_data = ad7998_readcvsdata(DUT0_IIC_1_ADDR, ADC2_ADDR, cfg->adc_ch, AVG_NUM);
        xil_printf("dut0 cc_adc_data = %d, i = %d\r\n", adc_data, i);

        uint16_t diff = abs((int)adc_data - cfg->ref);

        if(diff < min_diff)
        {
            min_diff = diff;
            *offset = i;
            best_vol = adc_data;
        }
    }
    xil_printf("dut0 signbit = %x, best_offset = %x\r\n", *signbit, *offset);

    if((best_vol < cfg->min) || (best_vol > cfg->max))	return -1;

    return 0;
}
