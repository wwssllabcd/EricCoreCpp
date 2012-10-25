#include "stdafx.h"
#include ".\TransferType.h"

TransferType::TransferType(void)
:_value(0)
{
}

TransferType::TransferType(int value)
:_value(value)
{
}

TransferType::~TransferType(void)
{
}

int TransferType::getValue(void) const{
	 return _value;
}


TransferType TransferType::null(void){
	 return TransferType(-100);
}


TransferType TransferType::MDMASettingFail(void){
	return TransferType(-1);
}

//PIO
//TransferType TransferType::PIO_Disable(void){
//	return TransferType(1);
//}

TransferType TransferType::PIO_0(void){
	return TransferType(2);
}

TransferType TransferType::PIO_1(void){
	return TransferType(3);
}

TransferType TransferType::PIO_2(void){
	return TransferType(4);
}

TransferType TransferType::PIO_3(void){
	return TransferType(5);
}

TransferType TransferType::PIO_4(void){
	return TransferType(6);
}

TransferType TransferType::PIO_5(void){
	return TransferType(7);
}

TransferType TransferType::PIO_6(void){
	return TransferType(8);
}

//MDMA
//TransferType TransferType::MDMA_Disable(void){
//	return TransferType(11);
//}

TransferType TransferType::MDMA_0(void){
	return TransferType(12);
}

TransferType TransferType::MDMA_1(void){
	return TransferType(13);
}

TransferType TransferType::MDMA_2(void){
	return TransferType(14);
}
//TransferType TransferType::MDMA_3(void){
//	return TransferType(15);
//}
//
//TransferType TransferType::MDMA_4(void){
//	return TransferType(16);
//}

//UDMA
//TransferType TransferType::UDMA_Disable(void){
//	return TransferType(21);
//}

TransferType TransferType::UDMA_0(void){
	return TransferType(22);
}

TransferType TransferType::UDMA_1(void){
	return TransferType(23);
}

TransferType TransferType::UDMA_2(void){
	return TransferType(24);
}

TransferType TransferType::UDMA_3(void){
	return TransferType(25);
}

TransferType TransferType::UDMA_4(void){
	return TransferType(26);
}

TransferType TransferType::UDMA_5(void){
	return TransferType(27);
}

TransferType TransferType::UDMA_6(void){
	return TransferType(28);
}

TransferType TransferType::UDMA_7(void){
	return TransferType(29);
}

CString TransferType::toString(void) const{
	CString s;
	switch(_value){
		case -1:
			s = "Support Multiword DMA mode but setting fail";	
			break;
		case 1:
			s = "PIO disable";			
			break;
		case 2:
			s = "PIO Mode 0";			
			break;
		case 3:
			s = "PIO Mode 1";			
			break;
		case 4:
			s = "PIO Mode 2";			
			break;
		case 5:
			s = "PIO Mode 3";
			break;
		case 6:
			s = "PIO Mode 4";
			break;
		case 7:
			s = "PIO Mode 5";
			break;
		case 8:
			s = "PIO Mode 6";
			break;


		case 11:
			s = "Multiword DMA Disable";
			break;
		case 12:
			s = "Multiword DMA Mode 0";
			break;
		case 13:
			s = "Multiword DMA Mode 1";
			break;
		case 14:
			s = "Multiword DMA Mode 2";
			break;
		//case 15:
		//	s = "Multiword DMA Mode 3";
		//	break;
		//case 16:
		//	s = "Multiword DMA Mode 4";
		//	break;


		case 21:
			s = "Ultra DMA Disable";			
			break;
		case 22:
			s = "Ultra DMA 0";			
			break;
		case 23:
			s = "Ultra DMA 1";			
			break;
		case 24:
			s = "Ultra DMA 2";			
			break;
		case 25:
			s = "Ultra DMA 3";			
			break;
		case 26:
			s = "Ultra DMA 4";			
			break;
		case 27:
			s = "Ultra DMA 5";			
			break;
		case 28:
			s = "Ultra DMA 6";			
			break;
		case 29:
			s = "Ultra DMA 7";			
			break;

		default:
			s = "UnKnow Type";			
			break;
	}
	return s;
}

//
bool TransferType::operator ==(const TransferType& rhs) const {
	return (this->_value == rhs._value);
}

bool TransferType::operator !=(const TransferType& rhs) const {
	return (this->_value != rhs._value);
}

bool TransferType::operator >=(const TransferType& rhs) const {
	return (this->_value >= rhs._value);
}

bool TransferType::isValidPIO(const TransferType& PIOType){
	if( PIOType == null() ){
		return true;
	}
	return (PIOType._value >= TransferType::PIO_0()._value ) && (PIOType._value <=  TransferType::PIO_6()._value );
}

bool TransferType::isValidMDMA(const TransferType& MDMAType){
	if( MDMAType == null() ){
		return true;
	}
	return (MDMAType._value >= TransferType::MDMA_0()._value ) && (MDMAType._value <=  TransferType::MDMA_2()._value );
}

bool TransferType::isValidUDMA(const TransferType& UDMAType){
	if( UDMAType == null() ){
		return true;
	}
	return  (UDMAType._value >= TransferType::UDMA_0()._value) && (UDMAType._value <=  TransferType::UDMA_6()._value);
}


