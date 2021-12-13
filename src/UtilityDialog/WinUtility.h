#pragma once
#include "DefineFiles\DefineFile.h"

namespace EricCore {
	class WinUtility
	{
	public:

		WinUtility();
		~WinUtility();

		estring get_current_directory();
	};
}

