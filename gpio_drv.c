#include "platform.h"

void xgpio_write_word(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, const uint8_t xgpio_data[])
{
	uint32_t offset = xgpio_ch ? XGPIO_CHAN_OFFSET : 0;
	uint32_t data;
	uint32_t tri;

	data = ((uint32_t)xgpio_data[6] << 24) |
	       ((uint32_t)xgpio_data[4] << 16) |
	       ((uint32_t)xgpio_data[2] << 8)  |
	       ((uint32_t)xgpio_data[0]);

	tri =  ((uint32_t)xgpio_data[7] << 24) |
	       ((uint32_t)xgpio_data[5] << 16) |
	       ((uint32_t)xgpio_data[3] << 8)  |
	       ((uint32_t)xgpio_data[1]);

	XGpio_WriteReg(xgpio_baseaddr, XGPIO_TRI_OFFSET + offset, tri);
	XGpio_WriteReg(xgpio_baseaddr, XGPIO_DATA_OFFSET + offset, data);
}

void xgpio_write_bit(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, uint8_t xgpio_bit_offset, uint8_t out_state)
{
	uint8_t ch_idx = xgpio_ch ? 1 : 0;
	uint32_t offset = ch_idx ? XGPIO_CHAN_OFFSET : 0;
	uint32_t mask = 1 << xgpio_bit_offset;
	uint32_t tri;

	tri = XGpio_ReadReg(xgpio_baseaddr, XGPIO_TRI_OFFSET + offset);
	XGpio_WriteReg(xgpio_baseaddr, XGPIO_TRI_OFFSET + offset, tri & ~mask);

	if(xgpio_baseaddr == DUT0_GPIO_0_ADDR)
	{
	    if(out_state)
	    {
	        dut0.g_gpio_0_shadow_state[ch_idx] |= mask;
	    }
	    else
	    {
	        dut0.g_gpio_0_shadow_state[ch_idx] &= ~mask;
	    }
	//	xil_printf("dut0.g_gpio_0_shadow_state[%x] = 0x%x\r\n", ch_idx, dut0.g_gpio_0_shadow_state[ch_idx]);

		XGpio_WriteReg(xgpio_baseaddr, XGPIO_DATA_OFFSET + offset, dut0.g_gpio_0_shadow_state[ch_idx]);
	}
	else if(xgpio_baseaddr == DUT0_GPIO_1_ADDR)
	{
	    if(out_state)
	    {
	        dut0.g_gpio_1_shadow_state[ch_idx] |= mask;
	    }
	    else
	    {
	        dut0.g_gpio_1_shadow_state[ch_idx] &= ~mask;
	    }
	//	xil_printf("dut0.g_gpio_1_shadow_state[%x] = 0x%x\r\n", ch_idx, dut0.g_gpio_1_shadow_state[ch_idx]);

		XGpio_WriteReg(xgpio_baseaddr, XGPIO_DATA_OFFSET + offset, dut0.g_gpio_1_shadow_state[ch_idx]);
	}
}

uint32_t xgpio_read_bit(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, uint8_t xgpio_bit_offset)
{
	uint32_t offset = xgpio_ch ? XGPIO_CHAN_OFFSET : 0;
	uint32_t mask = 1 << xgpio_bit_offset;
	uint32_t tri;
	uint32_t data;

	tri = XGpio_ReadReg(xgpio_baseaddr, XGPIO_TRI_OFFSET + offset);
	XGpio_WriteReg(xgpio_baseaddr, XGPIO_TRI_OFFSET + offset, tri | mask);

	data = XGpio_ReadReg(xgpio_baseaddr, XGPIO_DATA_OFFSET + offset);

	if(data & mask)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void result_output_for_v50(uint32_t xgpio_baseaddr, uint8_t result_data, uint8_t is_typec)
{
    uint8_t ch_idx = 0;
    uint32_t mask = 0x1F << 10;		//bit[14:10]: 5bit written to tester
    uint32_t tri;
    uint8_t data;

    tri = XGpio_ReadReg(xgpio_baseaddr, XGPIO_TRI_OFFSET);
    XGpio_WriteReg(xgpio_baseaddr, XGPIO_TRI_OFFSET, tri & ~mask);

    //type-c pattern runs in parallel with lrd pattern, so physical bit14(field bit[4])
    //is used as the type-c mark; special codes 0x1d/0x1e/0x1f stay above type-c range
    data = result_data & 0x1F;
    if(is_typec)
    {
        data |= DUT0_PATTERN_TYPEC_FLAG;
    }

    dut0.g_gpio_1_shadow_state[ch_idx] &= ~mask;
    dut0.g_gpio_1_shadow_state[ch_idx] |= ((uint32_t)data << 10);

    XGpio_WriteReg(xgpio_baseaddr, XGPIO_DATA_OFFSET, dut0.g_gpio_1_shadow_state[ch_idx]);
}
