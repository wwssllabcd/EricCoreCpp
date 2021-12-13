#include "stdafx.h"
#include "WinUtility.h"

#include "Windows.h"  //for GetCurrentDirectory()

using namespace EricCore;

WinUtility::WinUtility(){}
WinUtility::~WinUtility() {}


estring WinUtility::get_current_directory() {
	echar buffer[256];
	GetCurrentDirectory(sizeof(buffer), buffer);
	estring res(buffer);
	return res;
}

