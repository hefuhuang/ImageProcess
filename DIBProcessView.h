// DIBProcessView.h : interface of the CDIBProcessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBPROCESSVIEW_H__0096A263_A224_423F_9F3C_34023FCB8E48__INCLUDED_)
#define AFX_DIBPROCESSVIEW_H__0096A263_A224_423F_9F3C_34023FCB8E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDIBProcessView : public CView
{
protected: // create from serialization only
	DECLARE_DYNCREATE(CDIBProcessView)

// Attributes
public:
	CDIBProcessDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBProcessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDIBProcessView)
	afx_msg void OnProcHisto();
	afx_msg void OnProcTemplate();
	afx_msg void OnMenuCstj();
	afx_msg void OnNXBJZLB();
	afx_msg void Onxbjzlb();
	afx_msg void OnMejhh();
	afx_msg void OnMeqfs();
	afx_msg void OnMEYuZhiHua();
	afx_msg void OnMpengzhangyufushi();
	afx_msg void Onzhongzhilvbo();
	afx_msg void OnGeochuli();
	afx_msg void Onzuixiaozhichuli();
	afx_msg void OnGeopro();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DIBProcessView.cpp
inline CDIBProcessDoc* CDIBProcessView::GetDocument()
   { return (CDIBProcessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBPROCESSVIEW_H__0096A263_A224_423F_9F3C_34023FCB8E48__INCLUDED_)
