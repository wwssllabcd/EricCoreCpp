#pragma once

#include <windows.h>
#include <tchar.h>

#include <string>
#include <vector>
#include "../Utility/DefineFiles/DefineFile.h"

using namespace std;


class DeviceManger
{
public:
	DeviceManger(void);
	~DeviceManger(void);

	void loadDeviceColl(void);
	bool isRemovableDevice(const TCHAR* deviceName);
	bool checkDeviceExist(ULONG deviceRef, ULONG deviceNo);


	const int maxDeviceCnt;

	vector< pair<tstring, HANDLE> > deviceHandleColl;


};
