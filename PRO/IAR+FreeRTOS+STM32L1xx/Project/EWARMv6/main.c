#include "includes.h"

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);

/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
    /* 硬件初始化 */
    bsp_Init(); 
    
    /* 创建任务 */
    AppTaskCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();

    /* 
      如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
      heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
      #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 17 * 1024 ) )
    */
    while(1);
}

/*
*********************************************************************************************************
*	函 数 名: vTaskTaskUserIF
*	功能说明: 接口消息处理，这里用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskTaskUserIF(void *pvParameters)
{
    while(1)
    {

    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    while(1)
    {

    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 信息处理，这里是用作LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    while(1)
    {

    }
}

int input_hex_to_int(void)
{
    uint8_t i = 0;
    char temp1 = 0,temp2 = 0,temp3 = 0,temp4 = 0;
    int temp = 0;

    while(1)
    {
        scanf("%c",&temp1);
        i++;
        if(temp1 == 0x0D)
        {
            break;
        }
        
        scanf("%c",&temp2);
        i++;
        if(temp2 == 0x0D)
        {
            break;
        }
        
        scanf("%c",&temp3);
        i++;
        if(temp3 == 0x0D)
        {
            break;
        }
        
        scanf("%c",&temp4);
        i++;
        if(temp4 == 0x0D)
        {
            break;
        }
    }   
    switch(i)
    {
        case 2:
            temp = temp1 - 0x30;
            break;

        case 3: 
            temp = (temp1 - 0x30) * 10 + temp2 - 0x30;
            break;

        case 4:
            temp = (temp1 - 0x30) * 100 + (temp2 - 0x30) * 10 + temp3 - 0x30;
            break;

        default:
            temp = 0;
            break;
    }

    return temp;
    
}

/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
  static char step;
  uint8_t i;
  uint32_t device_id;
  uint32_t device_revision;
  uint32_t flash_size;
  uint8_t unique_id[12];
  uint8_t boot_index;
  uint32_t version;
  opt3001_result_t opt3001_result;
  sht20_result_t sht20_result;
  adxl345_result_t adxl345_result;
  nt3h1101_id_t nt3h1101_id;
  int position;
  int color;
  int sequence;
  int brightness;
  nfc_info_flag_t nfc_info_flag;
  nfc_data_info_t nfc_data_info;

	printf("新版箱押卫士硬件测试!\r\n");
	printf("1. 蜂鸣器测试。\r\n");
    printf("2. 系统复位测试。\r\n");
    printf("3. 获取MCU的Device ID。\r\n");
    printf("4. 获取MCU的Revision ID。\r\n");
    printf("5. 获取MCU的Flash Size。\r\n");
    printf("6. 获取MCU的Unique ID。 \r\n");
    printf("7. 获取MCU的Boot Index。\r\n");
    printf("8. 获取版本号。\r\n");
    printf("9. 光敏传感器测试。\r\n");
    printf("A. 温湿度传感器测试。\r\n");
    printf("B. 加速度传感器测试。\r\n");
    printf("C. 获取NFC ID。\r\n");
    printf("D. RGB LED灯测试。\r\n");
    printf("E. NFC读写测试。\r\n");
    while(1)
    {
		scanf("%c",&step);
        printf("%c\r\n",step);
	    switch(step)
		{

			case 0x31:
				hal_beep(ENABLE);
				vTaskDelay(1000 / portTICK_RATE_MS);
				hal_beep(DISABLE);
				break;

            case 0x32:
                hal_system_reset();
                break;

            case 0x33:
                hal_get_device_id(&device_id);
                printf("Device ID = 0x%X\r\n",device_id);
                break;

            case 0x34:
                hal_get_device_revision(&device_revision);
                printf("Device Revision = 0x%X\r\n",device_revision);
                break;

            case 0x35:
                hal_get_flash_size(&flash_size);
                printf("Flash Size = %ld\r\n",flash_size);
                break;

            case 0x36:
                hal_get_unique_id(unique_id);
                printf("Unique ID = 0x");
                for(i = 0;i < 12;i++)
                {
                    printf("%2X",unique_id[i]);
                }
                printf("\r\n");
                break;

            case 0x37:
                hal_get_boot_index(&boot_index);
                printf("Boot Index = %d\r\n",boot_index);
                break;

            case 0x38:
                hal_get_version(&version);
                printf("Version = %d\r\n",version);
                break;

            case 0x39:
                hal_opt3001_reg_init();
                opt3001_result = hal_opt3001_get_lux();
                if(opt3001_result.opt3001_status == OPT3001_OK)
                {
                    printf("Lux = %.2f\r\n",opt3001_result.lux);
                }
                else
                {
                    printf("OPT3001 ERROR!\r\n");
                }
                break;

            case 0x41:
                sht20_result = hal_get_tempRH();
                if(sht20_result.sht20_status == SHT20_OK)
                {
                    printf("Temperature = %.2f℃,RH = %.2f%\r\n",sht20_result.temperature,sht20_result.rh);
                }
                else
                {
                    printf("SHT20 ERROR!\r\n");
                }
                break;

            case 0x42:
                adxl345_result = hal_getAcceleration();
                if(adxl345_result.adxl345_status == ADXL345_OK)
                {
                    printf("X = %.2f, y = %.2f, z = %.2f\r\n",adxl345_result.x,adxl345_result.y,adxl345_result.z);
                }
                else
                {
                    printf("ADXL345 ERROR!\r\n");
                }
                break;

            case 0x43:
                hal_nfc_init();
                hal_nfc_power_on();
                nt3h1101_id = hal_nfc_get_id();
                if(nt3h1101_id.nt3h1101_status == NT3H1101_OK)
                {
                    printf("NFC ID = ");
                    printf("%2X",nt3h1101_id.id_bytes[0]);
                    printf(":");
                    printf("%2X%2X%2X",nt3h1101_id.id_bytes[1],nt3h1101_id.id_bytes[2],nt3h1101_id.id_bytes[3]);
                    printf(":");
                    printf("%2X%2X%2X\r\n",nt3h1101_id.id_bytes[4],nt3h1101_id.id_bytes[5],nt3h1101_id.id_bytes[6]);
                }
                else
                {
                    printf("NT3H1101 ERROR!\r\n");
                }
                hal_nfc_power_off();
                break;

            case 0x44:
                printf("请输入position参数,以回车结束!\r\n");
                printf("0 = right_right,1 = right_left,2 = left_right,3 = left_left,4 = bottom_left,5 = top_left,6 = top_right,7 = bottom_right\r\n");
                position = input_hex_to_int();
                printf("输入的position为%d\r\n",position);

                printf("请输入color参数,以回车结束!\r\n");
                printf("0 = black,32 = red,64 = yellow,96 = white,128 = purple,160 = blue,192 = skyblue,224 = green\r\n");
                color = input_hex_to_int();
                printf("输入的color为%d\r\n",color);

                printf("请输入sequence参数,以回车结束!\r\n");
                sequence = input_hex_to_int();
                printf("输入的sequence为%d\r\n",sequence);     

                printf("请输入brightness参数,以回车结束!\r\n");
                brightness = input_hex_to_int();
                printf("输入的brightness为%d\r\n",brightness);                

                hal_set_led((ws2812_position_t)(position),(ws2812_color_t)(color),(uint8_t)(sequence),(uint8_t)(brightness));

                break;

            case 0x45:
                hal_nfc_init();
                hal_nfc_power_on();                
                //nfc_clear_info_flag(NFC_UNBIND_TYPE);
                nfc_read_info_flag(&nfc_info_flag);
                if(nfc_info_flag.sub_box_number == 0x01)
                {
                    nfc_read_info(&nfc_data_info,NFC_SUB_BOX_NUMBER_TYPE);
                }
                
                vTaskDelay(10 / portTICK_RATE_MS);//NFC断电前要有一段时间保证i2c读写操作可以完成
                hal_nfc_power_off();                
                break;
                
			default:
			    break;
		}

    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
	#if 1
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 512,         		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 512,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 4,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
	#endif
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;
        
	taskDISABLE_INTERRUPTS();
	for( ;; );
}