#include "platform.h"

int8_t sig_oobs_usb_calib_dut0(uint16_t reg_addr, uint8_t mux_sel)
{
	uint8_t reg_val;
	uint8_t usb_lnk_state,usb_sub_state;

	switch(dut0.g_lrd_pattern_substep)
	{
	case 0x00:
	{
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_U2_IDLE_TIME, 0x02);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_U0_IDLE_INACT_TIME);
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_U0_IDLE_INACT_TIME, (reg_val & 0xE0) | 7);
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_IDLE_INACT_TIME1, 12);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_A);
		mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_A, (reg_val & 0xF0) | 15);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX1_B);
		mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX1_B, (reg_val & 0xF0) | 15);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX2_B);
		mdio_master_write(DUT0_MDIO_BASEADDR, OOBS_TESTEN_VCM_SEN_RX2_B, (reg_val & 0xF0) | 15);

		dut0.g_sig_oobs_retry_bit = 0;
		dut0.g_sig_oobs_offset = 3;

		dut0.g_lrd_pattern_substep++;
		break;
	}

	case 0x01:
	{
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);
		mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | dut0.g_sig_oobs_offset);

		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
		reg_val &= ~0x03;
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

		//force U0
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
		reg_val &= ~(1 << 3);
		reg_val |= ((3 << 0) | (mux_sel << 3));
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

		dut0.g_lrd_pattern_substep++;
		break;
	}

	case 0x02:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(200);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			if(dut0.g_sig_oobs_retry_bit == 0)
			{
				dut0.g_lrd_pattern_substep++;
			}
			else
			{
				dut0.g_lrd_pattern_substep = 0x07;
			}
		}
		break;
	}

	case 0x03:
	{
		//disable force
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
		reg_val &= ~0x01;
		mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

		dut0.g_lrd_pattern_substep++;
		break;
	}

	case 0x04:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(15);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	case 0x05:
	{
		reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
		usb_lnk_state = reg_val & 0x03;
		usb_sub_state = (reg_val & 0x70) >> 4;
//		xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

		if(usb_lnk_state == 2)
		{
			xil_printf("dut0.g_sig_oobs_offset =%02d, dut0 USB LSSTM: U2(1st)!\r\n", dut0.g_sig_oobs_offset);

			//retry
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_U0_IDLE_INACT_TIME);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_U0_IDLE_INACT_TIME, (reg_val & 0xE0) | 15);
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_IDLE_INACT_TIME1, 15);

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~0x03;
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			//force U0
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~(1 << 3);
			reg_val |= ((3 << 0) | (mux_sel << 3));
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			dut0.g_lrd_pattern_substep++;
		}
		else
		{
//			xil_printf("dut0.g_sig_oobs_offset =%02d, dut0 USB LSSTM: not in U2!\r\n", dut0.g_sig_oobs_offset);
			if(dut0.g_sig_oobs_offset < 15)
			{
				dut0.g_sig_oobs_offset++;

				reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, reg_addr);
				mdio_master_write(DUT0_MDIO_BASEADDR, reg_addr, (reg_val & 0xF0) | dut0.g_sig_oobs_offset);

				dut0.g_lrd_pattern_substep--;
			}
			else
			{
				return -1;
			}
		}
		break;
	}

	case 0x06:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(200);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			dut0.g_lrd_pattern_substep++;
		}
		break;
	}

	case 0x07:
	{
		if(dut0.g_delay_control_bit == 0)
		{
			//disable force
			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, CFG_MODE_USB);
			reg_val &= ~0x01;
			mdio_master_write(DUT0_MDIO_BASEADDR, CFG_MODE_USB, reg_val);

			dut0.g_delay_count = 0;
			dut0.g_delay_control_bit = 1;
		}
		tmr0delay_dut0(15);

		if(dut0.g_delay_done_bit == 1)
		{
			dut0.g_delay_control_bit = 0;
			dut0.g_delay_done_bit = 0;

			reg_val = mdio_master_read(DUT0_MDIO_BASEADDR, USB_SUB_STATE);
			usb_lnk_state = reg_val & 0x03;
			usb_sub_state = (reg_val & 0x70) >> 4;
//			xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);

			if(usb_lnk_state == 2)
			{
				xil_printf("dut0.g_sig_oobs_offset =%02d, dut0 USB LSSTM: U2(2nd)!\r\n", dut0.g_sig_oobs_offset);

				dut0.g_lrd_pattern_substep = 0;
				return 1;
			}
			else
			{
				if(dut0.g_sig_oobs_retry_bit == 0)
				{
					print("dut0 sig_oobs Vth test (K+1)!\r\n");
					dut0.g_sig_oobs_retry_bit = 1;
					dut0.g_sig_oobs_offset++;

					dut0.g_lrd_pattern_substep = 0x01;
				}
				else
				{
//					xil_printf("usb_lnk_state =%x, usb_sub_state =%x\r\n", usb_lnk_state, usb_sub_state);
					return -1;
				}
			}
		}
		break;
	}
	}

	return 0;
}
