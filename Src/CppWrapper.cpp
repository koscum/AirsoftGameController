#include "CppWrapper.h"
#include "MyMain.h"

void cppWrapperExtiCallback(uint16_t pin)
{
	MyMain::getInstance()->extiCallback(pin);
}

void cppWrapperMain()
{
	MyMain::getInstance()->main();
}

void cppWrapperTimCallback(TIM_HandleTypeDef *handle)
{
	MyMain::getInstance()->timCallback(handle);
}
