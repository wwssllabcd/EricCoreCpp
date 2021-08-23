#pragma once


#include "DefineFiles/DefineFile.h"
#include "Utility/Utility.h"


#include <vector>
#include <afxcmn.h>  //CTabCtrl
using namespace std;

ERIC_GEN_OTHER_TYPE(CDialog);
ERIC_GEN_OTHER_TYPE(CComboBox);
ERIC_GEN_OTHER_TYPE(CButton);
ERIC_GEN_OTHER_TYPE(CEdit);
ERIC_GEN_OTHER_TYPE(CTabCtrl);
ERIC_GEN_OTHER_TYPE(CFont);

namespace EricCore {
    class DialogUtility
    {
    public:
        DialogUtility(void);
        ~DialogUtility(void);

        ERIC_STATIC int const onDeviceEvent_Arrival;
        ERIC_STATIC int const onDeviceEvent_Remove;

        //---------------------
        int get_cur_select(CComboBox_p cbobox);

        bool getCheck(CButton_p btnCheck);
        void setCheck(CButton_p btnCheck, bool isCheck);

        void setFont(CEdit_p item, int size, estring_cr fontName, bool reDraw, CFont_p font);
        bool getUserYesNo(estring_cr msg);
        estring getFilePathByDialog(estring_cr defaultFileName, estring_cr fileFilter, bool checkExist);
        estring getFilePathByDialog();

        // MFC platform
        ERIC_STATIC void update_message();
        void executeShell(estring_cr verb, estring_cr cmd, estring_cr param, bool isHide);

        //ComboBox
        void bind_combobox(CComboBox_p cbobox, vector<estring>& dataColl);

        // TabCtrl 
        void tabCtrl_moveLocation(CTabCtrl_sp mainTabPtr, CDialog_p targetDialog);
        void tabCtrl_init(void);
        void tabCtrl_addItem(CDialog& dlgPtr, int id, CTabCtrl& mainTabPtr, estring_cr description);

        int onDeviceEvent(eu32 wParam, DWORD_PTR lParam);
        void showMessageBox(echar_sp msg);
        void showMessageBox(estring_cr msg);

        SYSTEMTIME getTimeNow();
        estring genTimeString(const SYSTEMTIME& current_date_time);
        estring genTimeString(const SYSTEMTIME& current_date_time, estring_cr separateSign, estring format, const int separateType);

        void show_txt_msg(CEdit_p pMsgArea, bool isClean, estring_cr msg);

        // ------------ template ------------
        template <typename T>
        T* getDlgItemPtr(CDialog_p dlg, int Id) {
            return static_cast<T*>(dlg->GetDlgItem(Id));
        }

        template <typename T>
        estring getText(T* const pItem) {
            CString tmp;
            pItem->GetWindowText(tmp);
            estring s(tmp);
            return s;
        }

        template <typename T>
        eu32 getHexValue(T* pItem) {
            estring txt = getText(pItem);
            return m_u.hexToU32(txt);
        }

        template <typename T>
        void setText(T* const pItem, estring_cr msg) {
            pItem->SetWindowText(msg.c_str());
        }

    private:
        // For TabCtrl 
        struct TabStruct {
            CDialog* dlgPtr;
            eu32 id;
            estring descrption;
            CTabCtrl* mainTabPtr;
        };
        vector< TabStruct > m_tabColls;
        //CDialog* m_pDialog;
        Utility m_u;
    };
}