#pragma once
#include "UtilityDialog\DialogUtility.h"

class CmderViewUtility
{
public:
	CmderViewUtility();
	~CmderViewUtility();

	void send_msg(CEdit_p editbox, bool isClean, estring msg);
	void set_data_direction(CButton_p dataIn, CButton_p dataOut, eu8 direction);
	int get_drive_select(CComboBox_sp cbobox);

};

