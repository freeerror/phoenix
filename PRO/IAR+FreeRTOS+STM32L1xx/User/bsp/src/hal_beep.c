#include "includes.h"

/*
*********************************************************************************************************
*	�� �� ��:hal_beep_init(void)
*	����˵��:��ʼ���������������� 
*	��    ��:��  
*	�� �� ֵ:�� 
*********************************************************************************************************
*/

void hal_beep_init(void)
{
	
}

/*
*********************************************************************************************************
*	�� �� ��:hal_beep(FunctionalState NewState)
*	����˵��:������������ֹͣ����
*	��    ��:NewState:ENABLE,������DISABLE,ֹͣ����  
*	�� �� ֵ:�� 
*********************************************************************************************************
*/

void hal_beep(FunctionalState NewState)
{
	beep(NewState);
}


