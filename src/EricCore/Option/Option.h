#include <vector>
using namespace std;

#pragma once

class Option
{
public:
	Option(void);
	~Option(void);

	bool fFlashAccessLed;
	bool gf_CDROM;
	bool ReportTrueSecuredCapacity;
	bool fSupportModePage5;
	bool fSupportStringDesc;
	bool fHighPowerDevice;
	bool fBOTDataLenCheck;
	bool f_AES_Support;

	bool gf_cis_sig;
	bool gf_vdr_sig_lun0;
	bool gf_vdr_sig_lun1;
	bool gf_str_sig;
	bool gf_pwd_sig;
	bool gf_hid_En;
	bool gf_lun0_en_cap;
	bool gf_lun1_en_cap;
	bool homogeneous;

	bool secureBank;

	bool skipScanErase;
	bool reboot;
	bool strSigByFile;
	bool strSn;

	bool strSigLun0;

	bool hold_ce;
	bool disableSizeRestrict;
	bool support_15_cmd;
	bool gf_TSB_43nm_D3;
	bool gf_80h_program_cmd;
	bool gf_Xcmd_for_2plane_erase;
	bool gf_sec_shuffle_enable;
	bool stopPreformat;

	bool enableWriteCache;

	WORD eraseSkip;

	bool markScanAllFailBlock;
	bool markEraseAllFailBlock;

	//WORD eraseSkip_0;
	//WORD eraseSkip_1;
	//WORD eraseSkip_2;
	//WORD eraseSkip_3;


	ULONG fixLbaSize;

	bool enableSpMode;

	// chagne SLC/MLC Table
	bool enableExtSlc_mlcTable;
	bool enableAutoPage;
	BYTE maxSLC;
	BYTE maxMLC;


	BYTE cacheNumber;
	BYTE retryReadCnt;

	// manual ID( it's for skip id function)
	bool enableManualId;	
	int manualCeNo;
	BYTE manualMakerCode;
	BYTE manualDeviceCode;	
	BYTE manualID3;
	BYTE manualID4;
	BYTE manualID5;
	BYTE manualID6;
	int manualID_evenDieStart;
	int manualID_evenDieEnd;
	int manualID_oddDieStart;
	int manualID_oddDieEnd;

	BYTE enablePartialPage;
	bool gf_autoCE;

	


	BYTE x_flash_erase_CMD1_P0;
	BYTE x_flash_erase_CMD2_P0 ;
	BYTE x_flash_erase_CMD1_P1;
	BYTE x_flash_erase_CMD2_P1;


	BYTE reg130;
	BYTE reg145;
	BYTE reg20E;
	BYTE reg20F;
	BYTE HighPowerDevicemA;
	BYTE clusterOffset;

	BYTE frontEndVar3;
	WORD OEM_VID;
	WORD OEM_PID;
	BYTE pwdLun0Status;

	BYTE LED_PERIOD;
	BYTE LED_DUTY;

		
	// for 2092 PHY
	BYTE x_sr_tune0;
	BYTE x_sr_tune1;
	BYTE x_sr_tune2;
	BYTE x_sr_tune3;
	BYTE x_sr_tune4;
	BYTE x_sr_tune5;
	BYTE x_sr_tune6;
	BYTE x_sr_tune7;
	BYTE x_sr_tune8;
	BYTE x_sr_tune9;
	BYTE x_sr_swing;
	BYTE x_sr_slew;
	BYTE x_sr_terminateresistor;
	BYTE x_sr_osc_reg;
	BYTE x_sr_480_pll2;
	BYTE x_sr_cfg10;
	WORD x_sr_baseCode;
	WORD x_BulkCntMax;
	BYTE x_SIEResetCntMax;



	int interExterInterleave;
	int publicSize;

	int maxWGroup;

	string cdRomFilePath;
	string sn;

	bool enableMultiCe;
	BYTE sortRecordMap[16];

	vector<WORD> badBlockColl_0;
	vector<WORD> badBlockColl_1;
	vector<WORD> badBlockColl_2;
	vector<WORD> badBlockColl_3;

	// B.H Function
	bool enableExtraCap;
	int extraCapMB;
	ULONG extraLBA;

	// for IM
	BYTE setFeature_Timing;
	BYTE setFeature_DriveStrength;
	BYTE setFeature_PullDownStrength;

	//on card sort
	string onCardSortIniPath;
	BYTE enableRetryWrite;


	BYTE waitTimeBeforeInit;

	bool enableAutoSector;
	BYTE autoSectorMap;

	void load();
	BYTE getFrontEndVar1(void);
	BYTE getFrontEndVar2(void);
	BYTE getFrontEndVar3(void);

	
	void loadUserMarkBadInfo(string modelName, string iniPath);
	void for_manualFlashId(string modelName, string iniPath);
	void for_IM_setFeature(string modelName, string iniPath);
	void for_extendCapacity(string modelName, string iniPath);
	void for_onCardSort(string modelName, string iniPath);
	void for_MultiCe_loadSortRecordInfo(string modelName, string iniPath);
	void for_2092_PHY_Param(string modelName, string iniPath);
	void for_AutoPageMode(string modelName, string iniPath);


};
