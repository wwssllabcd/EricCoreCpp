#include "stdafx.h"
#include ".\drive.h"

Drive::Drive()
:address(0),
name(""),
testNo(0),
status(0)
{}

Drive::~Drive(void)
{
}


bool Drive::operator ==(const Drive& rhs) const{

	if(this->address == rhs.address){
		if(this->name == rhs.name){
			if(this->status == rhs.status){
				if(this->testNo == rhs.testNo){
					return true;
				}
			}
		}
	}
	return false;
}