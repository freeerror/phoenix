#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名: nfc_read_info_flag(nfc_info_flag_t *nfc_info_flag)
*	功能说明: 读nfc改写索引
*	形    参:  nfc_info_flag_t *nfc_info_flag，输出参数，对应索引标志位是否被改写
*	返 回 值: 操作是否成功
*********************************************************************************************************
*/

bool nfc_read_info_flag(nfc_info_flag_t *nfc_info_flag)
{
    nt3h1101_result_t result;
    //DBG_ASSERT(nfc_info_flag != NULL __DBG_LINE);
    result = hal_nfc_read_one_block(NFC_INFO_FLAG_ADDR);
    if(result.nt3h1101_status == NT3H1101_OK)
    {
        nfc_info_flag->out_package_bar_code =   (result.block_bytes[0] == 0x01) ? 1:0;
        nfc_info_flag->box_number = (result.block_bytes[1] == 0x01) ? 1:0;
        nfc_info_flag->unbind = (result.block_bytes[2] == 0x01) ? 1:0;
        nfc_info_flag->sub_box_number = (result.block_bytes[3] == 0x01) ? 1:0;
        nfc_info_flag->read_dev_log = (result.block_bytes[4] == 0x01) ? 1:0;
        nfc_info_flag->profile_id = (result.block_bytes[5] == 0x01) ? 1:0;
        nfc_info_flag->user_num = (result.block_bytes[6] == 0x01) ? 1:0;
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}


bool nfc_read_info(nfc_data_info_t *nfc_data_info)
{

}

bool nfc_clear_info_flag(nfc_info_flag_t *nfc_info_flag)
{
	uint8_t block_bytes[NT3H1101OneBlockBytes];
	nt3h1101_status_t nt3h1101_status;

	if(nfc_info_flag->out_package_bar_code == 0x00)
    {
    	block_bytes[0] = 0x00;
	}

	if(nfc_info_flag->box_number == 0x00)
	{
		block_bytes[1] = 0x00;
	}

	if(nfc_info_flag->unbind == 0x00)
	{
		block_bytes[2] = 0x00;
	}

	if(nfc_info_flag->sub_box_number == 0x00)
	{
		block_bytes[3] = 0x00;
	}

	if(nfc_info_flag->read_dev_log == 0x00)
	{
		block_bytes[4] = 0x00;
	}

	if(nfc_info_flag->profile_id == 0x00)
	{
		block_bytes[5] = 0x00;
	}

	if(nfc_info_flag->user_num == 0x00)
	{
		block_bytes[6] = 0x00;
	}

	nt3h1101_status = nt3h1101_write_one_block(NFC_INFO_FLAG_ADDR,block_bytes[NT3H1101OneBlockBytes]);

	if(nt3h1101_status == NT3H1101_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

