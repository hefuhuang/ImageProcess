// DlgMy.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "DlgMy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMy dialog


CDlgMy::CDlgMy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMy)
	m_dK = 0.0;
	m_dB = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgMy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMy)
	DDX_Text(pDX, IDC_EDIT1, m_dK);
	DDX_Text(pDX, IDC_EDIT2, m_dB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMy, CDialog)
	//{{AFX_MSG_MAP(CDlgMy)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMy message handlers
