/*
    GRAPH.CPP a module to interface screen routines with pixels[].
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include "CurvePlot.h"
#include "Dib.h"

#define RGB_COMPONENTS 3

extern	BYTE	GraphPalette[];
extern	CDib	Dib;				// Device Independent Bitmap

void	(*plot)(WORD, WORD, DWORD);

/**************************************************************************
	Get values of r, g, b
**************************************************************************/

BYTE	*GetRGB(DWORD colour)

    {
    DWORD	j, c;
    static	BYTE	rgb[RGB_COMPONENTS];

    c = (colour < MAXPALETTE) ? colour : (colour % MAXPALETTE);			// threshold can be bigger than palette
    j = c + c + c;
    rgb[0] = *(GraphPalette + j + 2);
    rgb[1] = *(GraphPalette + j + 1);
    rgb[2] = *(GraphPalette + j + 0);
    return (rgb);
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	outpoint(WORD x, WORD y, DWORD colour)
    {
    long	i;
    long	local_width;

    if (colour > MAXPALETTE)				// don't write past end of palette
	colour = MAXPALETTE;
							// first do screen
    if (x < 0 || x >= width || y < 0 || y >= height)
	return;
    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((long) (height - 1 - y) * (long) (local_width + 3 - ((local_width - 1) % 4)) + (long)(x * 3));
	memcpy(Dib.DibPixels + i, GetRGB(colour), 3);
    }

/**************************************************************************
	Do correct put_line
**************************************************************************/

void	output_line(WORD x0, WORD line, WORD length, DWORD *buffer)

    {
    long	i;
    long	local_width;
    WORD	j;
    if (x0 < 0 || x0 >= width || line < 0 || line >= height - 1 || (x0 + length) < 0 || (x0 + length) > width)
	return;

    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((long) (height - 1 - line) * (long) (local_width + 3 - ((local_width - 1) % 4)) + (long)(x0 * 3));
    for (j = 0; j <= length; ++j) 
	{
	memcpy(Dib.DibPixels + i, GetRGB((DWORD) (*(buffer + j))), 3);
	i += 3L;
	}
    }

/**************************************************************************
	Plot a vertical line of same colour on 256 colour graphics card
**************************************************************************/

void	verline(WORD x0, WORD y0, WORD y1, DWORD colour)
    {
    WORD 	length, ylo, yhi;
    long	i;

    ylo = (y0 < y1) ? y0: y1;
    yhi = (y0 > y1) ? y0: y1;
    length = yhi - ylo;

    for (i = 0; i <= length; ++i)
	(*plot)((WORD)(ylo + i), x0, colour);
    return;
    }

/**************************************************************************
	Horizontal line
**************************************************************************/

void	HorizontalLine(WORD x0, WORD line, WORD length, DWORD colour)
    {
    long	i;
    long	local_width;
    WORD	j;
    if (x0 < 0 || x0 >= width || line < 0 || line >= height - 1 || (x0 + length) < 0 || (x0 + length) > width)
	return;

    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((long) (height - 1 - line) * (long) (local_width + 3 - ((local_width - 1) % 4)) + (long)(x0 * 3));
    for (j = 0; j <= length; ++j) 
	{
	memcpy(Dib.DibPixels + i, GetRGB(colour), 3);
	i += 3L;
	}
    }


