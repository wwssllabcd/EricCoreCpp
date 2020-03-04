#pragma once
#include "DefineFiles\DefineFile.h"
namespace EricCore {
	class IniUtility
	{
	public:
		IniUtility(estring_cr iniFilePath);
		~IniUtility();
		
		estring getString(estring_cr appName, estring_cr keyName, estring_cr defaultString = _ET(""));
		bool getBool(estring_cr appName, estring_cr keyName, bool defaultVal = false);
		eu32 getInt(estring_cr appName, estring_cr keyName, eu32 defaultVal = NULL_32);

	private:
		estring iniFilePath_;

	};
}