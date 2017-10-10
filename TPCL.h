#if !defined(AFX_TPCL_H__55F49427_8742_4D35_A78A_6A8893A28D8D__INCLUDED_)
#define AFX_TPCL_H__55F49427_8742_4D35_A78A_6A8893A28D8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TPCL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTPCL dialog

class CTPCL : public CDialog
{
// Construction
public:
	CTPCL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTPCL)
	enum { IDD = IDD_DIALOG1 };
	int		m_n;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTPCL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTPCL)
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPCL_H__55F49427_8742_4D35_A78A_6A8893A28D8D__INCLUDED_)
