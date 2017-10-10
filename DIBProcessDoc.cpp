// DIBProcessDoc.cpp : implementation of the CDIBProcessDoc class
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "MainFrm.h"
#include "DIBProcessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessDoc

IMPLEMENT_DYNCREATE(CDIBProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIBProcessDoc, CDocument)
	//{{AFX_MSG_MAP(CDIBProcessDoc)
	ON_COMMAND(ID_EDIT_RELOAD, OnEditReload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessDoc construction/destruction

CDIBProcessDoc::CDIBProcessDoc()
{
	m_hDIB=NULL;
	m_sizeDoc=CSize(1, 1);
}

CDIBProcessDoc::~CDIBProcessDoc()
{
	// 关闭前清除DIB对象
	if (m_hDIB!=NULL)
	{
		::GlobalFree((HGLOBAL)m_hDIB);
	}
}

BOOL CDIBProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDIBProcessDoc serialization

void CDIBProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessDoc diagnostics

#ifdef _DEBUG
void CDIBProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIBProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessDoc commands

BOOL CDIBProcessDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 报错
		ReportSaveLoadException(lpszPathName, &fe, 
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}
	
	DeleteContents();
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 尝试调用ReadDIBFile()读取图像
	TRY
	{
		m_hDIB=ReadDIBFile(file);
	}
	// 读取失败
	CATCH (CFileException, eLoad)
	{
		// 恢复状态
		file.Abort();
		EndWaitCursor();
		m_hDIB=NULL;
		
		// 报错
		ReportSaveLoadException(lpszPathName, eLoad, 
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}
	END_CATCH
	
	// 初始化DIB
	InitDIBData();
	
	// 恢复光标形状
	EndWaitCursor();
	
	// 判断读取文件是否成功
	if (m_hDIB==NULL)
	{
		// 报错
		CString strMsg;
		strMsg="读取图像时出错！可能是不支持该类型的图像文件！";
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
		
		return FALSE;
	}
	
	// 设置文件名称
	SetPathName(lpszPathName);
	
	// 设置脏标记
	SetModifiedFlag(FALSE);

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "图像载入完毕。", TRUE);

	return TRUE;
}

BOOL CDIBProcessDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// 打开文件
	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// 报错
		ReportSaveLoadException(lpszPathName, &fe, 
			TRUE, AFX_IDP_INVALID_FILENAME);
		
		return FALSE;
	}

	// 尝试调用SaveDIB保存图像
	BOOL bSuccess=FALSE;

	// 更改光标形状
	BeginWaitCursor();

	TRY
	{
		// 尝试保存图像
		bSuccess=SaveDIB(m_hDIB, file);

		file.Close();
	}
	CATCH (CException, eSave)
	{
		// 恢复状态
		file.Abort();
		EndWaitCursor();

		//报错
		ReportSaveLoadException(lpszPathName, eSave, 
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		return FALSE;
	}
	END_CATCH

	// 恢复光标形状
	EndWaitCursor();
	
	if (!bSuccess)
	{
		// 报错
		CString strMsg;
		strMsg="无法保存BMP图像！";
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
	}

	// 设置脏标记
	SetModifiedFlag(FALSE);

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "图像保存完毕。", TRUE);

	return bSuccess;
}

void CDIBProcessDoc::InitDIBData()
{
	// 初始化DIB对象
	
	// 如果DIB对象为空，直接返回
	if (m_hDIB==NULL)
	{
		return;
	}
	
	// 锁定图像内存区
	LPSTR lpDIB=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	
	// 判断图像是否过大
	if (DIBWidth(lpDIB)>INT_MAX || DIBHeight(lpDIB)>INT_MAX)
	{
		// 释放DIB对象
		::GlobalUnlock((HGLOBAL)m_hDIB);
		::GlobalFree((HGLOBAL)m_hDIB);
		m_hDIB=NULL;
		
		// 报错
		CString strMsg;
		strMsg="BMP图像太大！";
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
		
		return;
	}
	
	// 记录文档大小
	m_sizeDoc=CSize((int)DIBWidth(lpDIB), (int)DIBHeight(lpDIB));
	
	// 图像内存区解锁
	::GlobalUnlock((HGLOBAL)m_hDIB);
}

void CDIBProcessDoc::OnEditReload() 
{
	// 判断当前图像是否已经被改动并提示
	if (IsModified())
	{
		if (MessageBox(NULL, "重新打开图像将丢失所有改动！是否继续？", "系统提示", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			return;
		}
	}

	CFile file;
 	CFileException fe;
 
 	CString strPathName;
  	
	// 获取当前文件路径
 	strPathName=GetPathName();
 
	// 判断是否有对应文件
	if ( strPathName=="" )
	{
		return;
	}
	
 	// 重新打开文件
 	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
 	{
 		// 报错
 		ReportSaveLoadException(strPathName, &fe, 
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		return;
 	}
 	
 	// 更改光标形状
 	BeginWaitCursor();
 	
 	// 尝试调用ReadDIBFile()读取图像
 	TRY
 	{
 		m_hDIB=ReadDIBFile(file);
 	}
 	CATCH (CFileException, eLoad)
 	{
 		// 恢复状态
 		file.Abort();
 		EndWaitCursor();
 		m_hDIB=NULL;
 		
 		// 报错
 		ReportSaveLoadException(strPathName, eLoad, 
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		return;
 	}
 	END_CATCH
 	
 	// 初始化DIB
 	InitDIBData();
 	
 	// 恢复光标形状
 	EndWaitCursor();

	// 判断读取文件是否成功
 	if (m_hDIB==NULL)
 	{
 		// 报错
 		CString strMsg;
 		strMsg="读取图像时出错！可能是不支持该类型的图像文件！";
		MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
  		
 		return;
 	}
 	
	// 设置脏标记
 	SetModifiedFlag(FALSE);
 	
	// 更新视图
 	UpdateAllViews(NULL);

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "图像重载完毕。", TRUE);

 	return;
}

void CDIBProcessDoc::ReplaceHDIB(HDIB hDIB)
{
	// 替换DIB，在功能粘贴中用到该函数
	
	// 判断DIB是否为空
	if (m_hDIB!=NULL)
	{
		// 非空，则清除
		::GlobalFree((HGLOBAL)m_hDIB);
	}

	// 替换成新的DIB对象
	m_hDIB=hDIB;
}
