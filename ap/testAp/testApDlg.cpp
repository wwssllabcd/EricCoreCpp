// testApDlg.cpp : ��@��
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


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CtestApDlg ��ܤ��



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


// CtestApDlg �T���B�z�`��

BOOL CtestApDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �N "����..." �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	m_pDlg = this;
	Utility::observerRegister(&ShowMsg);

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A�H�Kø�s�ϥܡC
// ���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A�ج[�|�۰ʧ������@�~�C

void CtestApDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//��ϥΪ̩즲�̤p�Ƶ����ɡA�t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CtestApDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestApDlg::OnBnClickedOk()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	//OnOK();

	TestCaseSuite tcu;
	tcu.start();


}
