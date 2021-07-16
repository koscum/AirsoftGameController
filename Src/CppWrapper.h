#ifndef AIRSOFTGAMECONTROLLER_CPPWRAPPER_H
#define AIRSOFTGAMECONTROLLER_CPPWRAPPER_H


#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <stm32f4xx_hal_i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

void cppWrapperExtiCallback(uint16_t pin);
void cppWrapperInit();
void cppWrapperMain();
void cppWrapperTimCallback(TIM_HandleTypeDef *handle);
void cppWrapperI2cCpltCallback(I2C_HandleTypeDef *handle);

#ifdef __cplusplus
}
#endif


#endif //AIRSOFTGAMECONTROLLER_CPPWRAPPER_H
