#ifndef __NFC_MODULE_H
#define __NFC_MODULE_H

#define OUT_PACKAGE_BAR_CODE_VALUE_LEN      16
#define BOX_NUMBER_VALUE                    16

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
    uint8_t box_number[BOX_NUMBER_VALUE];
}box_number_number_t;

typedef union
{
    out_package_bar_code_t  out_package_bar_code_value;
    box_number_t    box_number_value;
    


}nfc_data_info_t;



#define NFC_INFO_FLAG_ADDR  0x01

