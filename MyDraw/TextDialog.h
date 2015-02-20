#pragma once
#include "resource.h"

// TextDialog dialog

class TextDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TextDialog)

public:
	TextDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TextDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	CString ItemText;
};
