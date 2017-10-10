// DIBProcessView.cpp : implementation of the CDIBProcessView class
//

#include "stdafx.h"
#include "DIBProcess.h"
#include "MainFrm.h"
#include "DIBProcessDoc.h"
#include "DIBProcessView.h"
#include "ImageProcessor.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif  


////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessView

IMPLEMENT_DYNCREATE(CDIBProcessView, CView)

BEGIN_MESSAGE_MAP(CDIBProcessView, CView)
	//{{AFX_MSG_MAP(CDIBProcessView)
	ON_COMMAND(ID_PROC_HISTO, OnProcHisto)
	ON_COMMAND(ID_PROC_TEMPLATE, OnProcTemplate)
	ON_COMMAND(ID_MENU_CSTJ, OnMenuCstj)
	ON_COMMAND(NXBJZLB, OnNXBJZLB)
	ON_COMMAND(ID_MENUXBJZLB, Onxbjzlb)
	ON_COMMAND(ID_MEJHH, OnMejhh)
	ON_COMMAND(ID_MEQFS, OnMeqfs)
	ON_COMMAND(ID_MEYuZhiHua, OnMEYuZhiHua)
	ON_COMMAND(ID_Mpengzhangyufushi, OnMpengzhangyufushi)
	ON_COMMAND(ID_zhongzhilvbo, Onzhongzhilvbo)

	ON_COMMAND(ID_zuixiaozhichuli, Onzuixiaozhichuli)
	ON_COMMAND(ID_Geopro, OnGeopro)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessView construction/destruction


BOOL CDIBProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessView drawing

void CDIBProcessView::OnDraw(CDC* pDC)
{
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ��Ͷ�Ӧ��DIB
	CDIBProcessDoc* pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB=pDoc->GetHDIB();
	
	// ���DIB�ǿգ��ͽ��д���
	if (hDIB!=NULL)
	{
		// ����ͼ���ڴ���
		LPSTR lpDIB=(LPSTR)::GlobalLock((HGLOBAL)hDIB);
		
		// ��ȡDIB�ߴ�
		int cxDIB=(int)DIBWidth(lpDIB);
		int cyDIB=(int)DIBHeight(lpDIB);

		// ����ͼ���ڴ���
		::GlobalUnlock((HGLOBAL)hDIB);
		
		CRect rcDIB;
		rcDIB.top=rcDIB.left=0;
		rcDIB.right=cxDIB;
		rcDIB.bottom=cyDIB;
		
		CRect rcDest;
		
		// ���Ҫ��ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��
		if (pDC->IsPrinting())
		{
			// ��ȡ��ӡҳ���ˮƽ���(����)
			int cxPage=pDC->GetDeviceCaps(HORZRES);
			
			// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
			int cyPage=pDC->GetDeviceCaps(VERTRES);
			
			// ��ȡ��ӡ��ÿӢ��������
			int cxInch=pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch=pDC->GetDeviceCaps(LOGPIXELSY);
			
			// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
			rcDest.top=rcDest.left=0;
			rcDest.bottom=(int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right=cxPage;
			
			// �����ӡͼ��λ�ã���ֱ���У�
			int temp=cyPage-(rcDest.bottom-rcDest.top);
			rcDest.bottom+=temp/2;
			rcDest.top+=temp/2;
		}
		//����ӡ����������
		else   
		{
			rcDest=rcDIB;
		}
		
		// ���DIB
		PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(), &rcDIB);
	}

	// �ָ��������
	EndWaitCursor();
}


/////////////////////////////////////////////////////////////////////////////
// CDIBProcessView diagnostics

#ifdef _DEBUG
void CDIBProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDIBProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIBProcessDoc* CDIBProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIBProcessDoc)));
	return (CDIBProcessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIBProcessView message handlers



 ///////////////////////////////////////////////////////////////////////////
// ��Ӵ�����صĿ��ļ�

#include"TPCL.h"   

#include"niose1.h"  

#include"PINYI.h"

void CDIBProcessView::OnProcHisto() 
{
	// ֱ��ͼ����
	
	// ��ȡ�ĵ�
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ֱ��ͼ����", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
////////////////////////////// gai   /////////////////////////////////////



    CTPCL dlgPara;


	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״

	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ֱ��ͼ����...", TRUE);
 /////////////////////////////////////// ri //////////////////////////
	// ���ú�������ֱ��ͼ����
	BOOL blReturnV=HistogramProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB), dlgPara.m_n);  
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ֱ��ͼ������ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ֱ��ͼ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}


void CDIBProcessView::OnProcTemplate() 
{
	// ģ�崦��
	
	// ��ȡ�ĵ�
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ģ�崦��...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=TemplateProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ģ�崦����ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ģ�崦��ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}

//     ���δ���

//DEL void CDIBProcessView::OnProcGeo() 
//DEL {
//DEL 	// ���δ���
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CDIBProcessDoc* pDoc=GetDocument();
//DEL 	
//DEL 	// ָ��DIB��ָ��
//DEL 	LPSTR lpDIB;
//DEL 
//DEL 	// ����DIB
//DEL 	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
//DEL 	
//DEL 	// �ж��Ƿ���256ɫ����24λɫλͼ
//DEL 	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ�ļ��δ���", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 		// �������
//DEL 		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
//DEL 		
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// �����Ի���
//DEL 
//DEL 	// ��ʼ������ֵ
//DEL 
//DEL 	// ��ʾ�Ի�����ʾ�û��趨����
//DEL 
//DEL     PINYI dlgPara;
//DEL 	if ( dlgPara.DoModal()!=IDOK )
//DEL 	{
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	
//DEL 	// ��ȡ�û��趨�Ĳ���
//DEL 	
//DEL 	// ɾ���Ի���
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��״̬����ʾ��ǰ����
//DEL 	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
//DEL 	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
//DEL 	pStatus->SetPaneText(1, "���δ���...", TRUE);
//DEL 
//DEL 	// ���ú������м��δ���
//DEL 
//DEL 
//DEL 	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);
//DEL 
//DEL 	if ( hNewDIB!=NULL )
//DEL 	{
//DEL     
//DEL 		// �滻DIBͼ��ͬʱ�ͷž�DIBͼ�����
//DEL 		pDoc->ReplaceHDIB(hNewDIB);
//DEL 
//DEL 		// ����DIBͼ���С�͵�ɫ��
//DEL 		pDoc->InitDIBData();
//DEL 
//DEL 		// ��������
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 		// ������ͼ
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 		// ״̬������
//DEL 		pStatus->SetPaneText(1, "���δ�����ϡ�", TRUE);
//DEL 	}
//DEL 
//DEL 	else
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("���δ���ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// ״̬������
//DEL 		pStatus->SetPaneText(1, "", TRUE);
//DEL 	}
//DEL 
//DEL 	// �������
//DEL 	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
//DEL 
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL }

/*******************************************

   
	 �������
 


*********************************************/

#include"niose1.h"

void CDIBProcessView::OnMenuCstj() 
{	      
	
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ����������", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	 
    niose dlghz ;

	if ( dlghz.DoModal()!=IDOK )
	{
		return;
	}
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	 
	 BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "�������...", TRUE);

	// ���ú�������ģ�崦��
	 BOOL blReturnV = JHJZProcess2(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB),dlghz.m_nb,dlghz.m_nh,dlghz.m_xb,dlghz.m_xh);
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "������ӡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��������ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// �ָ����
	EndWaitCursor();
	
}





// ��г����ֵ����

void CDIBProcessView::OnNXBJZLB() 
{
	
CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ����������", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	 
	 BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "�������...", TRUE);

	// ���ú�������ģ�崦��
	 BOOL blReturnV=NXBJZProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "������ӡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��������ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// �ָ����
	EndWaitCursor();
	// TODO: Add your command handler code here
	
}


//г����ֵ����

void CDIBProcessView::Onxbjzlb() 
{
	// TODO: Add your command handler code here
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ����������", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	 
	 BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "�������...", TRUE);

	// ���ú�������ģ�崦��
	 BOOL blReturnV=XBJZProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "������ӡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��������ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// �ָ����
	EndWaitCursor();
	
}

void CDIBProcessView::OnMejhh() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ģ�崦��...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=JHHProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB)) ;
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ģ�崦����ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ģ�崦��ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	// TODO: Add your command handler code here
	
}


//  ȡ��ɫ

void CDIBProcessView::OnMeqfs() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ģ�崦��...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=QFSProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ģ�崦����ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ģ�崦��ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

//////////////////////////////////


//��ֵ��


/////////////////////////////////

void CDIBProcessView::OnMEYuZhiHua() 
{
	// TODO: Add your command handler code here
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ֱ��ͼ����", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
////////////////////////////// gai   /////////////////////////////////////



    CTPCL dlgPara;


	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״

	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ֱ��ͼ����...", TRUE);
 /////////////////////////////////////// ri //////////////////////////
	// ���ú�������ֱ��ͼ����
	BOOL blReturnV=YZHProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB), dlgPara.m_n);  
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ֱ��ͼ������ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ֱ��ͼ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
	
}

void CDIBProcessView::OnMpengzhangyufushi() 
{
		CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "ģ�崦��...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=pengzhangProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "ģ�崦����ϡ�", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("ģ�崦��ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

// �����˲�

void CDIBProcessView::Onzhongzhilvbo() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "��ֵ�˲�...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=zhongzhiProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "��ֵ�˲���", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��ֵ�˲�ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

//���δ���

void CDIBProcessView::OnGeochuli() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ�ļ��δ���", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����

  PINYI dlgPara;
	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();
	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "���δ���...", TRUE);

	// ���ú������м��δ���


	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);
	if ( hNewDIB!=NULL )
	{
    
		// �滻DIBͼ��ͬʱ�ͷž�DIBͼ�����
		pDoc->ReplaceHDIB(hNewDIB);

		// ����DIBͼ���С�͵�ɫ��
		pDoc->InitDIBData();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "���δ�����ϡ�", TRUE);
	}

	else
	{
		// ��ʾ�û�
		MessageBox("���δ���ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}

// ��Сֵ����

void CDIBProcessView::Onzuixiaozhichuli() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ָ��DIB����ָ��
	LPSTR lpDIBBits;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIB);
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ��ģ�崦��", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "��ֵ�˲�...", TRUE);

	// ���ú�������ģ�崦��
	BOOL blReturnV=ZUIXIAOProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "��ֵ�˲���", TRUE);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("��ֵ�˲�ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}
////////////////////////////

//  ��ת�� ƽ��

///////////////////////////


void CDIBProcessView::OnGeopro() 
{
	
	// ��ȡ�ĵ�
	CDIBProcessDoc* pDoc=GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;

	// ����DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	
	// �ж��Ƿ���256ɫ����24λɫλͼ
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫ����24λɫλͼ�ļ��δ���", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	
		// �������
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// �����Ի���

	// ��ʼ������ֵ

	// ��ʾ�Ի�����ʾ�û��趨����

    PINYI dlgPara;
	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}

	
	// ��ȡ�û��趨�Ĳ���
	
	// ɾ���Ի���
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ��״̬����ʾ��ǰ����
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "���δ���...", TRUE);

	// ���ú������м��δ���


	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);

	if ( hNewDIB!=NULL )
	{
		// �滻DIBͼ��ͬʱ�ͷž�DIBͼ�����
		pDoc->ReplaceHDIB(hNewDIB);

		// ����DIBͼ���С�͵�ɫ��
		pDoc->InitDIBData();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		pDoc->UpdateAllViews(NULL);

		// ״̬������
		pStatus->SetPaneText(1, "���δ�����ϡ�", TRUE);
	}

	else
	{
		// ��ʾ�û�
		MessageBox("���δ���ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// ״̬������
		pStatus->SetPaneText(1, "", TRUE);
	}

	// �������
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}
