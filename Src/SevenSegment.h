//
// Created by koscum on 01/07/2021.
//

#ifndef AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H
#define AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H


#include "LedBackpack.h"

class SevenSegment : public LedBackpack
{
public:
	SevenSegment(uint16_t address);

	void setDigit(uint8_t position, uint8_t value);

	void setDot(uint8_t position, bool value);

	void setColon(bool value);

	void toggleDot(uint8_t position);

	void toggleColon();

	void clearDigit(uint8_t position);

	void writeColon() const;

private:
	static const uint8_t COLON_ADDR;
	static const uint8_t COLON_BUFFER_POSITION;
	static const uint8_t COLON_ON;
	static const uint8_t DOT_ON;
};


#endif //AIRSOFTGAMECONTROLLER_SEVENSEGMENT_H
