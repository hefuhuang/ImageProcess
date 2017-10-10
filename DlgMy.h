#if !defined(AFX_DLGMY_H__7CB759ED_C335_46F0_9ED8_248ADDF3E76D__INCLUDED_)
#define AFX_DLGMY_H__7CB759ED_C335_46F0_9ED8_248ADDF3E76D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMy dialog

class CDlgMy : public CDialog
{
// Construction
public:
	CDlgMy(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMy)
	enum { IDD = IDD_DIALOG1 };
	double	m_dK;
	double	m_dB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMy)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMY_H__7CB759ED_C335_46F0_9ED8_248ADDF3E76D__INCLUDED_)
