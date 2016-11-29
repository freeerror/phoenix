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
    //nfc_info_flag_t *nfc_info_flag_temp;
    
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

    //nfc_info_flag = nfc_info_flag_temp;
    
    return TRUE;
}

/*
*********************************************************************************************************
*	函 数 名: bool nfc_read_info(nfc_data_info_t *nfc_data_info,uint8_t nfc_app_type)
*	功能说明: 读对应block内的数据
*	形    参: nfc_data_info_t *nfc_data_info:输出参数，返回独到的对应block内的数据
*             uint8_t nfc_app_type:输入参数，输入需要读取的block
*	返 回 值: 操作是否成功
*********************************************************************************************************
*/
bool nfc_read_info(nfc_data_info_t *nfc_data_info,nfc_app_type_t nfc_app_type)
{
    nt3h1101_result_t nt3h1101_result;

    switch(nfc_app_type)
    {
        case NFC_OUT_PACKAGE_BAR_CODE_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_OUT_PACKAGE_BAR_CODE_ADDR);
            memcpy(&(nfc_data_info->out_package_bar_code_value),nt3h1101_result.block_bytes,OUT_PACKAGE_BAR_CODE_VALUE_LEN);
            break;

        case NFC_BOX_NUMBER_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_BOX_NUMBER_ADDR);
            memcpy(&(nfc_data_info->box_number_value),nt3h1101_result.block_bytes,BOX_NUMBER_VALUE_LEN);
            break;

        case NFC_UNBIND_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_UBIND_ADDR);
            memcpy(&(nfc_data_info->unbind_value),nt3h1101_result.block_bytes,UNBIND_VALUE_LEN);
            break;

        case NFC_SUB_BOX_NUMBER_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_SUB_BOX_NUMBER_ADDR);
            memcpy(&(nfc_data_info->sub_box_number_value),nt3h1101_result.block_bytes,SUB_BOX_NUMBER_VALUE_LEN);
            break;

        case NFC_READ_DEV_LOG_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_READ_DEV_LOG_ADDR);
            memcpy(&(nfc_data_info->read_dev_log_value),nt3h1101_result.block_bytes,READ_DEV_LOG_VALUE_LEN);
            break;

        case NFC_PROFILE_ID_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_PROFILE_ID_ADDR);
            memcpy(&(nfc_data_info->profile_id_value),nt3h1101_result.block_bytes,PROFILE_ID_VALUE_LEN);
            break;

        case NFC_USER_NUMBER_TYPE:
            nt3h1101_result = hal_nfc_read_one_block(NFC_USER_NUMBER_ADDR);
            memcpy(&(nfc_data_info->user_num_value),nt3h1101_result.block_bytes,USER_NUM_VALUE_LEN);
            break;

        default:
            break;
    }

    if(nt3h1101_result.nt3h1101_status == NT3H1101_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
*********************************************************************************************************
*	函 数 名: bool nfc_clear_info_flag(uint8_t nfc_app_type)
*	功能说明: 清除对应的标志位
*	形    参: uint8_t nfc_app_type: 输入参数，需要清除的标志位
*	返 回 值: 操作是否成功
*********************************************************************************************************
*/

bool nfc_clear_info_flag(nfc_app_type_t nfc_app_type)
{
	nt3h1101_status_t nt3h1101_status;
    nt3h1101_result_t nt3h1101_result;
    uint8_t block_bytes[NT3H1101OneBlockBytes];

    nt3h1101_result = hal_nfc_read_one_block(NFC_INFO_FLAG_ADDR);

    if(nt3h1101_result.nt3h1101_status == NT3H1101_OK)
    {
        switch(nfc_app_type)
        {
            case NFC_OUT_PACKAGE_BAR_CODE_TYPE:
                nt3h1101_result.block_bytes[0] = 0x00;
                break;

            case NFC_BOX_NUMBER_TYPE:
                nt3h1101_result.block_bytes[1] = 0x00;
                break;

            case NFC_UNBIND_TYPE:
                nt3h1101_result.block_bytes[2] = 0x00;
                break;

            case NFC_SUB_BOX_NUMBER_TYPE:
                nt3h1101_result.block_bytes[3] = 0x00;
                break;

            case NFC_READ_DEV_LOG_TYPE:
                nt3h1101_result.block_bytes[4] = 0x00;
                break;

            case NFC_PROFILE_ID_TYPE:
                nt3h1101_result.block_bytes[5] = 0x00;
                break;

            case NFC_USER_NUMBER_TYPE:
                nt3h1101_result.block_bytes[6] = 0x00;
                break;
        }
    }
    else
    {
        return FALSE;
    }

    memcpy(block_bytes,nt3h1101_result.block_bytes,NT3H1101OneBlockBytes);

	nt3h1101_status = hal_nfc_write_one_block(NFC_INFO_FLAG_ADDR,block_bytes);

	if(nt3h1101_status == NT3H1101_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

