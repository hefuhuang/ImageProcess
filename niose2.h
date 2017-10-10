#if !defined(AFX_NIOSE2_H__9A3BEC5D_DD57_4451_8E58_6A1244129FE8__INCLUDED_)
#define AFX_NIOSE2_H__9A3BEC5D_DD57_4451_8E58_6A1244129FE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// niose2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// niose2 dialog

class niose2 : public CDialog
{
// Construction
public:
	niose2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(niose2)
	enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(niose2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(niose2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NIOSE2_H__9A3BEC5D_DD57_4451_8E58_6A1244129FE8__INCLUDED_)
