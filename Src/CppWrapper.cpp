#include "CppWrapper.h"
#include "MyMain.h"

void cppWrapperExtiCallback(uint16_t pin)
{
	MyMain::getInstance()->externalInterruptCallback(pin);
}

void cppWrapperInit()
{
	MyMain::getInstance()->init();
}

void cppWrapperMain()
{
	MyMain::getInstance()->main();
}

void cppWrapperTimCallback(TIM_HandleTypeDef *handle)
{
	MyMain::getInstance()->timCallback(handle);
}

void cppWrapperI2cCpltCallback(I2C_HandleTypeDef *handle, uint8_t error)
{
	MyMain::getInstance()->i2cCompletedCallback(handle, error);
}
