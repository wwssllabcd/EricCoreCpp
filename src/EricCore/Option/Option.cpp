#include "StdAfx.h"
#include ".\option.h"
#include "..\Utility\Utility.h"

using namespace EricCore;

Option::Option(void)
:fFlashAccessLed(0)
,gf_CDROM(0)
,ReportTrueSecuredCapacity(0)
,fSupportModePage5(0)
,fSupportStringDesc(0)
,fHighPowerDevice(0)
,fBOTDataLenCheck(0)
,f_AES_Support(0)

,x_flash_erase_CMD1_P0(0)
,x_flash_erase_CMD2_P0(0)
,x_flash_erase_CMD1_P1(0)
,x_flash_erase_CMD2_P1(0)
,x_BulkCntMax(0)
,x_SIEResetCntMax(0)


,gf_cis_sig(0)
,gf_vdr_sig_lun0(0)
,gf_vdr_sig_lun1(0)
,gf_str_sig(0)
,gf_pwd_sig(0)
,gf_hid_En(0)
,gf_lun0_en_cap(0)
,gf_lun1_en_cap(0)
,skipScanErase(0)
,frontEndVar3(0)
,OEM_VID(0)
,OEM_PID(0)
,homogeneous(0)
,reg130(0)
,reg145(0)
,reg20E(0)
,reg20F(0)
,HighPowerDevicemA(0)
,clusterOffset(0)
,interExterInterleave(0)
,cdRomFilePath("")
,publicSize(0)
,pwdLun0Status(0)
,reboot(0)
,strSigByFile(0)
,strSigLun0(0)
,strSn(0)
,sn("")
,secureBank(0)
,hold_ce(false)
,support_15_cmd(false)
,disableSizeRestrict(false)
,enableMultiCe(false)
,stopPreformat(false)
,enableWriteCache(false)

,gf_TSB_43nm_D3(0)
,gf_80h_program_cmd(0)
,gf_Xcmd_for_2plane_erase(0)
,gf_sec_shuffle_enable(0)

,cacheNumber(0)					/// 2009.05.21 Paul for turbo Jr.
,retryReadCnt(0)
,maxWGroup(4)

,enablePartialPage(0xFF)

//manual Flash ID code
,enableManualId(false)
,manualCeNo(0)
,manualMakerCode(0xFF)
,manualDeviceCode(0xFF)
,manualID3(0xFF)
,manualID4(0xFF)
,manualID5(0xFF)
,manualID6(0xFF)

,manualID_evenDieStart(0)
,manualID_evenDieEnd(0)
,manualID_oddDieStart(0)
,manualID_oddDieEnd(0)

//extend capacity
,enableExtraCap(0)
,extraCapMB(0)
,extraLBA(0)

// for IM
,setFeature_Timing(0)
,setFeature_DriveStrength(0)
,setFeature_PullDownStrength(0)


//on card sort
,enableRetryWrite(0)
,waitTimeBeforeInit(0x80)

//
,eraseSkip(0x10)

,markScanAllFailBlock(false)
,markEraseAllFailBlock(false)

//,eraseSkip_0(0x10)
//,eraseSkip_1(0x10)
//,eraseSkip_2(0x10)
//,eraseSkip_3(0x10)

,fixLbaSize(0)

,LED_PERIOD(0)
,LED_DUTY(0)

, x_sr_tune0(0)
, x_sr_tune1(0)
, x_sr_tune2(0)
, x_sr_tune3(0)
, x_sr_tune4(0)
, x_sr_tune5(0)
, x_sr_tune6(0)
, x_sr_tune7(0)
, x_sr_tune8(0)
, x_sr_tune9(0)
, x_sr_swing(0)
, x_sr_slew(0)
,x_sr_terminateresistor(0)
,x_sr_osc_reg(0)
,x_sr_cfg10(0)
,x_sr_480_pll2(0)
,x_sr_baseCode(0)

,enableSpMode(0)

// for auto page mode
,enableAutoPage(false)
,maxSLC(0)
,maxMLC(0)

,enableExtSlc_mlcTable(false)
//,defectOffset(0x3b)

,enableAutoSector(false)
,autoSectorMap(0xFF)

,gf_autoCE(false)
{
	memset(sortRecordMap, 0x0, sizeof(sortRecordMap));

}

Option::~Option(void)
{
}

#define NULL_INI_VALUE 0
void Option::load(){
	string modelName("Default");
	string iniPath =Utility::getCurrnetPath() + "//UsbPreformat.ini";
	
	this->gf_CDROM          = Utility::getBoolParamFromIni(modelName, "gf_CDROM", NULL_INI_VALUE, iniPath);
	this->f_AES_Support     = Utility::getBoolParamFromIni(modelName, "f_AES_Support", NULL_INI_VALUE, iniPath);
	this->fFlashAccessLed   = Utility::getBoolParamFromIni(modelName, "fFlashAccessLed", NULL_INI_VALUE, iniPath);
	this->fHighPowerDevice  = Utility::getBoolParamFromIni(modelName, "fHighPowerDevice", NULL_INI_VALUE, iniPath);
	this->fBOTDataLenCheck  = Utility::getBoolParamFromIni(modelName, "fBOTDataLenCheck", 1, iniPath);
	
	this->fSupportModePage5 = Utility::getBoolParamFromIni(modelName, "fSupportModePage5", NULL_INI_VALUE, iniPath);
	this->fSupportStringDesc = Utility::getBoolParamFromIni(modelName, "fSupportStringDesc", 1, iniPath);
	this->ReportTrueSecuredCapacity = Utility::getBoolParamFromIni(modelName, "ReportTrueSecuredCapacity", 1, iniPath);
	
	this->gf_cis_sig = Utility::getBoolParamFromIni(modelName, "gf_cis_sig", NULL_INI_VALUE, iniPath);
	this->strSigLun0 = Utility::getBoolParamFromIni(modelName, "strSigLun0", NULL_INI_VALUE, iniPath);
	this->gf_vdr_sig_lun0 = Utility::getBoolParamFromIni(modelName, "gf_vdr_sig_lun0", 1, iniPath);
	this->gf_vdr_sig_lun1 = Utility::getBoolParamFromIni(modelName, "gf_vdr_sig_lun1", NULL_INI_VALUE, iniPath);
	this->strSigByFile = Utility::getBoolParamFromIni(modelName, "strSigByFile", NULL_INI_VALUE, iniPath);
	this->gf_str_sig = Utility::getBoolParamFromIni(modelName, "gf_str_sig", 1, iniPath);
	this->gf_pwd_sig = Utility::getBoolParamFromIni(modelName, "gf_pwd_sig", NULL_INI_VALUE, iniPath);
	this->gf_hid_En = Utility::getBoolParamFromIni(modelName, "gf_hid_En", 0, iniPath);
	this->gf_lun0_en_cap = Utility::getBoolParamFromIni(modelName, "gf_lun0_en_cap", NULL_INI_VALUE, iniPath);
	this->gf_lun1_en_cap = Utility::getBoolParamFromIni(modelName, "gf_lun1_en_cap", NULL_INI_VALUE, iniPath);
	this->skipScanErase = Utility::getBoolParamFromIni(modelName, "skipScanErase", NULL_INI_VALUE, iniPath);





	this->frontEndVar3 = Utility::getIntParamFromIni(modelName, "FrontEndVar3", NULL_INI_VALUE, iniPath);



	this->OEM_VID = Utility::getIntParamFromIni(modelName, "OEM_VID", NULL_INI_VALUE, iniPath);
	this->OEM_PID = Utility::getIntParamFromIni(modelName, "OEM_PID", NULL_INI_VALUE, iniPath);
	this->homogeneous = Utility::getBoolParamFromIni(modelName, "homogeneous", NULL_INI_VALUE, iniPath);
	this->reg130 = Utility::getIntParamFromIni(modelName, "reg130", 0x02, iniPath);
	this->reg145 = Utility::getIntParamFromIni(modelName, "reg145", 0x80, iniPath);
	this->reg20E = Utility::getIntParamFromIni(modelName, "reg20E", 0x01, iniPath);
	this->reg20F = Utility::getIntParamFromIni(modelName, "reg20F", 0, iniPath);
	this->HighPowerDevicemA = Utility::getIntParamFromIni(modelName, "HighPowerDevicemA", 0x64, iniPath);
	this->clusterOffset = Utility::getIntParamFromIni(modelName, "clusterOffset", 0, iniPath);
	this->interExterInterleave = Utility::getIntParamFromIni(modelName, "interExterInterleave", -1, iniPath);
	this->publicSize = Utility::getIntParamFromIni(modelName, "publicSize", -1, iniPath);
	this->pwdLun0Status = Utility::getIntParamFromIni(modelName, "pwdLun0Status", 0, iniPath);
	this->reboot = Utility::getBoolParamFromIni(modelName, "reboot", 0, iniPath);
	this->gf_TSB_43nm_D3 = Utility::getBoolParamFromIni(modelName, "gf_TSB_43nm_D3", 0, iniPath);
	this->gf_80h_program_cmd = Utility::getBoolParamFromIni(modelName, "gf_80h_program_cmd", 0, iniPath);
	this->gf_Xcmd_for_2plane_erase = Utility::getBoolParamFromIni(modelName, "gf_Xcmd_for_2plane_erase", 0, iniPath);
	this->gf_sec_shuffle_enable = Utility::getBoolParamFromIni(modelName, "gf_sec_shuffle_enable", 0, iniPath);

	this->x_flash_erase_CMD1_P0 = Utility::getIntParamFromIni(modelName, "x_flash_erase_CMD1_P0", 0, iniPath);
	this->x_flash_erase_CMD2_P0 = Utility::getIntParamFromIni(modelName, "x_flash_erase_CMD2_P0", 0, iniPath);
	this->x_flash_erase_CMD1_P1 = Utility::getIntParamFromIni(modelName, "x_flash_erase_CMD1_P1", 0, iniPath);
	this->x_flash_erase_CMD2_P1 = Utility::getIntParamFromIni(modelName, "x_flash_erase_CMD2_P1", 0, iniPath);

	this->strSn = Utility::getBoolParamFromIni(modelName, "strSn", NULL_INI_VALUE, iniPath);
	this->secureBank = Utility::getBoolParamFromIni(modelName, "secureBank", NULL_INI_VALUE, iniPath);

	this->hold_ce = Utility::getBoolParamFromIni(modelName, "hold_ce", NULL_INI_VALUE, iniPath);
	this->support_15_cmd = Utility::getBoolParamFromIni(modelName, "support_15_cmd", NULL_INI_VALUE, iniPath);
	this->disableSizeRestrict = Utility::getBoolParamFromIni(modelName, "disableSizeRestrict", NULL_INI_VALUE, iniPath);
	this->cacheNumber = Utility::getIntParamFromIni(modelName, "cacheNumber", 0x00, iniPath);							///2009.05.21 Paul cache blcok No. for turbo-mode
	this->retryReadCnt = Utility::getIntParamFromIni(modelName, "retryReadCnt", 0x01, iniPath);

	this->stopPreformat = Utility::getBoolParamFromIni(modelName, "stopPreformat", 0x00, iniPath);		
	this->enableWriteCache = Utility::getBoolParamFromIni(modelName, "enableWriteCache", 0x00, iniPath);	
	this->waitTimeBeforeInit = Utility::getIntParamFromIni(modelName, "waitTimeBeforeInit", 0x80, iniPath);
	this->enableExtSlc_mlcTable = Utility::getBoolParamFromIni(modelName, "enableExtSlc_mlcTable", 0x0, iniPath);
	this->eraseSkip = Utility::getIntParamFromIni(modelName, "eraseSkip", 0x10, iniPath);
	this->fixLbaSize = Utility::getIntParamFromIni(modelName, "fixLbaSize", 0, iniPath);
	this->LED_PERIOD = Utility::getIntParamFromIni(modelName, "LED_PERIOD", 0, iniPath);
	this->LED_DUTY = Utility::getIntParamFromIni(modelName, "LED_DUTY", 0, iniPath);

	for_manualFlashId(modelName, iniPath);
	for_IM_setFeature(modelName, iniPath);
	for_extendCapacity(modelName, iniPath);
	for_onCardSort(modelName, iniPath);
	for_MultiCe_loadSortRecordInfo(modelName, iniPath);
	for_2092_PHY_Param(modelName, iniPath);
	for_AutoPageMode(modelName, iniPath);

	this->markScanAllFailBlock = Utility::getBoolParamFromIni(modelName, "markScanAllFailBlock", 0, iniPath);
	this->markEraseAllFailBlock = Utility::getBoolParamFromIni(modelName, "markEraseAllFailBlock", 0, iniPath);

	//enablePartialPage =0 ==> FULL page (default)
    this->enablePartialPage = Utility::getIntParamFromIni(modelName, "enablePartialPage", 0xFF, iniPath);
	this->enableRetryWrite = Utility::getIntParamFromIni(modelName, "enableRetryWrite", 0, iniPath);

	this->maxWGroup = Utility::getIntParamFromIni(modelName, "maxWGroup", 4, iniPath);

	this->enableSpMode = Utility::getBoolParamFromIni(modelName, "enableSpMode", 0x0, iniPath);

	this->enableAutoSector = Utility::getBoolParamFromIni(modelName, "enableAutoSector", 0, iniPath);
	this->autoSectorMap = Utility::getIntParamFromIni(modelName, "autoSectorMap", 0xFF, iniPath);

	this->cdRomFilePath = Utility::getStringParamFromIni(iniPath, modelName, "cdRomFilePath");
	this->sn = Utility::getStringParamFromIni(iniPath, modelName, "sn");

	this->gf_autoCE  = Utility::getBoolParamFromIni(modelName, "gf_autoCE", false, iniPath);

	
	loadUserMarkBadInfo(modelName, iniPath);

}

void Option::for_AutoPageMode(string modelName, string iniPath){
	this->enableAutoPage = Utility::getBoolParamFromIni(modelName, "enableAutoPage", NULL_INI_VALUE, iniPath); 
	this->maxSLC = Utility::getIntParamFromIni(modelName, "maxSLC", 0x40, iniPath);
	this->maxMLC = Utility::getIntParamFromIni(modelName, "maxMLC", 0x80, iniPath);
}

void Option::for_2092_PHY_Param(string modelName, string iniPath){
	this->x_sr_tune0 = Utility::getIntParamFromIni(modelName, "x_sr_tune0", NULL_INI_VALUE, iniPath); 
	this->x_sr_tune1 = Utility::getIntParamFromIni(modelName, "x_sr_tune1", NULL_INI_VALUE, iniPath);
	this->x_sr_tune2 = Utility::getIntParamFromIni(modelName, "x_sr_tune2", NULL_INI_VALUE, iniPath);
	this->x_sr_tune3 = Utility::getIntParamFromIni(modelName, "x_sr_tune3", NULL_INI_VALUE, iniPath);
	this->x_sr_tune4 = Utility::getIntParamFromIni(modelName, "x_sr_tune4", NULL_INI_VALUE, iniPath);
	this->x_sr_tune5 = Utility::getIntParamFromIni(modelName, "x_sr_tune5", NULL_INI_VALUE, iniPath);
	this->x_sr_tune6 = Utility::getIntParamFromIni(modelName, "x_sr_tune6", NULL_INI_VALUE, iniPath);
	this->x_sr_tune7 = Utility::getIntParamFromIni(modelName, "x_sr_tune7", NULL_INI_VALUE, iniPath);
	this->x_sr_tune8 = Utility::getIntParamFromIni(modelName, "x_sr_tune8", NULL_INI_VALUE, iniPath);
	this->x_sr_tune9 = Utility::getIntParamFromIni(modelName, "x_sr_tune9", NULL_INI_VALUE, iniPath);

	this->x_sr_swing = Utility::getIntParamFromIni(modelName, "x_sr_swing", 0x22, iniPath);
	this->x_sr_slew  = Utility::getIntParamFromIni(modelName, "x_sr_slew", 0x36, iniPath);

	this->x_sr_terminateresistor = Utility::getIntParamFromIni(modelName, "x_sr_terminateresistor", 0x3A, iniPath);
	this->x_sr_osc_reg = Utility::getIntParamFromIni(modelName, "x_sr_osc_reg", 0xFF, iniPath);
	this->x_sr_480_pll2	 = Utility::getIntParamFromIni(modelName, "x_sr_480_pll2", 0x93, iniPath);
	this->x_sr_cfg10 = Utility::getIntParamFromIni(modelName, "x_sr_cfg10", 0x80, iniPath);
	this->x_sr_baseCode	 = Utility::getIntParamFromIni(modelName, "x_sr_baseCode", 0x2774, iniPath);


	this->x_BulkCntMax = Utility::getIntParamFromIni(modelName, "x_BulkCntMax", 5000, iniPath);
	this->x_SIEResetCntMax	 = Utility::getIntParamFromIni(modelName, "x_SIEResetCntMax", 4, iniPath);
}

void Option::for_onCardSort(string modelName, string iniPath){
	this->onCardSortIniPath = Utility::getStringParamFromIni(iniPath, modelName, "onCardSortIniPath");
}

void Option::for_extendCapacity(string modelName, string iniPath){
	this->enableExtraCap = Utility::getBoolParamFromIni(modelName, "enableExtraCap", 0, iniPath);
	this->extraCapMB = Utility::getIntParamFromIni(modelName, "extraCapMB", 0, iniPath);
	this->extraLBA = Utility::getIntParamFromIni(modelName, "extraLBA", 0, iniPath);
}

void Option::for_manualFlashId(string modelName, string iniPath){
	this->enableManualId = Utility::getBoolParamFromIni(modelName, "enableManualId", NULL_INI_VALUE, iniPath);
	this->manualCeNo = Utility::getIntParamFromIni(modelName, "manualCeNo", NULL_INI_VALUE, iniPath);
	this->manualMakerCode = Utility::getIntParamFromIni(modelName, "manualMakerCode", 0xFF, iniPath);
	this->manualDeviceCode = Utility::getIntParamFromIni(modelName, "manualDeviceCode", 0xFF, iniPath);
	this->manualID3 = Utility::getIntParamFromIni(modelName, "manualID3", 0xFF, iniPath);
	this->manualID4 = Utility::getIntParamFromIni(modelName, "manualID4", 0xFF, iniPath);
	this->manualID5 = Utility::getIntParamFromIni(modelName, "manualID5", 0xFF, iniPath);
	this->manualID6 = Utility::getIntParamFromIni(modelName, "manualID6", 0xFF, iniPath);

	this->manualID_evenDieStart = Utility::getIntParamFromIni(modelName, "manualID_evenDieStart", 0, iniPath);
	this->manualID_evenDieEnd = Utility::getIntParamFromIni(modelName, "manualID_evenDieEnd", 0, iniPath);
	this->manualID_oddDieStart = Utility::getIntParamFromIni(modelName, "manualID_oddDieStart", 0, iniPath);
	this->manualID_oddDieEnd = Utility::getIntParamFromIni(modelName, "manualID_oddDieEnd", 0, iniPath);

}

void Option::for_IM_setFeature(string modelName, string iniPath){
	// for im setfeature cmd
	this->setFeature_Timing = Utility::getIntParamFromIni(modelName, "setFeature_Timing", 0, iniPath);
	this->setFeature_DriveStrength = Utility::getIntParamFromIni(modelName, "setFeature_DriveStrength", 0, iniPath);
	this->setFeature_PullDownStrength = Utility::getIntParamFromIni(modelName, "setFeature_PullDownStrength", 0, iniPath);

}

BYTE Option::getFrontEndVar1(void){
	BYTE result(0);
	Utility::setBit(result, 0, fFlashAccessLed);
	Utility::setBit(result, 1, gf_CDROM);
	Utility::setBit(result, 2, ReportTrueSecuredCapacity);
	Utility::setBit(result, 3, fSupportModePage5);
	Utility::setBit(result, 4, fSupportStringDesc);
	Utility::setBit(result, 5, fHighPowerDevice);
	Utility::setBit(result, 6, fBOTDataLenCheck);
	Utility::setBit(result, 7, f_AES_Support);
	return result;
}

BYTE Option::getFrontEndVar2(void){
	BYTE result(0);
	Utility::setBit(result, 0, gf_cis_sig);
	Utility::setBit(result, 1, gf_vdr_sig_lun0);
	Utility::setBit(result, 2, gf_vdr_sig_lun1);
	Utility::setBit(result, 3, gf_str_sig);
	Utility::setBit(result, 4, gf_pwd_sig);
	Utility::setBit(result, 5, gf_hid_En);
	Utility::setBit(result, 6, gf_lun0_en_cap);
	Utility::setBit(result, 7, gf_lun1_en_cap);
	return result;
}

BYTE Option::getFrontEndVar3(void){
	return frontEndVar3;
}

void Option::for_MultiCe_loadSortRecordInfo(string modelName, string iniPath){
	this->enableMultiCe = Utility::getBoolParamFromIni(modelName, "enableMultiCe", 0, iniPath);

	sortRecordMap[0] = Utility::getIntParamFromIni(modelName, "ce0_sortRecord", 0xFF, iniPath);
	sortRecordMap[1] = Utility::getIntParamFromIni(modelName, "ce1_sortRecord", 0xFF, iniPath);
	sortRecordMap[2] = Utility::getIntParamFromIni(modelName, "ce2_sortRecord", 0xFF, iniPath);
	sortRecordMap[3] = Utility::getIntParamFromIni(modelName, "ce3_sortRecord", 0xFF, iniPath);
	sortRecordMap[4] = Utility::getIntParamFromIni(modelName, "ce4_sortRecord", 0xFF, iniPath);
	sortRecordMap[5] = Utility::getIntParamFromIni(modelName, "ce5_sortRecord", 0xFF, iniPath);
	sortRecordMap[6] = Utility::getIntParamFromIni(modelName, "ce6_sortRecord", 0xFF, iniPath);
	sortRecordMap[7] = Utility::getIntParamFromIni(modelName, "ce7_sortRecord", 0xFF, iniPath);
	sortRecordMap[8] = Utility::getIntParamFromIni(modelName, "ce8_sortRecord", 0xFF, iniPath);
	sortRecordMap[9] = Utility::getIntParamFromIni(modelName, "ce9_sortRecord", 0xFF, iniPath);
	sortRecordMap[10] = Utility::getIntParamFromIni(modelName, "ce10_sortRecord", 0xFF, iniPath);
	sortRecordMap[11] = Utility::getIntParamFromIni(modelName, "ce11_sortRecord", 0xFF, iniPath);
	sortRecordMap[12] = Utility::getIntParamFromIni(modelName, "ce12_sortRecord", 0xFF, iniPath);
	sortRecordMap[13] = Utility::getIntParamFromIni(modelName, "ce13_sortRecord", 0xFF, iniPath);
	sortRecordMap[14] = Utility::getIntParamFromIni(modelName, "ce14_sortRecord", 0xFF, iniPath);
	sortRecordMap[15] = Utility::getIntParamFromIni(modelName, "ce15_sortRecord", 0xFF, iniPath);
}



void Option::loadUserMarkBadInfo(string modelName, string iniPath){
	WORD temp(0);

	temp = Utility::getIntParamFromIni(modelName, "badBlock0-0", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-1", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-2", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-3", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-4", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-5", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-6", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-7", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-8", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock0-9", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_0.push_back(temp);}

	temp = Utility::getIntParamFromIni(modelName, "badBlock1-0", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-1", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-2", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-3", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-4", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-5", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-6", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-7", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-8", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock1-9", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_1.push_back(temp);}

	temp = Utility::getIntParamFromIni(modelName, "badBlock2-0", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-1", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-2", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-3", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-4", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-5", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-6", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-7", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-8", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock2-9", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_2.push_back(temp);}

	temp = Utility::getIntParamFromIni(modelName, "badBlock3-0", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-1", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-2", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-3", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-4", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-5", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-6", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-7", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-8", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}
	temp = Utility::getIntParamFromIni(modelName, "badBlock3-9", NULL_INI_VALUE, iniPath);
	if(temp!=0){this->badBlockColl_3.push_back(temp);}

}






