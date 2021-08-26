#pragma once

#include "DefineFiles/DefineFile.h"
#include "Scsi/ScsiIf.h"
#include "DefineScsi.h"

#include <vector>

#ifdef _ENABLE_FAKE_DEVICE
typedef struct {
    estring devicePath;
    HANDLE handle;
    eu32 busType;
    estring description;
}DeviceInfo;
typedef bool(*CheckFun)(DeviceInfo&);
#else

#endif

#include "Device/DeviceHandle.h"

class ScsiFun
{
public:

    ScsiFun();
    ~ScsiFun();

    vector<DeviceInfo> scan_device(CheckFun filterFun);
    void put_into_singleton(vector<DeviceInfo> deviceInfos);
    ScsiIf get_form_singleton(eu32 num);
    vector<estring> get_device_name();
    void release();
private:
    

};

