#include "StdAfx.h"
#include ".\dialogutility.h"
#include "Dbt.h"

#include "../Utility/Utility.h"

using namespace EricCore;

int const DialogUtility::onDeviceEvent_Arrival=1;
int const DialogUtility::onDeviceEvent_Remove=2;

DialogUtility::DialogUtility(CDialog* dialogObject)
:m_pDialog(0)
{
	m_pDialog = dialogObject;
}

DialogUtility::DialogUtility(void)
:m_pDialog(0)
{
}


DialogUtility::~DialogUtility(void)
{
}

void DialogUtility::setDialogPoint(CDialog* dialogObject){
	m_pDialog = dialogObject;
}

CWnd* DialogUtility::getDlgItem(int Id){
	return m_pDialog->GetDlgItem(Id);
}

ULONG DialogUtility::getCEditTextToInt(int Id){
	return Utility::toInt( this->getCEditText(Id) );
}

ULONG DialogUtility::getCEditTextToHex(int Id){
	tstring tmp = this->getCEditText(Id) ;
	return Utility::hexStringToULONG(tmp);
}

tstring DialogUtility::getCEditText(int Id){
	return _getText<CEdit*>(Id);
}

tstring DialogUtility::getComboBoxText(int Id){
	return _getText<CComboBox*>(Id);
}

tstring DialogUtility::getCButtonText(int Id){
	return _getText<CButton*>(Id);
}

template <typename T>tstring DialogUtility::_getText(int Id){
	CString tmp;
	static_cast<T>(m_pDialog->GetDlgItem(Id))->GetWindowText( tmp );
	tstring s = tmp;
	return s;
}


void DialogUtility::setCButtonText(int id, tstring msg){
	static_cast<CButton*>(m_pDialog->GetDlgItem(id))->SetWindowText(msg.c_str() );
}

void DialogUtility::setCEditText(int id, tstring msg){
	static_cast<CEdit*>(m_pDialog->GetDlgItem(id))->SetWindowText(msg.c_str() );
}

void DialogUtility::setCEditText(int id, const char* msg){
	static_cast<CEdit*>(m_pDialog->GetDlgItem(id))->SetWindowText( msg );
}

void DialogUtility::setCEditTextToInt(int id, int value){
	setCEditText(id, Utility::toString(value) );
}

void DialogUtility::setCEditTextToHex(int id, int value){
	setCEditText(id, Utility::toHexString(value) );
}


//// Do not implement setCEditText by int value
//// if u want to set hex text, you should convert text first
//void DialogUtility::setCEditText(int id, int value){
//	tstring tmp = Utility::toString(value);
//	this->setCEditText(id, tmp);
//}

bool DialogUtility::getCheck(int id){
	int stop = static_cast<CButton*>(m_pDialog->GetDlgItem(id))->GetCheck();
	if(stop==1){
		return true;
	}else{
		return false;
	}
}

void DialogUtility::setCheck(int id, bool isCheck){
	static_cast<CButton*>( m_pDialog->GetDlgItem(id))->SetCheck(isCheck);
}

int DialogUtility::getComboBoxSel(int id){
	return static_cast<CComboBox*>( m_pDialog->GetDlgItem(id))->GetCurSel();
}


vector< pair<int, CFont*> > DialogUtility::m_ItemFontColl;
void DialogUtility::setFont(int id, int size, tstring fontName, bool reDraw){
	bool hasFind=false;
	size_t idx=0;
	for(idx=0; idx<m_ItemFontColl.size(); idx++){
		if( m_ItemFontColl[idx].first == id){
			hasFind=true;
		}
	}

	CFont* pFont;
	if(hasFind){
		pFont = m_ItemFontColl[idx].second;
		m_ItemFontColl[idx].first =0;
		m_ItemFontColl[idx].second =0;
		delete pFont;
	}else{
		pair<int, CFont*> tmp;
		m_ItemFontColl.push_back(tmp);
		idx = m_ItemFontColl.size()-1;
	}

	pFont = new CFont;
	pFont->CreatePointFont(size, fontName.c_str() );
	m_ItemFontColl[idx].first = id;
	m_ItemFontColl[idx].second = pFont;
	static_cast<CEdit*>(m_pDialog->GetDlgItem(id))->SetFont(pFont, reDraw);
}

bool DialogUtility::getUserYesNo(tstring msg){
	int userSelect = AfxMessageBox(msg.c_str(), MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2);
	if(userSelect == IDOK){
		return true;
	}else{
		return false;
	}
}

void DialogUtility::comboBoxBinding( int comboBoxId, vector<tstring>& dataColl ){
	CComboBox* pCbx = 	static_cast<CComboBox*>( m_pDialog->GetDlgItem(comboBoxId) );
	pCbx->ResetContent();
	size_t cnt = dataColl.size();
	size_t i=0;
	for(i=0; i<cnt; i++){
		pCbx->AddString( dataColl[i].c_str() );
	}
	pCbx->SetCurSel(0);
}

void DialogUtility::setComboBox_addString(int comboBoxId, const tstring& item){
	static_cast<CComboBox*>( m_pDialog->GetDlgItem(comboBoxId) )->AddString( item.c_str() ) ;
}


tstring DialogUtility::getFilePathByDialog(const tstring& defaultFileName, const tstring& fileFilter, bool checkExist)
{
	// TODO: 在此加入控制項告知處理常式程式碼
	tstring fileName;
	while(1){

		CFileDialog fdlg(false, NULL, defaultFileName.c_str(), NULL, fileFilter.c_str() );
		if( fdlg.DoModal() == IDOK){

			fileName = fdlg.GetPathName();
			if( checkExist ){
				if( Utility::isFileExist(fileName) == true ){
					tstring msg = _T("File exist, Do you want to continue?");
					int userSelect = AfxMessageBox(msg.c_str(), MB_OKCANCEL|MB_ICONINFORMATION|MB_DEFBUTTON2);
					if( userSelect==IDCANCEL ){
						continue;
					}
				}
			}
			break;
		}else{
			fileName="";
			break;
		}

	}

	return fileName;
}

tstring DialogUtility::getFilePathByDialog()
{
	return getFilePathByDialog("", "", false);
}


int DialogUtility::onDeviceEvent(UINT wParam, DWORD_PTR lParam){
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
	switch (wParam)	{
		case DBT_DEVICEARRIVAL:
			if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)	{
				PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
				if (lpdbv->dbcv_flags == 0 ){
					return DialogUtility::onDeviceEvent_Arrival;
				}
			}
			break;

		case DBT_DEVICEREMOVECOMPLETE:
			if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)	{
				PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
				if (lpdbv->dbcv_flags == 0 ){
					return DialogUtility::onDeviceEvent_Remove;
				}
			}
			break;
		default:
			break;
	}
	return 0;
}


void DialogUtility::updateOS(){
	MSG message;
	while( ::PeekMessage( &message, NULL, 0, 0, PM_REMOVE) ){
		::TranslateMessage( &message );
		::DispatchMessage( &message );
	}
}

void DialogUtility::executeShell( tstring verb, tstring cmd, tstring param, bool isHide){
#if _MSC_VER == 1200 //VC6 compiler
	throw MyException(DU_NOT_SUPPORT_IN_VC6, "DU_NOT_SUPPORT_IN_VC6");
#else
	SHELLEXECUTEINFO info;
	memset(&info, 0, sizeof(info));
	info.fMask= SEE_MASK_NOCLOSEPROCESS; //否則info.hProcess永遠是NULL
	info.nShow=SW_SHOW;
	if( isHide ){
		info.nShow=SW_HIDE; 
	}
	info.cbSize = sizeof(info);
	info.lpVerb = verb.c_str();
	info.lpFile = cmd.c_str();
	info.lpParameters = param.c_str();

	ShellExecuteEx(&info);
	WaitForSingleObject(info.hProcess, INFINITE); //傳回值為 1
	
	
#endif
}

void DialogUtility::tabCtrl_addItem(CDialog& dlgPtr, int id, CTabCtrl& mainTabPtr, tstring description){
	DialogUtility::TabStruct tmp;

	tmp.dlgPtr = &dlgPtr;
	tmp.mainTabPtr = &mainTabPtr;

	tmp.id = id;
	tmp.descrption = description;

	m_tabColls.push_back(tmp);
}


void DialogUtility::tabCtrl_init(void){
	int  tabCnt = (int)m_tabColls.size();
	TabStruct tmp;
	for(int i=0; i<tabCnt; i++){
		tmp = m_tabColls[i];
		tmp.mainTabPtr->InsertItem (i, tmp.descrption.c_str() );
		tmp.dlgPtr->Create(tmp.id, tmp.mainTabPtr);
		tabCtrl_moveLocation(tmp.mainTabPtr, tmp.dlgPtr);

		if( i==0){
			tmp.dlgPtr->ShowWindow(SW_SHOW);
		}else{
			tmp.dlgPtr->ShowWindow(SW_HIDE);
		}
	}
}

void DialogUtility::tabCtrl_moveLocation(CTabCtrl* mainTabPtr, CDialog* targetDialog){

	CRect rc;
	mainTabPtr->GetClientRect(rc);
	rc.top += 25;
	rc.bottom -= 10;
	rc.left += 10;
	rc.right -= 10;
	targetDialog->MoveWindow(&rc);
}

