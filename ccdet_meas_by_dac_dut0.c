#include "platform.h"

uint16_t ccdet_meas_by_dac_dut0(uint8_t gear, const ccdet_calib_config_t *cfg, uint8_t cc_id)
{
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	uint16_t base_val;
	uint16_t reg_val;

	int8_t ret;

	ret = i2c_read_phy((cfg->vref_reg) & 0xFF, (cfg->vref_reg) >> 8, rx_buf);
	if(ret != 0)	return -1;

	base_val  = (rx_buf[1] << 8) | rx_buf[0];
	base_val &= ~(VREF_MASK << cfg->vref_shift);
	reg_val = base_val | (gear << cfg->vref_shift);

	tx_buf[0] = reg_val & 0xFF;
    tx_buf[1] = reg_val >> 8;
	ret = i2c_write_phy((cfg->vref_reg) & 0xFF, (cfg->vref_reg) >> 8, tx_buf[0], tx_buf[1]);
	if(ret != 0)	return -1;
	delay_ms(1);

    uint16_t dac_low = 0;
    uint16_t dac_high = 4095;
    uint16_t dac_best = 0;

    while(dac_low <= dac_high)
    {
        uint16_t dac_mid = dac_low + (dac_high - dac_low) / 2;
//		xil_printf("dac_low = %x, dac_high = %x, dac_mid = %x\r\n", dac_low, dac_high, dac_mid);

        if(cc_id == 1)
        {
            i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, CC1_LV_DAC1_CH, dac_mid);
        }
        else
        {
            i2c_max5825_output(DUT0_IIC_1_ADDR, DAC_MAX5825_ADDR1, CC2_LV_DAC1_CH, dac_mid);
        }
		delay_ms(10);

        ret = i2c_read_mem(0xA9, 0xE0, rx_buf);
		if(ret != 0)	return -1;

		uint8_t bit_status = (rx_buf[0] >> cfg->check_bit) & 0x01;
        if(bit_status == 1)
        {
            dac_best = dac_mid;

            if(dac_mid == 0) break;

            dac_high = dac_mid - 1;
        }
        else
        {
            dac_low = dac_mid + 1;
        }
    }

    return dac_best;
}
