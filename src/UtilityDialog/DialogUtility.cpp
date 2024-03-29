﻿

#include "stdafx.h"
#include "DialogUtility.h"
#include "Dbt.h"               // PDEV_BROADCAST_HDR
#include <afxdlgs.h> //CFileDialog
#include "Utility/Utility.h"
#include "Utility/EricException.h"

using namespace EricCore;

int const DialogUtility::onDeviceEvent_Arrival = 1;
int const DialogUtility::onDeviceEvent_Remove = 2;
#define MAX_TEXTBOX_SIZE (_128K)

DialogUtility::DialogUtility(void){}
DialogUtility::~DialogUtility(void) {}

int DialogUtility::get_cur_select(CComboBox_p cbobox) {
	return cbobox->GetCurSel();
}

bool DialogUtility::getCheck(CButton_p btnCheck) {
	return m_u.toBool(btnCheck->GetCheck());
}

void DialogUtility::setCheck(CButton_p btnCheck, bool isCheck) {
	btnCheck->SetCheck(isCheck);
}

void DialogUtility::setFont(CEdit_p item, int size, estring_cr fontName, bool reDraw, CFont_p font) {
	if(item == 0) {
		THROW_MYEXCEPTION(GENERAL_FAIL, _ET("DialogUtility::setFont, item = nullPtr"));
	}
	if(font == 0) {
		THROW_MYEXCEPTION(GENERAL_FAIL, _ET("DialogUtility::setFont, font = nullPtr"));
	}

	font->CreatePointFont(size, fontName.c_str());
	
	item->SetFont(font, reDraw);
}

bool DialogUtility::getUserYesNo(estring_cr msg) {
	int userSelect = AfxMessageBox(msg.c_str(), MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2);
	if(userSelect == IDOK) {
		return true;
	} else {
		return false;
	}
}

void DialogUtility::bind_combobox(CComboBox_p cbobox, vector<estring>& dataColl) {
	cbobox->ResetContent();
	for (auto item : dataColl) {
		cbobox->AddString(item.c_str());
	}
	cbobox->SetCurSel(0);
}

estring DialogUtility::getFilePathByDialog(estring_cr defaultFileName, estring_cr fileFilter, bool checkExist) {
	estring fileName;
    Utility u;
	while(1) {
		CFileDialog fdlg(false, NULL, defaultFileName.c_str(), NULL, fileFilter.c_str());
		if(fdlg.DoModal() == IDOK) {

			fileName = fdlg.GetPathName();
			if(checkExist) {
				if(u.isFileExist(fileName) == true) {
					estring msg = _ET("File exist, Do you want to continue?");
					int userSelect = AfxMessageBox(msg.c_str(), MB_OKCANCEL | MB_ICONINFORMATION | MB_DEFBUTTON2);
					if(userSelect == IDCANCEL) {
						continue;
					}
				}
			}
			break;
		} else {
			fileName = _ET("");
			break;
		}
	}
	return fileName;
}

int DialogUtility::onDeviceEvent(eu32 wParam, DWORD_PTR lParam) {
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
	switch(wParam) {
	case DBT_DEVICEARRIVAL:
		if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			if(lpdbv->dbcv_flags == 0) {
				return DialogUtility::onDeviceEvent_Arrival;
			}
		}
		break;

	case DBT_DEVICEREMOVECOMPLETE:
		if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			if(lpdbv->dbcv_flags == 0) {
				return DialogUtility::onDeviceEvent_Remove;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

void DialogUtility::update_message() {
	MSG message;
	while(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}

void DialogUtility::executeShell(estring_cr verb, estring_cr cmd, estring_cr param, bool isHide) {
#if _MSC_VER == 1200 //VC6 compiler
	throw MyException(DU_NOT_SUPPORT_IN_VC6, "DU_NOT_SUPPORT_IN_VC6");
#else
    SHELLEXECUTEINFO info = { 0 };
	info.fMask = SEE_MASK_NOCLOSEPROCESS; //否則info.hProcess永遠是NULL
	info.nShow = SW_SHOW;
	if(isHide) {
		info.nShow = SW_HIDE;
	}
	info.cbSize = sizeof(info);
	info.lpVerb = verb.c_str();
	info.lpFile = cmd.c_str();
	info.lpParameters = param.c_str();

	ShellExecuteEx(&info);
	WaitForSingleObject(info.hProcess, INFINITE); //傳回值為 1
#endif
}

void DialogUtility::tabCtrl_addItem(CDialog& dlgPtr, int id, CTabCtrl& mainTabPtr, estring_cr description) {
	DialogUtility::TabStruct tmp;

	tmp.dlgPtr = &dlgPtr;
	tmp.mainTabPtr = &mainTabPtr;

	tmp.id = id;
	tmp.descrption = description;

	m_tabColls.push_back(tmp);
}

void DialogUtility::tabCtrl_init(void) {
	int  tabCnt = (int)m_tabColls.size();
	TabStruct tmp;
	for(int i = 0; i < tabCnt; i++) {
		tmp = m_tabColls[i];
		tmp.mainTabPtr->InsertItem(i, tmp.descrption.c_str());
		tmp.dlgPtr->Create(tmp.id, tmp.mainTabPtr);
		tabCtrl_moveLocation(tmp.mainTabPtr, tmp.dlgPtr);

		if(i == 0) {
			tmp.dlgPtr->ShowWindow(SW_SHOW);
		} else {
			tmp.dlgPtr->ShowWindow(SW_HIDE);
		}
	}
}

void DialogUtility::tabCtrl_moveLocation(CTabCtrl_sp mainTabPtr, CDialog_p targetDialog) {
	CRect rc;
	mainTabPtr->GetClientRect(rc);
	rc.top += 25;
	rc.bottom -= 10;
	rc.left += 10;
	rc.right -= 10;
	targetDialog->MoveWindow(&rc);
}

void DialogUtility::showMessageBox(echar_sp msg) {
	AfxMessageBox(msg);
}

void DialogUtility::showMessageBox(estring_cr msg) {
	AfxMessageBox(msg.c_str());
}

SYSTEMTIME DialogUtility::getTimeNow() {
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st;
}

estring DialogUtility::genTimeString(const SYSTEMTIME& current_date_time) {
	return genTimeString(current_date_time, _ET(":"), _ET("%02d"), 0);
}

estring DialogUtility::genTimeString(const SYSTEMTIME& current_date_time, estring_cr separateSign, estring format, const int separateType) {
	estring yy, mo, dd, hh, mm, ss;
	Utility u;
	if(format == _ET("")) {
        format = _ET("04%");
	} 
    yy = u.toString(current_date_time.wYear);
    mo = u.strFormat(format.c_str(), current_date_time.wMonth);
    dd = u.strFormat(format.c_str(), current_date_time.wDay);
    hh = u.strFormat(format.c_str(), current_date_time.wHour);
    mm = u.strFormat(format.c_str(), current_date_time.wMinute);
    ss = u.strFormat(format.c_str(), current_date_time.wSecond);
	estring res;
	if(separateType == 0) {
		res = yy + separateSign + mo + separateSign + dd + separateSign + hh + separateSign + mm + separateSign + ss;
	} else if(separateType == 1) {
		res = yy + separateSign + mo + dd;
	} else {
		res = yy + separateSign + mo + dd + separateSign + hh + mm + ss;
	}
	return res;
}

void DialogUtility::show_msg_to_txtbox(CEdit_p msgItem, bool isClean, estring_cr msg) {
	if (!msgItem) {
		THROW_MYEXCEPTION(GENERAL_FAIL, _ET("show_msg_to_txtbox: text item NULL"));
	}

	int end = 0;
	if (isClean) {
		msgItem->SetWindowText(msg.c_str());
	} else {
		if (end < MAX_TEXTBOX_SIZE) {
			end = msgItem->GetWindowTextLength();
			msgItem->SetSel(end, end);
			msgItem->ReplaceSel(msg.c_str());
		} else {
			//force clean text
			msgItem->SetWindowText(msg.c_str());
		}
	}
	msgItem->SetSel(end, end);
	update_message();
}