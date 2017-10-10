// Dialogzhou.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "Dialogzhou.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogzhou dialog


CDialogzhou::CDialogzhou(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogzhou::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogzhou)
	m_zhou2 = 0.0;
	m_zhou1 = 0;
	//}}AFX_DATA_INIT
}


void CDialogzhou::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogzhou)
	DDX_Text(pDX, IDC_EDIT1, m_zhou2);
	DDX_Text(pDX, IDC_EDIT2, m_zhou1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogzhou, CDialog)
	//{{AFX_MSG_MAP(CDialogzhou)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogzhou message handlers

void CDialogzhou::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogzhou::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogzhou::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	
}
