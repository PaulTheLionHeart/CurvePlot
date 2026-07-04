// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Dib.h"
#define BITMAPCOMPRESSION(lpbi)     (*(LPDWORD)lpbi >= sizeof(BITMAPINFOHEADER) ? lpbi->biCompression : BI_RGB)
#define BITMAPREDMASK(lpbi)         ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4RedMask : 0)
#define BITMAPGREENMASK(lpbi)       ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4GreenMask : 0)
#define BITMAPBLUEMASK(lpbi)        ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4BlueMask : 0)

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((i+31)/32*4)
#define PIXELS2BYTES(n)	((n+7)/8)
#define	GREYVALUE(r,g,b) ((((r*30)/100) + ((g*59)/100) + ((b*11)/100)))
#define	ORTHOMATCH(r,g,b)  (((r) & 0x00e0) | (((g) >> 3) & 0x001c)  | (((b) >> 6) & 0x0003))

#define	LPBimage(lpbi)	((BYTE *)lpbi+lpbi->biSize+(long)(lpbi->biClrUsed*sizeof(RGBQUAD)))
#define	LPBwidth(lpbi)	(lpbi->biWidth)
#define	LPBdepth(lpbi)	(lpbi->biHeight)
#define	LPBbits(lpbi)   (lpbi->biBitCount)
#define	LPBcolours(lpbi) (lpbi->biClrUsed)
#define	LPBlinewidth(lpbi) (WIDTHBYTES((WORD)lpbi->biWidth*lpbi->biBitCount))

#define	LPBcolourmap(lpbi) (LPRGBQUAD)((BYTE *)lpbi+lpbi->biSize)
// Macro to return pointer to DIB pixels
#define DIBPEL(pDib) LPBimage(((LPBITMAPINFOHEADER)pDib))
// Macro to return pointer to DIB palette
#define DIBPAL(pDib) LPBcolourmap(((LPBITMAPINFOHEADER)pDib))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()			// device independent bitmap
    {
    DibPixels = NULL;
    pDibInf = NULL;
    DibErrorCode = 0;
    DibHeight = DibWidth = BitsPerPixel = 0;
    }

CDib::CDib(const CDib & Dib1)	// Copy Constructor
    {
    SizeImage = Dib1.SizeImage;
    DibHeight = Dib1.DibHeight;
    DibWidth = Dib1.DibWidth;
    DibSize = Dib1.DibSize;
    BitsPerPixel = Dib1.BitsPerPixel;
    DibErrorCode = 0;
    pDibInf = (LPBITMAPINFO)new BYTE[DibSize];
    if(pDibInf == NULL)
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
    memcpy(pDibInf, Dib1.pDibInf, DibSize);
    DibPixels = DIBPEL(pDibInf);
    }

CDib::~CDib()
    {
    CloseDibPtrs();
    }

CDib& CDib::operator=(const CDib & Dib1)	// Assignment Operator
    {
    if(this == & Dib1)	    return * this;	// Attempt to assign to self
    CloseDibPtrs();				// release memory for target DIB
    SizeImage = Dib1.SizeImage;
    DibHeight = Dib1.DibHeight;
    DibWidth = Dib1.DibWidth;
    DibSize = Dib1.DibSize;
    BitsPerPixel = Dib1.BitsPerPixel;
    DibErrorCode = 0;
    pDibInf = (LPBITMAPINFO)new BYTE[DibSize];
    if(pDibInf == NULL)
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
    memcpy(pDibInf, Dib1.pDibInf, DibSize);
    DibPixels = DIBPEL(pDibInf);

    return * this;
    }

//	Initialise DIB Palette (This is not a member of CDib)
void	SetDibPalette(LPBITMAPINFO pDib, BYTE *palette)
    {
    RGBQUAD *pRgb;
    WORD i, i3;
    if(palette != NULL)
	{
	pRgb = DIBPAL(pDib);
	for(i = i3 = 0; i < pDib->bmiHeader.biClrUsed; ++i, i3+=RGB_SIZE)
	    {
	    pRgb[i].rgbRed   = (char)palette[i3+RGB_RED];
	    pRgb[i].rgbGreen = (char)palette[i3+RGB_GREEN];
	    pRgb[i].rgbBlue  = (char)palette[i3+RGB_BLUE];
	    pRgb[i].rgbReserved = 0;
	    }
	}
    }

//	Round bit depths up to legal values for a bitmap
// IB 2009-05-28 add 16 bit (and tidy up)
static WORD	AdjustDIBits(WORD n)
    {
    if (n == 1)	return(1);
    if (n <= 4)	return(4);
    if (n <= 8)	return(8);
    if (n <= 16)	return(16);
    if (n <= 24)	return(24);
    if (n <= 32)	return(32);
	return(64);			// found 64 bit PNG files recently PHD 2009-05-16
    }

//	Create an empty bitmap (This is not a member of CDib)
//	NOTE it is the responsibility of the caller to release memory allocated by this function
//	NOTE does not currently handle 16 or 32 bit DIB (which require bit field masks for BI_BITFIELD)
LPBITMAPINFO	CreateEmptyDib(int Width, int Height, WORD bits)
    {
    LPBITMAPINFO	pDib;		// pointer to the DIB info
    bits = AdjustDIBits(bits);
    DWORD	colours_used = bits > 8 ? 0L : (DWORD)((1<<bits) & 0xffff);
    DWORD	SizeImage = WIDTHBYTES(Width * bits) * Height;	// Size, in bytes, of the image made of RGB triplets.
    DWORD	SizeCount = Width * Height;			// Size, in WORDS, of the count array.
    DWORD	DibSize = sizeof(BITMAPINFOHEADER) + colours_used*sizeof(RGBQUAD) + SizeImage + SizeCount;

    // allocate a device-independent bitmap header + palette + pixel array
    if(pDib = (LPBITMAPINFO)new BYTE[DibSize] )
	{
	// fill in the header
	pDib->bmiHeader.biSize	= (DWORD)sizeof(BITMAPINFOHEADER);
	pDib->bmiHeader.biWidth  	= (DWORD)Width;
	pDib->bmiHeader.biHeight 	= (DWORD)Height;
	pDib->bmiHeader.biPlanes 	= 1;
	pDib->bmiHeader.biBitCount 	= bits;
	pDib->bmiHeader.biCompression 	= BI_RGB;
	pDib->bmiHeader.biSizeImage	= SizeImage;	// Size, in bytes, of the image. May be set to zero for BI_RGB bitmaps.
	pDib->bmiHeader.biXPelsPerMeter = 0L;
	pDib->bmiHeader.biYPelsPerMeter = 0L;
	pDib->bmiHeader.biClrUsed 	= colours_used;
	pDib->bmiHeader.biClrImportant 	= 0;
	}
    else
	{
//	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
	return NULL;
	}
    return pDib;
    }

//	Create an empty bitmap, Initialise Palette (This is not a member of CDib)
//	NOTE it is the responsibility of the caller to release memory allocated by this function
LPBITMAPINFO	CreateEmptyDib(int Width, int Height, BYTE *palette, WORD bits)
    {
    // allocate a device-independent bitmap header + palette + pixel array
    LPBITMAPINFO pDib = CreateEmptyDib(Width, Height, bits);
    if(pDib == NULL )
	{
	return NULL;
	}
    SetDibPalette(pDib, palette);
    return pDib;
    }

//////////////////////////////////////////////////////////////////////
//	Initialise Windows Device Independent Bitmap
//////////////////////////////////////////////////////////////////////

unsigned char	*CDib::InitDib(int Width, int Height, WORD bits)
    {
    CloseDibPtrs();			// give back memory for new DIB

    // allocate a device-independent bitmap header + palette + pixel array
    pDibInf = CreateEmptyDib(Width, Height, bits);
    if(pDibInf == NULL )
	{
	DibErrorCode = NODIBMEMORY;
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
	return NULL;
	}

    // Initialise CDib members
    DibPixels = DIBPEL(pDibInf);
    SizeImage = pDibInf->bmiHeader.biSizeImage;	// Size, in bytes, of the image made of RGB triplets.
    DibHeight = Height;			// dimensions in pixels, useful in undo() and redo()
    DibWidth = Width;			// may be useful to replace these as globals in the future
    BitsPerPixel = pDibInf->bmiHeader.biBitCount ;
    DibSize = sizeof(BITMAPINFOHEADER) + pDibInf->bmiHeader.biClrUsed*sizeof(RGBQUAD) + SizeImage;
    WidthBytes = WIDTHBYTES(Width * bits);
    DibNumColours = 0L;
    memset(DibPixels, 0x55, SizeImage);	// this is code for debugging
    return DibPixels;
    }

//////////////////////////////////////////////////////////////////////
//	Close Dib Pointers
//////////////////////////////////////////////////////////////////////

void	CDib::CloseDibPtrs(void)
    {
    if (DibPixels)
	{
	DibPixels = NULL;
	}
    if (pDibInf)
	{
	delete [] pDibInf;
	pDibInf = NULL;
	}
    }

//////////////////////////////////////////////////////////////////////
//	Populate the count of colours in the image
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	hwnd;					// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////


void CDib::ClearDib(int Red, int Green, int Blue)
    {
    int		i, j;

    RGBTRIPLE *	Dest;
    DWORD	DestWidthBytes   = WIDTHBYTES((DWORD)DibWidth * 24L);

    switch(BitsPerPixel)
	{
	case 24:
	    for(i = 0; i < DibHeight; ++i)
		{
		Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
		for(j = 0; j < DibWidth; ++j)
		    {
		    Dest->rgbtBlue  = Blue;
		    Dest->rgbtGreen = Green;
		    Dest->rgbtRed   = Red;
		    Dest++;
		    }
		}
		break;
	default:
	    memset((BYTE *)DibPixels, 0, (size_t)SizeImage);	// if not 24 bit set to 0
	    break;
	}
    }


void CDib::ClearDib(DWORD colour)
    {
    ClearDib((colour>>16)&0xFF, (colour>>8)&0xFF, colour&0xFF); 
    }

// Insert text into 24 bit CDib at the location rect 

void	CDib::Text2Dib(HDC hDc, RECT *rect, TCHAR *text)
    {
    Text2Dib(hDc, rect, GetTextColor(hDc), GetBkColor(hDc), NULL, GetBkMode(hDc),text);
    }

void	CDib::Text2Dib(HDC hDc, RECT *rect, LOGFONT *lf, TCHAR *text)
    {
    Text2Dib(hDc, rect, GetTextColor(hDc), GetBkColor(hDc), lf, GetBkMode(hDc), text);
    }
void	CDib::Text2Dib(HDC hDc, RECT *rect, COLORREF FontColour, COLORREF BkgColour, LOGFONT *lf, int TransparentText, TCHAR *text)
    {
    HDC		hdc;
    HBITMAP	hBitmap;

    if ((hdc = CreateCompatibleDC(hDc)) == NULL)
	return;
    SetTextColor(hdc, FontColour);
    SetBkColor(hdc, BkgColour);
    SetBkMode(hdc, TransparentText);
    if (lf)
	SelectObject (hdc, CreateFontIndirect (lf)) ;
    hBitmap = CreateCompatibleBitmap (hDc, DibWidth, DibHeight);
    SelectObject (hdc, hBitmap);
    if (SetDIBits(hDc, hBitmap, 0, DibHeight, DibPixels, (LPBITMAPINFO)pDibInf, DIB_RGB_COLORS) == 0)
	return;
    DrawText(hdc, text, -1, rect, DT_CENTER);
    if (GetDIBits(hDc, hBitmap, 0, DibHeight, DibPixels, (LPBITMAPINFO)pDibInf, DIB_RGB_COLORS) == 0)
	return;
    DeleteObject(hBitmap);
    }

void	CDib::Rectangle2Dib(const RECT& rect, COLORREF colour)
    {
    int left = max(0, rect.left);
    int top = max(0, rect.top);
    int right = min((int)DibWidth, rect.right);
    int bottom = min((int)DibHeight, rect.bottom);

    if (BitsPerPixel != 24)
	return;
    RGBTRIPLE *Dest;
    DWORD DestWidthBytes = WIDTHBYTES((DWORD)DibWidth * 24L);

    RGBTRIPLE rgb;

    rgb.rgbtBlue = GetBValue(colour);
    rgb.rgbtGreen = GetGValue(colour);
    rgb.rgbtRed = GetRValue(colour);

    int rectHeight = bottom - top;

    Dest = (RGBTRIPLE *)(DibPixels + (DibHeight - rectHeight - top) * DestWidthBytes) + left;
    for (int y = top; y < bottom; y++)
	{
	RGBTRIPLE *p = Dest;

	Dest = (RGBTRIPLE *)(DibPixels + (DibHeight - 1 - y) * DestWidthBytes) + left;
	for (int x = left; x < right; x++)
	    *p++ = rgb;
	Dest += DestWidthBytes;
	}
    }
