#include "stdafx.h"
#include "CmderViewUtility.h"
#include "Utility/EricException.h"

CmderViewUtility::CmderViewUtility() {
}


CmderViewUtility::~CmderViewUtility() {
}

void CmderViewUtility::send_msg(CEdit_p editbox, bool isClean, estring msg) {
	//init
	if(isClean) {
		editbox->SetWindowText(msg.c_str());
		editbox->RedrawWindow();
	} else {
		int end = editbox->GetWindowTextLength();
		editbox->SetSel(end, end);
		editbox->ReplaceSel(msg.c_str());
	}
}

void CmderViewUtility::set_data_direction(CButton_p dataIn, CButton_p dataOut, eu8 direction) {
	if(direction == 0x02) {
		dataIn->SetCheck(true);
		dataOut->SetCheck(false);
	} else {
		dataIn->SetCheck(false);
		dataOut->SetCheck(true);
	}
}

int CmderViewUtility::get_drive_select(CComboBox_sp cbobox) {
	int res = cbobox->GetCurSel();
	if(res == -1) {
		THROW_MYEXCEPTION(0, "get_drive_select error");
	}
	return res;
}