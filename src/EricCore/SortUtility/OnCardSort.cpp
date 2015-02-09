#include "StdAfx.h"
#include ".\OnCardSort.h"
#include "..\Drive\UsbCommand.h"
#include "..\Drive\Drive.h"
#include "..\Utility\Utility.h"
#include "..\Utility\EricException\MyException.h"
#include "..\Utility_Dialog\DialogUtility.h"

#include "SortBase.h"
#include "SbOption.h"


using namespace EricCore;


OnCardSort::OnCardSort(void)
{
}

OnCardSort::~OnCardSort(void)
{
}


bool OnCardSort::doOnCardSort(HANDLE handle, string iniPathOfSorting){
	UsbCommand usbCmd(handle);
	laod_7800_8000( handle, iniPathOfSorting );
	usbCmd.onCardSort();
	bool result = showOnCardSortStatus( handle);
	if( result==false){
		return true;
	}
	return false;
}

void OnCardSort::laod_0_7FF(HANDLE handle, string iniPathOfSorting){
	SbOption sbOpt;
	sbOpt.m_iniPath = iniPathOfSorting;
	sbOpt.load();

	if(sbOpt.sortInitBinCode.length() ==0 ){
		throw MyException(OCS_laod_0_7FF_FAIL, "Error: Please check sortInitBinCode setting");
	}

	UsbCommand usbCmd(handle);
	BYTE aryIsp_7FF[2048];
	memset(aryIsp_7FF, 0xFF, sizeof(aryIsp_7FF));
	Utility::getFileData(sbOpt.sortInitBinCode, 0, 2048, aryIsp_7FF);
	Utility::sendMsg("downLoad OnCard Bin code(0x00~0x7FF)");
	usbCmd.downLoadISP(aryIsp_7FF, 1);


}

void OnCardSort::laod_7800_8000(HANDLE handle, string iniPathOfSorting){
	SbOption sbOpt;
	SortBase sb;
	sbOpt.m_iniPath = iniPathOfSorting;
	sbOpt.load();

	UsbCommand usbCmd(handle);
	BYTE aryIsp_7800[2048];
	BYTE aryIsp_8[4096];
	memset(aryIsp_7800, 0xFF, sizeof(aryIsp_7800));
	memset(aryIsp_8, 0xFF, sizeof(aryIsp_8));

	Utility::getFileData(sbOpt.sortInitBinCode, 2048, 2048, aryIsp_7800);
	Utility::getFileData(sbOpt.sortBinCode, 0, 4096, aryIsp_8);

	sb.modify8000(aryIsp_8, sbOpt, 1, 0);

	Utility::sendMsg("downLoad OnCard Bin code(0x7800)");
	usbCmd.downLoadISP(aryIsp_7800, 2);
	Utility::sendMsg("downLoad OnCard Bin code(0x8000)");
	usbCmd.downLoadISP(aryIsp_8);

	ispRamTest( handle, aryIsp_7800, 0xD8, 4);
	ispRamTest( handle, aryIsp_8, 0xE0, 8);
}

void OnCardSort::ispRamTest(HANDLE handle, BYTE* source, int ram_h, int sec){
	BYTE aryTarget[4096];
	memset(aryTarget, 0xFF, sizeof(aryTarget));

	UsbCommand usbCmd( handle );
	for(int i=0; i<sec; i++){
		usbCmd.readRam(ram_h+i*2, aryTarget+512*i);
	}

	int result = 0;
	result = memcmp(source, aryTarget, sizeof( sec*512 ) );
	if(result!=0){throw MyException(OCS_ISP_RAM_TEST_FAIL, "ISP code cmp fail");}
}


bool OnCardSort::showOnCardSortStatus(HANDLE hansle){
	UsbCommand usbCmd( hansle );
	BYTE aryStatus[31];

	CTime startTime, curTime;
	startTime = CTime::GetCurrentTime(); // 取得目前時間
	string msg = startTime.Format("%Y/%m/%d %a %h:%m:%s");
	Utility::sendMsg( msg );
	CTimeSpan spantime;


	while(1){
		for(int i=0; i<10; i++){
			DialogUtility::updateOS();
			Sleep(200);
		}
		curTime = CTime::GetCurrentTime();
		spantime = curTime-startTime;
		msg = Utility::strFormat("%d:%d:%d", spantime.GetHours(), spantime.GetMinutes(), spantime.GetSeconds() );
		Utility::sendMsg( msg );

		usbCmd.pollingOnCardSort( aryStatus );
		Utility::sendMsg( Utility::makeHexTable(31, aryStatus) + Utility::CrLf() );
 
		if( aryStatus[5] ==0x01 ){
			string tmp = Utility::strFormat("On Card Sort: Finish, This is Bin %d", aryStatus[2]);
			Utility::sendMsg(tmp.c_str());
			return true;
		}

		if( aryStatus[6] !=0x00 ){
			throw MyException(OCS_SHOW_ONCARDSORT_STATUS_FAIL, "On Card Sort: fail! ");
			return false;
		}
	}
}