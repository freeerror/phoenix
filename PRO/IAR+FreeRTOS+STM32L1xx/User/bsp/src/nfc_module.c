#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名: nfc_read_info_flag(nfc_info_flag_t *nfc_info_flag)
*	功能说明: 读nfc改写索引
*	形    参:  nfc_info_flag_t *nfc_info_flag
*	返 回 值: 无
*********************************************************************************************************
*/

bool nfc_read_info_flag(nfc_info_flag_t *nfc_info_flag)
{
    nt3h1101_result_t result;
    //DBG_ASSERT(nfc_info_flag != NULL __DBG_LINE);
    result = hal_nfc_read_one_block(NFC_INFO_FLAG_ADDR);
    if(result == NT3H1101_OK)
    {
        if(result.block_bytes[0] & 0x01 == 1) nfc_info_flag->out_package_bar_code = 1;
        else nfc_info_flag->out_package_bar_code = 0;
        
        nfc_info_flag->out_package_bar_code =   (result.block_bytes[0] == 0x01) ? 1:0;
        nfc_info_flag->box_number = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->unbind = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->sub_box_number = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->read_dev_log = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->profile_id = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->user_num = (result.block_bytes[1] == 0x01) ? 1:0;
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}


bool nfc_read_info()
{

}

bool nfc_clear_info_flag()
{
}

