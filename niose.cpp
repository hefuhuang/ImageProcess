// niose.cpp : implementation file
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "niose.h"


#include "ImageProcessor.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int panbie=0;
/////////////////////////////////////////////////////////////////////////////
// niose dialog


niose::niose(CWnd* pParent /*=NULL*/)
	: CDialog()
{
	//{{AFX_DATA_INIT(niose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void niose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(niose)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(niose, CDialog)
	//{{AFX_MSG_MAP(niose)
	ON_BN_CLICKED(IDC_HUJIAOpanbie, OnHUJIAOpanbie)
	ON_BN_CLICKED(IDC_BAIYANpanbie, OnBAIYANpanbie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// niose message handlers

void niose::OnHUJIAOpanbie() 
{
	// TODO: Add your control notification handler code here
	panbie =1;
}

void niose::OnBAIYANpanbie() 
{
	// TODO: Add your control notification handler code here
	panbie =2;
	
}
