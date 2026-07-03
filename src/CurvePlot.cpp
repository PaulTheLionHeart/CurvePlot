/*-----------------------------------------
	Paul's Curve Plotter
  -----------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Dib.h"
#include "CurvePlot.h"
#include "resource.h"
#include "zoom.h"
#include "complex.h"

extern	CDib	Dib;				// Device Independent Bitmap

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM);
void	MenuCommand (HWND hwnd, WPARAM wParam);
void	DisplayLocation(HWND, POINTS);
void	ChangeView(HWND, int, int, int, int, int, int, int, int, char);

extern	HWND	GlobalHwnd;			// to allow passing of hwnd to find_file_item()
extern	BOOL	ZoomEdge;			// Zooming process
extern	RECT 	r;
extern	PAINTSTRUCT 	ps;
extern	HPALETTE 	hpal;
extern  int	caption;			// size of windows caption and scroll bars
extern  int	scroll_width;			// size of horizontal scroll bars
extern  int	display_width, display_height;
extern  int	screenx;
extern  int	screeny;
extern	double	ViewLeft;			// horizontal address
extern	double	ViewBottom;			// vertical address
extern	double	ViewHeight;			// width of display
extern	double	ScreenRatio;			// ratio of width / height for the screen

short	vscroll_count = 0;			// vertical scroll count
short	hscroll_count = 0;			// horizontal scroll count 
BOOL	bTrack = FALSE;				// TRUE if user is selecting a region
RECT	Rect;
int	Shape = SL_BLOCK;			// Shape to use for the selection rectangle
POINT	ptSize;					// Stores DIB dimensions

static	char    szTitleName[MAX_STATUS_TEXT] = "CurvePlot";
static	char	szFileName[MAX_PATH];
static	HINSTANCE  hInst;
static	HCURSOR  hCursor;
static	BOOL	DisplayLoc = FALSE;		// display the current cursor location in the caption bar
static	int	HorOffset, VertOffset;

extern	int	mainview(HWND, BOOL);
extern	void	InitSamples(void);
extern	int	CurvePlot(HWND);
extern	int	StartSelection(HWND, POINTS, LPRECT, int);
extern	int	UpdateSelection(HWND, POINTS, LPRECT, int);
extern	int	EndSelection(POINTS, LPRECT);
extern	int	ClearSelection(HWND, LPRECT, int);
extern	void	SetScrollRanges(HWND);
extern	int	ZoomIn(HWND, RECT *);			// zoom functions in Zoom.c
extern	int	ZoomOut(HWND, RECT *);
extern	void	OutputStatusBar(HWND);
extern	void	ClosePtrs(void);
extern	void	ProcessKeys(HWND, UINT);
extern	int	CopyPictureToClipboard(HWND hwnd);

extern	INT_PTR CALLBACK	ScrnFormDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK	ImageSizeDlg(HWND, UINT, WPARAM, LPARAM);

const DWORD WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
	WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME; 

/*-----------------------------------------
	Main Windows Entry Point
  -----------------------------------------*/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
    {
    HWND	hwnd;
    WNDCLASS	wndclass;
    MSG		msg;

    if (!hPrevInstance)
	{
	wndclass.style	 	= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon		= LoadIcon (hInstance, "CurvePlot");
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName	= "CurvePlot";
	wndclass.lpszClassName	= "CurvePlot";

	if (!RegisterClass (&wndclass))
	    return FALSE;
	}

    hwnd = CreateWindow ("CurvePlot", "Paul's Function Plot",
			  WindowStyle,
			  160, 120, 				// nice initial location
			  500, 300, 				// nice initial size
			  NULL, NULL, hInstance, NULL);
    GlobalHwnd = hwnd;

    strcpy(szTitleName, "CurvePlot");

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow (hwnd);

    while (GetMessage (&msg, NULL, 0, 0))
	{
	TranslateMessage (&msg);
	DispatchMessage (&msg);
	}

    return (int)msg.wParam;
    }

/*-----------------------------------------
	Just a quicky
  -----------------------------------------*/

void	DoCaption (HWND hwnd, char *szTitleName)
     {
     char szCaption [184];

     wsprintf (szCaption, "%s", (LPSTR) (szTitleName));
     SetWindowText (hwnd, szCaption);
     }

/*-----------------------------------------
	Let's Process a few messages
  -----------------------------------------*/

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
     {
     int            iMax;
     int            iMin;
     int            iPos;
     int            dn;
                   
     switch (message)
	  {
	  case WM_CREATE:
	        hInst = ((LPCREATESTRUCT)lParam)->hInstance;
	        //		lpfnAboutDlgProc = MakeProcInstance (AboutDlgProc, hInst);
		ZoomEdge = TRUE;
		vscroll_count = 0;				// no scrolling yet
		hscroll_count = 0;
		szFileName[0] = '\0';				// can't reopen if not there
		SetScrollPos(hwnd, SB_VERT, 0, FALSE);
		SetScrollPos(hwnd, SB_HORZ, 0, FALSE);
		mainview(hwnd, FALSE);
		Dib.ClearDib(255,255,255);
		InitSamples();
		if (DialogBox(hInst, "ScrnFormDlg", hwnd, ScrnFormDlg))
		    {
		    hCursor = LoadCursor(NULL, IDC_WAIT);
		    SetCursor(hCursor);
		    CurvePlot(hwnd);
		    SetCursor(hCursor);
		    InvalidateRect(hwnd, &r, FALSE);
		    }
		return 0;

	  case WM_INITMENUPOPUP:
	      return 0;

	  case WM_COMMAND:
                // Process menu commands
                MenuCommand (hwnd, wParam);
                break;

	  case WM_SIZE:
		InvalidateRect(hwnd, &r, FALSE);
		SetScrollRanges(hwnd);
		GetClientRect(hwnd, &r);
		xdots = width;
		ydots = height;
		return 0;

	  case WM_LBUTTONDOWN:				// message: left mouse button pressed
	  case WM_RBUTTONDOWN:				// message: right mouse button pressed
	      bTrack = TRUE;
	      SetRectEmpty(&Rect);
	      StartSelection(hwnd, MAKEPOINTS(lParam), &Rect, 
					(wParam & MK_SHIFT) ? (SL_EXTEND | Shape) : Shape);
	      break;

	  case WM_MOUSEMOVE:				// message: mouse movement
	      if (bTrack)
		  UpdateSelection(hwnd, MAKEPOINTS(lParam), &Rect, Shape);
	      break;

	  case WM_LBUTTONUP:				// message: left mouse button released
		if (bTrack) 
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (ZoomIn(hwnd, &Rect))
			{
			hCursor = LoadCursor(NULL, IDC_WAIT);
			SetCursor(hCursor);
			CurvePlot(hwnd);
			SetCursor(hCursor);
			InvalidateRect(hwnd, &r, FALSE);
			}
		    }
		return 0;

	  case WM_RBUTTONUP:				// message: left mouse button released
		if (bTrack) 
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (ZoomOut(hwnd, &Rect))
			{
			hCursor = LoadCursor(NULL, IDC_WAIT);
			SetCursor(hCursor);
			CurvePlot(hwnd);
			SetCursor(hCursor);
			InvalidateRect(hwnd, &r, FALSE);
			}
		    }
		return 0;

	  case WM_CLOSE:
	      if (MessageBox (hwnd, "Do you want to Quit?", "CurvePlot", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		  SendMessage (hwnd, WM_DESTROY, 0, 0L);
	      else
		  return 0;
	      return 0;

	  case WM_PAINT:
		ChangeView(hwnd, -GetScrollPos (hwnd, SB_HORZ), -GetScrollPos (hwnd, SB_VERT),
						ptSize.x, ptSize.y, 0, 0, width, height, TRUE);
		OutputStatusBar(hwnd);
		return 0;
        

          case WM_VSCROLL:
            						// Calculate new vertical scroll position 
            	GetScrollRange (hwnd, SB_VERT, &iMin, &iMax);
            	iPos = GetScrollPos (hwnd, SB_VERT);
            	GetClientRect (hwnd, &r);

	        switch ((int) LOWORD(wParam))
            	    {
                    case SB_LINEDOWN:
                        dn =  r.bottom / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -r.bottom / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn =  r.bottom / 2  + 1;
                        break;
                        
                    case SB_PAGEUP:
                        dn = -r.bottom / 2  + 1;
                        break;

                    case SB_THUMBTRACK:
                    case SB_THUMBPOSITION:
#ifdef	WIN95								// 32 bit code
                        dn = HIWORD(wParam)-iPos;
#else									// 16 bit code
                        dn = LOWORD(lParam)-iPos;
#endif
                        break;

                    default:               dn = 0;
	            }
							// Limit scrolling to current scroll range
                if (dn = BOUND (iPos + dn, iMin, iMax) - iPos) 
                    {
                    ScrollWindow (hwnd, 0, -dn, NULL, NULL);
                    SetScrollPos (hwnd, SB_VERT, iPos + dn, TRUE);
                    }
		InvalidateRect(hwnd, &r, FALSE);
                break;

          case WM_HSCROLL:
            						// Calculate new horizontal scroll position
                GetScrollRange (hwnd, SB_HORZ, &iMin, &iMax);
                iPos = GetScrollPos (hwnd, SB_HORZ);
                GetClientRect (hwnd, &r);

	        switch ((int) LOWORD(wParam))
                    {
                    case SB_LINEDOWN:
                        dn =  r.right / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -r.right / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn =  r.right / 2  + 1;
                        break;

                    case SB_PAGEUP:
                        dn = -r.right / 2  + 1;
                        break;

                    case SB_THUMBTRACK:
                    case SB_THUMBPOSITION:
#ifdef	WIN95								// 32 bit code
                        dn = HIWORD (wParam) -iPos;
#else									// 16 bit code
                        dn = LOWORD (lParam) - iPos;
#endif
                        break;

                    default:
                        dn = 0;
	            }
            						// Limit scrolling to current scroll range
                if (dn = BOUND (iPos + dn, iMin, iMax) - iPos) 
                    {
                    ScrollWindow (hwnd, -dn, 0, NULL, NULL);
                    SetScrollPos (hwnd, SB_HORZ, iPos + dn, TRUE);
                    }
		InvalidateRect(hwnd, &r, TRUE);
//		EndPaint(hwnd, &ps);
                break;

        case WM_KEYDOWN:					// Handle any keyboard messages
	    ProcessKeys(hwnd, (UINT)wParam);
	    break;

	case WM_DESTROY :
		ClosePtrs();
		PostQuitMessage (0);
	       	return 0;
	  }
     return DefWindowProc (hwnd, message, wParam, lParam);
     }	

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuCommand (HWND hwnd, WORD wParam)                      *
 *                                                                          *
 *  PURPOSE    : Processes menu commands.                                   *
 *                                                                          *
 *  RETURNS    : TRUE  - if command could be processed.                     *
 *               FALSE - otherwise                                          *
 *                                                                          *
 ****************************************************************************/

void	MenuCommand (HWND hwnd, WPARAM wParam)
    {
    switch (wParam)
	{
	case IDM_ABOUT:
	    DialogBox (hInst, "AboutBox", hwnd, AboutDlgProc);
	    return;

	case IDM_COPY:
	    CopyPictureToClipboard(hwnd);
	    break;

	case IDM_EXIT:
	    if (MessageBox (hwnd, "Do you want to Quit?", "CurvePlot", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		SendMessage (hwnd, WM_DESTROY, 0, 0L);
	    break;

			// Note that there is no 'break' as secondary process is still req
	case IDC_FRACTYPE:
	    if (DialogBox (hInst, "ScrnFormDlg", hwnd, ScrnFormDlg))
		{
		hCursor = LoadCursor(NULL, IDC_WAIT);
		SetCursor(hCursor);
		CurvePlot(hwnd);
		SetCursor(hCursor);
		InvalidateRect(hwnd, &r, FALSE);
		}
	    break;

	case IDM_IMAGE_SIZE:
	    if (DialogBox (hInst, "ImageSizeDlg", hwnd, ImageSizeDlg) == FALSE)
		return;
	    else
		{
		ClosePtrs();				// ready for next screen
		mainview(hwnd, FALSE);			// all screen specific stuff
		hCursor = LoadCursor(NULL, IDC_WAIT);
		SetCursor(hCursor);
		CurvePlot(hwnd);
		SetCursor(hCursor);
		InvalidateRect(hwnd, &r, FALSE);
		}
	    break;

	return;
	}

    return;
    }

/*-----------------------------------------
	Handle View Options
  -----------------------------------------*/

void	ChangeView(HWND hwnd, int xdest, int ydest, int widthdest, int heightdest,
			int xsrc, int ysrc, int widthsrc, int heightsrc, char stretchflag)
    { 
    HPALETTE hpalT;

    BeginPaint(hwnd, &ps);
    hpalT = SelectPalette (ps.hdc, hpal, FALSE);
    RealizePalette(ps.hdc);
    SetMapMode(ps.hdc,MM_TEXT);
    SetBkMode(ps.hdc,TRANSPARENT);
    SetStretchBltMode (ps.hdc, STRETCH_DELETESCANS) ;	// correct stretch mode for zoom etc


    StretchDIBits (ps.hdc, xdest, ydest, widthdest, heightdest, xsrc, ysrc, widthsrc, heightsrc,
			       (LPSTR)Dib.DibPixels, (LPBITMAPINFO)Dib.pDibInf, DIB_PAL_COLORS, SRCCOPY);
    EndPaint(hwnd, &ps);
    }

/*-----------------------------------------
	Setup View Options
  -----------------------------------------*/

void	SetupView(HWND hwnd)
    {
    static	BOOL	first = TRUE;				// start window maximised

    InvalidateRect(hwnd, &r, FALSE);
    caption = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYMENU) * 2;
    scroll_width = GetSystemMetrics(SM_CXFRAME) * 2;     
    
    display_width = ptSize.x + scroll_width;
    display_height = ptSize.y + caption;					// include caption
    display_width = (display_width < screenx) ? display_width : screenx;	// large pictures
    display_height = (display_height < screeny) ? display_height : screeny;
    MoveWindow (hwnd, (screenx - display_width) / 2, (screeny - display_height) / 2,
							    display_width, display_height, TRUE);

    HorOffset = (screenx - display_width) / 2 + GetSystemMetrics(SM_CXFRAME);
    VertOffset = (screeny - display_height) / 2 + GetSystemMetrics(SM_CYCAPTION) + 
				    GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYMENU);
    if (first)
	{
	ShowWindow (hwnd, SW_MAXIMIZE);
	first = FALSE;
	}
    }

/*-----------------------------------------------------------
	Setup Fractal defaults when new fractal selected
  -----------------------------------------------------------*/

void	FindCursorRealPos(POINTS *ptReal)

    {
    static	POINT	CursorLocLong;

    GetCursorPos(&CursorLocLong);
    ptReal->x = (short)CursorLocLong.x - HorOffset;
    ptReal->y = (short)CursorLocLong.y - VertOffset;
    }

/*-----------------------------------------
	About View
  -----------------------------------------*/

INT_PTR CALLBACK AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     switch (message)
	  {
	  case WM_INITDIALOG:
	       return TRUE;

	  case WM_COMMAND:
	       switch (wParam)
		    {
		    case IDOK:
			EndDialog (hDlg, TRUE);
			return TRUE;
		    }
	       break;
	  }
     return FALSE;
     }


