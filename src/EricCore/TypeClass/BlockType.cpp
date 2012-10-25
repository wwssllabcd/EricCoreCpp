#include "stdafx.h"
#include ".\blocktype.h"

BlockType::BlockType(int value)
:_value(value)
{
}

BlockType::~BlockType(void)
{
}

BlockType BlockType::null(void){
	return BlockType(0);
}

BlockType BlockType::smallBlock(void){
	return BlockType(1);
}

BlockType BlockType::largeBlock(void){
	return BlockType(2);
}

bool BlockType::operator ==(const BlockType& rhs) const{
	return (this->_value == rhs._value);
}

string BlockType::toString(void) const{
	string msg;
	switch(_value){
		case 0:
			msg = "Null Block Type";
			break;
		case 1:
			msg = "Small Block";
			break;
		case 2:
			msg = "Large Block";
			break;
		default:
			msg = "UnKnow Block Type";			
			break;
	}
	return msg;
}