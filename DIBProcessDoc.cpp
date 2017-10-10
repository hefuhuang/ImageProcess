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
	// �ر�ǰ���DIB����
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

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ����
		ReportSaveLoadException(lpszPathName, &fe, 
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}
	
	DeleteContents();
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���Ե���ReadDIBFile()��ȡͼ��
	TRY
	{
		m_hDIB=ReadDIBFile(file);
	}
	// ��ȡʧ��
	CATCH (CFileException, eLoad)
	{
		// �ָ�״̬
		file.Abort();
		EndWaitCursor();
		m_hDIB=NULL;
		
		// ����
		ReportSaveLoadException(lpszPathName, eLoad, 
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);

		return FALSE;
	}
	END_CATCH
	
	// ��ʼ��DIB
	InitDIBData();
	
	// �ָ������״
	EndWaitCursor();
	
	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (m_hDIB==NULL)
	{
		// ����
		CString strMsg;
		strMsg="��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		return FALSE;
	}
	
	// �����ļ�����
	SetPathName(lpszPathName);
	
	// ��������
	SetModifiedFlag(FALSE);

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ͼ��������ϡ�", TRUE);

	return TRUE;
}

BOOL CDIBProcessDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// ����
		ReportSaveLoadException(lpszPathName, &fe, 
			TRUE, AFX_IDP_INVALID_FILENAME);
		
		return FALSE;
	}

	// ���Ե���SaveDIB����ͼ��
	BOOL bSuccess=FALSE;

	// ���Ĺ����״
	BeginWaitCursor();

	TRY
	{
		// ���Ա���ͼ��
		bSuccess=SaveDIB(m_hDIB, file);

		file.Close();
	}
	CATCH (CException, eSave)
	{
		// �ָ�״̬
		file.Abort();
		EndWaitCursor();

		//����
		ReportSaveLoadException(lpszPathName, eSave, 
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		return FALSE;
	}
	END_CATCH

	// �ָ������״
	EndWaitCursor();
	
	if (!bSuccess)
	{
		// ����
		CString strMsg;
		strMsg="�޷�����BMPͼ��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	}

	// ��������
	SetModifiedFlag(FALSE);

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ͼ�񱣴���ϡ�", TRUE);

	return bSuccess;
}

void CDIBProcessDoc::InitDIBData()
{
	// ��ʼ��DIB����
	
	// ���DIB����Ϊ�գ�ֱ�ӷ���
	if (m_hDIB==NULL)
	{
		return;
	}
	
	// ����ͼ���ڴ���
	LPSTR lpDIB=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
	
	// �ж�ͼ���Ƿ����
	if (DIBWidth(lpDIB)>INT_MAX || DIBHeight(lpDIB)>INT_MAX)
	{
		// �ͷ�DIB����
		::GlobalUnlock((HGLOBAL)m_hDIB);
		::GlobalFree((HGLOBAL)m_hDIB);
		m_hDIB=NULL;
		
		// ����
		CString strMsg;
		strMsg="BMPͼ��̫��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		return;
	}
	
	// ��¼�ĵ���С
	m_sizeDoc=CSize((int)DIBWidth(lpDIB), (int)DIBHeight(lpDIB));
	
	// ͼ���ڴ�������
	::GlobalUnlock((HGLOBAL)m_hDIB);
}

void CDIBProcessDoc::OnEditReload() 
{
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�����ʾ
	if (IsModified())
	{
		if (MessageBox(NULL, "���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", "ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			return;
		}
	}

	CFile file;
 	CFileException fe;
 
 	CString strPathName;
  	
	// ��ȡ��ǰ�ļ�·��
 	strPathName=GetPathName();
 
	// �ж��Ƿ��ж�Ӧ�ļ�
	if ( strPathName=="" )
	{
		return;
	}
	
 	// ���´��ļ�
 	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
 	{
 		// ����
 		ReportSaveLoadException(strPathName, &fe, 
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		return;
 	}
 	
 	// ���Ĺ����״
 	BeginWaitCursor();
 	
 	// ���Ե���ReadDIBFile()��ȡͼ��
 	TRY
 	{
 		m_hDIB=ReadDIBFile(file);
 	}
 	CATCH (CFileException, eLoad)
 	{
 		// �ָ�״̬
 		file.Abort();
 		EndWaitCursor();
 		m_hDIB=NULL;
 		
 		// ����
 		ReportSaveLoadException(strPathName, eLoad, 
 			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		return;
 	}
 	END_CATCH
 	
 	// ��ʼ��DIB
 	InitDIBData();
 	
 	// �ָ������״
 	EndWaitCursor();

	// �ж϶�ȡ�ļ��Ƿ�ɹ�
 	if (m_hDIB==NULL)
 	{
 		// ����
 		CString strMsg;
 		strMsg="��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
  		
 		return;
 	}
 	
	// ��������
 	SetModifiedFlag(FALSE);
 	
	// ������ͼ
 	UpdateAllViews(NULL);

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ͼ��������ϡ�", TRUE);

 	return;
}

void CDIBProcessDoc::ReplaceHDIB(HDIB hDIB)
{
	// �滻DIB���ڹ���ճ�����õ��ú���
	
	// �ж�DIB�Ƿ�Ϊ��
	if (m_hDIB!=NULL)
	{
		// �ǿգ������
		::GlobalFree((HGLOBAL)m_hDIB);
	}

	// �滻���µ�DIB����
	m_hDIB=hDIB;
}
