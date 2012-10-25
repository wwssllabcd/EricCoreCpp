#include "StdAfx.h"
#include ".\devicemanger.h"

DeviceManger::DeviceManger(void)
:maxDeviceCnt(32)
{
}

DeviceManger::~DeviceManger(void)
{
}

void DeviceManger::loadDeviceColl(void){

	ULONG logicDevices = GetLogicalDrives();
	tstring deviceName = _T("A:");

	for(int i=2; i<this->maxDeviceCnt; i++){
		if( checkDeviceExist(logicDevices, (ULONG)(1<<i) )==false ){
			continue;
		}

		// force char to implement 'A' +i's function
		char tmp = 'A' + i;
		deviceName[0] = (TCHAR)tmp;
		if( isRemovableDevice(deviceName.c_str() )==false ){
			continue;
		}

		//Open the Drive's handle
		tstring accessSymbol = _T("\\\\.\\");
		accessSymbol += deviceName;
		HANDLE  drvhandle = CreateFile( accessSymbol.c_str() ,
										GENERIC_WRITE|GENERIC_READ, 
			                            FILE_SHARE_READ|FILE_SHARE_WRITE, 
										NULL,
										OPEN_EXISTING,
										0, 
										NULL);


		if (drvhandle==INVALID_HANDLE_VALUE){
			continue;
		}

		pair<tstring, HANDLE> tmpPair;
		tmpPair.first = deviceName.at(0);
		tmpPair.second = drvhandle;
		deviceHandleColl.push_back(tmpPair);
	}
}

bool DeviceManger::checkDeviceExist(ULONG deviceRef, ULONG deviceNo){
	if(deviceRef & deviceNo){
		return true;
	}else{
		return false;
	}
}

bool DeviceManger::isRemovableDevice(const TCHAR* deviceName){
	if ( GetDriveType(deviceName) == DRIVE_REMOVABLE ){
		return true;
	}else{
		return false;
	}
}

