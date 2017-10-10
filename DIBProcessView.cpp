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
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档和对应的DIB
	CDIBProcessDoc* pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	HDIB hDIB=pDoc->GetHDIB();
	
	// 如果DIB非空，就进行处理
	if (hDIB!=NULL)
	{
		// 锁定图像内存区
		LPSTR lpDIB=(LPSTR)::GlobalLock((HGLOBAL)hDIB);
		
		// 获取DIB尺寸
		int cxDIB=(int)DIBWidth(lpDIB);
		int cyDIB=(int)DIBHeight(lpDIB);

		// 解锁图像内存区
		::GlobalUnlock((HGLOBAL)hDIB);
		
		CRect rcDIB;
		rcDIB.top=rcDIB.left=0;
		rcDIB.right=cxDIB;
		rcDIB.bottom=cyDIB;
		
		CRect rcDest;
		
		// 如果要打印，计算输出图像的位置和大小，以便符合页面
		if (pDC->IsPrinting())
		{
			// 获取打印页面的水平宽度(象素)
			int cxPage=pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage=pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch=pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch=pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top=rcDest.left=0;
			rcDest.bottom=(int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right=cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp=cyPage-(rcDest.bottom-rcDest.top);
			rcDest.bottom+=temp/2;
			rcDest.top+=temp/2;
		}
		//不打印则正常处理
		else   
		{
			rcDest=rcDIB;
		}
		
		// 输出DIB
		PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(), &rcDIB);
	}

	// 恢复正常光标
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
// 添加窗口相关的库文件

#include"TPCL.h"   

#include"niose1.h"  

#include"PINYI.h"

void CDIBProcessView::OnProcHisto() 
{
	// 直方图处理
	
	// 获取文档
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的直方图处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
////////////////////////////// gai   /////////////////////////////////////



    CTPCL dlgPara;


	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状

	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "直方图处理...", TRUE);
 /////////////////////////////////////// ri //////////////////////////
	// 调用函数进行直方图处理
	BOOL blReturnV=HistogramProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB), dlgPara.m_n);  
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "直方图处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("直方图处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}


void CDIBProcessView::OnProcTemplate() 
{
	// 模板处理
	
	// 获取文档
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "模板处理...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=TemplateProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "模板处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("模板处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

//     几何处理

//DEL void CDIBProcessView::OnProcGeo() 
//DEL {
//DEL 	// 几何处理
//DEL 	
//DEL 	// 获取文档
//DEL 	CDIBProcessDoc* pDoc=GetDocument();
//DEL 	
//DEL 	// 指向DIB的指针
//DEL 	LPSTR lpDIB;
//DEL 
//DEL 	// 锁定DIB
//DEL 	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
//DEL 	
//DEL 	// 判断是否是256色或者24位色位图
//DEL 	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色或者24位色位图的几何处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 		// 解除锁定
//DEL 		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
//DEL 		
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// 参数对话框
//DEL 
//DEL 	// 初始化变量值
//DEL 
//DEL 	// 显示对话框，提示用户设定参数
//DEL 
//DEL     PINYI dlgPara;
//DEL 	if ( dlgPara.DoModal()!=IDOK )
//DEL 	{
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	
//DEL 	// 获取用户设定的参数
//DEL 	
//DEL 	// 删除对话框
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 在状态栏显示当前操作
//DEL 	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
//DEL 	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
//DEL 	pStatus->SetPaneText(1, "几何处理...", TRUE);
//DEL 
//DEL 	// 调用函数进行几何处理
//DEL 
//DEL 
//DEL 	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);
//DEL 
//DEL 	if ( hNewDIB!=NULL )
//DEL 	{
//DEL     
//DEL 		// 替换DIB图像，同时释放旧DIB图像对象
//DEL 		pDoc->ReplaceHDIB(hNewDIB);
//DEL 
//DEL 		// 更新DIB图像大小和调色板
//DEL 		pDoc->InitDIBData();
//DEL 
//DEL 		// 设置脏标记
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 		// 更新视图
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 		// 状态栏更新
//DEL 		pStatus->SetPaneText(1, "几何处理完毕。", TRUE);
//DEL 	}
//DEL 
//DEL 	else
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("几何处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// 状态栏更新
//DEL 		pStatus->SetPaneText(1, "", TRUE);
//DEL 	}
//DEL 
//DEL 	// 解除锁定
//DEL 	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
//DEL 
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL }

/*******************************************

   
	 噪声添加
 


*********************************************/

#include"niose1.h"

void CDIBProcessView::OnMenuCstj() 
{	      
	
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的噪声处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	 
    niose dlghz ;

	if ( dlghz.DoModal()!=IDOK )
	{
		return;
	}
	
	// 删除对话框
	
	// 更改光标形状
	 
	 BeginWaitCursor();

	// 在状态栏显示当前操作
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "噪声添加...", TRUE);

	// 调用函数进行模板处理
	 BOOL blReturnV = JHJZProcess2(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB),dlghz.m_nb,dlghz.m_nh,dlghz.m_xb,dlghz.m_xh);
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "噪声添加。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("噪声处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// 恢复光标
	EndWaitCursor();
	
}





// 逆谐波均值处理

void CDIBProcessView::OnNXBJZLB() 
{
	
CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的噪声处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	 
	 BeginWaitCursor();

	// 在状态栏显示当前操作
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "噪声添加...", TRUE);

	// 调用函数进行模板处理
	 BOOL blReturnV=NXBJZProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "噪声添加。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("噪声处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// 恢复光标
	EndWaitCursor();
	// TODO: Add your command handler code here
	
}


//谐波均值处理

void CDIBProcessView::Onxbjzlb() 
{
	// TODO: Add your command handler code here
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的噪声处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	 
	 BeginWaitCursor();

	// 在状态栏显示当前操作
	 CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	 CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	 pStatus->SetPaneText(1, "噪声添加...", TRUE);

	// 调用函数进行模板处理
	 BOOL blReturnV=XBJZProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "噪声添加。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("噪声处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
	// 恢复光标
	EndWaitCursor();
	
}

void CDIBProcessView::OnMejhh() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "模板处理...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=JHHProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB)) ;
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "模板处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("模板处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	// TODO: Add your command handler code here
	
}


//  取反色

void CDIBProcessView::OnMeqfs() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "模板处理...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=QFSProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "模板处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("模板处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

//////////////////////////////////


//阈值化


/////////////////////////////////

void CDIBProcessView::OnMEYuZhiHua() 
{
	// TODO: Add your command handler code here
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的直方图处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
////////////////////////////// gai   /////////////////////////////////////



    CTPCL dlgPara;


	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状

	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "直方图处理...", TRUE);
 /////////////////////////////////////// ri //////////////////////////
	// 调用函数进行直方图处理
	BOOL blReturnV=YZHProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB), dlgPara.m_n);  
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "直方图处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("直方图处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CDIBProcessView::OnMpengzhangyufushi() 
{
		CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "模板处理...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=pengzhangProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "模板处理完毕。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("模板处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

// 中置滤波

void CDIBProcessView::Onzhongzhilvbo() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "中值滤波...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=zhongzhiProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "中值滤波。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("中值滤波失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	// TODO: Add your command handler code here
	
}

//几何处理

void CDIBProcessView::OnGeochuli() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的几何处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
	
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数

  PINYI dlgPara;
	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();
	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "几何处理...", TRUE);

	// 调用函数进行几何处理


	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);
	if ( hNewDIB!=NULL )
	{
    
		// 替换DIB图像，同时释放旧DIB图像对象
		pDoc->ReplaceHDIB(hNewDIB);

		// 更新DIB图像大小和调色板
		pDoc->InitDIBData();

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "几何处理完毕。", TRUE);
	}

	else
	{
		// 提示用户
		MessageBox("几何处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

// 最小值处理

void CDIBProcessView::Onzuixiaozhichuli() 
{
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR lpDIBBits;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIB);
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的模板处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数
	/*if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}*/
	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "中值滤波...", TRUE);

	// 调用函数进行模板处理
	BOOL blReturnV=ZUIXIAOProcess(lpDIBBits, DIBBitCount(lpDIB), DIBWidth(lpDIB), DIBHeight(lpDIB));
	if ( blReturnV )
	{
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "中值滤波。", TRUE);
	}
	else
	{
		// 提示用户
		MessageBox("中值滤波失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}
////////////////////////////

//  旋转加 平移

///////////////////////////


void CDIBProcessView::OnGeopro() 
{
	
	// 获取文档
	CDIBProcessDoc* pDoc=GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 锁定DIB
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)pDoc->GetHDIB());
	
	// 判断是否是256色或者24位色位图
	if ( DIBNumColors(lpDIB)!=256 && DIBBitCount(lpDIB)!=24 )
	{
		// 提示用户
		MessageBox("目前只支持256色或者24位色位图的几何处理！", "系统提示", MB_ICONINFORMATION | MB_OK);
	
		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
		
		return;
	}

	// 参数对话框

	// 初始化变量值

	// 显示对话框，提示用户设定参数

    PINYI dlgPara;
	if ( dlgPara.DoModal()!=IDOK )
	{
		return;
	}

	
	// 获取用户设定的参数
	
	// 删除对话框
	
	// 更改光标形状
	BeginWaitCursor();

	// 在状态栏显示当前操作
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(1, "几何处理...", TRUE);

	// 调用函数进行几何处理


	HDIB hNewDIB=(HDIB)GeoProcess(lpDIB,dlgPara.m_x,dlgPara.m_y,dlgPara.theta);

	if ( hNewDIB!=NULL )
	{
		// 替换DIB图像，同时释放旧DIB图像对象
		pDoc->ReplaceHDIB(hNewDIB);

		// 更新DIB图像大小和调色板
		pDoc->InitDIBData();

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	
		// 更新视图
		pDoc->UpdateAllViews(NULL);

		// 状态栏更新
		pStatus->SetPaneText(1, "几何处理完毕。", TRUE);
	}

	else
	{
		// 提示用户
		MessageBox("几何处理失败！", "系统提示", MB_ICONINFORMATION | MB_OK);

		// 状态栏更新
		pStatus->SetPaneText(1, "", TRUE);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}
