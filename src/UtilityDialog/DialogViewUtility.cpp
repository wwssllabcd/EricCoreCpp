

#include "stdafx.h"
#include "DialogViewUtility.h"
#include "Utility/Utility.h"

using namespace EricCore;

#define MAX_TEXTBOX_SIZE (_128K)

DialogViewUtility::DialogViewUtility(void){}
DialogViewUtility::~DialogViewUtility(void){}

void DialogViewUtility::send_msg(CEdit_p msgItem, bool isClean, estring_cr msg) {
    m_du.show_msg_to_txtbox(msgItem, isClean, msg);
}

void DialogViewUtility::set_values_to_txtbox(eu32* values, CEdit_p* uiTxtBoxs,  eu32 size, eu32 ctrlBit) {
    Utility u;
    for (eu32 i = 0; i < size; i++) {
        if (ctrlBit & (1 << i)) {
            uiTxtBoxs[i]->SetWindowText(u.toHexString(values[i]).c_str());
        }
    }
}

void DialogViewUtility::get_values_from_txtbox(eu32* values, CEdit_p* uiTxtBoxs, eu32 size, eu32 ctrlBit) {
    Utility u;
    DialogUtility du;
    for (eu32 i = 0; i < size; i++) {
        if (ctrlBit & (1 << i)) {
            values[i] = u.hexToU32(du.getText<CEdit>(uiTxtBoxs[i]));
        }
    }
}

void DialogViewUtility::set_text_to_cmdbox(CComboBox_p cboCmdSel, vector<estring>& strVector) {
    Utility u;
    for (eu32 i = 0; i < strVector.size(); i++) {
        estring desc = u.strFormat(_ET("%02d-"), i) + strVector[i];
        cboCmdSel->AddString(desc.c_str());
    }
}

void DialogViewUtility::set_2_check_box(CButton_p rdo1, CButton_p rdo2, bool set_param_1st) {
    if (set_param_1st) {
        rdo1->SetCheck(true);
        rdo2->SetCheck(false);
    } else {
        rdo1->SetCheck(false);
        rdo2->SetCheck(true);
    }
}