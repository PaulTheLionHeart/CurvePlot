/*------------------------
   CurvePlot.H header file
  ------------------------*/

#include <Windows.h>

#define	MAXHORIZONTAL	10000
#define	VGA_PAL_SIZE	768
#define	EGA_PAL_SIZE	16
#define VGA_COLOURS	256

#define MAX_FORMULA_TEXT    1200
#define MAX_FORMULA_STRING  3600
#define MAX_STATUS_TEXT     1200
#define MAXPALETTE	255

#define	FixedGlobalAlloc(n) 	(char *)GlobalAlloc(GMEM_FIXED, n)
#define	FixedGlobalFree(p)	GlobalFree(p)
#define	FixedGlobalRealloc(p,n) GlobalRealloc(p,n)

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

// Macro to restrict a given value to an upper or lower boundary value
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

// Macro to determine to round off the given value to the closest byte
#define WIDTHBYTES(i)   ((i+31)/32*4)

extern	int	height, xdots, ydots, width, bits_per_pixel, planes;
extern	void	outpoint(WORD, WORD, DWORD);		// functions
extern	int	GenerateFormula(HWND hwnd);


