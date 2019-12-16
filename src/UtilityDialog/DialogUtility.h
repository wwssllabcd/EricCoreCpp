#pragma once


#include "DefineFiles/DefineFile.h"
#include "Utility/Utility.h"
#include "Utility/StringUtility.h"

#include <vector>
#include <afxcmn.h>  //CTabCtrl
using namespace std;

ERIC_GEN_OTHER_TYPE(CDialog);
ERIC_GEN_OTHER_TYPE(CComboBox);
ERIC_GEN_OTHER_TYPE(CButton);
ERIC_GEN_OTHER_TYPE(CEdit);

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

        void setFont(CEdit_p item, int size, const estring& fontName, bool reDraw, CFont* font);
        bool getUserYesNo(const estring& msg);
        estring getFilePathByDialog(const estring& defaultFileName, const estring& fileFilter, bool checkExist);
        estring getFilePathByDialog();

        // MFC platform
        ERIC_STATIC void updateOS();
        void executeShell(const estring& verb, const estring& cmd, const estring& param, bool isHide);

        //ComboBox
        void bind_combobox(CComboBox_p cbobox, vector<estring>& dataColl);

        // TabCtrl 
        void tabCtrl_moveLocation(CTabCtrl* mainTabPtr, CDialog* targetDialog);
        void tabCtrl_init(void);
        void tabCtrl_addItem(CDialog& dlgPtr, int id, CTabCtrl& mainTabPtr, const estring& description);

        int onDeviceEvent(eu32 wParam, DWORD_PTR lParam);
        void showMessageBox(const echar* msg);
        void showMessageBox(estring& msg);

        SYSTEMTIME getTimeNow();
        estring genTimeString(const SYSTEMTIME& current_date_time);
        estring genTimeString(const SYSTEMTIME& current_date_time, const estring& separateSign, estring format, const int separateType);

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
            return m_su.hexToU32(txt);
        }

        template <typename T>
        void setText(T* const pItem, const estring& msg) {
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
        StringUtility m_su;
    };
}