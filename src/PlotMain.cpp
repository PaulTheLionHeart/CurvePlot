/**************************************************************************
	PLOTMAIN.CPP
	Main graphics file decoder
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <fcntl.h>
#define  VIEWMAIN
#include "CurvePlot.h"
#include "resource.h"
#include "Dib.h"
							// routines in this module
void	putline(WORD, BYTE *);
void	ClearScreen(void);
void	GetRealClientRect (HWND, PRECT);
//void	SetScrollRanges(HWND);


HPALETTE 	hpal = NULL;

RECT 		WARect;					// this is the usable screen taking taskbar into account
double		ScreenRatio;				// ratio of width / height for the screen

int		height, xdots, ydots, width, bits_per_pixel;
WORD		NewXdots = 800, NewYdots = 450;		// for non standard image sizes
int		screenx, screeny;
int		scroll_width;				// size of horizontal scroll bars
RECT 		r;
PAINTSTRUCT 	ps;
int		display_width, display_height;
BOOL		NonStandardImage;			// has user changed image size?
int		caption;				// size of windows caption and scroll bars
CDib		Dib;					// Device Independent Bitmap

extern	POINT	ptSize;					// Stores DIB dimensions
extern	double	ViewLeft;				// horizontal address 
extern	double	ViewHeight;				// width of display 

extern	void	SetupView(HWND);
extern	void	ClosePtrs(void);

/**************************************************************************
	Main application view
**************************************************************************/

int	mainview(HWND hwnd, BOOL FileFlag)
    {
    const char	*szAppName = "Curve Plot";

    ScreenRatio = (double) width / (double) height;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &WARect, 0);	// let's get usable area including task bar

    screenx = WARect.right - WARect.left;
    screeny = WARect.bottom - WARect.top;
    bits_per_pixel = 24;
			// Some initial parameters
    if (NonStandardImage)
	{
	height = NewYdots;                  
	width = NewXdots;
	}
    else
	{
	width = WARect.right - WARect.left;
	height = WARect.bottom - WARect.top - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENU);
	}

    ScreenRatio = (double) width / (double) height;

    // Compute the size of the window rectangle based on the given
    // client rectangle size and the window style, then size the window.
         
    GetClientRect(hwnd, &r);
    ptSize.x = width;
    ptSize.y = height - GetSystemMetrics(SM_CYMENU);				    // remember status bar

//    if (IsZoomed (hwnd))
//	SetScrollRanges (hwnd);
    xdots = width;
    ydots = height - GetSystemMetrics(SM_CYMENU);				    // remember status bar
    ViewLeft = -(ViewHeight * ScreenRatio) / 2.0;
    SetupView(hwnd);

    ClosePtrs();							    // ready for next screen

    if (Dib.InitDib(width, height, bits_per_pixel) == NULL)
	{
	switch(Dib.DibErrorCode)
	    {
	    case NODIBMEMORY:
		MessageBox (hwnd, "Can't allocate DIB memory. Using windows default palette", szAppName, MB_ICONEXCLAMATION | MB_OK);
		break;
	    case NOPIXELMEMORY:
		MessageBox (hwnd, "Can't allocate memory for pixels", szAppName, MB_ICONEXCLAMATION | MB_OK);
		break;
	    }
	return -1;
	}
    return (TRUE);
    }

/**************************************************************************
	Clear Screen 
**************************************************************************/

void	ClearScreen(void)
    {
    memset(Dib.DibPixels, 0, WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel) * (DWORD)height);
    } 

/**************************************************************************
	Set line array to Dib memory
**************************************************************************/

void	putline(WORD row, BYTE *buf)
    {
    long	diff;
    long	local_width;
    DWORD	i;
    WORD	j;
    BYTE	c;

    if (bits_per_pixel == 24)				// Windows DIBs store 24-bit pixels in BGR order.
	for (j = 0; j < width; ++j)			// Convert incoming RGB data to BGR.
	    {
	    c = *(buf + j * 3);
	    *(buf + j * 3) = *(buf + j * 3 + 2);
	    *(buf + j * 3 + 2) = c;
	    }

    if (width > MAXHORIZONTAL)
	return;
    local_width = (WORD)(((DWORD) width * (DWORD)bits_per_pixel) / 8);
    if (row < height && row >= 0)
	{
	i = ((long) (height - 1 - row) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	if ((i + local_width) >> 16 != i >> 16)			// 64K boundary
	    {
	    diff = (i + local_width) & 0x0000ffff;
	    memcpy(Dib.DibPixels + i, buf, (size_t)(local_width - diff));
	    memcpy(Dib.DibPixels + i + local_width - diff, buf + local_width - diff, (size_t)diff);
	    }
	else
  	    memcpy(Dib.DibPixels + i, buf, (size_t)local_width);
	}
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : GetRealClientRect(HWND hwnd, LPRECT lprc)                  *
 *                                                                          *
 *  PURPOSE    : Calculates the client rectangle taking scrollbars into     *
 *               consideration.                                             *
 *                                                                          *
 ****************************************************************************/
void GetRealClientRect (HWND hwnd, PRECT lprc)
    {
    DWORD dwStyle;

    dwStyle = GetWindowLong (hwnd, GWL_STYLE);
    GetClientRect (hwnd,lprc);

    if (dwStyle & WS_HSCROLL)
        lprc->bottom += GetSystemMetrics (SM_CYHSCROLL);

    if (dwStyle & WS_VSCROLL)
        lprc->right  += GetSystemMetrics (SM_CXVSCROLL);
    }

/*-----------------------------------------
	Close all pointers
  -----------------------------------------*/

void	ClosePtrs(void)
    {
    Dib.CloseDibPtrs();
    }

/**************************************************************************
	Dialog Control for Image Size
**************************************************************************/

static	BOOL	MaintainAspect = TRUE;

INT_PTR CALLBACK ImageSizeDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     HWND		hCtrl ;
     BOOL		bTrans ;
     static	UINT	tempX; 
     static     UINT	tempY;

     switch (message)
	  {
	  case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_WIDTH, Dib.DibWidth, TRUE);
		SetDlgItemInt(hDlg, IDC_HEIGHT, Dib.DibHeight, TRUE);
		SetDlgItemInt(hDlg, IDC_X_SIZE, NewXdots, TRUE);
		SetDlgItemInt(hDlg, IDC_Y_SIZE, NewYdots, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT);
		SendMessage(hCtrl, BM_SETCHECK, MaintainAspect, 0L);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_X_SIZE:
			tempX = GetDlgItemInt(hDlg, IDC_X_SIZE, &bTrans, TRUE);
			if (MaintainAspect)
			    SetDlgItemInt(hDlg, IDC_Y_SIZE, (int)(((double) tempX + 0.5) / 1.7777777777), TRUE);
		        return TRUE ;

		    case IDC_Y_SIZE:
			tempY = GetDlgItemInt(hDlg, IDC_Y_SIZE, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_MAINTASPECT:
			hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT) ;
			MaintainAspect = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return TRUE ;

		    case IDC_FULLSCREEN:
			NonStandardImage = FALSE;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDOK:
			NewXdots = tempX;
			if (NewXdots < 10)
			    NewXdots = 10;
			if (NewXdots >= MAXHORIZONTAL)
			    NewXdots = MAXHORIZONTAL - 1;

			NewYdots = tempY;
			if (NewYdots < 10)
			    NewYdots = 10;
			if (NewYdots > 4000)
			    NewYdots = 4000;

			ScreenRatio = 16.0 / 9.0;			// force reasonable aspect ratio
			NonStandardImage = TRUE;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }


                