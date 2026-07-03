#ifndef PROTOTYP_H
#define PROTOTYP_H

/* includes needed to define the prototypes */

#include "mpmath.h"
#include "fractint.h"
#include "externs.h"
#ifdef WIN32
#include <windows.h>
#include <math.h>
#endif
#include <stdio.h>

/*  fpu087 -- assembler file prototypes */

extern void cdecl FPUcplxmul(_CMPLX *, _CMPLX *, _CMPLX *);
extern void cdecl FPUcplxdiv(_CMPLX *, _CMPLX *, _CMPLX *);
extern void cdecl FPUsincos(double *, double *, double *);
extern void cdecl FPUsinhcosh(double *, double *, double *);
extern void cdecl FPUcplxlog(_CMPLX *, _CMPLX *);
extern void cdecl SinCos086(long , long *, long *);
extern void cdecl SinhCosh086(long , long *, long *);

extern _CMPLX ComplexSqrtFloat(double, double);

extern void Arcsinz(_CMPLX z, _CMPLX *rz);
extern void Arccosz(_CMPLX z, _CMPLX *rz);
extern void Arcsinhz(_CMPLX z, _CMPLX *rz);
extern void Arccoshz(_CMPLX z, _CMPLX *rz);
extern void Arctanhz(_CMPLX z, _CMPLX *rz);
extern void Arctanz(_CMPLX z, _CMPLX *rz);

extern void dStkSqr(void );
extern void dStkSin(void );
extern void dStkTan(void );
extern void dStkTanh(void );
extern void dStkCoTan(void );
extern void dStkCoTanh(void );
extern void dStkRecip(void );
extern void dStkSinh(void );
extern void dStkCos(void );
extern void dStkCosXX(void );
extern void dStkCosh(void );

extern void free_workarea(void);
#endif
