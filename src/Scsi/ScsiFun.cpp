#include "stdafx.h"
#include "ScsiFun.h"
#include "DefineScsi.h"

#include "Utility/Singleton.h"


ScsiFun::ScsiFun()
{
}

ScsiFun::~ScsiFun() {
}

#ifdef _ENABLE_FAKE_DEVICE
vector<DeviceInfo> ScsiFun::scan_device(CheckFun filterFun) {
    vector<DeviceInfo> deviceInfo;
    DeviceInfo di;

    di.handle = 0;
    di.devicePath = _ET("FAKE_C");
    deviceInfo.push_back(di);

    di.devicePath = _ET("FAKE_D");
    deviceInfo.push_back(di);
    return deviceInfo;
}
#else
vector<DeviceInfo> ScsiFun::scan_device(CheckFun filterFun) {

    vector<DeviceInfo> deviceInfo;
    DeviceHandle deviceHandle;

    deviceInfo = deviceHandle.get_device_handle_colls(deviceHandle.get_device_path());

    if (filterFun) {
        deviceInfo = deviceHandle.filter(deviceInfo, filterFun);
    }
    return deviceInfo;
}
#endif

void ScsiFun::put_into_singleton(vector<DeviceInfo> deviceInfos) {
    Singleton<ScsiIf>* s = Singleton<ScsiIf>::get_instance();

    for (eu32 i = 0; i < deviceInfos.size(); i++) {
        DeviceInfo di = deviceInfos[i];
        ScsiIf obj(di.handle, di.devicePath);
        s->push_back(obj);
    }
}

ScsiIf ScsiFun::get_form_singleton(eu32 num) {
    Singleton<ScsiIf>* s = Singleton<ScsiIf>::get_instance();
    eu32 cnt = s->size();
    if (cnt == 0) {
        THROW_MYEXCEPTION(0, _ET("No Device Found"));
    }
    if (num >= cnt) {
        THROW_MYEXCEPTION(0, _ET("wrong select num"));
    }

    return s->get_item(num);
}

void ScsiFun::release() {
    Singleton<ScsiIf>* s = Singleton<ScsiIf>::get_instance();
    s->release();
}

vector<estring> ScsiFun::get_device_name() {



    Singleton<ScsiIf>* s = Singleton<ScsiIf>::get_instance();

    vector<estring> colls;
    eu32 cnt = s->size();
    for (eu32 i = 0; i < cnt; i++) {
        ScsiIf obj = s->get_item(i);
        colls.push_back(obj.m_deviceName);
    }

    return colls;
}

