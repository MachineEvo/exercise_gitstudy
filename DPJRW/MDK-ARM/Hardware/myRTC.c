#include "stm32f1xx_hal.h"
#include "rtc.h"
#include "myRTC.h"

RTC_DateTypeDef GetData;    //获取日期结构体
RTC_TimeTypeDef GetTime;    //获取时间结构体

void Time_Get(void)
{
	HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);
}
