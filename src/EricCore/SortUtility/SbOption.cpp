#include "StdAfx.h"
#include ".\SbOption.h"
#include "..\Utility\Utility.h"

using namespace EricCore;

SbOption::SbOption(void)
:m_iniPath("")
,m_modelName("Sorting Board")
,pcNo(0)
,flashPartNo("")
,sortBinCode("")
,qcBinCode("")
,sortInitBinCode("")
,qcInitBinCode("")
,maxErrorBit(0)
//,multiPlane(false)
,extInterleave(false)

,pettern0(false)
,pettern1(false)
,pettern2(false)
,pettern3(false)
,pettern4(false)
,pettern5(false)
,pettern6(false)

,quickPattern0(false)
,quickPattern1(false)
,quickPattern2(false)
,quickPattern3(false)
,quickPattern4(false)
,quickPattern5(false)

,lowEven(0)
,lowOdd(0)
,highEven(0)
,highOdd(0)

,bin1(0)
,bin2(0)
,bin3(0)
,bin4(0)
,bin5(0)
,bin6(0)
,bin7(0)
,bin8(0)
,bin9(0)
,bin10(0)
,bin11(0)
,bin12(0)
,bin13(0)
,bin14(0)
,bin15(0)
,bin16(0)

,custom_bin1(0)
,custom_bin2(0)
,custom_bin3(0)
,custom_bin4(0)
,custom_bin5(0)
,custom_bin6(0)
,custom_bin7(0)
,custom_bin8(0)
,custom_bin9(0)
,custom_bin10(0)
,custom_bin11(0)
,custom_bin12(0)
,custom_bin13(0)
,custom_bin14(0)
,custom_bin15(0)
,custom_bin16(0)


,blockCnt(0)
,makerCode(0)
,deviceCode(0)
,flash_ID3(0)
,flash_ID4(0)
,flash_ID5(0)
,flash_ID6(0)
,flash_ID7(0)
,flash_ID8(0)

,testType(0)
,flashIdCheck(0)

,debug(false)
,onCardSort(false)
,is_2097(false)
{
}

SbOption::~SbOption(void)
{
}

void SbOption::save(void){

	string iniPath = this->m_iniPath;

	if( this->pcNo!=NULL_INI_VALUE)  {Utility::saveToIni(m_modelName,"pcNo",this->pcNo ,iniPath);}
	if( this->flashPartNo.length() !=NULL_INI_VALUE ) {Utility::saveToIni(m_modelName,"flashPartNo",this->flashPartNo ,iniPath);}

	if(this->flashIdCheck!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flashIdCheck",this->flashIdCheck ,iniPath);}
	if(this->makerCode!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"makerCode",this->makerCode ,iniPath);}
	if(this->deviceCode!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"deviceCode",this->deviceCode ,iniPath);}		
	if(this->testType!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"testType",this->testType ,iniPath);}

	if(this->maxErrorBit!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"maxErrorBit",this->maxErrorBit ,iniPath);}
	if(this->extInterleave!=false) {Utility::saveToIni(m_modelName,"extInterleave",this->extInterleave ,iniPath);}
	if(this->blockCnt!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"blockCnt",this->blockCnt ,iniPath);}

	Utility::saveToIni(m_modelName,"pettern0",this->pettern0 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern1",this->pettern1 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern2",this->pettern2 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern3",this->pettern3 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern4",this->pettern4 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern5",this->pettern5 ,iniPath);
	Utility::saveToIni(m_modelName,"pettern6",this->pettern6 ,iniPath);
	
	Utility::saveToIni(m_modelName,"quickPattern0",this->quickPattern0 ,iniPath);
	Utility::saveToIni(m_modelName,"quickPattern1",this->quickPattern1 ,iniPath);
	Utility::saveToIni(m_modelName,"quickPattern2",this->quickPattern2 ,iniPath);
	Utility::saveToIni(m_modelName,"quickPattern3",this->quickPattern3 ,iniPath);
	Utility::saveToIni(m_modelName,"quickPattern4",this->quickPattern4 ,iniPath);
	Utility::saveToIni(m_modelName,"quickPattern5",this->quickPattern5 ,iniPath);
	
	if(this->lowEven!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"lowEven",this->lowEven ,iniPath);}
	if(this->lowOdd!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"lowOdd",this->lowOdd ,iniPath);}
	if(this->highEven!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"highEven",this->highEven ,iniPath);}
	if(this->highOdd!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"highOdd",this->highOdd ,iniPath);}

	if(this->bin1!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin1",this->bin1 ,iniPath);}
	if(this->bin2!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin2",this->bin2 ,iniPath);}
	if(this->bin3!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin3",this->bin3 ,iniPath);}
	if(this->bin4!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin4",this->bin4 ,iniPath);}
	if(this->bin5!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin5",this->bin5 ,iniPath);}
	if(this->bin6!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin6",this->bin6 ,iniPath);}
	if(this->bin7!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin7",this->bin7 ,iniPath);}
	if(this->bin8!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin8",this->bin8 ,iniPath);}
	if(this->bin9!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin9",this->bin9 ,iniPath);}
	if(this->bin10!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin10",this->bin10 ,iniPath);}
	if(this->bin11!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin11",this->bin11 ,iniPath);}
	if(this->bin12!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin12",this->bin12 ,iniPath);}
	if(this->bin13!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin13",this->bin13 ,iniPath);}
	if(this->bin14!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin14",this->bin14 ,iniPath);}
	if(this->bin15!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin15",this->bin15 ,iniPath);}
	if(this->bin16!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"bin16",this->bin16 ,iniPath);}


	if(this->custom_bin1!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin1",this->custom_bin1 ,iniPath);}
	if(this->custom_bin2!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin2",this->custom_bin2 ,iniPath);}
	if(this->custom_bin3!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin3",this->custom_bin3 ,iniPath);}
	if(this->custom_bin4!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin4",this->custom_bin4 ,iniPath);}
	if(this->custom_bin5!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin5",this->custom_bin5 ,iniPath);}
	if(this->custom_bin6!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin6",this->custom_bin6 ,iniPath);}
	if(this->custom_bin7!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin7",this->custom_bin7 ,iniPath);}
	if(this->custom_bin8!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin8",this->custom_bin8 ,iniPath);}
	if(this->custom_bin9!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin9",this->custom_bin9 ,iniPath);}
	if(this->custom_bin10!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin10",this->custom_bin10 ,iniPath);}
	if(this->custom_bin11!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin11",this->custom_bin11 ,iniPath);}
	if(this->custom_bin12!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin12",this->custom_bin12 ,iniPath);}
	if(this->custom_bin13!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin13",this->custom_bin13 ,iniPath);}
	if(this->custom_bin14!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin14",this->custom_bin14 ,iniPath);}
	if(this->custom_bin15!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin15",this->custom_bin15 ,iniPath);}
	if(this->custom_bin16!=NULL_CUSTON_BIN_VALUE) {Utility::saveToIni(m_modelName,"custom_bin16",this->custom_bin16 ,iniPath);}


	//Utility::saveToIni(m_modelName,"debug",this->debug ,iniPath);

	if(this->onCardSort!=false) {Utility::saveToIni(m_modelName,"onCardSort",this->onCardSort ,iniPath);}

	if(this->qcInitBinCode!="") {Utility::saveToIni(m_modelName,"qcInitBinCode",this->qcInitBinCode ,iniPath);}
	if(this->sortBinCode!="") {Utility::saveToIni(m_modelName,"sortBinCode",this->sortBinCode ,iniPath);}
	
	if(this->sortInitBinCode!="") {Utility::saveToIni(m_modelName,"sortInitBinCode",this->sortInitBinCode ,iniPath);}
	if(this->qcBinCode!="") {Utility::saveToIni(m_modelName,"qcBinCode",this->qcBinCode ,iniPath);}


	if(this->flash_ID3!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID3",this->flash_ID3 ,iniPath);}
	if(this->flash_ID4!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID4",this->flash_ID4 ,iniPath);}
	if(this->flash_ID5!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID5",this->flash_ID5 ,iniPath);}
	if(this->flash_ID6!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID6",this->flash_ID6 ,iniPath);}
	if(this->flash_ID7!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID7",this->flash_ID7 ,iniPath);}
	if(this->flash_ID8!=NULL_INI_VALUE) {Utility::saveToIni(m_modelName,"flash_ID8",this->flash_ID8 ,iniPath);}



}

void SbOption::load(){
	this->pcNo = Utility::getIntParamFromIni(m_modelName, "pcNo", NULL_INI_VALUE, m_iniPath);
	this->flashPartNo = Utility::getStringParamFromIni(m_iniPath, m_modelName, "flashPartNo");
	this->sortBinCode = Utility::getStringParamFromIni(m_iniPath, m_modelName, "sortBinCode");
	this->qcBinCode = Utility::getStringParamFromIni(m_iniPath, m_modelName, "qcBinCode");

	this->sortInitBinCode = Utility::getStringParamFromIni(m_iniPath, m_modelName, "sortInitBinCode");
	this->qcInitBinCode = Utility::getStringParamFromIni(m_iniPath, m_modelName, "qcInitBinCode");


	this->maxErrorBit = Utility::getIntParamFromIni(m_modelName, "maxErrorBit", NULL_INI_VALUE, m_iniPath);
	//this->multiPlane = Utility::getIntParamFromIni(m_modelName, "multiPlane", NULL_INI_VALUE, m_iniPath);
	this->extInterleave = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "extInterleave", NULL_INI_VALUE, m_iniPath) );
	this->blockCnt = Utility::getIntParamFromIni(m_modelName, "blockCnt", NULL_INI_VALUE, m_iniPath);

	this->pettern0 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern0", 0, m_iniPath) );
	this->pettern1 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern1", 0, m_iniPath) );
	this->pettern2 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern2", 0, m_iniPath) );
	this->pettern3 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern3", 0, m_iniPath) );
	this->pettern4 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern4", 0, m_iniPath) );
	this->pettern5 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern5", 0, m_iniPath) );
	this->pettern6 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "pettern6", 0, m_iniPath) );

	this->quickPattern0 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern0", 0, m_iniPath) );
	this->quickPattern1 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern1", 0, m_iniPath) );
	this->quickPattern2 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern2", 0, m_iniPath) );
	this->quickPattern3 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern3", 0, m_iniPath) );
	this->quickPattern4 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern4", 0, m_iniPath) );
	this->quickPattern5 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "quickPattern5", 0, m_iniPath) );

	this->lowEven = Utility::getIntParamFromIni(m_modelName, "lowEven", NULL_INI_VALUE, m_iniPath);
	this->lowOdd = Utility::getIntParamFromIni(m_modelName, "lowOdd", NULL_INI_VALUE, m_iniPath);
	this->highEven = Utility::getIntParamFromIni(m_modelName, "highEven", NULL_INI_VALUE, m_iniPath);
	this->highOdd = Utility::getIntParamFromIni(m_modelName, "highOdd", NULL_INI_VALUE, m_iniPath);

	this->bin1 = Utility::getIntParamFromIni(m_modelName, "bin1", NULL_INI_VALUE, m_iniPath);
	this->bin2 = Utility::getIntParamFromIni(m_modelName, "bin2", NULL_INI_VALUE, m_iniPath);
	this->bin3 = Utility::getIntParamFromIni(m_modelName, "bin3", NULL_INI_VALUE, m_iniPath);
	this->bin4 = Utility::getIntParamFromIni(m_modelName, "bin4", NULL_INI_VALUE, m_iniPath);
	this->bin5 = Utility::getIntParamFromIni(m_modelName, "bin5", NULL_INI_VALUE, m_iniPath);
	this->bin6 = Utility::getIntParamFromIni(m_modelName, "bin6", NULL_INI_VALUE, m_iniPath);
	this->bin7 = Utility::getIntParamFromIni(m_modelName, "bin7", NULL_INI_VALUE, m_iniPath);
	this->bin8 = Utility::getIntParamFromIni(m_modelName, "bin8", NULL_INI_VALUE, m_iniPath);
	this->bin9 = Utility::getIntParamFromIni(m_modelName, "bin9", NULL_INI_VALUE, m_iniPath);
	this->bin10 = Utility::getIntParamFromIni(m_modelName, "bin10", NULL_INI_VALUE, m_iniPath);
	this->bin11 = Utility::getIntParamFromIni(m_modelName, "bin11", NULL_INI_VALUE, m_iniPath);
	this->bin12 = Utility::getIntParamFromIni(m_modelName, "bin12", NULL_INI_VALUE, m_iniPath);
	this->bin13 = Utility::getIntParamFromIni(m_modelName, "bin13", NULL_INI_VALUE, m_iniPath);
	this->bin14 = Utility::getIntParamFromIni(m_modelName, "bin14", NULL_INI_VALUE, m_iniPath);
	this->bin15 = Utility::getIntParamFromIni(m_modelName, "bin15", NULL_INI_VALUE, m_iniPath);
	this->bin16 = Utility::getIntParamFromIni(m_modelName, "bin16", NULL_INI_VALUE, m_iniPath);


	this->custom_bin1 = Utility::getIntParamFromIni(m_modelName, "custom_bin1", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin2 = Utility::getIntParamFromIni(m_modelName, "custom_bin2", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin3 = Utility::getIntParamFromIni(m_modelName, "custom_bin3", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin4 = Utility::getIntParamFromIni(m_modelName, "custom_bin4", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin5 = Utility::getIntParamFromIni(m_modelName, "custom_bin5", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin6 = Utility::getIntParamFromIni(m_modelName, "custom_bin6", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin7 = Utility::getIntParamFromIni(m_modelName, "custom_bin7", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin8 = Utility::getIntParamFromIni(m_modelName, "custom_bin8", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin9 = Utility::getIntParamFromIni(m_modelName, "custom_bin9", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin10 = Utility::getIntParamFromIni(m_modelName, "custom_bin10", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin11 = Utility::getIntParamFromIni(m_modelName, "custom_bin11", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin12 = Utility::getIntParamFromIni(m_modelName, "custom_bin12", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin13 = Utility::getIntParamFromIni(m_modelName, "custom_bin13", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin14 = Utility::getIntParamFromIni(m_modelName, "custom_bin14", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin15 = Utility::getIntParamFromIni(m_modelName, "custom_bin15", NULL_CUSTON_BIN_VALUE, m_iniPath);
	this->custom_bin16 = Utility::getIntParamFromIni(m_modelName, "custom_bin16", NULL_CUSTON_BIN_VALUE, m_iniPath);


	this->makerCode = Utility::getIntParamFromIni(m_modelName, "makerCode", NULL_INI_VALUE, m_iniPath);
	this->deviceCode = Utility::getIntParamFromIni(m_modelName, "deviceCode", NULL_INI_VALUE, m_iniPath);
	this->testType = Utility::getIntParamFromIni(m_modelName, "testType", NULL_INI_VALUE, m_iniPath);
	this->flashIdCheck = Utility::getIntParamFromIni(m_modelName, "flashIdCheck", NULL_INI_VALUE, m_iniPath);


	this->debug = Utility::getBoolParamFromIni(m_modelName, "debug", 0, m_iniPath);
	this->onCardSort = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "onCardSort", 0, m_iniPath) );
	this->is_2097 = Utility::uintToBool( Utility::getIntParamFromIni(m_modelName, "is_2097", 0, m_iniPath) );


	

	this->flash_ID3 = Utility::getIntParamFromIni( m_modelName, "flash_ID3", 0, m_iniPath );
	this->flash_ID4 = Utility::getIntParamFromIni( m_modelName, "flash_ID4", 0, m_iniPath );
	this->flash_ID5 = Utility::getIntParamFromIni( m_modelName, "flash_ID5", 0, m_iniPath );
	this->flash_ID6 = Utility::getIntParamFromIni( m_modelName, "flash_ID6", 0, m_iniPath );
	this->flash_ID7 = Utility::getIntParamFromIni( m_modelName, "flash_ID7", 0, m_iniPath );
	this->flash_ID8 = Utility::getIntParamFromIni( m_modelName, "flash_ID8", 0, m_iniPath );

}

void SbOption::load(string path){
	this->m_iniPath = path;
	load();
}

bool SbOption::isValid(string iniPath){
	SbOption opt;
	opt.m_iniPath = iniPath;
	opt.load();

	if( opt.pcNo == NULL_INI_VALUE ){
		return false;
	}

	return true;
}