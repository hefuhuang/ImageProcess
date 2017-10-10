#if !defined(AFX_DIALOGZHOU_H__3777782B_D033_429E_8AB9_615041139CD7__INCLUDED_)
#define AFX_DIALOGZHOU_H__3777782B_D033_429E_8AB9_615041139CD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialogzhou.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogzhou dialog

class CDialogzhou : public CDialog
{
// Construction
public:
	CDialogzhou(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogzhou)
	enum { IDD = IDD_DIALOG1 };
	double	m_zhou2;
	int		m_zhou1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogzhou)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogzhou)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGZHOU_H__3777782B_D033_429E_8AB9_615041139CD7__INCLUDED_)
