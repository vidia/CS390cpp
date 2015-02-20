// TextDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyDraw.h"
#include "TextDialog.h"
#include "afxdialogex.h"


// TextDialog dialog

IMPLEMENT_DYNAMIC(TextDialog, CDialogEx)

TextDialog::TextDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TextDialog::IDD, pParent)
	, ItemText(_T(""))
{

}

TextDialog::~TextDialog()
{
}

void TextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ItemText);
}


BEGIN_MESSAGE_MAP(TextDialog, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &TextDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// TextDialog message handlers


void TextDialog::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
