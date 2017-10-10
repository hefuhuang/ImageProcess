// ************************************************************************
//  文件名：ImageProcessor.cpp
// ************************************************************************

#include "stdafx.h"
#include "ImageProcessor.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


/**************************************************************************
 *
 *	DIB(Independent Bitmap) API函数库：（由老师提供给学生）
 *
 *	PaintDIB()									- 绘制DIB对象
 *
 *	FindDIBBits()								- 返回DIB图像象素起始位置
 *
 *	FindPalette()								- 返回DIB调色板起始位置
 *
 *	DIBWidth()									- 返回DIB宽度
 *
 *	DIBHeight()									- 返回DIB高度
 *
 *	PaletteSize()								- 返回DIB调色板大小
 *
 *	DIBNumColors()								- 计算DIB调色板颜色数目
 *
 *	DIBBitCount()								- 返回DIB的颜色位数
 *
 *	CopyHandle()								- 拷贝内存块
 *
 *	SaveDIB()									- 将DIB保存到指定文件中
 *
 *	ReadDIBFile()								- 从指定文件中读取DIB对象
 *
 ************************************************************************/

/*************************************************************************
 *
 * 函数名称：
 *	PaintDIB()
 *
 * 参数:
 *	HDC			hDC			- 输出设备DC的句柄
 *	LPRECT		lpDCRect	- 绘制矩形区域
 *	HDIB		hDIB		- 指向DIB对象的句柄
 *	LPRECT		lpDIBRect	- 要输出的DIB区域
 *	CPalette*	pPal		- 指向DIB对象调色板的指针
 *
 * 返回值:
 *	BOOL					- 绘制成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *	该函数主要用来绘制DIB对象。其中调用了StretchDIBits()或者
 * SetDIBitsToDevice()来绘制DIB对象。输出的设备由由参数hDC指
 * 定；绘制的矩形区域由参数lpDCRect指定；输出DIB的区域由参数
 * lpDIBRect指定。
 *
************************************************************************/

BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect)
{
	LPSTR lpDIBHdr;				// BITMAPINFOHEADER指针
	LPSTR lpDIBBits;				// DIB象素指针
	BOOL blSuccess=FALSE;		// 成功标志

	// 判断输入参数是否有误
	if ( hDC==NULL || lpDCRect==NULL || lpDIBRect==NULL )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}
	
	// 判断DIB对象是否为空
	if (hDIB==NULL)
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 锁定DIB
	lpDIBHdr=(LPSTR)::GlobalLock((HGLOBAL)hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits=FindDIBBits(lpDIBHdr);

	// 设置显示模式
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// 判断是调用StretchDIBits()还是SetDIBitsToDevice()来绘制DIB对象
	if ((RECTWIDTH(lpDCRect)==RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect)==RECTHEIGHT(lpDIBRect)))
	{
		// 原始大小，不用拉伸。
		blSuccess=::SetDIBitsToDevice(hDC,  						// hDC
								   lpDCRect->left,				// DestX
								   lpDCRect->top,				// DestY
								   RECTWIDTH(lpDCRect),			// nDestWidth
								   RECTHEIGHT(lpDCRect),		// nDestHeight
								   lpDIBRect->left,				// SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),	// SrcY
								   0,							// nStartScan
								   (WORD)DIBHeight(lpDIBHdr),	// nNumScans
								   lpDIBBits,					// lpBits
								   (LPBITMAPINFO)lpDIBHdr,		// lpBitsInfo
								   DIB_RGB_COLORS);				// wUsage
	}
    else
	{
		// 非原始大小，拉伸。
		blSuccess=::StretchDIBits(hDC,							// hDC
							   lpDCRect->left,					// DestX
							   lpDCRect->top,					// DestY
							   RECTWIDTH(lpDCRect),				// nDestWidth
							   RECTHEIGHT(lpDCRect),			// nDestHeight
							   lpDIBRect->left,					// SrcX
							   lpDIBRect->top,					// SrcY
							   RECTWIDTH(lpDIBRect),			// wSrcWidth
							   RECTHEIGHT(lpDIBRect),			// wSrcHeight
							   lpDIBBits,						// lpBits
							   (LPBITMAPINFO)lpDIBHdr,			// lpBitsInfo
							   DIB_RGB_COLORS,					// wUsage
							   SRCCOPY);						// dwROP
	}
	
    // 解除锁定
	::GlobalUnlock((HGLOBAL)hDIB);

	return blSuccess;
}

/*************************************************************************
 *
 * 函数名称：
 *	FindDIBBits()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	LPSTR			- 指向DIB图像象素起始位置
 *
 * 说明:
 *	该函数计算DIB中图像象素的起始位置，并返回指向它的指针。
 *
************************************************************************/

LPSTR FindDIBBits(LPSTR lpDIB)
{
	return (lpDIB+(*((LPDWORD)lpDIB))+PaletteSize(lpDIB));
}

/*************************************************************************
 *
 * 函数名称：
 *	FindPalette()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	LPSTR			- 指向DIB调色板的起始位置
 *
 * 说明:
 *	该函数计算DIB中调色板的起始位置，并返回指向它的指针。
 *
************************************************************************/

LPSTR FindPalette(LPSTR lpDIB)
{
	return (DIBNumColors(lpDIB)==0?NULL:lpDIB+(*((LPDWORD)lpDIB)));
}

/*************************************************************************
 *
 * 函数名称：
 *	DIBWidth()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	DWORD			- DIB中图像的宽度
 *
 * 说明:
 *	该函数返回DIB中图像的宽度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
 * 中的biWidth值；对于其它返回BITMAPCOREHEADER中的bcWidth值。
 *
************************************************************************/

DWORD DIBWidth(LPSTR lpDIB)
{
	// 指向BITMAPINFOHEADER结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREHEADER结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 获取指针
	lpbmi=(LPBITMAPINFOHEADER)lpDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{
		// 对于Windows 3.0 DIB，返回lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// 对于其它格式的DIB，返回lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}

/*************************************************************************
 *
 * 函数名称：
 *	DIBHeight()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	DWORD			- DIB中图像的高度
 *
 * 说明:
 *	该函数返回DIB中图像的高度。对于Windows 3.0 DIB，返回BITMAPINFOHEADER
 * 中的biHeight值；对于其它返回BITMAPCOREHEADER中的bcHeight值。
 *
************************************************************************/

DWORD DIBHeight(LPSTR lpDIB)
{
	// 指向BITMAPINFOHEADER结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREHEADER结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 获取指针
	lpbmi=(LPBITMAPINFOHEADER)lpDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpDIB;

	// 返回DIB中图像的宽度
	if (IS_WIN30_DIB(lpDIB))
	{
		// 对于Windows 3.0 DIB，返回lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// 对于其它格式的DIB，返回lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}

/*************************************************************************
 *
 * 函数名称：
 *	PaletteSize()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	WORD			- DIB中调色板的大小
 *
 * 说明:
 *	该函数返回DIB中调色板的大小。对于Windows 3.0 DIB，返回颜色数目×
 * RGBQUAD的大小；对于其它返回颜色数目×RGBTRIPLE的大小。
 *
************************************************************************/

WORD PaletteSize(LPSTR lpDIB)
{
	// 计算DIB中调色板的大小
	if (IS_WIN30_DIB (lpDIB))
	{
		//返回颜色数目×RGBQUAD的大小
		return (WORD)(DIBNumColors(lpDIB)*sizeof(RGBQUAD));
	}
	else
	{
		//返回颜色数目×RGBTRIPLE的大小
		return (WORD)(DIBNumColors(lpDIB)*sizeof(RGBTRIPLE));
	}
}

/*************************************************************************
 *
 * 函数名称：
 *	DIBNumColors()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	WORD			- 返回调色板中颜色的种数
 *
 * 说明:
 *	该函数返回DIB中调色板的颜色的种数。对于单色位图，返回2，
 * 对于16色位图，返回16，对于256色位图，返回256；对于真彩色
 * 位图（24位），没有调色板，返回0。
 *
************************************************************************/

WORD DIBNumColors(LPSTR lpDIB)
{
	BYTE bBitCount;

	// 对于Windows的DIB, 实际颜色的数目可以比象素的位数要少。
	// 对于这种情况，则返回一个近似的数值。
	
	// 判断是否是WIN3.0 DIB
	if (IS_WIN30_DIB(lpDIB))
	{
		DWORD dwClrUsed;
		
		// 读取dwClrUsed值
		dwClrUsed=((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
		
		if (dwClrUsed!=0)
		{
			// 如果dwClrUsed（实际用到的颜色数）不为0，直接返回该值
			return (WORD)dwClrUsed;
		}
	}

	// 读取象素的位数
	if (IS_WIN30_DIB(lpDIB))
	{
		// 读取biBitCount值
		bBitCount=(BYTE)((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	}
	else
	{
		// 读取biBitCount值
		bBitCount=(BYTE)((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	}
	
	// 按照象素的位数计算颜色数目
	switch (bBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

/*************************************************************************
 *
 * 函数名称：
 *	DIBBitCount()
 *
 * 参数:
 *	LPSTR	lpDIB	- 指向DIB对象的指针
 *
 * 返回值:
 *	BYTE			- 返回图像的颜色位数
 *
 * 说明:
 *	该函数返回DIB的颜色位数。
 *
************************************************************************/

BYTE DIBBitCount(LPSTR lpDIB)
{
	BYTE bBitCount;

	// 读取象素的位数
	if (IS_WIN30_DIB(lpDIB))
	{
		// 读取biBitCount值
		bBitCount=(BYTE)((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	}
	else
	{
		// 读取biBitCount值
		bBitCount=(BYTE)((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	}
	
	return bBitCount;
}

/*************************************************************************
 *
 * 函数名称：
 *	CopyHandle()
 *
 * 参数:
 *	HGLOBAL	h		- 要复制的内存区域
 *
 * 返回值:
 *	HGLOBAL			- 复制后的新内存区域
 *
 * 说明:
 *	该函数复制指定的内存区域。返回复制后的新内存区域，出错时返回0。
 *
************************************************************************/

HGLOBAL CopyHandle (HGLOBAL h)
{
	// 检查输入参数是否有误
	if (h==NULL)
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return NULL;
	}

	// 获取指定内存区域大小
	DWORD dwLen=::GlobalSize((HGLOBAL) h);
	
	// 分配新内存空间
	HGLOBAL hCopy=::GlobalAlloc(GHND, dwLen);
	
	// 判断分配是否成功
	if (hCopy!=NULL)
	{
		// 锁定
		void* lpCopy=::GlobalLock((HGLOBAL) hCopy);
		void* lp=::GlobalLock((HGLOBAL) h);
		
		// 复制
		memcpy(lpCopy, lp, dwLen);
		
		// 解除锁定
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

/*************************************************************************
 *
 * 函数名称：
 *	SaveDIB()
 *
 * 参数:
 *	HDIB	hDIB		- 要保存的DIB的句柄
 *	CFile&	file		- 保存文件CFile
 *
 * 返回值:
 *	BOOL				- 成功返回TRUE，否则返回FALSE或者CFileException
 *
 * 说明:
 *	该函数将指定的DIB对象保存到指定的CFile中。该CFile由调用程序打开和关闭。
 *
*************************************************************************/

BOOL SaveDIB(HDIB hDIB, CFile& file)
{
	// Bitmap文件头
	BITMAPFILEHEADER bmfHdr;
	
	// 指向BITMAPINFOHEADER的指针
	LPBITMAPINFOHEADER lpBI;
	
	// DIB大小
	DWORD dwDIBSize;

	// 检查输入参数是否有误
	if (hDIB==NULL)
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 读取BITMAPINFO结构，并锁定
	lpBI=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDIB);
	
	// 检查输入参数是否有误
	if (lpBI==NULL)
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}
	
	// 判断是否是WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// 不支持其它类型的DIB保存
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)hDIB);
		
		AfxMessageBox("不支持非WIN3.0类型DIB的保存！", MB_OK);
		return FALSE;
	}

	// 填充文件头

	// 文件类型"BM"
	bmfHdr.bfType=DIB_HEADER_MARKER;

	// 计算DIB大小时，最简单的方法是调用GlobalSize()函数。但是全局内存大小并
	// 不是DIB真正的大小，它总是多几个字节。这样就需要计算一下DIB的真实大小。
	
	// 文件头大小＋颜色表大小
	// （BITMAPINFOHEADER和BITMAPCOREHEADER结构的第一个DWORD都是该结构的大小）
	dwDIBSize=*(LPDWORD)lpBI+PaletteSize((LPSTR)lpBI);
	
	// 计算图像大小
	if ((lpBI->biCompression==BI_RLE8) || (lpBI->biCompression==BI_RLE4))
	{
		// 对于RLE位图，没法计算大小，只能信任biSizeImage内的值
		dwDIBSize+=lpBI->biSizeImage;
	}
	else
	{
		// 象素的大小
		DWORD dwBmBitsSize;

		// 大小为Width*Height
		dwBmBitsSize=WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount))*lpBI->biHeight;
		
		// 计算出DIB真正的大小
		dwDIBSize+=dwBmBitsSize;

		// 更新biSizeImage（很多BMP文件头中biSizeImage的值是错误的）
		lpBI->biSizeImage=dwBmBitsSize;
	}


	// 计算文件大小：DIB大小＋BITMAPFILEHEADER结构大小
	bmfHdr.bfSize=dwDIBSize+sizeof(BITMAPFILEHEADER);
	
	// 两个复制到字
	bmfHdr.bfReserved1=0;
	bmfHdr.bfReserved2=0;

	// 计算偏移量bfOffBits，它的大小为Bitmap文件头大小＋DIB头大小＋颜色表大小
	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+lpBI->biSize
											 +PaletteSize((LPSTR)lpBI);
	// 尝试写文件
	TRY
	{
		// 写文件头
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// 写DIB头和象素
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// 解除锁定
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// 抛出异常
		THROW_LAST();
	}
	END_CATCH
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL)hDIB);
	
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *  ReadDIBFile()
 *
 * 参数:
 *	CFile& file		- 要读取得文件文件CFile
 *
 * 返回值:
 *	hDIB			- 成功返回DIB的句柄，否则返回NULL。
 *
 * 说明:
 *	该函数将指定的文件中的DIB对象读到指定的内存区域中。除BITMAPFILEHEADER
 * 外的内容都将被读入内存。
 *
*************************************************************************/

HDIB ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR lpDIB;

	// 获取DIB（文件）长度（字节）
	dwBitsSize=(DWORD)file.GetLength();

	// 尝试读取DIB文件头
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
	{
		AfxMessageBox("文件头读取有误！", MB_OK);
		return NULL;
	}

	// 判断是否是DIB对象，检查头两个字节是否是"BM"
	if (bmfHeader.bfType!=DIB_HEADER_MARKER)
	{
		AfxMessageBox("该文件并非DIB图像文件！", MB_OK);
		return NULL;
	}

	// 为DIB分配内存
	hDIB=(HDIB)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB==0)
	{
		AfxMessageBox("内存分配有误！", MB_OK);
		return NULL;
	}
	
	// 锁定
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)hDIB);

	// 读象素
	if (file.Read(lpDIB, dwBitsSize-sizeof(BITMAPFILEHEADER))!=dwBitsSize-sizeof(BITMAPFILEHEADER) )
	{
		// 大小不对。
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// 释放内存
		::GlobalFree((HGLOBAL)hDIB);
		
		AfxMessageBox("文件内容读取有误！", MB_OK);
		return NULL;
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL)hDIB);
	
	return hDIB;
}

/*************************************************************************
 *
 * 函数名称：
 *	IF_BilinearInterpolation()
 *
 * 参数:
 *	LPSTR					lpDIBBits			- 指向DIB图像数据区的指针
 *	BYTE					bBitCount			- DIB图像的颜色位数
 *	LONG					lWidth				- 源DIB图像宽度（像素数）
 *	LONG					lHeight				- 源DIB图像高度（像素数）
 *	double					dX					- 插值元素的dX坐标
 *	double					dY					- 插值元素的dY坐标
 *
 * 返回值:
 *	LONG										- 返回插值计算结果。
 *
 * 说明:
 *	该函数利用双线性插值算法来估算像素值。对于超出DIB图像范围的像素，
 * 直接返回255；此外，24位色按BGR顺序装成一个LONG返回。
 *
 ************************************************************************/

inline LONG IF_BilinearInterpolation(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, double dX, double dY)
{
	// 四个最临近像素的坐标(j1, i1), (j2, i1), (j1, i2), (j2, i2)
	LONG i1, i2;
	LONG j1, j2;
	
	// 四个最临近像素值
	BYTE bP1, bP2, bP3, bP4;
	
	// 二个插值中间值
	BYTE bP12, bP34;

	// RGB分量返回值
	LONG lRGB;
	
	// DIB图像每行的字节数
	LONG lLineByteNum;
	
	// 计算DIB图像每行的字节数
	lLineByteNum=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 根据不同情况分别处理
	if ( dX<-1.0 || dX>=(double)lWidth || dY<-1.0 || dY>=(double)lHeight )
	{
		// 要计算的点不在源图范围内，直接返回bwmDIBWhite。
		if ( bBitCount==8 )
		{
			// 对单通道色图
			return 255;
		}
		else
		{
			// 对三通道色图
			return (255<<16)|(255<<8)|255;	//即连续三个bwmDIBWhite
		}
	}
	else
	{
		// 计算四个最临近像素的坐标
		j1=(LONG)dX;
		j2=j1+1;
		if ( j1<0 )
		{
			j1=0;
		}
		if ( j2>lWidth-1 )
		{
			j2=lWidth-1;
		}
		i1=(LONG)dY;
		i2=i1+1;
		if ( i1<0 )
		{
			i1=0;
		}
		if ( i2>lHeight-1 )
		{
			i2=lHeight-1;
		}

		// 计算四个最临近像素值
		if ( bBitCount==8 )
		{
			// 对单通道色图
			bP1=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j1];
			bP2=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j2];
			bP3=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j1];
			bP4=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j2];
			bP12=DoubleToByte(bP1+(dX-j1)*(bP2-bP1));
			bP34=DoubleToByte(bP3+(dX-j1)*(bP4-bP3));
			return DoubleToByte(bP12+(dY-i1)*(bP34-bP12));
		}
		else
		{
			// 对三通道色图
			bP1=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j1*3+scBlue];
			bP2=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j2*3+scBlue];
			bP3=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j1*3+scBlue];
			bP4=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j2*3+scBlue];
			bP12=DoubleToByte(bP1+(dX-j1)*(bP2-bP1));
			bP34=DoubleToByte(bP3+(dX-j1)*(bP4-bP3));
			lRGB=DoubleToByte(bP12+(dY-i1)*(bP34-bP12));
			lRGB<<=8;
			bP1=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j1*3+scGreen];
			bP2=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j2*3+scGreen];
			bP3=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j1*3+scGreen];
			bP4=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j2*3+scGreen];
			bP12=DoubleToByte(bP1+(dX-j1)*(bP2-bP1));
			bP34=DoubleToByte(bP3+(dX-j1)*(bP4-bP3));
			lRGB|=DoubleToByte(bP12+(dY-i1)*(bP34-bP12));
			lRGB<<=8;
			bP1=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j1*3+scRed];
			bP2=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i1)+j2*3+scRed];
			bP3=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j1*3+scRed];
			bP4=((BYTE*)lpDIBBits)[lLineByteNum*(lHeight-1-i2)+j2*3+scRed];
			bP12=DoubleToByte(bP1+(dX-j1)*(bP2-bP1));
			bP34=DoubleToByte(bP3+(dX-j1)*(bP4-bP3));
			lRGB|=DoubleToByte(bP12+(dY-i1)*(bP34-bP12));
			return lRGB;
		}
	}
	return 0;
	}




/**************************************************************************

**************************************************************************/

// 学生自定义函数集

/*************************************************************************
 *
 * 函数名称：
 *	HistogramProcess()
 *
 * 参数:
 *	LPSTR	lpDIBBits	- 指向DIB数据区的指针
 *	BYTE	bBitCount	- 图像的颜色位数
 *	LONG	lWidth		- 源图像宽度（象素数）
 *	LONG	lHeight		- 源图像高度（象素数）
 *
 * 返回值:
 *	BOOL				- 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *	该函数用来对图像进行直方图处理。
 *
**************************************************************************/

BOOL HistogramProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n)
{
	// 判断输入参数是否有误
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));

	// 直方图处理

   
	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
			
		
	// 对256色位图（需要对bpSrc[j]进行处理
			}
			else
			{
				
				
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
				/* bpSrc[j*3+scBlue]=255-bpSrc[j*3+scBlue];
					bpSrc[j*3+scGreen]=255-bpSrc[j*3+scGreen];
					bpSrc[j*3+scRed]=255-bpSrc[j*3+scRed];
				*/



/*
	 	    int a=100,b;
		    b = (bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3;
			if(b>a)
			{
				bpSrc[j*3+scBlue] = 255;
				bpSrc[j*3+scGreen] = 255;
				bpSrc[j*3+scRed] = 255;
			}
			else
			{
				bpSrc[j*3+scBlue] = 0;
				bpSrc[j*3+scGreen] = 0;
				bpSrc[j*3+scRed] = 0;
			
			}
*/

			}
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *	TemplateProcess()
 *
 * 参数:
 *	LPSTR	lpDIBBits	- 指向DIB数据区的指针
 *	BYTE	bBitCount	- 图像的颜色位数
 *	LONG	lWidth		- 源图像宽度（象素数）
 *	LONG	lHeight		- 源图像高度（象素数）
 *
 * 返回值:
 *	BOOL				- 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *	该函数用来对图像进行模板处理。
 *
**************************************************************************/

BYTE midd(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);     //求中值处理

BOOL TemplateProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// 判断输入参数是否有误
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// 模板处理

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight-1; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // 图片宽度
		{


			if ( bBitCount==8)
			{

				    b0=bpSrc[j];
					b1=bpSrc[j-1];
					b2=bpSrc[j+1];
					b3=bpSrc[j-lLineBytes];
					b4=bpSrc[j+lLineBytes];

        bpDst[j]=(bpSrc[j]-midd(b0,b1,b2,b3,b4));
		  
				// 对256色位图（需要对bpSrc[j]进行处理）
			}
			else
			{
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
				 bpDst[j*3+scBlue] = (bpSrc[j*3+scBlue]- midd(bpSrc[j*3+scBlue],bpSrc[(j-1)*3+scBlue],bpSrc[(j+1)*3 +scBlue], bpSrc[j*3+scBlue-lLineBytes], bpSrc[j*3+scBlue+lLineBytes]));
				 bpDst[j*3+scGreen] = (bpSrc[j*3+scGreen]- midd(bpSrc[j*3+scGreen],bpSrc[(j-1)*3+scGreen],bpSrc[(j+1)*3 +scGreen], bpSrc[j*3+scGreen-lLineBytes], bpSrc[j*3+scGreen+lLineBytes]));
			     bpDst[j*3+scRed] = (bpSrc[j*3+scRed]-midd(bpSrc[j*3+scRed],bpSrc[(j-1)*3+scRed],bpSrc[(j+1)*3 +scRed], bpSrc[j*3+scRed-lLineBytes], bpSrc[j*3+scRed+lLineBytes]));

			}
		}
	}

	// 复制处理后的图像
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}



BYTE midd(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4)
{

BYTE num[5]={a0,a1,a2,a3,a4};
int i,j,m;
for( i=0;i<5;i++)
{
for( j=i;j<4;j++)
{
if(num[j]>num[j+1])
{
m=num[j];
num[j+1]=num[j];
num[j]=m;
}
}
}
return num[2];
}

/*************************************************************************
 *
 * 函数名称：
 *	GeoProcess()
 *
 * 参数:
 *	LPSTR	lpDIB		- 指向DIB的指针
 *
 * 返回值:
 *	HGLOBAL				- 成功返回新图像的句柄，否则返回NULL。
 *
 * 说明:
 *	该函数用来对图像进行几何处理。
 *
**************************************************************************/

#define PI 3.1415926535 

HGLOBAL GeoProcess(LPSTR lpDIB, int m_x,int m_y, double theta)
{
	// 判断输入参数是否有误
	if ( lpDIB==NULL || (DIBBitCount(lpDIB)!=8&&DIBBitCount(lpDIB)!=24) )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// DIB图像的颜色位数
	BYTE bBitCount;

	// 源DIB图像的宽度和高度
	LONG lWidth;
	LONG lHeight;
	
	// 几何处理后DIB图像的宽度和高度
	LONG lNewWidth;
	LONG lNewHeight;
	
	// 几何处理后DIB图像每行的字节数
	LONG lNewLineBytes;
	
	// 指向源DIB图像的指针
	BYTE *bpDIBBits;
	
	// 几何处理后新DIB图像句柄
	HDIB hDIB;
	
	// 指向几何处理DIB图像对应像素的指针
	BYTE *bpDst;
	
	// 指向几何处理DIB图像的指针
	BYTE *lpNewDIB;
	BYTE *bpNewDIBBits;
	
	// 指向BITMAPINFOHEADER结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;
	
	// 指向BITMAPCOREHEADER结构的指针
	LPBITMAPCOREHEADER lpbmc;
	
	// 循环变量（像素在新DIB图像中的坐标）
	LONG i;
	LONG j;
	
	// 像素在源DIB图像中的坐标
	double i0;
	double j0;
	
	// DIB图像每行的字节数
	LONG lLineBytes;
	
	// 找到源DIB图像像素起始位置
	bpDIBBits=(BYTE*)FindDIBBits(lpDIB);
	
	// 获取DIB图像的宽度和高度
	lWidth=DIBWidth(lpDIB);
	lHeight=DIBHeight(lpDIB);

	// 获取DIB图像的颜色位数
	bBitCount=DIBBitCount(lpDIB);

	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 计算新DIB图像实际宽度和高度
	//lNewWidth=f(lWidth, lHeight);
	//lNewHeight=f(lWidth, lHeight);
	lNewWidth=lWidth;
	lNewHeight=lHeight;
	
	// 计算新DIB图像每行的字节数
	lNewLineBytes=(bBitCount==8?WIDTHBYTES(lNewWidth*8):WIDTHBYTES(lNewWidth*24));
	
	// 分配内存，以保存新DIB图像
	hDIB=(HDIB)::GlobalAlloc(GHND, lNewLineBytes*lNewHeight*sizeof(BYTE)+(*(LPDWORD)lpDIB)+PaletteSize(lpDIB));
	
	// 判断是否内存分配失败
	if ( hDIB==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return NULL;
	}
	
	// 锁定内存
	lpNewDIB=(BYTE*)::GlobalLock((HGLOBAL)hDIB);
	
	// 复制DIB图像信息头和调色板
	memcpy(lpNewDIB, lpDIB, (*(LPDWORD)lpDIB)+PaletteSize(lpDIB));
	
	// 找到新DIB图像像素起始位置
	bpNewDIBBits=(BYTE*)FindDIBBits((LPSTR)lpNewDIB);
	
	// 获取指针
	lpbmi=(LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpNewDIB;
	
	// 更新DIB图像中DIB图像的高度和宽度
	if ( IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB图像
		lpbmi->biWidth=lNewWidth;
		lpbmi->biHeight=lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB图像
		lpbmc->bcWidth=(unsigned short)lNewWidth;
		lpbmc->bcHeight=(unsigned short)lNewHeight;
	}

/*	假设图像的左上角为（left, top),右下角为（right, bottom)，则图像上任意点（x0, y0）绕其中心
（xcenter,ycenter)逆时针旋转angle角度后，新的坐标位置（x′, y′）的计算公式为：

xcenter = (right － left ＋ 1) ／ 2 ＋ left;

ycenter = (bottom － top ＋ 1) ／ 2 ＋ top;

x′ = (x0 － xcenter) cosθ － (y0 － ycenter) sinθ ＋ xcenter;
y′ = (x0 － xcenter) sinθ ＋ (y0 － ycenter) cosθ ＋ ycenter;

*/////////////////////////////////////
	
	double x_center,y_center;  //定义 中心点
    x_center = (lWidth-1)/2;

	y_center = (lHeight-1)/2; 

	theta = PI*theta/180;
   
	if ( bBitCount==8 )       
	{
		// 对单通道色图
		for (i=0; i<lNewHeight; i++)
		{
			// 指向本行的像素指针
			bpDst=bpNewDIBBits+lNewLineBytes*(lNewHeight-1-i);

			// 计算该像素在源DIB图像中的坐标 	lNewWidth=lWidth;
                    
			//x′ = (x0 － xcenter) cosθ － (y0 － ycenter) sinθ ＋ xcenter;
             //y′ = (x0 － xcenter) sinθ ＋ (y0 － ycenter) cosθ ＋ ycenter;
			//	lNewHeight=lHeight;
			//i0=f(i);
	

			for (j=0; j<lNewWidth; j++)
			{
				// 计算该像素在源DIB图像中的坐标
				//j0=f(j);
			//	j0=lWidth-j+m_y;
	    	i0= m_x + (i-x_center)*cos(theta)-(j-y_center) *sin(theta)+ x_center;
			j0=m_y + (i-x_center) *sin(theta) +(j-y_center)* cos(theta)+ y_center;
			
				// 判断是否在源图范围内
				if ( j0>=-1.0 && j0<(double)lWidth && i0>=-1.0 && i0<(double)lHeight )
				{
					bpDst[j]=(BYTE)IF_BilinearInterpolation((LPSTR)bpDIBBits, bBitCount, lWidth, lHeight, j0, i0);
				}
				else
				{
					// 对于源图中没有的像素，直接赋值为bwmDIBWhite
					bpDst[j]=(BYTE)255;
					bpDst[j]=bpDst[j];
				}
			}
		}
	}
	else    
	{
		// 对三通道色图

		for (i=0; i<lNewHeight; i++)
		{
			// 指向本行的像素指针
			bpDst=bpNewDIBBits+lNewLineBytes*(lNewHeight-1-i);

			// 计算该像素在源DIB图像中的坐标
			//i0=f(i);
			//i0=i;
			//	i0=lHeight-i+m_x;

			for (j=0; j<lNewWidth; j++)
			{
				// 计算该像素在源DIB图像中的坐标
				//j0=f(j);
			//	j0=j;
			//	j0=lWidth-j+m_y;
		    i0= m_x + (i-x_center)*cos(theta)-(j-y_center) *sin(theta)+ x_center;
			j0=m_y + (i-x_center) *sin(theta) +(j-y_center)* cos(theta)+ y_center;
			
				// 判断是否在源图范围内
				if ( j0>=-1.0 && j0<(double)lWidth && i0>=-1.0 && i0<(double)lHeight )
				{
					LONG lTmpRGB=IF_BilinearInterpolation((LPSTR)bpDIBBits, bBitCount, lWidth, lHeight, j0, i0);
					bpDst[j*3+scBlue]=(BYTE)(lTmpRGB>>16);
					bpDst[j*3+scGreen]=(BYTE)((lTmpRGB&((LONG)255<<8))>>8);
					bpDst[j*3+scRed]=(BYTE)(lTmpRGB&(LONG)255);
				}
				else
				{
					// 对于源图中没有的像素，直接赋值为bwmDIBWhite
					bpDst[j*3+scBlue]=bpDst[j*3+scGreen]=bpDst[j*3+scRed]=(BYTE)255;
				
				}
			}
		}
	}
	
	return hDIB;
}


/*////////////////////////////////////////////////////////


  
	    噪声添加
	  


*******************************************************/

BOOL JHJZProcess2(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight,int m_nh,int m_nb, int m_xb, int m_xh)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
    
//	srand(time(0)); 
	

	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
	bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;
     
		for (j=0; j<lWidth; j++)
		{
     

 	if(m_xh==0)      //胡椒噪声
 {
			if ( bBitCount==8 )
		
	  {


       int SJS1 = rand()%255;

	if(SJS1 < m_nb)                   //胡椒噪声
			 
              bpSrc[j] = 255;
  else 
	     bpSrc[j]= bpSrc[j];
            
			 
				// 对256色位图（需要对bpSrc[j]进行处理）
			}


else
			{
						
         int SJS = rand()%255;

		 if(SJS < m_nb )
		 {
			
				bpSrc[j*3+scBlue] = 255;
				bpSrc[j*3+scGreen] = 255;
				bpSrc[j*3+scRed] = 255;
		 }
				else {
			
				bpSrc[j*3+scBlue] = bpSrc[j*3+scBlue];
				bpSrc[j*3+scGreen] = bpSrc[j*3+scGreen];
				bpSrc[j*3+scRed] = bpSrc[j*3+scRed];
			
				}
			}

			 }
			 	 

		if(m_xb==0)      //白噪声添加
 
 {
			
			if ( bBitCount==8 )
		
	  {


        int SJS1 = rand()%255;

			 if(SJS1 < m_nh)                 
			 
              bpSrc[j] = 0;
			 else 
       bpSrc[j] = bpSrc[j] ;
            //  bpDst[j] = 255;
			 
				// 对256色位图（需要对bpSrc[j]进行处理）
			}

else
			{				
         int SJS = rand()%255;

		 if(SJS< m_nh )
		 {
			
				bpSrc[j*3+scBlue] = 0;
				bpSrc[j*3+scGreen] = 0;
				bpSrc[j*3+scRed] = 0;
		 }
				else {
			
				bpSrc[j*3+scBlue] = bpSrc[j*3+scBlue];
				bpSrc[j*3+scGreen] = bpSrc[j*3+scGreen];
				bpSrc[j*3+scRed] = bpSrc[j*3+scRed];
			
				}
			}
			 }


		}
		}

	return TRUE;
	}
	


// 逆谐波均值处理 



BOOL NXBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// 判断输入参数是否有误
		// 判断输入参数是否有误
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// 模板处理

    //	int b0,b1,b2,b3,b4;
	double product1,product2;
    int n,m,Q=0; 
	for (i=0; i<lHeight-1; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // 图片宽度
		{

		if ( bBitCount==8 )
			{
				for ( m=j-1; m<=j+1; m++)//分子
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//分母
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n]),Q);
					}
				}
				
				 bpDst[j]=int (bpSrc[j]-(product1/product2)); 
		

			}
			else
			{

		////////////////////////////

				for ( m=j-1; m<=j+1; m++)//分子
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scBlue]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//分母
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n*3+scBlue]),Q);
					}
				}
				
           bpDst[j*3+scBlue]=int((product1/product2)); 

		   ////////////////////

		   				for ( m=j-1; m<=j+1; m++)//分子
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scGreen]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//分母
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n*3+scGreen]),Q);
					}
				}
				
           bpDst[j*3+scGreen]=int ((product1/product2)); 
		   /////////////////////////
		   		for ( m=j-1; m<=j+1; m++)//分子
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scRed]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//分母
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n*3+scRed]),Q);
					}
				}
				
           bpDst[j*3+scRed]=int((product1/product2)); 
		   
				// bpDst[j*3+scBlue]=int(bpSrc[j*3+scBlue]-(product1/product2)); 
				// bpSrc[j*3+scGreen] = int(bpSrc[j*3+scGreen]-(product1/product2)); 
                // bpSrc[j*3+scRed] = int(bpSrc[j*3+scRed]-(product1/product2)); 


			}
		}
	}

	// 复制处理后的图像
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;

}

// 谐波均值

BOOL  XBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// 判断输入参数是否有误
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// 模板处理

    //	int b0,b1,b2,b3,b4;
	double product1,product2;
    int n,m,Q= -1; 
	for (i=0; i<lHeight-1; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // 图片宽度
		{


		if ( bBitCount==8 )
			{
				for ( m=j-1; m<=j+1; m++)//分子
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//分母
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n]),Q);
					}
				}
				
				 bpDst[j]=int (bpSrc[j]-(product1/product2)); 
		

			}

			else
			{
			;
				
			}
		}
	}

	// 复制处理后的图像
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}


////////////////////////////////////////////

//均衡化


/////////////////////////////////////////

BOOL JHHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
//	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
	int k=0;
	int a[256];
	int sum=0;
	int ave=0;
	double H=0;
	for (k=0; k<255; k++)
	{
		a[k]=0;
	}	

	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// 对256色位图（需要对bpSrc[j]进行处理）
				a[bpSrc[j]]=a[bpSrc[j]]+1;
			}
			else
			{
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
					ave=int((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3+0.5);
					a[ave]=a[ave]+1;
			}
		}
	}

	H=lWidth*lHeight/255;

	//遍历所有像素进行均衡化
	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			sum=0;//每个像素点计算前清零
			if ( bBitCount==8 )
			{
				// 对256色位图（需要对bpSrc[j]进行处理）
				for (k=0; k<bpSrc[j]; k++)
				{
					sum=sum+a[k];
				}
				bpSrc[j]=int(sum/H+0.5);

			}
			else
			{
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
				ave=int((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3+0.5);
				for (k=0; k<ave; k++)
				{
					sum=sum+a[k];
				}
				bpSrc[j*3+scBlue]=int(sum/H+0.5);
				bpSrc[j*3+scGreen]=int(sum/H+0.5);
				bpSrc[j*3+scRed]=int(sum/H+0.5);
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////

// 取反色


///////////////////////////////////////

BOOL QFSProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 循环变量
	LONG i;
	LONG j;
	int k;

	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
  
	int a[256];
	int sum=0;
	int ave=0;
	double H=0;
	for (k=0; k<255; k++)
	{
		a[k]=0;
	}	

	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// 对256色位图（需要对bpSrc[j]进行处理）
				a[bpSrc[j]]=a[bpSrc[j]]+1;
			}
			else
			{
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
					ave=int((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3+0.5);
					a[ave]=a[ave]+1;
			}
		}
	}

	
	H=lWidth*lHeight/255;

	//遍历所有像素进行均衡化
	
	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				bpSrc[j]=255-bpSrc[j];// 对256色位图（需要对bpSrc[j]进行处理）
			}
			else
			{
				bpSrc[j*3+scBlue]=255-bpSrc[j*3+scBlue];
				bpSrc[j*3+scGreen]=255-bpSrc[j*3+scGreen];
				bpSrc[j*3+scRed]=255-bpSrc[j*3+scRed];

				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
			}
		}
	}

	return TRUE;
}


//  阈值化 

BOOL YZHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n)
{
	// 判断输入参数是否有误
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));

	// 直方图处理

   
	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// 对256色位图（需要对bpSrc[j]进行处理）
			
				if (bpSrc[j]<=m_n)
					bpSrc[j]=0;
			}
			else			
			{	
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
			
				if ((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3<=m_n)
				{
					bpSrc[j*3+scBlue]=0;
					bpSrc[j*3+scGreen]=0;
					bpSrc[j*3+scRed]=0;
				}
	
			}
		}
	}

	return TRUE;
}

///////////////////////////

//膨胀与腐蚀

///////////////////////////


BOOL pengzhangProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	
		if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);

	
	for (i=0; i<lHeight-1; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);
		for (j=0; j<lWidth-1; j++)
		{
		if ( bBitCount==8 )
			{
			// 对256色位图（需要对bpSrc[j]进行处理）
       if((bpSrc[j]==255)&&(bpSrc[j-1]==255)&&(bpSrc[j+1]==255)&&(bpSrc[j-lLineBytes]==255)&&(bpSrc[j+lLineBytes]==255))
	   {
		   bpDst[j]= 255 ;
	   }
       else 
	   {
        bpDst[j] = 0;
	   }

			}
			else
			{
		if((bpSrc[j*3+scBlue]==255)&&(bpSrc[(j-1)*3+scBlue]==255)&&(bpSrc[(j+1)*3+scBlue]==255)&&(bpSrc[j*3-lLineBytes+scBlue]==255)&&(bpSrc[j*3+lLineBytes+scBlue]==255))
	   {
		bpDst[j*3+scBlue]= 255 ;
	   }
       else 
	   {
      bpDst[j*3+scBlue] = bpDst[j*3+scBlue];
	   }

	if((bpSrc[j*3+scGreen]==255)&&(bpSrc[(j-1)*3+scGreen]==255)&&(bpSrc[(j+1)*3+scGreen]==255)&&(bpSrc[j*3-lLineBytes+scGreen]==255)&&(bpSrc[j*3+lLineBytes+scGreen]==255))
	   {
	 bpDst[j*3+scGreen]= 255 ;
	   }
       else 
	   {
      bpDst[j*3+scGreen] = bpDst[j*3+scGreen];
	   }
	 
	   if((bpSrc[j*3+scRed]==255)&&(bpSrc[(j-1)*3+scRed]==255)&&(bpSrc[(j+1)*3+scRed]==255)&&(bpSrc[j*3-lLineBytes+scRed]==255)&&(bpSrc[j*3+lLineBytes+scRed]==255))
	   {
		   bpDst[j*3+scRed]= 255 ;
	   }
       else 
	   {
      bpDst[j*3+scRed] = bpDst[j*3+scRed];
	   }
			}
			}
		}

		// 复制处理后的图像

	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




//中值滤波


BOOL zhongzhiProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	
		if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// 模板处理

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight-1; i++)
	{
		// 指向本行的象素指针
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // 图片宽度
		{


			if ( bBitCount==8)
			{

				    b0=bpSrc[j];
					b1=bpSrc[j-1];
					b2=bpSrc[j+1];
					b3=bpSrc[j-lLineBytes];
					b4=bpSrc[j+lLineBytes];

        bpDst[j]=(bpSrc[j]-midd(b0,b1,b2,b3,b4));
		  
				// 对256色位图（需要对bpSrc[j]进行处理）
			}
			else
			{
				// 对24位色位图（需要分别对bpSrc[j*3+scBlue]、bpSrc[j*3+scGreen]和bpSrc[j*3+scRed]进行处理）
 bpDst[j*3+scBlue] =(bpSrc[j*3+scBlue]- midd(bpSrc[j*3+scBlue],bpSrc[(j-1)*3+scBlue],bpSrc[(j+1)*3 +scBlue], bpSrc[j*3+scBlue-lLineBytes], bpSrc[j*3+scBlue+lLineBytes]));
 bpDst[j*3+scGreen] =(bpSrc[j*3+scGreen]- midd(bpSrc[j*3+scGreen],bpSrc[(j-1)*3+scGreen],bpSrc[(j+1)*3 +scGreen], bpSrc[j*3+scGreen-lLineBytes], bpSrc[j*3+scGreen+lLineBytes]));
 bpDst[j*3+scRed] =(bpSrc[j*3+scRed]-midd(bpSrc[j*3+scRed],bpSrc[(j-1)*3+scRed],bpSrc[(j+1)*3 +scRed], bpSrc[j*3+scRed-lLineBytes], bpSrc[j*3+scRed+lLineBytes]));

			}
		}
	}

	// 复制处理后的图像
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}




//最小值处理

BYTE minm(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4)
{

BYTE num[5]={a0,a1,a2,a3,a4};
int i,j,m;
for( i=0;i<5;i++)
{
for( j=i;j<4;j++)
{

if(num[j]<num[j+1])
{
m=num[j];
num[j]=num[j+1];
num[j+1]=m;
}

}
}
return num[4];
}


BOOL ZUIXIAOProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{

		if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("输入参数有误！", MB_OK);
		return FALSE;
	}

	// 指向源图像的指针
	BYTE *bpSrc;
	
	// 指向目标图像的指针
	BYTE *bpDst;
	
	// 指向复制DIB图像的指针
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算DIB图像每行的字节数
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// 暂时分配内存，以保存新DIB图像
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// 判断是否内存分配失败
	if ( hNewDIBBits==NULL )
	{
		// 分配内存失败
		AfxMessageBox("内存分配有误！", MB_OK);
		return FALSE;
	}

	// 锁定内存
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// 模板处理

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight; i++)
	{
		// 指向本行的象素指针

		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth; j++)
		{
			if (bBitCount==8 )
			{
				// 对256色位图（需要对bpSrc[j]进行处理）
					b0=bpSrc[j];
					b1=bpSrc[j-1];
					b2=bpSrc[j+1];
					b3=bpSrc[j-lLineBytes];
					b4=bpSrc[j+lLineBytes];

  
       
		
			 bpDst[j]=(bpSrc[j]-minm(b0,b1,b2,b3,b4));
         
			}
			else
			{
 bpDst[j*3+scBlue] =(bpSrc[j*3+scBlue]- minm(bpSrc[j*3+scBlue],bpSrc[(j-1)*3+scBlue],bpSrc[(j+1)*3 +scBlue], bpSrc[j*3+scBlue-lLineBytes], bpSrc[j*3+scBlue+lLineBytes]));
 bpDst[j*3+scGreen] =(bpSrc[j*3+scGreen]- minm(bpSrc[j*3+scGreen],bpSrc[(j-1)*3+scGreen],bpSrc[(j+1)*3 +scGreen], bpSrc[j*3+scGreen-lLineBytes], bpSrc[j*3+scGreen+lLineBytes]));
 bpDst[j*3+scRed] =(bpSrc[j*3+scRed]-minm(bpSrc[j*3+scRed],bpSrc[(j-1)*3+scRed],bpSrc[(j+1)*3 +scRed], bpSrc[j*3+scRed-lLineBytes], bpSrc[j*3+scRed+lLineBytes]));

				
			}
		}
	}

memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	return TRUE;
}



	

