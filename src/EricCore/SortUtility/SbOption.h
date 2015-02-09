#pragma once

#include <string>
using namespace std;

#define NULL_INI_VALUE  0xFFFFFFFF

#define NULL_CUSTON_BIN_VALUE  0


class SbOption
{
public:
	SbOption(void);
	~SbOption(void);

	string m_iniPath;
	string m_modelName;

	void save(void);
	void load(void);
	void load(string path);
	bool isValid(string iniPath);

	int pcNo;
	string flashPartNo;
	
	string qcInitBinCode;
	string qcBinCode;
	string qcBinCode2;

	string sortInitBinCode;
	string sortBinCode;
	string sortBinCode2;

	int maxErrorBit;
	int Ecc_Mode;
	//bool multiPlane;
	//bool extInterleave;

	int blockCnt;

	//user Define pattern
	int lowEven;
	int lowOdd;
	int highEven;
	int highOdd;

	bool pettern0;
	bool pettern1;
	bool pettern2;
	bool pettern3;
	bool pettern4;
	bool pettern5;
	bool pettern6;

	bool quickPattern0;
	bool quickPattern1;
	bool quickPattern2;
	bool quickPattern3;
	bool quickPattern4;
	bool quickPattern5;

	bool is_2097;

	//bin Code
	int bin1;
	int bin2;
	int bin3;
	int bin4;
	int bin5;
	int bin6;
	int bin7;
	int bin8;
	int bin9;
	int bin10;
	int bin11;
	int bin12;
	int bin13;
	int bin14;
	int bin15;
	int bin16;

	WORD custom_bin1;
	WORD custom_bin2;
	WORD custom_bin3;
	WORD custom_bin4;
	WORD custom_bin5;
	WORD custom_bin6;
	WORD custom_bin7;
	WORD custom_bin8;
	WORD custom_bin9;
	WORD custom_bin10;
	WORD custom_bin11;
	WORD custom_bin12;
	WORD custom_bin13;
	WORD custom_bin14;
	WORD custom_bin15;
	WORD custom_bin16;


	int makerCode;
	int deviceCode;
	int testType;
	int flashIdCheck;


	int flash_ID3;
	int flash_ID4;
	int flash_ID5;
	int flash_ID6;
	int flash_ID7;
	int flash_ID8;


	BYTE passBin;

	BYTE dieGap;
	BYTE blockGap;
	WORD blkPerDie;


	bool debug;
	bool onCardSort;

};
