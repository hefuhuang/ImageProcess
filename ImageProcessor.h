// ImageProcessor.h

/**************************************************************/
// DIB(Independent Bitmap) API函数库：
#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// 源通道
enum nSourceChannel
{
	scBlue	=0,
	scGreen	=1,
	scRed	=2,
};

// DIB句柄
DECLARE_HANDLE(HDIB);

// DIB文件头标志（字符串"BM"，写DIB时用到该常数）
#define DIB_HEADER_MARKER ((WORD)('M'<<8)|'B')

// 判断是否是Win 3.0的DIB
#define IS_WIN30_DIB(lpDIB) ((*(LPDWORD)(lpDIB))==sizeof(BITMAPINFOHEADER))

// 计算矩形区域的宽度
#define RECTWIDTH(lpRect) ((lpRect)->right-(lpRect)->left)

// 计算矩形区域的高度
#define RECTHEIGHT(lpRect) ((lpRect)->bottom-(lpRect)->top)

// 在计算图像大小时，采用公式：biSizeImage=biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

// 该函数将输入的双精度变量转换为BYTE型的变量
inline BYTE DoubleToByte(double d)
{
	if ( d<=0 )
	{
		return (BYTE)0;
	}
	else
	{
		if ( d>=254.5 )
		{
			return (BYTE)255;
		}
		else
		{
			return (BYTE)(d+0.5);
		}
	}
}

BOOL	PaintDIB(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect);

LPSTR	FindDIBBits(LPSTR lpDIB);

LPSTR	FindPalette(LPSTR lpDIB);

DWORD	DIBWidth(LPSTR lpDIB);

DWORD	DIBHeight(LPSTR lpDIB);

WORD	PaletteSize(LPSTR lpDIB);

WORD	DIBNumColors(LPSTR lpDIB);

BYTE	DIBBitCount(LPSTR lpDIB);

HGLOBAL	CopyHandle(HGLOBAL h);

BOOL	SaveDIB(HDIB hDib, CFile& file);

HDIB	ReadDIBFile(CFile& file);

inline	LONG	IF_BilinearInterpolation(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, double dX, double dY);

////////////////////////////////////////////////////////////////////

BOOL	HistogramProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight ,int m_n);

BOOL	TemplateProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);

// HGLOBAL	GeoProcess(LPSTR lpDIB);
HGLOBAL GeoProcess(LPSTR lpDIB, int m_x,int m_y,double theta);

BOOL   JHJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);    //几何均值滤波

BOOL JHJZProcess2(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight,int m_nh, int m_nb, int m_xb, int m_xh);    //噪声添加


BOOL  NXBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ;  //逆谐波均值处理  

BOOL  XBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);    //谐波均值处理 

BOOL JHHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //均衡化

BOOL QFSProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //取反色

BOOL YZHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n); //阈值化

BYTE midd(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);

BOOL pengzhangProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);  //膨胀与腐蚀

BOOL zhongzhiProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight); //中置滤波

BOOL ZUIXIAOProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //最小值滤波
BYTE minm(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);  //最小值处理



 
#endif //!_INC_DIBAPI
