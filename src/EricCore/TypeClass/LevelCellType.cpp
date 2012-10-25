#include "stdafx.h"
#include ".\levelcelltype.h"

LevelCellType::LevelCellType(int value)
:_value(value)
{
}

LevelCellType::~LevelCellType(void)
{
}

LevelCellType LevelCellType::null(void){
	return LevelCellType(0);
}

LevelCellType LevelCellType::SLC(void){
	return LevelCellType(1);
}

LevelCellType LevelCellType::MLC(void){
	return LevelCellType(2);
}

LevelCellType LevelCellType::TLC(void){
	return LevelCellType(3);
}

bool LevelCellType::operator ==(const LevelCellType& rhs) const{
	return (this->_value == rhs._value);
}

bool LevelCellType::operator !=(const LevelCellType& rhs) const{
	bool result = !((*this)==rhs);
	return result;
}

string LevelCellType::toString(void) const {
	string msg;
	switch(_value){
		case 0:
			msg = "Null Level Cell Type";
			break;
		case 1:
			msg = "SLC";
			break;
		case 2:
			msg = "MLC";
			break;
		case 3:
			msg = "TLC";
			break;
		default:
			msg = "UnKnow Level Cell";			
			break;
	}
	return msg;
}