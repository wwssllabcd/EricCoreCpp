#pragma once

class TransferType
{
public:
	~TransferType(void);
	static TransferType null();

	//static TransferType PIO_Disable();
	static TransferType PIO_0();
	static TransferType PIO_1();
	static TransferType PIO_2();
	static TransferType PIO_3();
	static TransferType PIO_4();
	static TransferType PIO_5();
	static TransferType PIO_6();

	//static TransferType MDMA_Disable();
	static TransferType MDMA_0();
	static TransferType MDMA_1();
	static TransferType MDMA_2();
	//static TransferType MDMA_3();
	//static TransferType MDMA_4();

	//static TransferType UDMA_Disable();
	static TransferType UDMA_0();
	static TransferType UDMA_1();
	static TransferType UDMA_2();
	static TransferType UDMA_3();
	static TransferType UDMA_4();
	static TransferType UDMA_5();
	static TransferType UDMA_6();
	static TransferType UDMA_7();

	static TransferType MDMASettingFail(void);

	bool operator==(const TransferType& rhs) const;
	bool operator>=(const TransferType& rhs) const;
	bool operator!=(const TransferType& rhs) const;

	CString toString(void) const;

	static bool isValidPIO(const TransferType& PIOType);
	static bool isValidMDMA(const TransferType& MDMAType) ;	
	static bool isValidUDMA(const TransferType& UDMAType) ;

	int getValue() const;

private:
	TransferType(void);
	TransferType(int value);
	int _value;	
};
