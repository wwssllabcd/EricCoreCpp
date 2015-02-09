#pragma once

class DiskModeType
{
public:
	~DiskModeType(void)
	{
	}

	static DiskModeType null(void){
		return DiskModeType(0);
	}

	static DiskModeType fix(void){
		return DiskModeType(1);
	}
	static DiskModeType removeable(void){
		return DiskModeType(2);
	}

	bool operator ==(const DiskModeType& rhs) const{
		return (this->_value == rhs._value);
	}

	CString toString(void){
		switch(_value){
			case 0:
				return "Null";
				break;
			case 1:
				return "Fix";
				break;
			case 2:
				return "removeable";
				break;
			default:
				return "UnKnow Type";			
				break;
		}
	}

	int getValue(void){
		return _value;
	}

private:
	DiskModeType(int value)
		:_value(value)
	{
	}

	int _value;

};
