#include "platform.h"

int8_t pattern_vcon_sw_ocp_dut0()
{
	int8_t i;

//	uint8_t tx_data;
	uint8_t tx_buf[10];
	uint8_t rx_buf[10];

	int8_t ret;

	switch(dut0.g_type_c_pattern_step)
	{
	case 0x00:
	{
		//enable IBIAS
		ret = i2c_read_phy(0x05, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 15);
		uint16_t reg_val = base_val | (1 << 15);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x05, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//set OCP debounce time(400ms)
		ret = i2c_write_mem(0xB0, 0xE0, 0xA3);
		if(ret != 0)	return -1;
		delay_ms(1);

		//set SCP debounce time(400ms)
		ret = i2c_write_mem(0xB1, 0xE0, 0xA3);
		if(ret != 0)	return -1;
		delay_ms(1);

		//SCP test mode, without current limit
		ret = i2c_read_phy(0x06, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 13);
		reg_val = base_val | (1 << 13);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x06, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x01:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//relay control
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 24, 0);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 23, 1);	//VCON 5V

			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 8, 0);		//CC1 floating
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 17, 0);	//CC2 floating

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x02:
	{
		//CC1 VCON_SW on
		ret = i2c_write_mem(0xA2, 0xE0, 0x0C);
		if(ret != 0)	return -1;
		delay_ms(1);

		//enable CC1 OCP offset trim
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(1 << 3);
		uint16_t reg_val = base_val | (1 << 3);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//start CC1 OCP offset trim
		for(i=0;i<16;i++)
		{
			ret = i2c_read_phy(0x2C, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 12);
			reg_val = base_val | (i << 12);
			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x2C, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read flag
			ret = i2c_read_phy(0x04, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			xil_printf("dut0 status_val = %x\r\n", base_val);
			uint8_t ocp_kos_flag = (base_val >> 3) & 0x01;

			if(ocp_kos_flag == 0)
			{
				if(i == 0)	return -1;

				dut0.g_cc1_ocp_kos_offset = i;
				break;
			}
		}
		if(i == 16)	return -1;

		xil_printf("dut0 cc1_ocp_kos_offset = %x\r\n", dut0.g_cc1_ocp_kos_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x03:
	{
		//CC2 VCON_SW on
		ret = i2c_write_mem(0xA3, 0xE0, 0x0C);
		if(ret != 0)	return -1;
		delay_ms(1);

		//enable CC2 OCP offset trim
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~((1 << 3) | (1 << 2));
		uint16_t reg_val = base_val | (1 << 2);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		//start CC2 OCP offset trim
		for(i=0;i<16;i++)
		{
			ret = i2c_read_phy(0x2C, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 8);
			reg_val = base_val | (i << 8);
			xil_printf("dut0 reg_val = %x\r\n", reg_val);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x2C, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read flag
			ret = i2c_read_phy(0x04, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			xil_printf("dut0 status_val = %x\r\n", base_val);
			uint8_t ocp_kos_flag = (base_val >> 2) & 0x01;

			if(ocp_kos_flag == 0)
			{
				if(i == 0)	return -1;

				dut0.g_cc2_ocp_kos_offset = i;
				break;
			}
		}
		if(i == 16)	return -1;

		xil_printf("dut0 cc2_ocp_kos_offset = %x\r\n", dut0.g_cc2_ocp_kos_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x04:
	{
		//disable OCP offset trim
		ret = i2c_read_phy(0x08, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~((1 << 3) | (1 << 2));
		xil_printf("dut0 base_val = %x\r\n", base_val);

		tx_buf[0] = base_val & 0xFF;
		tx_buf[1] = base_val >> 8;
		ret = i2c_write_phy(0x08, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		ret = i2c_write_mem(0xA2, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);
		ret = i2c_write_mem(0xA3, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		//write CC1 OCP offset 15
		ret = i2c_read_phy(0x07, 0x00, rx_buf);
		if(ret != 0)	return -1;

		base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(0xF << 12);
		uint16_t reg_val = base_val | (15 << 12);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x05:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//CC1 350ma loading
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 4, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 5, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 6, 0);

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			//CC1 VCON_SW on
			ret = i2c_write_mem(0xA2, 0xE0, 0x0C);
			if(ret != 0)	return -1;

			delay_ms(10);

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x06:
	{
		//start CC1 OCP 350ma trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x07, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 12);
			uint16_t reg_val = base_val | (i << 12);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read SCP flag
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t scp_flag = (base_val >> 3) & 0x01;

			//read OCP flag
			ret = i2c_read_phy(0x05, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t ocp_flag = (base_val >> 6) & 0x01;
			xil_printf("i = %02d, dut0 scp_flag = %x, ocp_flag = %x\r\n", i, scp_flag, ocp_flag);

			if(ocp_flag == 1)
			{
				if(i == 15)	return -1;

				dut0.g_cc1_ocp_350ma_offset = i + 1;
				break;
			}
		}
		if(i == -1)	return -1;

		xil_printf("dut0 cc1_ocp_350ma_offset = %x\r\n\r\n", dut0.g_cc1_ocp_350ma_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x07:
	{
		//CC1 VCON_SW off
		ret = i2c_write_mem(0xA2, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		//write CC1 OCP offset 15
		ret = i2c_read_phy(0x07, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(0xF << 12);
		uint16_t reg_val = base_val | (15 << 12);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x08:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//CC1 500ma loading
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 4, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 5, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 6, 1);

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			//CC1 VCON_SW on
			ret = i2c_write_mem(0xA2, 0xE0, 0x0C);
			if(ret != 0)	return -1;
			delay_ms(10);

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x09:
	{
		//start CC1 OCP 500ma trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x07, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 12);
			uint16_t reg_val = base_val | (i << 12);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read SCP flag
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t scp_flag = (base_val >> 3) & 0x01;

			//read OCP flag
			ret = i2c_read_phy(0x05, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t ocp_flag = (base_val >> 6) & 0x01;
			xil_printf("i = %02d, dut0 scp_flag = %x, ocp_flag = %x\r\n", i, scp_flag, ocp_flag);

			if(ocp_flag == 1)
			{
				if(i == 15)	return -1;

				dut0.g_cc1_ocp_500ma_offset = i + 1;
				break;
			}
		}
		if(i == -1)	return -1;

		xil_printf("dut0 cc1_ocp_500ma_offset = %x\r\n\r\n", dut0.g_cc1_ocp_500ma_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0A:
	{
		//CC1 VCON_SW off
		ret = i2c_write_mem(0xA2, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		//CC1 floating
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 4, 0);
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 5, 0);
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 6, 0);

		//write CC2 OCP offset 15
		ret = i2c_read_phy(0x07, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(0xF << 8);
		uint16_t reg_val = base_val | (15 << 8);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0B:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//CC2 350ma loading
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 13, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 14, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 15, 0);

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			//CC2 VCON_SW on
			ret = i2c_write_mem(0xA3, 0xE0, 0x0C);
			if(ret != 0)	return -1;

			delay_ms(10);

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x0C:
	{
		//start CC2 OCP 350ma trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x07, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 8);
			uint16_t reg_val = base_val | (i << 8);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read SCP flag
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t scp_flag = (base_val >> 2) & 0x01;

			//read OCP flag
			ret = i2c_read_phy(0x05, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t ocp_flag = (base_val >> 5) & 0x01;
			xil_printf("i = %02d, dut0 scp_flag = %x, ocp_flag = %x\r\n", i, scp_flag, ocp_flag);

			if(ocp_flag == 1)
			{
				if(i == 15)	return -1;

				dut0.g_cc2_ocp_350ma_offset = i + 1;
				break;
			}
		}
		if(i == -1)	return -1;

		xil_printf("dut0 cc2_ocp_350ma_offset = %x\r\n\r\n", dut0.g_cc2_ocp_350ma_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0D:
	{
		//CC2 VCON_SW off
		ret = i2c_write_mem(0xA3, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		//write CC2 OCP offset 15
		ret = i2c_read_phy(0x07, 0x00, rx_buf);
		if(ret != 0)	return -1;

		uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
		base_val &= ~(0xF << 8);
		uint16_t reg_val = base_val | (15 << 8);

		tx_buf[0] = reg_val & 0xFF;
		tx_buf[1] = reg_val >> 8;
		ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
		if(ret != 0)	return -1;
		delay_ms(1);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x0E:
	{
		if(dut0.g_delay2_control_bit == 0)
		{
			//CC2 500ma loading
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 13, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 14, 1);
			xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 15, 1);

			dut0.g_delay2_count = 0;
			dut0.g_delay2_control_bit = 1;
		}
		tmr0delay2_dut0(50);

		if(dut0.g_delay2_done_bit == 1)
		{
			dut0.g_delay2_control_bit = 0;
			dut0.g_delay2_done_bit = 0;

			//CC2 VCON_SW on
			ret = i2c_write_mem(0xA3, 0xE0, 0x0C);
			if(ret != 0)	return -1;

			delay_ms(10);

			dut0.g_type_c_pattern_step++;
		}

		break;
	}

	case 0x0F:
	{
		//start CC2 OCP 500ma trim
		for(i=15;i>-1;i--)
		{
			ret = i2c_read_phy(0x07, 0x00, rx_buf);
			if(ret != 0)	return -1;

			uint16_t base_val = (rx_buf[1] << 8) | rx_buf[0];
			base_val &= ~(0xF << 8);
			uint16_t reg_val = base_val | (i << 8);

			tx_buf[0] = reg_val & 0xFF;
			tx_buf[1] = reg_val >> 8;
			ret = i2c_write_phy(0x07, 0x00, tx_buf[0], tx_buf[1]);
			if(ret != 0)	return -1;
			delay_ms(10);

			//read SCP flag
			ret = i2c_read_phy(0x06, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t scp_flag = (base_val >> 2) & 0x01;

			//read OCP flag
			ret = i2c_read_phy(0x05, 0x00, rx_buf);
			if(ret != 0)	return -1;

			base_val = (rx_buf[1] << 8) | rx_buf[0];
			uint8_t ocp_flag = (base_val >> 5) & 0x01;
			xil_printf("i = %02d, dut0 scp_flag = %x, ocp_flag = %x\r\n", i, scp_flag, ocp_flag);

			if(ocp_flag == 1)
			{
				if(i == 15)	return -1;

				dut0.g_cc2_ocp_500ma_offset = i + 1;
				break;
			}
		}
		if(i == -1)	return -1;

		xil_printf("dut0 cc2_ocp_500ma_offset = %x\r\n\r\n", dut0.g_cc2_ocp_500ma_offset);

		dut0.g_type_c_pattern_step++;
		break;
	}

	case 0x10:
	{
		//CC2 VCON_SW off
		ret = i2c_write_mem(0xA3, 0xE0, 0x04);
		if(ret != 0)	return -1;
		delay_ms(1);

		//CC2 floating
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 13, 0);
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 14, 0);
		xgpio_write_bit(DUT0_GPIO_0_ADDR, 0, 15, 0);

		dut0.g_pattern_timer = 0x5fff;
		dut0.g_type_c_pattern_step = 0;
		dut0.g_type_c_pattern_num++;
		result_output_for_v50(DUT0_GPIO_1_ADDR, dut0.g_type_c_pattern_num);
		print("dut0 pattern_vcon_sw_ocp pass!\r\n\r\n\r\n");

		break;
	}

	default:
		break;
	}

	return 0;
}
