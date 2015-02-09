#pragma once

class ClockType
{
public:
	~ClockType(void)
	{
	}

	static ClockType null(void){
		return ClockType(0);
	}

	static ClockType c85MHz(void){
		return ClockType(1);
	}
	static ClockType c40MHz(void){
		return ClockType(2);
	}

	bool operator ==(const ClockType& rhs) const{
		return (this->_value == rhs._value);
	}

	CString toString(void){
		switch(_value){
			case 0:
				return "Null";
				break;
			case 1:
				return "85MHz";
				break;
			case 2:
				return "40MHz";
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
	ClockType(int value)
		:_value(value)
	{
	}

	int _value;

};
