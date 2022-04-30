#pragma once
#include "DefineFiles/DefineFile.h"
#include "Utility/Utility.h"

#include "UtilityDialog/TypeDialog.h"
#include "UtilityDialog/DialogUtility.h"
#include "UtilityDialog/DialogViewUtility.h"


using namespace std;

enum UiIoDir {
    DATA_OUT = 0,
    DATA_IN = 1,
    NON_DATA = 2
};

#define TXT_CDB_CNT (16)


namespace EricCore {
    class CmderUi
    {
    public:
        CmderUi(void);
        ~CmderUi(void);

        
        CDialog_usp m_pDialog;
        CComboBox_usp m_cboCmdSel;

        CEdit_usp m_txtCdb[TXT_CDB_CNT];
        CEdit_usp m_txtDataLen;

        CButton_usp m_rdoDataIn;
        CButton_usp m_rdoDataOut;

        //nvme
        CButton_usp m_rdoAdminCmd;
        CButton_usp m_rdoIoCmd;

        CEdit_usp m_txtMsgArea_0;
        CEdit_usp m_txtMsgArea_1;

        //HexTable format
        CButton_usp m_show4Byte;


        void init(CDialog* dialog, int cmdSel, int cdw[16], int dataLen, int dataIn, int dataOut, int msgAread_0, int msgAread_1, int adminCmd, int ioCmd, int show4Byte);
        void send_msg_to_area_0(bool isClean, estring_cr msg);
        void send_msg_to_area_1(bool isClean, estring_cr msg);
        estring get_file_path_by_dlg();
        estring get_msg_area(eu32 select);

        void set_text_to_cmdbox(vector<estring>& strVector);
        void set_cmd_to_ui(eu32 cdb[TXT_CDB_CNT], eu32 datalen, bool isWrite, bool isAdminCmd, eu32 ctrlBit);

        bool get_data_dir(void);
        void set_data_dir(bool isWrite);

        bool get_cmd_type(void);
        void set_cmd_type(bool isAdmin);

        bool get_show_4b();

        
    private:
        DialogUtility m_du;
        DialogViewUtility m_dvu;
        Utility m_u;

    };
}