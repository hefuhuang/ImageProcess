// DIBProcess.h : main header file for the DIBPROCESS application
//

#if !defined(AFX_DIBPROCESS_H__A625C7E5_8ACF_4BB5_8537_4C72E7D3D884__INCLUDED_)
#define AFX_DIBPROCESS_H__A625C7E5_8ACF_4BB5_8537_4C72E7D3D884__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessApp:
// See DIBProcess.cpp for the implementation of this class
//

class CDIBProcessApp : public CWinApp
{
public:
	CDIBProcessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBProcessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDIBProcessApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBPROCESS_H__A625C7E5_8ACF_4BB5_8537_4C72E7D3D884__INCLUDED_)
