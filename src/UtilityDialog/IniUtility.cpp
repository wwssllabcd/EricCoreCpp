#include "stdafx.h"
#include "IniUtility.h"
#include "Utility\Utility.h"

#include <afxcmn.h> 

using namespace EricCore;

IniUtility::IniUtility(estring_cr iniFilePath)
	:iniFilePath_(iniFilePath)
{
}


IniUtility::~IniUtility()
{
}

estring IniUtility::getString(estring_cr appName, estring_cr keyName, estring_cr defaultString) {
	echar cKey[200];
	eu32 cnt = GetPrivateProfileString(appName.c_str(), keyName.c_str(), _ET(""), cKey, 200, iniFilePath_.c_str());
	Utility u;
	estring res(cKey);
	if(res.length() == 0) {
		res = defaultString;
	}
	return res;
}

bool IniUtility::getBool(estring_cr appName, estring_cr keyName, bool defaultVal) {
	bool res = false;
	if(GetPrivateProfileInt(appName.c_str(), keyName.c_str(), defaultVal, iniFilePath_.c_str())) {
		res = true;
	} else {
		res = false;
	}
	return res;
}

eu32 IniUtility::getInt(estring_cr appName, estring_cr keyName, eu32 defaultVal){
	return GetPrivateProfileInt(appName.c_str(), keyName.c_str(), defaultVal, iniFilePath_.c_str());
}

