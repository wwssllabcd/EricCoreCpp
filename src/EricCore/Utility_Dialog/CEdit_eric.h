#pragma once


// CEdit_eric

class CEdit_eric : public CEdit
{
	DECLARE_DYNAMIC(CEdit_eric)

public:
	CEdit_eric();
	virtual ~CEdit_eric();

	BOOL CEdit_eric::PreTranslateMessage(MSG* pMsg);

protected:
	DECLARE_MESSAGE_MAP()
};


