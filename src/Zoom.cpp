/****************************************************************************

    PROGRAM: Zoom.cpp (From Winfract V2.00)

    PURPOSE: Contains library routines for selecting a region

    FUNCTIONS:

	StartSelection(HWND, POINT, LPRECT, int) - begin selection area
	UpdateSelection(HWND, POINT, LPRECT, int) - update selection area
	EndSelection(POINT, LPRECT) - end selection area
	ClearSelection(HWND, LPRECT, int) - clear selection area

*******************************************************************************/

#include <windows.h>
#include "zoom.h"
#include "CurvePlot.h"

extern	double	ViewLeft;			// horizontal address
extern	double	ViewBottom;			// vertical address
extern	double	ViewHeight;			// width of display
extern	BOOL	bTrack;				// TRUE if user is selecting a region
extern	BOOL	ZoomEdge;			// Zooming process

extern	BOOL	NonStandardImage;

POINT	ZoomClick, ZoomCenter;
static	RECT	oldRect;

int	ClearSelection(HWND, LPRECT, int);

/****************************************************************************

    FUNCTION: StartSelection(HWND, POINT, LPRECT, int)

    PURPOSE: Begin selection of region

****************************************************************************/

int	StartSelection(HWND hWnd, POINTS ptCurrent, LPRECT lpSelectRect, int fFlags)
    {
    if (lpSelectRect->left != lpSelectRect->right || lpSelectRect->top != lpSelectRect->bottom)
	ClearSelection(hWnd, lpSelectRect, fFlags);

    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;

    // If you are extending the box, then invert the current rectangle

    if ((fFlags & SL_SPECIAL) == SL_EXTEND)
	ClearSelection(hWnd, lpSelectRect, fFlags);

    // Otherwise, set origin to current location

    else 
	{
	lpSelectRect->left = ptCurrent.x;
	lpSelectRect->top = ptCurrent.y;
	}

    if (!ZoomEdge)
	{
	ZoomCenter.x = ptCurrent.x;
	ZoomCenter.y = ptCurrent.y;
	oldRect.right = ptCurrent.x;
	oldRect.left = ptCurrent.x;
	oldRect.top = ptCurrent.y;
	oldRect.bottom = ptCurrent.y;
	}

    SetCapture(hWnd);
    return 0;
    }

/****************************************************************************

    FUNCTION: UpdateSelection(HWND, POINT, LPRECT, int) - update selection area

    PURPOSE: Update selection

****************************************************************************/

int	UpdateSelection(HWND hWnd, POINTS ptCurrent, LPRECT lpSelectRect, int fFlags)
    {
    HDC hDC;
    short OldROP;
    hDC = GetDC(hWnd);
    int	temp;

		// maintain aspect ratio by only using ptCurrent.y and calculating ptCurrent.x
    ptCurrent.x = (short)(lpSelectRect->left 
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)xdots / (float)ydots));

    if (ZoomEdge)
	{
	switch (fFlags & SL_TYPE) 
	    {
	    case SL_BOX:
		OldROP = SetROP2(hDC, R2_NOTXORPEN);
//		MoveTo(hDC, lpSelectRect->left, lpSelectRect->top);
		MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
		LineTo(hDC, ptCurrent.x, lpSelectRect->top);
		LineTo(hDC, ptCurrent.x, ptCurrent.y);
		LineTo(hDC, lpSelectRect->left, ptCurrent.y);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
		SetROP2(hDC, OldROP);
		break;
        
	    case SL_BLOCK:
		PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->bottom,
		    lpSelectRect->right - lpSelectRect->left,
		    ptCurrent.y - lpSelectRect->bottom,
		    DSTINVERT);
		PatBlt(hDC,
		    lpSelectRect->right,
		    lpSelectRect->top,
		    ptCurrent.x - lpSelectRect->right,
		    ptCurrent.y - lpSelectRect->top,
	  	    DSTINVERT);
		break;
	    }
	}
    else
	{
	temp = (int)(((float)ptCurrent.y - (float)ZoomCenter.y) / ((float)ydots / (float)xdots));
	lpSelectRect->left = ZoomCenter.x - temp;
	lpSelectRect->right = ZoomCenter.x + temp;
	lpSelectRect->top = ptCurrent.y - (ptCurrent.y - ZoomCenter.y) * 2;
	switch (fFlags & SL_TYPE) 
	    {
	    case SL_BOX:
		OldROP = SetROP2(hDC, R2_NOTXORPEN);
		MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);

		LineTo(hDC, oldRect.right, oldRect.top);
		LineTo(hDC, oldRect.right, oldRect.bottom);
		LineTo(hDC, oldRect.left, oldRect.bottom);
		LineTo(hDC, oldRect.left, oldRect.top);
		SetROP2(hDC, OldROP);
		break;
        
	    case SL_BLOCK:
		PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->bottom,
		    lpSelectRect->right - lpSelectRect->left,
		    lpSelectRect->top - lpSelectRect->bottom,
		    DSTINVERT);
		PatBlt(hDC,
		    oldRect.left,
		    oldRect.bottom,
		    oldRect.right - oldRect.left,
		    oldRect.top - oldRect.bottom,
		    DSTINVERT);
		break;
	    }

	oldRect.right = lpSelectRect->right;
	oldRect.left = lpSelectRect->left;
	oldRect.top = lpSelectRect->top;
	oldRect.bottom = lpSelectRect->bottom;
	}
    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;
    ReleaseDC(hWnd, hDC);
    return 0;
    }

/****************************************************************************

    FUNCTION: EndSelection(POINT, LPRECT)

    PURPOSE: End selection of region, release capture of mouse movement

****************************************************************************/

int	EndSelection(POINTS ptCurrent, LPRECT lpSelectRect)
    {
		// maintain aspect ratio by only using ptCurrent.y and calculating ptCurrent.x
    ptCurrent.x = (short)(lpSelectRect->left 
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)xdots / (float)ydots));

    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;
    ReleaseCapture();
    return 0;
    }

/****************************************************************************

    FUNCTION: ClearSelection(HWND, LPRECT, int) - clear selection area

    PURPOSE: Clear the current selection

****************************************************************************/

int	ClearSelection(HWND hWnd, LPRECT lpSelectRect, int fFlags)
    {
    HDC hDC;
    short OldROP;

    hDC = GetDC(hWnd);
    switch (fFlags & SL_TYPE) 
	{
	case SL_BOX:
	    OldROP = SetROP2(hDC, R2_NOTXORPEN);
	    MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
	    LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
	    LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
	    LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
	    LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
	    SetROP2(hDC, OldROP);
	    break;

	case SL_BLOCK:
	    PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->top,
		    lpSelectRect->right - lpSelectRect->left,
		    lpSelectRect->bottom - lpSelectRect->top,
		    DSTINVERT);
	    break;
	}
    ReleaseDC(hWnd, hDC);
    return 0;
    }

//*************************************************************************
//
//  FUNCTION   : NormalizeRect(RECT *prc)
//
//  PURPOSE    : If the rectangle coordinates are reversed, swaps them.
//               This is used to make sure that the first coordinate of
//               our rect is the upper left, and the second is lower right.
//
//*************************************************************************

// Macro to swap two values
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

void WINAPI NormalizeRect(RECT *prc)
    {
    if (prc->right  < prc->left) SWAP(prc->right,  prc->left);
    if (prc->bottom < prc->top)  SWAP(prc->bottom, prc->top);
    }

/****************************************************************************

    FUNCTION: ZoomIn(HWND, LPRECT, int)

    PURPOSE: Zoom In by setting horizontal, vertical and width

****************************************************************************/

int	ZoomIn(HWND hwnd, RECT *Rect)
    {
    double	hor_factor, vert_factor, width_factor;
    int		ytop, ybottom, xleft, xright;
    BYTE	WasBigNum = FALSE;
                       
							    // End the selection
    bTrack = FALSE;
    if (abs(Rect->bottom - Rect->top) <= 5 || abs(Rect->right  - Rect->left) <= 5)
	return FALSE; 

    NormalizeRect(Rect);
    ytop    = Rect->top;
    ybottom = Rect->bottom;
    xleft   = Rect->left;
    xright  = Rect->right;

    if (NonStandardImage)
	{
	ytop    += GetScrollPos(hwnd, SB_VERT);
	ybottom += GetScrollPos(hwnd, SB_VERT);
	xleft   += GetScrollPos(hwnd, SB_HORZ);
	xright  += GetScrollPos(hwnd, SB_HORZ);
	}
							    // adjust the zoom-box for aspect ratio
    hor_factor = (double) xleft / (double) ydots;
    vert_factor = (double) (ydots - ybottom) / (double) ydots;
    width_factor = (double) (abs(ybottom - ytop)) / (double) ydots;
    ViewLeft = ViewLeft + ViewHeight * hor_factor;
    ViewBottom = ViewBottom + ViewHeight * vert_factor;
    ViewHeight = ViewHeight * width_factor;
    return TRUE;
    }

/****************************************************************************

    FUNCTION: ZoomOut(HWND, LPRECT, int)

    PURPOSE: Zoom Out by setting horizontal, vertical and width

****************************************************************************/

int	ZoomOut(HWND hwnd, RECT *Rect)
    {
    double	half_win_aspect, width_factor;
    double	centrex, centrey;				// centre of window
    int	ytop, ybottom, xleft, xright;                                                         
    BYTE	WasBigNum = FALSE;

    bTrack = FALSE;
    if (abs(Rect->bottom - Rect->top ) <= 5 || abs(Rect->right  - Rect->left) <= 5)
	return FALSE; 
    NormalizeRect(Rect);
    ytop    = Rect->top;
    ybottom = Rect->bottom;
    xleft   = Rect->left;
    xright  = Rect->right;
						    // adjust the zoom-box for aspect ratio
    half_win_aspect = (double)ydots/(double)xdots * 0.5;
    width_factor = 1.0 / (double) (abs(ybottom - ytop)) * (double) ydots;

    centrex = ViewLeft + (ViewHeight * half_win_aspect);		// centre 
    centrey = ViewBottom + (ViewHeight * 0.5);
    ViewHeight = ViewHeight * width_factor;			// new width
    ViewLeft = centrex - ViewHeight * half_win_aspect;
    ViewBottom = centrey - ViewHeight * 0.5;
    return TRUE;
    }


