// DIBProcessDoc.h : interface of the CDIBProcessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "ImageProcessor.h"

#if !defined(AFX_DIBPROCESSDOC_H__3F3A7CB8_2FD1_458D_A330_704AA2E9C038__INCLUDED_)
#define AFX_DIBPROCESSDOC_H__3F3A7CB8_2FD1_458D_A330_704AA2E9C038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif   // _MSC_VER > 1000


class CDIBProcessDoc : public CDocument
{
protected: // create from serialization only
	CDIBProcessDoc();
	DECLARE_DYNCREATE(CDIBProcessDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
protected:
	HDIB m_hDIB;
	CSize m_sizeDoc;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIBProcessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDIBProcessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDIBProcessDoc)
	afx_msg void OnEditReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIBPROCESSDOC_H__3F3A7CB8_2FD1_458D_A330_704AA2E9C038__INCLUDED_)
