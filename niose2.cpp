// niose2.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "niose2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// niose2 dialog


niose2::niose2(CWnd* pParent /*=NULL*/)
	: CDialog(niose2::IDD, pParent)
{
	//{{AFX_DATA_INIT(niose2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void niose2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(niose2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(niose2, CDialog)
	//{{AFX_MSG_MAP(niose2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// niose2 message handlers
