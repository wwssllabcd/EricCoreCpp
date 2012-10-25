// testApDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "testAp.h"
#include "testApDlg.h"
#include ".\testapdlg.h"

#include "..\..\Test\TestCase\TestCaseSuite.h"

#include "..\..\src\Utility\Utility.h"


#include <string>
using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestApDlg 對話方塊



CtestApDlg::CtestApDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestApDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestApDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_msg, m_txtMsgArea);
}

BEGIN_MESSAGE_MAP(CtestApDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// showMessage function
CtestApDlg* m_pDlg;
void ShowMsg(string& tmsg, bool isClean)
{
	CString m_previous;
	CEdit* pMsgArea = &m_pDlg->m_txtMsgArea;

	//init
	m_previous="";
	if(isClean==false){
		pMsgArea->GetWindowText(m_previous);
	}

	pMsgArea->SetWindowText(m_previous + tmsg.c_str() );
}


// CtestApDlg 訊息處理常式

BOOL CtestApDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 將 "關於..." 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	m_pDlg = this;
	Utility::observerRegister(&ShowMsg);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CtestApDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，以便繪製圖示。
// 對於使用文件/檢視模式的 MFC 應用程式，框架會自動完成此作業。

void CtestApDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//當使用者拖曳最小化視窗時，系統呼叫這個功能取得游標顯示。
HCURSOR CtestApDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestApDlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//OnOK();

	TestCaseSuite tcu;
	tcu.start();


}
