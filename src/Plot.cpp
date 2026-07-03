/*
    PLOT.CPP a program to plot functions, their derivatives and integrals.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.
*/

#include <stdio.h>
#include <string.h>
#include "CurvePlot.h"
#include "complex.h"
#include "resource.h"
#include "Dib.h"

#define	M   40

double	ViewLeft = -2.0;			// horizontal address 
double	ViewBottom = -2.0;			// vertical address 
double	ViewHeight = 4.0;			// width of display 
BOOL	ZoomEdge;				// Zooming process
char	FormulaString[MAX_FORMULA_STRING];	// used to hold the full formula
BOOL	PlotCurve = TRUE;			// What are we going to plot?
BOOL	PlotIntegral = TRUE;
BOOL	PlotDerivative = TRUE;

static	double	xscale, yscale;
static	char	FormulaText[MAX_FORMULA_TEXT] = "z*z";
static	char	szStatus[MAX_STATUS_TEXT] = "Formula f(z): Not specified yet";		// status bar text
static	int	TransparentText = TRANSPARENT;
static	double  magnitude = 1.0;
static	int	SampleCount = -1;

void	StatusBarInfo(void);
void	OutputStatusBar(HWND hwnd);

extern	int	ProcessFormulaString(char *FormulaString);
extern	INT_PTR CALLBACK	ScrnFormDlg(HWND, UINT, WPARAM, LPARAM);
extern	void	genline(int x0, int y0, int x1, int y1, DWORD col);
extern	int	mainview(HWND, BOOL);
extern	int	form_per_pixel(void); 
extern	int	Formula(void);
extern	int	ParseStr(char *Str, int pass);
extern	void	HorizontalLine(WORD x0, WORD line, WORD length, DWORD colour);

extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	PAINTSTRUCT 	ps;
extern	RECT 	r;
extern	int	col, row;
extern	char	ErrorMessage[];
extern	Complex	z, q, c;
extern	CDib	Dib;				// Device Independent Bitmap
extern	const	char	*Samples[];

/**************************************************************************
	Axis Plotting Routine
**************************************************************************/

void	PlotAxes(void)
    {
    int	    row, col, i, BigTick, LittleTick, TickPos;

    BigTick = xdots / 100;
    LittleTick = BigTick / 2;
    magnitude = 1.0;
    if (ViewHeight < 1.0)
	for (i = 0; magnitude > ViewHeight; i++)
	    magnitude /= sqrt(10.0);
    else
	for (i = 0; magnitude < ViewHeight; i++)
	    magnitude *= sqrt(10.0);

    magnitude /= 100.0;
    col = -(int)((ViewLeft * (double)xdots / (ViewHeight * ScreenRatio)));
    row = ydots + (int)((ViewBottom * (double)ydots / ViewHeight));
    if (ViewLeft < 0.0 && ViewLeft + (ViewHeight * ScreenRatio) > 0.0)	// y axis visible
	{
	genline(col, 0, col, ydots, 2);		// vertical
	for (i = 0; i < 100; i++)
	    {
	    TickPos = row - (int)(xscale * (double)i * magnitude);
	    HorizontalLine(col, TickPos, ((i % 10) ? LittleTick : BigTick), 2);
	    TickPos = row + (int)(xscale * (double)i * magnitude);
	    HorizontalLine(col, TickPos, ((i % 10) ? LittleTick : BigTick), 2);
	    }
	}
    if (ViewBottom < 0.0 && ViewBottom + ViewHeight > 0.0)// x axis visible
	{
	genline(0, row, xdots, row, 4);		// horizontal
	for (i = 0; i < 1000; i++)
	    {
	    TickPos = col + (int)(yscale * (double)i * magnitude);
	    genline(TickPos, row, TickPos, row - ((i % 10) ? LittleTick : BigTick), 4);
	    TickPos = col - (int)(yscale * (double)i * magnitude);
	    genline(TickPos, row, TickPos, row - ((i % 10) ? LittleTick : BigTick), 4);
	    }
	}
    }

/**************************************************************************
	Curve Plotting Routine
**************************************************************************/

int	CurvePlot(HWND hwnd)
    {
    int	    i, iZero, u;
//    int	    uOld, vOld, wOld;
    long    v, w, q;
    double  dx, dy, area, slope, oldFx = 0;

    if (ParseStr(FormulaString,1))
	return -1;			// parse failed, don't change fn pointers 
    xscale = (double) (xdots - 1) / (ViewHeight * ScreenRatio);
    yscale = (double) (ydots - 1) / ViewHeight;
    dx = (ViewHeight * ScreenRatio) / (double) xdots / M;
    dy = ViewHeight / (double) ydots;
    area = 0;
    iZero = - (int)(ViewLeft * xscale);
    Dib.ClearDib(255,255,255);
    PlotAxes();
    for (i = iZero * M; i < xdots * M; i++)
	{
	col = i / M;
	row = 0;
	z.x = (double)i / xscale / M + ViewLeft;
	z.y = 0.0;					// not ready for complex functions yet
//	c.x = 0.0;
//	c.y = 0.0;
	if (form_per_pixel())
	    Formula();
	else
	    break;
	area += z.x * dx;
	v = (long)((ViewBottom + ViewHeight - z.x) * yscale);
	w = (long)((ViewBottom + ViewHeight - area) * yscale);
	u = i / M;
	if (i > 0)
	    {
	    slope = (z.x - oldFx) / dx;
	    q = (long)((ViewBottom + ViewHeight - slope) * yscale);
	    if (v >= 0 && v < ydots && PlotCurve)
		outpoint((WORD)u, (WORD)v, 1);
	    if (w >= 0 && w < ydots && PlotIntegral) 
		outpoint((WORD)u, (WORD)w, 3);
	    if (q >= 0 && q < ydots && PlotDerivative) 
		outpoint((WORD)u, (WORD)q, 5);
	    }
	oldFx = z.x;
	}
    area = 0;
    for (i = iZero * M; i > 0; i--)
	{
	col = i / M;
	row = 0;
	z.x = (double)i / xscale / M + ViewLeft;
	z.y = 0.0;
	c.x = 0.0;
	c.y = 0.0;
	if (form_per_pixel())
	    Formula();
	else
	    break;
	area -= z.x * dx;
	v = (long)((ViewBottom + ViewHeight - z.x) * yscale);
	w = (long)((ViewBottom + ViewHeight - area) * yscale);
	u = i / M;
	if (i < iZero * M)
	    {
	    slope = (oldFx - z.x) / dx;
	    q = (long)((ViewBottom + ViewHeight - slope) * yscale);
	    if (v >= 0 && v < ydots && PlotCurve)
		outpoint((WORD)u, (WORD)v, 1);
	    if (w >= 0 && w < ydots && PlotIntegral) 
		outpoint((WORD)u, (WORD)w, 3);
	    if (q >= 0 && q < ydots && PlotDerivative) 
		outpoint((WORD)u, (WORD)q, 5);
	    }
	oldFx = z.x;
	}
    StatusBarInfo();
    OutputStatusBar(hwnd);
    return 0;
    } 

/**************************************************************************
	Generate  Status Info
**************************************************************************/

void	StatusBarInfo(void)
    {     
    char	PositionStr[3600];

    sprintf(PositionStr, "X=%10.10f,Y=%10.10f,Width=%10.10f", ViewLeft, ViewBottom, ViewHeight);
    sprintf(szStatus, "Formula f(z): %s, with screen dimensions: %s Magnitude = %f", FormulaText, PositionStr, magnitude*10.0);
    }

/**************************************************************************
	Status Bar generator
**************************************************************************/

void	OutputStatusBar(HWND hwnd)
    {
    HBRUSH  hbr;         // handle to brush
    RECT    StatusBarRect, LegendBarRect[4];
    DWORD   BackroundColour[4] = {0x00FFD0FF, 0x00FFFFD0, 0x00FFD0D0, 0x00C0FFFF};
    DWORD   TextColour[4] = {0x00C000C0, 0x00A0A000, 0x00FF0000, 0x00000000};
    const char    *Legend[4] = {"Derivative", "Integral", "Function", "Legend"};
    static  LOGFONT lf ;
    HDC	    hdc = GetDC(hwnd); 
    int	    i;

    GetClientRect (hwnd, &r);
    StatusBarRect.left = r.left;
    StatusBarRect.top = r.bottom - GetSystemMetrics(SM_CYMENU);// make status same size as menu
    StatusBarRect.right = r.right; 
    StatusBarRect.bottom = r.bottom;
    hbr = CreateSolidBrush(0x00C0FFFF);
    FillRect(ps.hdc, &StatusBarRect, hbr);
    DeleteObject(hbr);
    SetBkColor(ps.hdc, 0x00D0FFFF);
    DrawText(ps.hdc, szStatus, (int)strlen(szStatus), &StatusBarRect, DT_CENTER);  

    LegendBarRect[0].bottom = StatusBarRect.top;
    for (i = 0; i < 4; i++)
	{
	LegendBarRect[i].right = StatusBarRect.right; 
	LegendBarRect[i].left = StatusBarRect.right - 80;
	if (i > 0)
	    LegendBarRect[i].bottom = LegendBarRect[i - 1].top;
	LegendBarRect[i].top = LegendBarRect[i].bottom - GetSystemMetrics(SM_CYMENU);
	hbr = CreateSolidBrush(BackroundColour[i]);
	FillRect(ps.hdc, &LegendBarRect[i], hbr);
	DeleteObject(hbr);
	SetBkColor(ps.hdc, BackroundColour[i]);
	SetTextColor(ps.hdc, TextColour[i]);
	DrawText(ps.hdc, Legend[i], (int)strlen(Legend[i]), &LegendBarRect[i], DT_CENTER);
	}
    }

/**************************************************************************
	Count the number of samples
**************************************************************************/

void	InitSamples(void)
    {
    SampleCount = 0;
    while (Samples[SampleCount])
	SampleCount++;
    }

/**************************************************************************
	Dialog Box for loading functions
**************************************************************************/

INT_PTR CALLBACK ScrnFormDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	int	index = 4, i;
    static	char	Startup[720] = "z=c=pixel";
    char		*p;
    HWND		hCtrl; 

    switch (message) 
	{
        case WM_INITDIALOG:
            for (i = 0; i < SampleCount; i++) 
                SendDlgItemMessage(hDlg, IDC_FUNCTIONLIST, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(Samples[i]));
            SendDlgItemMessage(hDlg, IDC_FUNCTIONLIST, LB_SETCURSEL, (WPARAM)index, 0L);
	    hCtrl = GetDlgItem (hDlg, IDC_PLOTCURVE);
	    SendMessage(hCtrl, BM_SETCHECK, PlotCurve, 0L);
	    hCtrl = GetDlgItem (hDlg, IDC_PLOTINTEGRAL);
	    SendMessage(hCtrl, BM_SETCHECK, PlotIntegral, 0L);
	    hCtrl = GetDlgItem (hDlg, IDC_PLOTDERIVATIVE);
	    SendMessage(hCtrl, BM_SETCHECK, PlotDerivative, 0L);
	    SetDlgItemText(hDlg, IDC_FORMULAVALUE, FormulaText);
	    ViewLeft = -3.0;		// horizontal address
	    ViewBottom = -2.5;		// vertical address
	    ViewHeight = 5.0;		// width of display
            return (TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
		{
                case IDOK:
		    hCtrl = GetDlgItem (hDlg, IDC_PLOTCURVE);
		    PlotCurve = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    hCtrl = GetDlgItem (hDlg, IDC_PLOTINTEGRAL);
		    PlotIntegral = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    hCtrl = GetDlgItem (hDlg, IDC_PLOTDERIVATIVE);
		    PlotDerivative = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
                    index = (int)SendDlgItemMessage(hDlg, IDC_FUNCTIONLIST, LB_GETCURSEL, 0, 0L);
                    if (index == LB_ERR) 
			{
                        MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
                        break;
                        }
		    GetDlgItemText(hDlg, IDC_FORMULAVALUE, FormulaText, 2400);
		    sprintf(FormulaString, "\n%s:z = %s,|z| <= 4.0 \n}\n", Startup, FormulaText);
		    p = FormulaString;
		    while (*p++)		    // just for those who insist on using x instead of z
			{
			if (*p == 'x')
			    {
			    if (*(p+1) != 'p')	// exp?
				*p = 'z';
			    }
			}
		    if (ProcessFormulaString(FormulaString) == 0)
			EndDialog(hDlg, TRUE);
		    return (TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);

                case IDC_FUNCTIONLIST:
                    switch (HIWORD(wParam) & 0x0003) 
			{
                        case LBN_SELCHANGE:
                            index = (int)SendDlgItemMessage(hDlg, IDC_FUNCTIONLIST, LB_GETCURSEL, 0, 0L);
                            if (index == LB_ERR)
                                break;
                            break;
                         
                       case LBN_DBLCLK:
			    index = (int)SendDlgItemMessage(hDlg, IDC_FUNCTIONLIST, LB_GETCURSEL, 0, 0L);
			    if (index == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    }
		    SetDlgItemText(hDlg, IDC_FORMULAVALUE, Samples[index]);
		    break;

	    return (TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
	Dialog Box for Displaying Errors
**************************************************************************/

INT_PTR CALLBACK ErrorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    HFONT   hfnt = NULL;
    HFONT   hOldFont = NULL;
    HWND    hCtrl;
    char    m[10][40];
    char    *p;

    switch (message) 
	{
        case WM_INITDIALOG:
            return (TRUE);

	case WM_PAINT:
	    {
	    PAINTSTRUCT 	ps;
	    int			i, j;

	    hCtrl = GetDlgItem (hDlg, IDC_ERROR);
	    BeginPaint(hCtrl, &ps);
 	    hfnt = (HFONT)GetStockObject(ANSI_FIXED_FONT);	// fixed spacing allows error report to point to fault in the formula string
	    p = ErrorMessage;
	    i = 0;
	    j = 0;
	    while (*p++)			// create a separate string for each line of text
		{
		if (*p == '\n')
		    {
		    m[i][j] = '\0';
		    i++;
		    j = 0;
		    }
		else
		    m[i][j++] = *p;
		}

	    if (hOldFont = (HFONT)SelectObject(ps.hdc, hfnt)) 
		{ 
		for (j = 0; j <= i; j++)
		    TextOut(ps.hdc, 10, 16 * j, m[j], (int)strlen(m[j]));
		SelectObject(ps.hdc, hOldFont); 
		} 
	    EndPaint(hCtrl, &ps);
	    return TRUE ;
	    }

	case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
		{
                case IDOK:
		    EndDialog(hDlg, TRUE);
                    return (TRUE);
                  
	    return (TRUE);
	    }
	}
    return (FALSE);
    }

