// ImageProcessor.h

/**************************************************************/
// DIB(Independent Bitmap) API�����⣺
#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// Դͨ��
enum nSourceChannel
{
	scBlue	=0,
	scGreen	=1,
	scRed	=2,
};

// DIB���
DECLARE_HANDLE(HDIB);

// DIB�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
#define DIB_HEADER_MARKER ((WORD)('M'<<8)|'B')

// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpDIB) ((*(LPDWORD)(lpDIB))==sizeof(BITMAPINFOHEADER))

// �����������Ŀ��
#define RECTWIDTH(lpRect) ((lpRect)->right-(lpRect)->left)

// �����������ĸ߶�
#define RECTHEIGHT(lpRect) ((lpRect)->bottom-(lpRect)->top)

// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage=biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

// �ú����������˫���ȱ���ת��ΪBYTE�͵ı���
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

BOOL   JHJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);    //���ξ�ֵ�˲�

BOOL JHJZProcess2(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight,int m_nh, int m_nb, int m_xb, int m_xh);    //�������


BOOL  NXBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ;  //��г����ֵ����  

BOOL  XBJZProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);    //г����ֵ���� 

BOOL JHHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //���⻯

BOOL QFSProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //ȡ��ɫ

BOOL YZHProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight, int m_n); //��ֵ��

BYTE midd(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);

BOOL pengzhangProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight);  //�����븯ʴ

BOOL zhongzhiProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight); //�����˲�

BOOL ZUIXIAOProcess(LPSTR lpDIBBits, BYTE bBitCount, LONG lWidth, LONG lHeight) ; //��Сֵ�˲�
BYTE minm(BYTE a0,BYTE a1,BYTE a2,BYTE a3,BYTE a4);  //��Сֵ����



 
#endif //!_INC_DIBAPI
