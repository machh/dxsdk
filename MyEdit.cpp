// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "dd.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{

}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

WORD WINAPI CMyEdit::GetPaletteSize(LPSTR lpbi)
{
	DWORD dwClrUsed;
      
	dwClrUsed=((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
	if (dwClrUsed == 0)
		switch ( ((LPBITMAPINFOHEADER)lpbi)->biBitCount )
		{
		case 1:
			dwClrUsed=2;
			break;
		case 4:
			dwClrUsed=16;
			break;
		case 8:
			dwClrUsed=256;
			break;
		default:
			dwClrUsed=0;
			break;
		}
	return (WORD)(dwClrUsed * sizeof(RGBQUAD));
}

HANDLE WINAPI CMyEdit::BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
	ASSERT(hBitmap);
   
	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwLen;
	HANDLE hDib,h;
	HDC hDC;
	WORD biBits;
	UINT wLineLen;
	DWORD dwSize;
	DWORD wColSize;

	if (!hBitmap) return NULL;
	if (!::GetObject(hBitmap, sizeof(bm), &bm)) return NULL;

	if (hPal == NULL)
		hPal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);

	biBits = (WORD) (bm.bmPlanes * bm.bmBitsPixel);
	wLineLen = ( bm.bmWidth * biBits + 31 ) / 32 * 4;
	wColSize = sizeof(RGBQUAD) * (( biBits <= 8 ) ? 1 << biBits : 0 );
	dwSize = sizeof( BITMAPINFOHEADER ) + wColSize +
		(DWORD)(UINT)wLineLen * (DWORD)(UINT)bm.bmHeight;

	if (biBits <= 1)
		biBits = 1;
	else if (biBits <= 4)
		biBits = 4;
	else if (biBits <= 8)
		biBits = 8;
	else
      biBits = 24;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;	
	bi.biClrImportant = 0;

	dwLen = bi.biSize + GetPaletteSize((LPSTR) &bi);

	hDC = ::GetDC(NULL);
	hPal = ::SelectPalette(hDC, hPal, FALSE);
	::RealizePalette(hDC);

	hDib = (HANDLE)::GlobalAlloc(GHND, dwLen);
	if (!hDib)
	{
		::SelectPalette(hDC, hPal, TRUE);
		::RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
	if (!lpbi)
	{
		::SelectPalette(hDC, hPal, TRUE);
		::RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}
	*lpbi = bi;

	::GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, 
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	bi = *lpbi;
	bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;
	::GlobalUnlock(hDib);

	if (bi.biSizeImage == 0)
		bi.biSizeImage = ( ( ( (DWORD)bm.bmWidth * biBits ) + 31 ) / 32 * 4) * bm.bmHeight;

	dwLen = bi.biSize+GetPaletteSize((LPSTR)&bi)+bi.biSizeImage;
	h = (HANDLE)::GlobalReAlloc(hDib, dwLen, 0);
	if ( h )
	{
		hDib = h;
	}
	else
	{
		::GlobalFree(hDib);
		hDib = NULL;
		::SelectPalette(hDC, hPal, TRUE);
		::RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
	if (!lpbi)
	{
		::GlobalFree(hDib);
		hDib = NULL;
		::SelectPalette(hDC, hPal, TRUE);
		::RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	if (::GetDIBits(hDC,hBitmap,0,(WORD)bi.biHeight, 
		(LPSTR)lpbi + (WORD)lpbi->biSize + GetPaletteSize((LPSTR) lpbi),
		(LPBITMAPINFO)lpbi,DIB_RGB_COLORS) == 0)
   {
		::GlobalUnlock(hDib);
		hDib = NULL;
		::SelectPalette(hDC, hPal, TRUE);
		::RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}
   
	bi = *lpbi;

	::GlobalUnlock(hDib);
	::SelectPalette(hDC, hPal, TRUE);
	::RealizePalette(hDC);
	::ReleaseDC(NULL, hDC);

	return hDib;
}

HANDLE WINAPI CMyEdit::DIBFromWindow(CWnd *pWnd, CRect* pRect)
{
	CBitmap 	   bitmap;
	CWindowDC	dc(pWnd);
	CDC 		   memDC;
	CRect		   rect;
   
	memDC.CreateCompatibleDC(&dc); 
   
	if ( pRect == NULL )
	{
		pWnd->GetWindowRect(rect);
		rect.OffsetRect(-rect.left,-rect.top);
	}
	else
		rect = *pRect;
   
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height() );
   
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, rect.left, rect.top, SRCCOPY); 

	CPalette pal;
	if ( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize=sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP=(LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion=0x300;
		pLP->palNumEntries = (USHORT) GetSystemPaletteEntries( dc, 0, 255, pLP->palPalEntry );
		pal.CreatePalette( pLP );
		delete[] pLP;
	}
	memDC.SelectObject(pOldBitmap);
	HANDLE hDib = BitmapToDIB( bitmap, pal );
	return hDib;
}

BOOL WINAPI CMyEdit::SaveToFile(HANDLE hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr;
	LPBITMAPINFOHEADER lpBI;
	DWORD dwDIBSize;
   
	if (hDib == NULL)
		return FALSE;

	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	bmfHdr.bfType = ((WORD) ('M' << 8) | 'B');  // "BM"
   
	dwDIBSize = *(LPDWORD)lpBI + GetPaletteSize((LPSTR)lpBI);

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;
		dwBmBitsSize = ( ( (lpBI->biWidth)*((DWORD)lpBI->biBitCount) + 31) / 32 * 4) * lpBI->biHeight;
		dwDIBSize += dwBmBitsSize;
		lpBI->biSizeImage = dwBmBitsSize;
	}

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
		+ GetPaletteSize((LPSTR)lpBI);

	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	file.WriteHuge(lpBI, dwDIBSize);

	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}


void CMyEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->GetParent()->SetFocus();
	CFileDialog savePath(FALSE ,NULL,"save.bmp",OFN_HIDEREADONLY,"Bitmap Files(*.bmp)|*.bmp||",NULL);
	savePath.m_ofn.lpstrTitle="请选择保存路径并输入文件名";
	if(savePath.DoModal()==IDOK)
	{
		CString fullName;
		fullName=savePath.GetPathName();
		//获得屏幕窗口的对象句柄
		CWnd *pWnd=this;
		//对话框窗口最小化，使截到的屏幕不包含对话框窗口
		//将屏幕窗口拷贝到DIB位图中
		HANDLE hDib=DIBFromWindow(pWnd,NULL);  
		CFile file;
		file.Open(fullName,CFile::modeCreate|CFile::modeWrite);
		SaveToFile(hDib,file);//将位图存为指定名字的文件
		file.Close();
		GlobalFree(hDib);
	}
	CEdit::OnLButtonDblClk(nFlags, point);
}
