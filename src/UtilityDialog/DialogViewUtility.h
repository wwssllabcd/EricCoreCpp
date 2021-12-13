#pragma once
#include "DefineFiles/DefineFile.h"
#include "TypeDialog.h"
#include <vector>
#include "DialogUtility.h"

using namespace std;

namespace EricCore {
    class DialogViewUtility
    {
    public:
        DialogViewUtility(void);
        ~DialogViewUtility(void);
        void send_msg(CEdit_p msgItem, bool isClean, estring_cr msg);
        
        void set_values_to_txtbox(eu32* values, CEdit_p* uiTxtBoxs, eu32 size, eu32 ctrlBit);
        void get_values_from_txtbox(eu32* values, CEdit_p* uiTxtBoxs, eu32 size, eu32 ctrlBit);
        
        void set_text_to_cmdbox(CComboBox_p cboCmdSel, vector<estring>& strVector);

        void set_2_check_box(CButton_p rdo1, CButton_p rdo2, bool set_param_1st);

        template<typename T>
        vector<estring>& convert_to_string_vector(vector<T>& tVector, vector<estring>& strVector) {
            for (eu32 i = 0; i < tVector.size(); i++) {
                strVector[i] = tVector[i].description;
            }
            return strVector;
        }

    private:
        DialogUtility m_du;

    };
}