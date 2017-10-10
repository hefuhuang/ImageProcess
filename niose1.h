#if !defined(AFX_NIOSE1_H__26C9360B_75C5_4B6F_BBD2_242C90E6E303__INCLUDED_)
#define AFX_NIOSE1_H__26C9360B_75C5_4B6F_BBD2_242C90E6E303__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
// niose1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// niose dialog


class niose : public CDialog
{
// Construction
public:
	niose(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(niose)
	enum { IDD = IDD_DIALOG2 };
	int		m_nb;
	int		m_nh;
	int		m_xb;
	int		m_xh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(niose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(niose)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NIOSE1_H__26C9360B_75C5_4B6F_BBD2_242C90E6E303__INCLUDED_)
