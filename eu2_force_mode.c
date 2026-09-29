#include "platform.h"

int8_t eu2_force_mode_enter(uint8_t test_mode, uint8_t test_en)
{
    int8_t ret;

    ret = i2c_write_eu2(0x02, 0x01);  		if(ret != 0)	return -1;	delay_ms(1);
    ret = i2c_write_eu2(0x03, 0x00);  		if(ret != 0)	return -1; 	delay_ms(1);
    ret = i2c_write_eu2(0x04, 0x00);  		if(ret != 0) 	return -1; 	delay_ms(1);
    ret = i2c_write_eu2(0x02, 0x00);  		if(ret != 0) 	return -1; 	delay_ms(1);
    ret = i2c_write_eu2(0x04, 0x00);  		if(ret != 0) 	return -1; 	delay_ms(1);
    ret = i2c_write_eu2(0x03, test_mode);	if(ret != 0) 	return -1; 	delay_ms(1);
    ret = i2c_write_eu2(0x03, test_en);		if(ret != 0) 	return -1; 	delay_ms(1);

    return 0;
}

int8_t eu2_force_mode_reset()
{
    int8_t ret;

    ret = i2c_write_eu2(0x02, 0x01);		if(ret != 0)	return -1;	delay_ms(1);
    ret = i2c_write_eu2(0x03, 0x00);		if(ret != 0)	return -1;	delay_ms(1);
    ret = i2c_write_eu2(0x04, 0x00);		if(ret != 0)	return -1;	delay_ms(1);
    ret = i2c_write_eu2(0x02, 0x00);		if(ret != 0)	return -1;	delay_ms(1);

    return 0;
}
