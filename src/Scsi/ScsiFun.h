#pragma once

#include "DefineFiles/DefineFile.h"
#include "Scsi/ScsiIf.h"
#include "DefineScsi.h"

#include <vector>

#ifdef _ENABLE_PHYSICAL_DEVICE
#include "Device/DeviceHandle.h"
#else
typedef struct {
    estring devicePath;
    HANDLE handle;
    eu32 busType;
    estring description;
}DeviceInfo;
typedef bool(*CheckFun)(DeviceInfo&);
#endif


class ScsiFun
{
public:

    ScsiFun();
    ~ScsiFun();
    vector<DeviceInfo> scan_all_device();
    vector<DeviceInfo> filter_device(vector<DeviceInfo> deviceInfo, CheckFun filterFun);
    vector<DeviceInfo> scan_device(CheckFun filterFun);
    void setup_singleton(vector<DeviceInfo> deviceInfos);
    ScsiIf get_form_singleton(eu32 num);
    vector<estring> get_device_name();
    void release();

    bool is_usb_bus_type(DeviceInfo& deviceInfo);

private:
    

};

