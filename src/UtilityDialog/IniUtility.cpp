#include "stdafx.h"
#include "IniUtility.h"
#include "Utility\Utility.h"

#include <afxcmn.h> 

using namespace EricCore;

IniUtility::IniUtility(estring iniFilePath)
	:iniFilePath_(iniFilePath)
{
}


IniUtility::~IniUtility()
{
}


estring IniUtility::getString(estring appName, estring keyName, estring defaultString) {
	echar cKey[200];
	GetPrivateProfileString(appName.c_str(), keyName.c_str(), _T(""), cKey, 200, iniFilePath_.c_str());
	Utility u;
	estring res = u.strFormat(_T("%s"), cKey);
	if(res.length() == 0) {
		res = defaultString;
	}
	return res;
}

bool IniUtility::getBool(estring appName, estring keyName, bool defaultVal) {
	bool res = false;
	if(GetPrivateProfileInt(appName.c_str(), keyName.c_str(), defaultVal, iniFilePath_.c_str())) {
		res = true;
	} else {
		res = false;
	}
	return res;
}

eu32 IniUtility::getInt(estring appName, estring keyName, eu32 defaultVal){
	return GetPrivateProfileInt(appName.c_str(), keyName.c_str(), defaultVal, iniFilePath_.c_str());
}

