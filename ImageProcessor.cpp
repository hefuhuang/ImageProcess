// ************************************************************************
//  �ļ�����ImageProcessor.cpp
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
 *	DIB(Independent Bitmap) API�����⣺������ʦ�ṩ��ѧ����
 *
 *	PaintDIB()									- ����DIB����
 *
 *	FindDIBBits()								- ����DIBͼ��������ʼλ��
 *
 *	FindPalette()								- ����DIB��ɫ����ʼλ��
 *
 *	DIBWidth()									- ����DIB���
 *
 *	DIBHeight()									- ����DIB�߶�
 *
 *	PaletteSize()								- ����DIB��ɫ���С
 *
 *	DIBNumColors()								- ����DIB��ɫ����ɫ��Ŀ
 *
 *	DIBBitCount()								- ����DIB����ɫλ��
 *
 *	CopyHandle()								- �����ڴ��
 *
 *	SaveDIB()									- ��DIB���浽ָ���ļ���
 *
 *	ReadDIBFile()								- ��ָ���ļ��ж�ȡDIB����
 *
 ************************************************************************/

/*************************************************************************
 *
 * �������ƣ�
 *	PaintDIB()
 *
 * ����:
 *	HDC			hDC			- ����豸DC�ľ��
 *	LPRECT		lpDCRect	- ���ƾ�������
 *	HDIB		hDIB		- ָ��DIB����ľ��
 *	LPRECT		lpDIBRect	- Ҫ�����DIB����
 *	CPalette*	pPal		- ָ��DIB�����ɫ���ָ��
 *
 * ����ֵ:
 *	BOOL					- ���Ƴɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *	�ú�����Ҫ��������DIB�������е�����StretchDIBits()����
 * SetDIBitsToDevice()������DIB����������豸���ɲ���hDCָ
 * �������Ƶľ��������ɲ���lpDCRectָ�������DIB�������ɲ���
 * lpDIBRectָ����
 *
************************************************************************/

BOOL PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect)
{
	LPSTR lpDIBHdr;				// BITMAPINFOHEADERָ��
	LPSTR lpDIBBits;				// DIB����ָ��
	BOOL blSuccess=FALSE;		// �ɹ���־

	// �ж���������Ƿ�����
	if ( hDC==NULL || lpDCRect==NULL || lpDIBRect==NULL )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}
	
	// �ж�DIB�����Ƿ�Ϊ��
	if (hDIB==NULL)
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ����DIB
	lpDIBHdr=(LPSTR)::GlobalLock((HGLOBAL)hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits=FindDIBBits(lpDIBHdr);

	// ������ʾģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
	if ((RECTWIDTH(lpDCRect)==RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect)==RECTHEIGHT(lpDIBRect)))
	{
		// ԭʼ��С���������졣
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
		// ��ԭʼ��С�����졣
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
	
    // �������
	::GlobalUnlock((HGLOBAL)hDIB);

	return blSuccess;
}

/*************************************************************************
 *
 * �������ƣ�
 *	FindDIBBits()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	LPSTR			- ָ��DIBͼ��������ʼλ��
 *
 * ˵��:
 *	�ú�������DIB��ͼ�����ص���ʼλ�ã�������ָ������ָ�롣
 *
************************************************************************/

LPSTR FindDIBBits(LPSTR lpDIB)
{
	return (lpDIB+(*((LPDWORD)lpDIB))+PaletteSize(lpDIB));
}

/*************************************************************************
 *
 * �������ƣ�
 *	FindPalette()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	LPSTR			- ָ��DIB��ɫ�����ʼλ��
 *
 * ˵��:
 *	�ú�������DIB�е�ɫ�����ʼλ�ã�������ָ������ָ�롣
 *
************************************************************************/

LPSTR FindPalette(LPSTR lpDIB)
{
	return (DIBNumColors(lpDIB)==0?NULL:lpDIB+(*((LPDWORD)lpDIB)));
}

/*************************************************************************
 *
 * �������ƣ�
 *	DIBWidth()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	DWORD			- DIB��ͼ��Ŀ��
 *
 * ˵��:
 *	�ú�������DIB��ͼ��Ŀ�ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biWidthֵ��������������BITMAPCOREHEADER�е�bcWidthֵ��
 *
************************************************************************/

DWORD DIBWidth(LPSTR lpDIB)
{
	// ָ��BITMAPINFOHEADER�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREHEADER�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi=(LPBITMAPINFOHEADER)lpDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}

/*************************************************************************
 *
 * �������ƣ�
 *	DIBHeight()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	DWORD			- DIB��ͼ��ĸ߶�
 *
 * ˵��:
 *	�ú�������DIB��ͼ��ĸ߶ȡ�����Windows 3.0 DIB������BITMAPINFOHEADER
 * �е�biHeightֵ��������������BITMAPCOREHEADER�е�bcHeightֵ��
 *
************************************************************************/

DWORD DIBHeight(LPSTR lpDIB)
{
	// ָ��BITMAPINFOHEADER�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREHEADER�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi=(LPBITMAPINFOHEADER)lpDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}

/*************************************************************************
 *
 * �������ƣ�
 *	PaletteSize()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	WORD			- DIB�е�ɫ��Ĵ�С
 *
 * ˵��:
 *	�ú�������DIB�е�ɫ��Ĵ�С������Windows 3.0 DIB��������ɫ��Ŀ��
 * RGBQUAD�Ĵ�С����������������ɫ��Ŀ��RGBTRIPLE�Ĵ�С��
 *
************************************************************************/

WORD PaletteSize(LPSTR lpDIB)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB (lpDIB))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(DIBNumColors(lpDIB)*sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(DIBNumColors(lpDIB)*sizeof(RGBTRIPLE));
	}
}

/*************************************************************************
 *
 * �������ƣ�
 *	DIBNumColors()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	WORD			- ���ص�ɫ������ɫ������
 *
 * ˵��:
 *	�ú�������DIB�е�ɫ�����ɫ�����������ڵ�ɫλͼ������2��
 * ����16ɫλͼ������16������256ɫλͼ������256���������ɫ
 * λͼ��24λ����û�е�ɫ�壬����0��
 *
************************************************************************/

WORD DIBNumColors(LPSTR lpDIB)
{
	BYTE bBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��
	
	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpDIB))
	{
		DWORD dwClrUsed;
		
		// ��ȡdwClrUsedֵ
		dwClrUsed=((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
		
		if (dwClrUsed!=0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ��ȡbiBitCountֵ
		bBitCount=(BYTE)((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		bBitCount=(BYTE)((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	}
	
	// �������ص�λ��������ɫ��Ŀ
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
 * �������ƣ�
 *	DIBBitCount()
 *
 * ����:
 *	LPSTR	lpDIB	- ָ��DIB�����ָ��
 *
 * ����ֵ:
 *	BYTE			- ����ͼ�����ɫλ��
 *
 * ˵��:
 *	�ú�������DIB����ɫλ����
 *
************************************************************************/

BYTE DIBBitCount(LPSTR lpDIB)
{
	BYTE bBitCount;

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ��ȡbiBitCountֵ
		bBitCount=(BYTE)((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		bBitCount=(BYTE)((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	}
	
	return bBitCount;
}

/*************************************************************************
 *
 * �������ƣ�
 *	CopyHandle()
 *
 * ����:
 *	HGLOBAL	h		- Ҫ���Ƶ��ڴ�����
 *
 * ����ֵ:
 *	HGLOBAL			- ���ƺ�����ڴ�����
 *
 * ˵��:
 *	�ú�������ָ�����ڴ����򡣷��ظ��ƺ�����ڴ����򣬳���ʱ����0��
 *
************************************************************************/

HGLOBAL CopyHandle (HGLOBAL h)
{
	// �����������Ƿ�����
	if (h==NULL)
	{
		AfxMessageBox("�����������", MB_OK);
		return NULL;
	}

	// ��ȡָ���ڴ������С
	DWORD dwLen=::GlobalSize((HGLOBAL) h);
	
	// �������ڴ�ռ�
	HGLOBAL hCopy=::GlobalAlloc(GHND, dwLen);
	
	// �жϷ����Ƿ�ɹ�
	if (hCopy!=NULL)
	{
		// ����
		void* lpCopy=::GlobalLock((HGLOBAL) hCopy);
		void* lp=::GlobalLock((HGLOBAL) h);
		
		// ����
		memcpy(lpCopy, lp, dwLen);
		
		// �������
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

/*************************************************************************
 *
 * �������ƣ�
 *	SaveDIB()
 *
 * ����:
 *	HDIB	hDIB		- Ҫ�����DIB�ľ��
 *	CFile&	file		- �����ļ�CFile
 *
 * ����ֵ:
 *	BOOL				- �ɹ�����TRUE�����򷵻�FALSE����CFileException
 *
 * ˵��:
 *	�ú�����ָ����DIB���󱣴浽ָ����CFile�С���CFile�ɵ��ó���򿪺͹رա�
 *
*************************************************************************/

BOOL SaveDIB(HDIB hDIB, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// DIB��С
	DWORD dwDIBSize;

	// �����������Ƿ�����
	if (hDIB==NULL)
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI=(LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDIB);
	
	// �����������Ƿ�����
	if (lpBI==NULL)
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}
	
	// �ж��Ƿ���WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// ��֧���������͵�DIB����
		
		// �������
		::GlobalUnlock((HGLOBAL)hDIB);
		
		AfxMessageBox("��֧�ַ�WIN3.0����DIB�ı��棡", MB_OK);
		return FALSE;
	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType=DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
	
	// �ļ�ͷ��С����ɫ���С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
	dwDIBSize=*(LPDWORD)lpBI+PaletteSize((LPSTR)lpBI);
	
	// ����ͼ���С
	if ((lpBI->biCompression==BI_RLE8) || (lpBI->biCompression==BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize+=lpBI->biSizeImage;
	}
	else
	{
		// ���صĴ�С
		DWORD dwBmBitsSize;

		// ��СΪWidth*Height
		dwBmBitsSize=WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount))*lpBI->biHeight;
		
		// �����DIB�����Ĵ�С
		dwDIBSize+=dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage=dwBmBitsSize;
	}


	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize=dwDIBSize+sizeof(BITMAPFILEHEADER);
	
	// �������Ƶ���
	bmfHdr.bfReserved1=0;
	bmfHdr.bfReserved2=0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ���С
	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER)+lpBI->biSize
											 +PaletteSize((LPSTR)lpBI);
	// ����д�ļ�
	TRY
	{
		// д�ļ�ͷ
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// дDIBͷ������
		file.Write(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// �������
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// �׳��쳣
		THROW_LAST();
	}
	END_CATCH
	
	// �������
	::GlobalUnlock((HGLOBAL)hDIB);
	
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *  ReadDIBFile()
 *
 * ����:
 *	CFile& file		- Ҫ��ȡ���ļ��ļ�CFile
 *
 * ����ֵ:
 *	hDIB			- �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *	�ú�����ָ�����ļ��е�DIB�������ָ�����ڴ������С���BITMAPFILEHEADER
 * ������ݶ����������ڴ档
 *
*************************************************************************/

HDIB ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR lpDIB;

	// ��ȡDIB���ļ������ȣ��ֽڣ�
	dwBitsSize=(DWORD)file.GetLength();

	// ���Զ�ȡDIB�ļ�ͷ
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader))!=sizeof(bmfHeader))
	{
		AfxMessageBox("�ļ�ͷ��ȡ����", MB_OK);
		return NULL;
	}

	// �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
	if (bmfHeader.bfType!=DIB_HEADER_MARKER)
	{
		AfxMessageBox("���ļ�����DIBͼ���ļ���", MB_OK);
		return NULL;
	}

	// ΪDIB�����ڴ�
	hDIB=(HDIB)::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB==0)
	{
		AfxMessageBox("�ڴ��������", MB_OK);
		return NULL;
	}
	
	// ����
	lpDIB=(LPSTR)::GlobalLock((HGLOBAL)hDIB);

	// ������
	if (file.Read(lpDIB, dwBitsSize-sizeof(BITMAPFILEHEADER))!=dwBitsSize-sizeof(BITMAPFILEHEADER) )
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL)hDIB);
		
		AfxMessageBox("�ļ����ݶ�ȡ����", MB_OK);
		return NULL;
	}
	
	// �������
	::GlobalUnlock((HGLOBAL)hDIB);
	
	return hDIB;
}

/*************************************************************************
 *
 * �������ƣ�
 *	IF_BilinearInterpolation()
 *
 * ����:
 *	LPSTR					lpDIBBits			- ָ��DIBͼ����������ָ��
 *	BYTE					bBitCount			- DIBͼ�����ɫλ��
 *	LONG					lWidth				- ԴDIBͼ���ȣ���������
 *	LONG					lHeight				- ԴDIBͼ��߶ȣ���������
 *	double					dX					- ��ֵԪ�ص�dX����
 *	double					dY					- ��ֵԪ�ص�dY����
 *
 * ����ֵ:
 *	LONG										- ���ز�ֵ��������
 *
 * ˵��:
 *	�ú�������˫���Բ�ֵ�㷨����������ֵ�����ڳ���DIBͼ��Χ�����أ�
 * ֱ�ӷ���255�����⣬24λɫ��BGR˳��װ��һ��LONG���ء�
 *
 ************************************************************************/

inline LONG IF_BilinearInterpolation(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, double dX, double dY)
{
	// �ĸ����ٽ����ص�����(j1, i1), (j2, i1), (j1, i2), (j2, i2)
	LONG i1, i2;
	LONG j1, j2;
	
	// �ĸ����ٽ�����ֵ
	BYTE bP1, bP2, bP3, bP4;
	
	// ������ֵ�м�ֵ
	BYTE bP12, bP34;

	// RGB��������ֵ
	LONG lRGB;
	
	// DIBͼ��ÿ�е��ֽ���
	LONG lLineByteNum;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineByteNum=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ���ݲ�ͬ����ֱ���
	if ( dX<-1.0 || dX>=(double)lWidth || dY<-1.0 || dY>=(double)lHeight )
	{
		// Ҫ����ĵ㲻��Դͼ��Χ�ڣ�ֱ�ӷ���bwmDIBWhite��
		if ( bBitCount==8 )
		{
			// �Ե�ͨ��ɫͼ
			return 255;
		}
		else
		{
			// ����ͨ��ɫͼ
			return (255<<16)|(255<<8)|255;	//����������bwmDIBWhite
		}
	}
	else
	{
		// �����ĸ����ٽ����ص�����
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

		// �����ĸ����ٽ�����ֵ
		if ( bBitCount==8 )
		{
			// �Ե�ͨ��ɫͼ
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
			// ����ͨ��ɫͼ
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

// ѧ���Զ��庯����

/*************************************************************************
 *
 * �������ƣ�
 *	HistogramProcess()
 *
 * ����:
 *	LPSTR	lpDIBBits	- ָ��DIB��������ָ��
 *	BYTE	bBitCount	- ͼ�����ɫλ��
 *	LONG	lWidth		- Դͼ���ȣ���������
 *	LONG	lHeight		- Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *	BOOL				- �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *	�ú���������ͼ�����ֱ��ͼ����
 *
**************************************************************************/

BOOL HistogramProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n)
{
	// �ж���������Ƿ�����
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));

	// ֱ��ͼ����

   
	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
			
		
	// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
			}
			else
			{
				
				
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
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
 * �������ƣ�
 *	TemplateProcess()
 *
 * ����:
 *	LPSTR	lpDIBBits	- ָ��DIB��������ָ��
 *	BYTE	bBitCount	- ͼ�����ɫλ��
 *	LONG	lWidth		- Դͼ���ȣ���������
 *	LONG	lHeight		- Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *	BOOL				- �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *	�ú���������ͼ�����ģ�崦��
 *
**************************************************************************/

BYTE midd(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);     //����ֵ����

BOOL TemplateProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// �ж���������Ƿ�����
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// ģ�崦��

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight-1; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // ͼƬ���
		{


			if ( bBitCount==8)
			{

				    b0=bpSrc[j];
					b1=bpSrc[j-1];
					b2=bpSrc[j+1];
					b3=bpSrc[j-lLineBytes];
					b4=bpSrc[j+lLineBytes];

        bpDst[j]=(bpSrc[j]-midd(b0,b1,b2,b3,b4));
		  
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
			}
			else
			{
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
				 bpDst[j*3+scBlue] = (bpSrc[j*3+scBlue]- midd(bpSrc[j*3+scBlue],bpSrc[(j-1)*3+scBlue],bpSrc[(j+1)*3 +scBlue], bpSrc[j*3+scBlue-lLineBytes], bpSrc[j*3+scBlue+lLineBytes]));
				 bpDst[j*3+scGreen] = (bpSrc[j*3+scGreen]- midd(bpSrc[j*3+scGreen],bpSrc[(j-1)*3+scGreen],bpSrc[(j+1)*3 +scGreen], bpSrc[j*3+scGreen-lLineBytes], bpSrc[j*3+scGreen+lLineBytes]));
			     bpDst[j*3+scRed] = (bpSrc[j*3+scRed]-midd(bpSrc[j*3+scRed],bpSrc[(j-1)*3+scRed],bpSrc[(j+1)*3 +scRed], bpSrc[j*3+scRed-lLineBytes], bpSrc[j*3+scRed+lLineBytes]));

			}
		}
	}

	// ���ƴ�����ͼ��
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// �ͷ��ڴ�
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
 * �������ƣ�
 *	GeoProcess()
 *
 * ����:
 *	LPSTR	lpDIB		- ָ��DIB��ָ��
 *
 * ����ֵ:
 *	HGLOBAL				- �ɹ�������ͼ��ľ�������򷵻�NULL��
 *
 * ˵��:
 *	�ú���������ͼ����м��δ���
 *
**************************************************************************/

#define PI 3.1415926535 

HGLOBAL GeoProcess(LPSTR lpDIB, int m_x,int m_y, double theta)
{
	// �ж���������Ƿ�����
	if ( lpDIB==NULL || (DIBBitCount(lpDIB)!=8&&DIBBitCount(lpDIB)!=24) )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// DIBͼ�����ɫλ��
	BYTE bBitCount;

	// ԴDIBͼ��Ŀ�Ⱥ͸߶�
	LONG lWidth;
	LONG lHeight;
	
	// ���δ����DIBͼ��Ŀ�Ⱥ͸߶�
	LONG lNewWidth;
	LONG lNewHeight;
	
	// ���δ����DIBͼ��ÿ�е��ֽ���
	LONG lNewLineBytes;
	
	// ָ��ԴDIBͼ���ָ��
	BYTE *bpDIBBits;
	
	// ���δ������DIBͼ����
	HDIB hDIB;
	
	// ָ�򼸺δ���DIBͼ���Ӧ���ص�ָ��
	BYTE *bpDst;
	
	// ָ�򼸺δ���DIBͼ���ָ��
	BYTE *lpNewDIB;
	BYTE *bpNewDIBBits;
	
	// ָ��BITMAPINFOHEADER�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREHEADER�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;
	
	// ѭ����������������DIBͼ���е����꣩
	LONG i;
	LONG j;
	
	// ������ԴDIBͼ���е�����
	double i0;
	double j0;
	
	// DIBͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// �ҵ�ԴDIBͼ��������ʼλ��
	bpDIBBits=(BYTE*)FindDIBBits(lpDIB);
	
	// ��ȡDIBͼ��Ŀ�Ⱥ͸߶�
	lWidth=DIBWidth(lpDIB);
	lHeight=DIBHeight(lpDIB);

	// ��ȡDIBͼ�����ɫλ��
	bBitCount=DIBBitCount(lpDIB);

	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ������DIBͼ��ʵ�ʿ�Ⱥ͸߶�
	//lNewWidth=f(lWidth, lHeight);
	//lNewHeight=f(lWidth, lHeight);
	lNewWidth=lWidth;
	lNewHeight=lHeight;
	
	// ������DIBͼ��ÿ�е��ֽ���
	lNewLineBytes=(bBitCount==8?WIDTHBYTES(lNewWidth*8):WIDTHBYTES(lNewWidth*24));
	
	// �����ڴ棬�Ա�����DIBͼ��
	hDIB=(HDIB)::GlobalAlloc(GHND, lNewLineBytes*lNewHeight*sizeof(BYTE)+(*(LPDWORD)lpDIB)+PaletteSize(lpDIB));
	
	// �ж��Ƿ��ڴ����ʧ��
	if ( hDIB==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return NULL;
	}
	
	// �����ڴ�
	lpNewDIB=(BYTE*)::GlobalLock((HGLOBAL)hDIB);
	
	// ����DIBͼ����Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, (*(LPDWORD)lpDIB)+PaletteSize(lpDIB));
	
	// �ҵ���DIBͼ��������ʼλ��
	bpNewDIBBits=(BYTE*)FindDIBBits((LPSTR)lpNewDIB);
	
	// ��ȡָ��
	lpbmi=(LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc=(LPBITMAPCOREHEADER)lpNewDIB;
	
	// ����DIBͼ����DIBͼ��ĸ߶ȺͿ��
	if ( IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIBͼ��
		lpbmi->biWidth=lNewWidth;
		lpbmi->biHeight=lNewHeight;
	}
	else
	{
		// ����������ʽ��DIBͼ��
		lpbmc->bcWidth=(unsigned short)lNewWidth;
		lpbmc->bcHeight=(unsigned short)lNewHeight;
	}

/*	����ͼ������Ͻ�Ϊ��left, top),���½�Ϊ��right, bottom)����ͼ��������㣨x0, y0����������
��xcenter,ycenter)��ʱ����תangle�ǶȺ��µ�����λ�ã�x��, y�䣩�ļ��㹫ʽΪ��

xcenter = (right �� left �� 1) �� 2 �� left;

ycenter = (bottom �� top �� 1) �� 2 �� top;

x�� = (x0 �� xcenter) cos�� �� (y0 �� ycenter) sin�� �� xcenter;
y�� = (x0 �� xcenter) sin�� �� (y0 �� ycenter) cos�� �� ycenter;

*/////////////////////////////////////
	
	double x_center,y_center;  //���� ���ĵ�
    x_center = (lWidth-1)/2;

	y_center = (lHeight-1)/2; 

	theta = PI*theta/180;
   
	if ( bBitCount==8 )       
	{
		// �Ե�ͨ��ɫͼ
		for (i=0; i<lNewHeight; i++)
		{
			// ָ���е�����ָ��
			bpDst=bpNewDIBBits+lNewLineBytes*(lNewHeight-1-i);

			// �����������ԴDIBͼ���е����� 	lNewWidth=lWidth;
                    
			//x�� = (x0 �� xcenter) cos�� �� (y0 �� ycenter) sin�� �� xcenter;
             //y�� = (x0 �� xcenter) sin�� �� (y0 �� ycenter) cos�� �� ycenter;
			//	lNewHeight=lHeight;
			//i0=f(i);
	

			for (j=0; j<lNewWidth; j++)
			{
				// �����������ԴDIBͼ���е�����
				//j0=f(j);
			//	j0=lWidth-j+m_y;
	    	i0= m_x + (i-x_center)*cos(theta)-(j-y_center) *sin(theta)+ x_center;
			j0=m_y + (i-x_center) *sin(theta) +(j-y_center)* cos(theta)+ y_center;
			
				// �ж��Ƿ���Դͼ��Χ��
				if ( j0>=-1.0 && j0<(double)lWidth && i0>=-1.0 && i0<(double)lHeight )
				{
					bpDst[j]=(BYTE)IF_BilinearInterpolation((LPSTR)bpDIBBits, bBitCount, lWidth, lHeight, j0, i0);
				}
				else
				{
					// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪbwmDIBWhite
					bpDst[j]=(BYTE)255;
					bpDst[j]=bpDst[j];
				}
			}
		}
	}
	else    
	{
		// ����ͨ��ɫͼ

		for (i=0; i<lNewHeight; i++)
		{
			// ָ���е�����ָ��
			bpDst=bpNewDIBBits+lNewLineBytes*(lNewHeight-1-i);

			// �����������ԴDIBͼ���е�����
			//i0=f(i);
			//i0=i;
			//	i0=lHeight-i+m_x;

			for (j=0; j<lNewWidth; j++)
			{
				// �����������ԴDIBͼ���е�����
				//j0=f(j);
			//	j0=j;
			//	j0=lWidth-j+m_y;
		    i0= m_x + (i-x_center)*cos(theta)-(j-y_center) *sin(theta)+ x_center;
			j0=m_y + (i-x_center) *sin(theta) +(j-y_center)* cos(theta)+ y_center;
			
				// �ж��Ƿ���Դͼ��Χ��
				if ( j0>=-1.0 && j0<(double)lWidth && i0>=-1.0 && i0<(double)lHeight )
				{
					LONG lTmpRGB=IF_BilinearInterpolation((LPSTR)bpDIBBits, bBitCount, lWidth, lHeight, j0, i0);
					bpDst[j*3+scBlue]=(BYTE)(lTmpRGB>>16);
					bpDst[j*3+scGreen]=(BYTE)((lTmpRGB&((LONG)255<<8))>>8);
					bpDst[j*3+scRed]=(BYTE)(lTmpRGB&(LONG)255);
				}
				else
				{
					// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪbwmDIBWhite
					bpDst[j*3+scBlue]=bpDst[j*3+scGreen]=bpDst[j*3+scRed]=(BYTE)255;
				
				}
			}
		}
	}
	
	return hDIB;
}


/*////////////////////////////////////////////////////////


  
	    �������
	  


*******************************************************/

BOOL JHJZProcess2(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight,int m_nh,int m_nb, int m_xb, int m_xh)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
    
//	srand(time(0)); 
	

	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��
	bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;
     
		for (j=0; j<lWidth; j++)
		{
     

 	if(m_xh==0)      //��������
 {
			if ( bBitCount==8 )
		
	  {


       int SJS1 = rand()%255;

	if(SJS1 < m_nb)                   //��������
			 
              bpSrc[j] = 255;
  else 
	     bpSrc[j]= bpSrc[j];
            
			 
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
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
			 	 

		if(m_xb==0)      //���������
 
 {
			
			if ( bBitCount==8 )
		
	  {


        int SJS1 = rand()%255;

			 if(SJS1 < m_nh)                 
			 
              bpSrc[j] = 0;
			 else 
       bpSrc[j] = bpSrc[j] ;
            //  bpDst[j] = 255;
			 
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
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
	


// ��г����ֵ���� 



BOOL NXBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// �ж���������Ƿ�����
		// �ж���������Ƿ�����
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// ģ�崦��

    //	int b0,b1,b2,b3,b4;
	double product1,product2;
    int n,m,Q=0; 
	for (i=0; i<lHeight-1; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // ͼƬ���
		{

		if ( bBitCount==8 )
			{
				for ( m=j-1; m<=j+1; m++)//����
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//��ĸ
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

				for ( m=j-1; m<=j+1; m++)//����
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scBlue]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//��ĸ
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n*3+scBlue]),Q);
					}
				}
				
           bpDst[j*3+scBlue]=int((product1/product2)); 

		   ////////////////////

		   				for ( m=j-1; m<=j+1; m++)//����
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scGreen]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//��ĸ
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
						product2=product2+pow(double(bpSrc[n*3+scGreen]),Q);
					}
				}
				
           bpDst[j*3+scGreen]=int ((product1/product2)); 
		   /////////////////////////
		   		for ( m=j-1; m<=j+1; m++)//����
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n*3+scRed]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//��ĸ
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

	// ���ƴ�����ͼ��
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;

}

// г����ֵ

BOOL  XBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	// �ж���������Ƿ�����
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// ģ�崦��

    //	int b0,b1,b2,b3,b4;
	double product1,product2;
    int n,m,Q= -1; 
	for (i=0; i<lHeight-1; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // ͼƬ���
		{


		if ( bBitCount==8 )
			{
				for ( m=j-1; m<=j+1; m++)//����
				{
					for (n=m-lLineBytes; n<=m+lLineBytes; n=n+lLineBytes)
					{
					 product1=product1+pow(double(bpSrc[n]),Q+1);

					}
				}

				for (m=j-1; m<=j+1; m++)//��ĸ
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

	// ���ƴ�����ͼ��
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}


////////////////////////////////////////////

//���⻯


/////////////////////////////////////////

BOOL JHHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
//	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
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
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
				a[bpSrc[j]]=a[bpSrc[j]]+1;
			}
			else
			{
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
					ave=int((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3+0.5);
					a[ave]=a[ave]+1;
			}
		}
	}

	H=lWidth*lHeight/255;

	//�����������ؽ��о��⻯
	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			sum=0;//ÿ�����ص����ǰ����
			if ( bBitCount==8 )
			{
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
				for (k=0; k<bpSrc[j]; k++)
				{
					sum=sum+a[k];
				}
				bpSrc[j]=int(sum/H+0.5);

			}
			else
			{
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
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

// ȡ��ɫ


///////////////////////////////////////

BOOL QFSProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ѭ������
	LONG i;
	LONG j;
	int k;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
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
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
				a[bpSrc[j]]=a[bpSrc[j]]+1;
			}
			else
			{
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
					ave=int((bpSrc[j*3+scBlue]+bpSrc[j*3+scGreen]+bpSrc[j*3+scRed])/3+0.5);
					a[ave]=a[ave]+1;
			}
		}
	}

	
	H=lWidth*lHeight/255;

	//�����������ؽ��о��⻯
	
	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				bpSrc[j]=255-bpSrc[j];// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
			}
			else
			{
				bpSrc[j*3+scBlue]=255-bpSrc[j*3+scBlue];
				bpSrc[j*3+scGreen]=255-bpSrc[j*3+scGreen];
				bpSrc[j*3+scRed]=255-bpSrc[j*3+scRed];

				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
			}
		}
	}

	return TRUE;
}


//  ��ֵ�� 

BOOL YZHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n)
{
	// �ж���������Ƿ�����
	if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));

	// ֱ��ͼ����

   
	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*i;

		for (j=0; j<lWidth; j++)
		{
			if ( bBitCount==8 )
			{
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
			
				if (bpSrc[j]<=m_n)
					bpSrc[j]=0;
			}
			else			
			{	
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
			
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

//�����븯ʴ

///////////////////////////


BOOL pengzhangProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	
		if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);

	
	for (i=0; i<lHeight-1; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);
		for (j=0; j<lWidth-1; j++)
		{
		if ( bBitCount==8 )
			{
			// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
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

		// ���ƴ�����ͼ��

	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




//��ֵ�˲�


BOOL zhongzhiProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight)
{
	
		if ( lpDIBBits==NULL || (bBitCount!=8&&bBitCount!=24) || lWidth<=0 || lHeight<=0 )
	{
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// ģ�崦��

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight-1; i++)
	{
		// ָ���е�����ָ��
		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth-1; j++)     // ͼƬ���
		{


			if ( bBitCount==8)
			{

				    b0=bpSrc[j];
					b1=bpSrc[j-1];
					b2=bpSrc[j+1];
					b3=bpSrc[j-lLineBytes];
					b4=bpSrc[j+lLineBytes];

        bpDst[j]=(bpSrc[j]-midd(b0,b1,b2,b3,b4));
		  
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
			}
			else
			{
				// ��24λɫλͼ����Ҫ�ֱ��bpSrc[j*3+scBlue]��bpSrc[j*3+scGreen]��bpSrc[j*3+scRed]���д���
 bpDst[j*3+scBlue] =(bpSrc[j*3+scBlue]- midd(bpSrc[j*3+scBlue],bpSrc[(j-1)*3+scBlue],bpSrc[(j+1)*3 +scBlue], bpSrc[j*3+scBlue-lLineBytes], bpSrc[j*3+scBlue+lLineBytes]));
 bpDst[j*3+scGreen] =(bpSrc[j*3+scGreen]- midd(bpSrc[j*3+scGreen],bpSrc[(j-1)*3+scGreen],bpSrc[(j+1)*3 +scGreen], bpSrc[j*3+scGreen-lLineBytes], bpSrc[j*3+scGreen+lLineBytes]));
 bpDst[j*3+scRed] =(bpSrc[j*3+scRed]-midd(bpSrc[j*3+scRed],bpSrc[(j-1)*3+scRed],bpSrc[(j+1)*3 +scRed], bpSrc[j*3+scRed-lLineBytes], bpSrc[j*3+scRed+lLineBytes]));

			}
		}
	}

	// ���ƴ�����ͼ��
	memcpy(lpDIBBits, bpNewDIBBits, lLineBytes*lHeight*sizeof(BYTE));
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}




//��Сֵ����

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
		AfxMessageBox("�����������", MB_OK);
		return FALSE;
	}

	// ָ��Դͼ���ָ��
	BYTE *bpSrc;
	
	// ָ��Ŀ��ͼ���ָ��
	BYTE *bpDst;
	
	// ָ����DIBͼ���ָ��
	BYTE *bpNewDIBBits;
	HLOCAL hNewDIBBits;
	
	// ѭ������
	LONG i;
	LONG j;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����DIBͼ��ÿ�е��ֽ���
	lLineBytes=(bBitCount==8?WIDTHBYTES(lWidth*8):WIDTHBYTES(lWidth*24));
	
	// ��ʱ�����ڴ棬�Ա�����DIBͼ��
	hNewDIBBits=LocalAlloc(LHND, lLineBytes*lHeight*sizeof(BYTE));
	

	// �ж��Ƿ��ڴ����ʧ��
	if ( hNewDIBBits==NULL )
	{
		// �����ڴ�ʧ��
		AfxMessageBox("�ڴ��������", MB_OK);
		return FALSE;
	}

	// �����ڴ�
	bpNewDIBBits=(BYTE*)LocalLock(hNewDIBBits);
   
	// ģ�崦��

	int b0,b1,b2,b3,b4;

	for (i=0; i<lHeight; i++)
	{
		// ָ���е�����ָ��

		bpSrc=(BYTE*)lpDIBBits+lLineBytes*(lHeight-1-i);
		bpDst=bpNewDIBBits+lLineBytes*(lHeight-1-i);

		for (j=0; j<lWidth; j++)
		{
			if (bBitCount==8 )
			{
				// ��256ɫλͼ����Ҫ��bpSrc[j]���д���
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
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	return TRUE;
}



	

