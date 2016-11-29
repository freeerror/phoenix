#ifndef __NFC_MODULE_H
#define __NFC_MODULE_H

#define OUT_PACKAGE_BAR_CODE_VALUE_LEN      16
#define BOX_NUMBER_VALUE_LEN                16
#define UNBIND_VALUE_LEN                    16
#define SUB_BOX_NUMBER_VALUE_LEN            16
#define READ_DEV_LOG_VALUE_LEN              16
#define PROFILE_ID_VALUE_LEN                16
#define USER_NUM_VALUE_LEN                  16


typedef enum
{
    NFC_OUT_PACKAGE_BAR_CODE_TYPE = 0x01,
    NFC_BOX_NUMBER_TYPE = 0x02,
    NFC_UNBIND_TYPE = 0x03,
    NFC_SUB_BOX_NUMBER_TYPE = 0x04,
    NFC_READ_DEV_LOG_TYPE = 0x05,
    NFC_PROFILE_ID_TYPE = 0x06,
    NFC_USER_NUMBER_TYPE = 0x07
}nfc_app_type_t;


typedef struct 
{
	uint8_t  out_package_bar_code;    
    uint8_t  box_number;                     
    uint8_t  unbind;                  
    uint8_t  sub_box_number;         
    uint8_t  read_dev_log;      
    uint8_t  profile_id;          
    uint8_t  user_num;            
    uint8_t  reserved;        
} nfc_info_flag_t;


typedef struct
{
    uint8_t out_package_bar_code[OUT_PACKAGE_BAR_CODE_VALUE_LEN];
}out_package_bar_code_t;

typedef struct
{
    uint8_t box_number[BOX_NUMBER_VALUE_LEN];
}box_number_t;

typedef struct
{
	uint8_t unbind[UNBIND_VALUE_LEN];
}unbind_t;

typedef struct
{
	uint8_t sub_box_number[SUB_BOX_NUMBER_VALUE_LEN];
}sub_box_number_t;

typedef struct
{
	uint8_t read_dev_log[READ_DEV_LOG_VALUE_LEN];
}read_dev_log_t;

typedef struct
{
	uint8_t profile_id[PROFILE_ID_VALUE_LEN];
}profile_id_t;

typedef struct
{
	uint8_t user_num[USER_NUM_VALUE_LEN];
}user_num_t;

typedef union
{
    out_package_bar_code_t  out_package_bar_code_value;
    box_number_t            box_number_value;
	unbind_t                unbind_value;
	sub_box_number_t        sub_box_number_value;
    read_dev_log_t          read_dev_log_value;
	profile_id_t            profile_id_value;
	user_num_t              user_num_value;
}nfc_data_info_t;

bool nfc_read_info_flag(nfc_info_flag_t *nfc_info_flag);

bool nfc_read_info(nfc_data_info_t *nfc_data_info,nfc_app_type_t nfc_app_type);

bool nfc_clear_info_flag(nfc_app_type_t nfc_app_type);

#define NFC_INFO_FLAG_ADDR             0x01
#define NFC_OUT_PACKAGE_BAR_CODE_ADDR  0x02
#define NFC_BOX_NUMBER_ADDR            0x03
#define NFC_UBIND_ADDR                 0x04
#define NFC_SUB_BOX_NUMBER_ADDR        0x05
#define NFC_READ_DEV_LOG_ADDR          0x06
#define NFC_PROFILE_ID_ADDR            0x07
#define NFC_USER_NUMBER_ADDR           0X08

#endif

