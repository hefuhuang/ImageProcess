// niose1.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "niose1.h"   



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int panbie =0;
/////////////////////////////////////////////////////////////////////////////
// niose dialog


niose::niose(CWnd* pParent /*=NULL*/)
	: CDialog(niose::IDD, pParent)
{
	//{{AFX_DATA_INIT(niose)
	m_nb = 0;
	m_nh = 0;
	m_xb = -1;
	m_xh = -1;
	//}}AFX_DATA_INIT
}


void niose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(niose)
	DDX_Text(pDX, IDC_BAIYAN, m_nb);
	DDX_Text(pDX, IDC_HUJIAO, m_nh);
	DDX_Radio(pDX, IDC_BAIYANpanbie, m_xb);
	DDX_Radio(pDX, IDC_HUJIAOpanbie, m_xh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(niose, CDialog)
	//{{AFX_MSG_MAP(niose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()






