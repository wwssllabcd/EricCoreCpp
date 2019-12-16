#pragma once
#include "DefineFiles\DefineFile.h"
namespace EricCore {
	class IniUtility
	{
	public:
		IniUtility(estring iniFilePath);
		~IniUtility();
		
		estring getString(estring appName, estring keyName, estring defaultString = _ET(""));
		bool getBool(estring appName, estring keyName, bool defaultVal = false);
		eu32 getInt(estring appName, estring keyName, eu32 defaultVal = NULL_32);

	private:
		estring iniFilePath_;

	};
}