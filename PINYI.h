#if !defined(AFX_PINYI_H__53EA7E31_3C6F_43C3_B35F_53B24E13A7D2__INCLUDED_)
#define AFX_PINYI_H__53EA7E31_3C6F_43C3_B35F_53B24E13A7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PINYI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PINYI dialog

class PINYI : public CDialog
{
// Construction
public:
	PINYI(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PINYI)
	enum { IDD = IDD_DIALOG3 };
	int		m_x;
	int		m_y;
	double	theta;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PINYI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PINYI)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINYI_H__53EA7E31_3C6F_43C3_B35F_53B24E13A7D2__INCLUDED_)
