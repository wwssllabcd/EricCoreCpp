#include "stdafx.h"
#include ".\modetype.h"

ModeType::ModeType(int value)
:_value(value)
{
}

ModeType::~ModeType(void)
{
}

ModeType ModeType::null(void){
	return ModeType(0);
}

ModeType ModeType::byteMode(void){
	return ModeType(1);
}

ModeType ModeType::wordMode(void){
	return ModeType(2);
}

bool ModeType::operator ==(const ModeType& rhs) const{
	return (this->_value == rhs._value);
}

string ModeType::toString(void) const{
	string msg;
	switch(_value){
		case 0:
			msg = "Null Type";
			break;
		case 1:
			msg = "Byte Mode";
			break;
		case 2:
			msg = "Word Mode";
			break;
		default:
			msg = "UnKnow Type";			
			break;
	}
	return msg;
}

