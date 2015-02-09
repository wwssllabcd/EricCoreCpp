#pragma once

#include <string>
using namespace std;


class BootSec
{
public:
	BootSec(void);
	~BootSec(void);

	BYTE clusterOffset;
	BYTE maxWindow;
	BYTE bootVer;
	BYTE productVer;
	BYTE ispMainVer;
	BYTE ispMinorVer;
	BYTE reg130;
	
	BYTE reg20E;
	BYTE reg20F;
	BYTE HighPowerDevicemA;
	BYTE maxLbTableABS;
	BYTE ispSecureABSPage;
	BYTE ispHIDABSPage;



	BYTE regWordModeCtrl;

	ULONG bootBlock;
	ULONG bootBlock_Backup;
	BYTE ispABSstartPage;

	WORD max_LB;
	BYTE max_SLC_page;
	BYTE max_plane;
	BYTE max_MLC_page;

	BYTE ispBackEndInitABSPage;
	BYTE ispRWModuleABSPage;
	BYTE info_block[6];
	BYTE reserveSpareBlock[16];
	BYTE convertTb[0x40];

	BYTE frontEndConfig1;
	BYTE frontEndConfig2;
	BYTE frontEndConfig3;
	BYTE reg145;

	// for 2092
	BYTE x_isp2ABSPage;

	BYTE bootPage4_7_PageNo;


	ULONG bhBoundary;
	BYTE bhLbaMask;

	void toArray(BYTE* ary64);
	void load(BYTE* array);
	string toString();

};
