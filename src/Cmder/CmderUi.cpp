

#include "stdafx.h"
#include "CmderUi.h"

using namespace EricCore;

CmderUi::CmderUi(void){}
CmderUi::~CmderUi(void){}

void CmderUi::init(CDialog* dialog, int cmdSel, int cdw[16], int dataLen
    , int dataIn, int dataOut, int msgAread_0, int msgAread_1, int adminCmd, int ioCmd, int show4Byte) {

    DialogUtility du;

    m_pDialog = dialog;
    m_cboCmdSel = du.getDlgItemPtr<CComboBox>(m_pDialog, cmdSel);
    m_txtMsgArea_0 = du.getDlgItemPtr<CEdit>(m_pDialog, msgAread_0);
    m_txtMsgArea_1 = du.getDlgItemPtr<CEdit>(m_pDialog, msgAread_1);
    m_txtDataLen = du.getDlgItemPtr<CEdit>(m_pDialog, dataLen);
    m_rdoDataIn = du.getDlgItemPtr<CButton>(m_pDialog, dataIn);
    m_rdoDataOut = du.getDlgItemPtr<CButton>(m_pDialog, dataOut);

    m_rdoAdminCmd = du.getDlgItemPtr<CButton>(m_pDialog, adminCmd);
    m_rdoIoCmd = du.getDlgItemPtr<CButton>(m_pDialog, ioCmd);

    m_show4Byte = du.getDlgItemPtr<CButton>(m_pDialog, show4Byte);

    for (eu32 i = 0; i < TXT_CDB_CNT; i++) {
        m_txtCdb[i] = du.getDlgItemPtr<CEdit>(m_pDialog, cdw[i]);
    }
}

void CmderUi::send_msg_to_area_0(bool isClean, estring_cr msg) {
    m_dvu.send_msg(m_txtMsgArea_0, isClean, msg);
}

void CmderUi::send_msg_to_area_1(bool isClean, estring_cr msg) {
    m_dvu.send_msg(m_txtMsgArea_1, isClean, msg);
}

void CmderUi::set_text_to_cmdbox(vector<estring>& strVector) {
    Utility u;
    for (eu32 i = 0; i < strVector.size(); i++) {
        estring desc = u.strFormat(_ET("%02d-"), i) + strVector[i];
        m_cboCmdSel->AddString(desc.c_str());
    }
}

void CmderUi::set_cmd_to_ui(eu32 cdb[TXT_CDB_CNT], eu32 datalen, bool isWrite, bool isAdminCmd, eu32 ctrlBit) {
    m_dvu.set_values_to_txtbox(cdb, m_txtCdb, TXT_CDB_CNT, ctrlBit);
    if (ctrlBit & E_BIT_16) {
        m_txtDataLen->SetWindowText(m_u.toHexString(datalen).c_str());
    }
    if (ctrlBit & E_BIT_17) {
        set_data_dir(isWrite);
    }
    if (ctrlBit & E_BIT_18) {
        set_cmd_type(isAdminCmd);
    }
}

bool CmderUi::get_data_dir(void) {
    if (m_rdoDataOut->GetCheck() == 1) {
        return true;
    }
    return false;
}

void CmderUi::set_data_dir(bool isWrite) {
    m_dvu.set_2_check_box(m_rdoDataOut, m_rdoDataIn, isWrite);
}

bool CmderUi::get_cmd_type(void) {
    if (m_rdoAdminCmd->GetCheck() == 1) {
        return true;
    }
    return false;
}

void CmderUi::set_cmd_type(bool isAdmin) {
    m_dvu.set_2_check_box(m_rdoAdminCmd, m_rdoIoCmd, isAdmin);
}

bool CmderUi::get_show_4b() {
    return m_du.getCheck(m_show4Byte);
}

estring CmderUi::get_file_path_by_dlg() {
    estring defaultFileName = m_du.getText(m_cboCmdSel) + _ET(".txt");
    estring filePath = m_du.getFilePathByDialog(defaultFileName);
    return filePath;
}

estring CmderUi::get_msg_area(eu32 select) {

    if (select == 0) {
        return m_du.getText(m_txtMsgArea_0);
    }
    return m_du.getText(m_txtMsgArea_1);
    
}