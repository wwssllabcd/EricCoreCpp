#pragma once

#include "../Utility/UsbCmdSet/UsbCmdStruct.h"

class SS_UsbCmdStruct : public UsbCmdStruct
{
public:
	SS_UsbCmdStruct(void){};
	~SS_UsbCmdStruct(void){};


	template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP( description );
		ar & BOOST_SERIALIZATION_NVP( cdb );
		ar & BOOST_SERIALIZATION_NVP( length );
		ar & BOOST_SERIALIZATION_NVP( direction );
	}

};
