#pragma once

#include "../Utility/DefineFiles/DefineFile.h"
#include <vector>

using namespace std;

namespace EricCore{
	class DialogUtility
	{
	public:

		DialogUtility(CDialog* dialogObject);
		DialogUtility(void);
		~DialogUtility(void);

		void setDialogPoint(CDialog* dialogObject);
		CWnd* getDlgItem(int Id);


		tstring getCEditText(int Id);
		tstring getComboBoxText(int Id);

		ULONG getCEditTextToInt(int Id);
		ULONG getCEditTextToHex(int Id);

		void setCEditText(int id, tstring msg);
		void setCEditText(int id, const char* msg);
		void setCEditTextToInt(int id, int value);
		void setCEditTextToHex(int id, int value);

		tstring getCButtonText(int Id);
		void setCButtonText(int id, tstring msg);
		bool getCheck(int id);
		void setCheck(int id, bool isCheck);
		void setFont(int id, int size, tstring fontName, bool reDraw);

		bool getUserYesNo(tstring msg);
		tstring getFilePathByDialog(const tstring& defaultFileName, const tstring& fileFilter, bool checkExist);

		// MFC platform
		static void updateOS();
		static void executeShell(tstring verb, tstring cmd, tstring param, bool isHide);


		//ComboBox
		void comboBoxBinding( int comboBoxId, vector<tstring>& dataColl );
		void setComboBox_addString(int comboBoxId, const tstring& item);
		int getComboBoxSel(int id);

		// TabCtrl 
		void tabCtrl_moveLocation(CTabCtrl* mainTabPtr, CDialog* targetDialog);
		void tabCtrl_init(void);
		void tabCtrl_addItem(CDialog& dlgPtr, int id, CTabCtrl& mainTabPtr, tstring description);

		static int onDeviceEvent(UINT wParam, DWORD_PTR lParam);
		static int const onDeviceEvent_Arrival;
		static int const onDeviceEvent_Remove;

	private:
		template <typename T>tstring _getText(int Id);

		// For TabCtrl 
		struct TabStruct{
			CDialog* dlgPtr;
			UINT id;
			tstring descrption;
			CTabCtrl* mainTabPtr;
		};
		vector< TabStruct > m_tabColls;

		CDialog* m_pDialog;
		static vector< pair<int, CFont*> > m_ItemFontColl;
	};
}