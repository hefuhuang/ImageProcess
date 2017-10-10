// TPCL.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "TPCL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTPCL dialog


CTPCL::CTPCL(CWnd* pParent /*=NULL*/)
	: CDialog(CTPCL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTPCL)
	m_n = 0;
	//}}AFX_DATA_INIT
}


void CTPCL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTPCL)
	DDX_Text(pDX, IDC_EDIT1, m_n);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTPCL, CDialog)
	//{{AFX_MSG_MAP(CTPCL)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTPCL message handlers




void CTPCL::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
