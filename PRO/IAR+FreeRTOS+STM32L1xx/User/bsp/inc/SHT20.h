#ifndef __SHT20_H__
#define __SHT20_H__

#define SHT20ReadAddress  0x81
#define SHT20WriteAddress 0x80

#define SHT20TrigTMeasureNM  0xF3
#define SHT20TrigRHMeasureNM 0xF5

typedef enum
{
	SHT20_ERR = 0,
	SHT20_OK,
}sht20_status_t;

typedef struct
{
	float temperature;
	float rh;
	sht20_status_t sht20_status;
}sht20_result_t;

extern sht20_result_t getTempRh(void);

#endif
