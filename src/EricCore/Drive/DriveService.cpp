//注意!!這可能不是一個Thread Safe的Class，若非不得已，不要用在Thread上
//雖然有用Mutex保護資源，但還是不建議使用在thread上

#include "stdafx.h"
#include ".\driveservice.h"
//#include "..\USB_Driver.h"
#include "USBDrive.h"


#include "../Utility/EricException/MyException.h"
#include <algorithm>

using namespace std;
using namespace EricCore;

//因為也只有 A~Z 24個裝置名稱
#define	 MAX_DEVICE		32

DriveService*   DriveService::_instance  = 0;// initialize pointer
HANDLE hMutex;

DriveService::DriveService(int deviceType)
{	
	USBDrive objDrive;
	HANDLE DevHandle[MAX_DEVICE];
	string DevHandleName[MAX_DEVICE];
	objDrive.UDISK_OpenDevice( DevHandle,deviceType );
	objDrive.UDISK_getDeviceName( DevHandleName );
	int i=0;
	for(i=0; i<MAX_DEVICE; i++){
		if (DevHandle[i] != INVALID_HANDLE_VALUE){
			Drive d;
			d.address = DevHandle[i];
			d.testNo = i;

			//取得每個Handle的磁碟機名稱
			d.name = DevHandleName[i];
			d.status = IDEL;
			this->_driveCollection.push_back(d);
		}
	}
}//------------------------------------------------------------------------------------

DriveService::~DriveService(void)
{
}//------------------------------------------------------------------------------------

//找出最先被找到的IDEL的裝置，傳出去後，並且把他設為busy
Drive DriveService::getIdelDevice(){
	Drive d;
	vector<Drive>::iterator iter;

	//找出最先被找到的IDEL的裝置
	//注意!!這可能不是一個Thread Safe的Class，若非不得已，不要用在Thread上
	WaitForSingleObject(hMutex,INFINITE);
	for (iter = this->_driveCollection.begin(); iter!= this->_driveCollection.end(); ++iter){
		if (iter->status == IDEL){
			iter->status = BUSY;
			d = (*iter);
			break;
		}
	}
	ReleaseMutex(hMutex);
	//驗證Drive是否合法，會丟exceptioin
	_checkDrive(d);
	return d;
}//------------------------------------------------------------------------------------

Drive DriveService::getIdelDeviceByName(const string& deviceName){
	Drive d;
	vector<Drive>::iterator iter;
	WaitForSingleObject(hMutex,INFINITE);
	string deviceNameUp = deviceName;
	for (iter = this->_driveCollection.begin(); iter!= this->_driveCollection.end(); ++iter){
		transform( iter->name.begin(), iter->name.end(), iter->name.begin(), tolower);
		if (iter->name == deviceNameUp ){
			iter->status = BUSY;
			d = (*iter);
			break;
		}
	}
	ReleaseMutex(hMutex);
	//驗證Drive是否合法，會丟exceptioin
	_checkDrive(d);
	return d;
}//------------------------------------------------------------------------------------

Drive DriveService::getIdelDeviceById(int testId){
	Drive d;
	vector<Drive>::iterator iter;
	WaitForSingleObject(hMutex,INFINITE);
	for (iter = this->_driveCollection.begin(); iter!= this->_driveCollection.end(); ++iter){
		if (iter->testNo == testId){
			d = (*iter);
			iter->status = BUSY;
			d = (*iter);
			break;
		}
	}
	ReleaseMutex(hMutex);

	d.testNo = testId;
	//驗證Drive是否合法，會丟exceptioin
	_checkDrive(d);
	return d;
}//------------------------------------------------------------------------------------

DriveService* DriveService::getInstance(int deviceType){
	hMutex = CreateMutex(NULL,true,"DriveService");
	WaitForSingleObject(hMutex,INFINITE);
	if (_instance == 0){
		_instance = new DriveService(deviceType);
	}
	ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);
	return _instance;
}//------------------------------------------------------------------------------------

bool DriveService::powerCtrl(HANDLE hCF,BOOL OnOff,UCHAR dly10ms)
{
	USBDrive objDrive;
	int OP_POWER_CTRL = 0xC3;
	UCHAR	cdb[12];
	UCHAR	dat;

	ZeroMemory(cdb,12);

	cdb[0] = 0xFF;						// Vendor Command
	cdb[1] = 0x03;						// Backend function
	cdb[2] = OP_POWER_CTRL;
	cdb[3] = (OnOff) ? 1 : 0;
	cdb[4] = dly10ms;					// unit:10ms

	if (!objDrive.UDISK_SendCommand(hCF,cdb,&dat,1,FLAG_DATA_IN))		return( false );

	if (dat!=0)		return( false );
	return( true );
}//------------------------------------------------------------------------------------

void DriveService::releaseDevice(HANDLE handle){
	vector<Drive>::iterator iter;
	WaitForSingleObject(hMutex, INFINITE);

	for (iter = this->_driveCollection.begin(); iter!= this->_driveCollection.end(); ++iter){	
		if (iter->address == handle){
			if (iter->status  == BUSY){
				iter->status = IDEL;
			}
		}
	}
	ReleaseMutex(hMutex);
}//------------------------------------------------------------------------------------

void DriveService::_checkDrive(const Drive& objDrive){
	if (objDrive.address ==0){
		string msg,testNo;
		char tmp;
		itoa(objDrive.testNo, &tmp, 1);

		testNo = tmp;
		msg += "Can not Detect No."+ testNo +" Tester!\n\r";
		msg += "Please Check Tester ID or Reset Tester";
		throw MyException(DS_CHECK_DRIVE_FAIL, msg);
	}
}

int DriveService::count(void){
	return (int)this->_driveCollection.size();
}

void DriveService::releaseSingleton(void){
	_instance=0;
}

void DriveService::removeDevice(const Drive& objDrive){
	_driveCollection.erase( remove( _driveCollection.begin(), _driveCollection.end(), objDrive)	, _driveCollection.end() );
}

Drive DriveService::getAt(int i){

	size_t cnt = _driveCollection.size();
	if(cnt==0){
		throw MyException(DS_NO_HANDLE, "DS_NO_HANDLE");
	}
	return _driveCollection[i];
}
