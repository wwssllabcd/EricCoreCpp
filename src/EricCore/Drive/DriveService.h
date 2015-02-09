#pragma once
#include <vector>
#include "Drive.h"
//#include "..\..\MP\Drive\Drive.h"
using namespace std;

class DriveService
{
public:

			
	//----------------------- function ---------------------------------
	~DriveService(void);
	Drive getIdelDevice();
	Drive getIdelDeviceByName(const string&  deviceName);
	Drive getIdelDeviceById(int testId);
	

	static DriveService* getInstance(int deviceType=0);
	void releaseDevice(HANDLE driveAddr);
	bool powerCtrl(HANDLE hCF,BOOL OnOff,UCHAR dly10ms);
	int count(void);
	static void releaseSingleton(void);
	void removeDevice(const Drive& objDrive);
	Drive getAt(int i);
	
	
	

private :
	DriveService(int deviceType);
	DriveService(const DriveService&);
	DriveService& operator=(const DriveService&);
	vector<Drive> _driveCollection;

	
	static DriveService* _instance;
	void _checkDrive(const Drive& d);

};
