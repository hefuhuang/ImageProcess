// PINYI.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "PINYI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PINYI dialog


PINYI::PINYI(CWnd* pParent /*=NULL*/)
	: CDialog(PINYI::IDD, pParent)
{
	//{{AFX_DATA_INIT(PINYI)
	m_x = 0;
	m_y = 0;
	theta = 0.0;
	//}}AFX_DATA_INIT
}


void PINYI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PINYI)
	DDX_Text(pDX, IDC_EDIT_x, m_x);
	DDX_Text(pDX, IDC_EDIT_Y, m_y);
	DDX_Text(pDX, IDC_EDIT1, theta);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PINYI, CDialog)
	//{{AFX_MSG_MAP(PINYI)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PINYI message handlers
