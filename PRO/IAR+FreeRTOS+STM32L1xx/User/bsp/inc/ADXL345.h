#ifndef __ADXL345_H__
#define __ADXL345_H__

#define ADXL345IntPinOnPC15
//#define ADXL345IntPinOnPB2

#define ADXL345ReadAdd         0x3b
#define ADXL345WriteAdd        0x3a

#define ADXL345ScaleFactor          ((float)4.0)
#define ADXL345ThreshActFactor      ((float)62.5)

#define ADXL345DeviceIDAdd          0x00
#define ADXL345DataFormatRegAdd     0x31
#define ADXL345DATAX0RegAdd         0x32
#define ADXL345PowerCTLAdd          0x2D
#define ADXL345IntEnableAdd         0x2E
#define ADXL345IntMapAdd            0x2F
#define ADXL345IntSourceAdd         0x30
#define ADXL345BWRateAdd            0x2C
#define ADXL345IntMapAdd            0x2F
#define ADXL345ThreshActAdd         0x24
#define ADXL345ActInactCtlAdd       0x27
#define ADXL345ActTapStatus         0x2B
#define ADXL345ThreshFFAdd          0x28
#define ADXL345TimeFFAdd            0x29

#define ADXL345FullResolution       0x01
#define ADXL345Range2g              0x00
#define ADXL345Range4g              0x01
#define ADXL345Range8g              0x02
#define ADXL345Range16g             0x03
#define ADXL345StartMeasure         0x01
#define ADXL345StopMeasure          0x00
#define ADXL345DataRdyIntEn         0x01
#define ADXL345BWRate100Hz          0x0A
#define ADXL345NormalOper           0x00
#define ADXL345IntMapPin1           0x00
#define ADXL345ActIntEn             0x01
#define ADXL345ActIntDis            0x00
#define ADXL345FreefallIntDis       0x00
#define ADXL345ActDCEn              0x00
#define ADXL345ThreshFF             0x05
#define ADXL345TimeFF               0x14 //加速度的值至少要有100ms以上的时间小于600mg，才会触发freefall中断
                                         // ThreshFF = ADXL345ThreshFF * 62.5mg; TimeFF = ADXL345TimeFF * 5ms
#define ADXL345FFIntEn              0x01  

extern uint8_t ADXL345IntFlag;


/*
typedef enum
{
	FALSE = 0,
	TRUE = 1,
}bool;
*/
typedef enum
{
	ADXL345_ERR = 0,
	ADXL345_OK,
}adxl345_status_t;

typedef enum
{
  //none = 0x00,
  free_fall = 0x01,
  activity = 0x02,
  single_tap = 0x04,
  double_tap = 0x80,
}adxl345_int_type_t;

typedef struct
{
	float x;
	float y;
	float z;
	adxl345_status_t adxl345_status;
}adxl345_result_t;

typedef struct
{
  adxl345_int_type_t adxl345_int_type;
  adxl345_result_t adxl345_result;
}adxl345_int_result_t;

void adxl345_low_level_init(void);

adxl345_result_t getAcceleration(void);

adxl345_status_t observeActivity(int8_t thresh , bool x, bool y , bool z);
adxl345_int_result_t adxl345_act_freefall_int_process(void);

adxl345_status_t observeFreefall(void);
//adxl345_result_t adxl345_freefallint_process(void);

#endif
