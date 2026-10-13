#ifndef	GPIO_DRV_H_
#define	GPIO_DRV_H_

void xgpio_write_word(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, const uint8_t xgpio_data[]);
void xgpio_write_bit(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, uint8_t xgpio_bit_offset, uint8_t out_state);
uint32_t xgpio_read_bit(uint32_t xgpio_baseaddr, uint8_t xgpio_ch, uint8_t xgpio_bit_offset);

void result_output_for_v50(uint32_t xgpio_baseaddr, uint8_t result_data, uint8_t is_typec);

#endif /* GPIO_DRV_H_ */
