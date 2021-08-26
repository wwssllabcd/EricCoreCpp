#pragma once

#include "DefineFiles/DefineFile.h"
#include "Device/DeviceHandle.h"
#include "Scsi/ScsiIf.h"

#include <vector>
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

