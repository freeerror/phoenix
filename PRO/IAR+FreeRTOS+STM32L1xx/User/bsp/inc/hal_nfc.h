#ifndef __HAL_NFC_H__
#define __HAL_NFC_H__

extern void hal_nfc_init(void);
extern void hal_nfc_int_enable(void);
extern void hal_nfc_int_disable(void);

extern void hal_nfc_power_on(void);
extern void hal_nfc_power_off(void);

extern nt3h1101_status_t hal_nfc_write_one_block(uint8_t block_add,uint8_t block_bytes[NT3H1101OneBlockBytes]);
extern nt3h1101_result_t hal_nfc_read_one_block(uint8_t block_add);
extern nt3h1101_id_t hal_nfc_get_id(void);

#endif