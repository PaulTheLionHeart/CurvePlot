/*
 *  REMOVED FORMAL PARAMETERS FROM FUNCTION DEFINITIONS (1/4/92)
 */


#ifndef MPMATH_H
#define MPMATH_H

#ifndef _CMPLX_DEFINED
#include "cmplx.h"
#endif

/*** Formula Declarations ***/
enum MATH_TYPE { D_MATH};
extern enum MATH_TYPE MathType;

/* the following are declared 4 dimensional as an experiment */
/* changeing declarations to _CMPLX and _LCMPLX restores the code */
/* to 2D */
union Arg {
   _CMPLX     d;
};

struct ConstArg {
   char *s;
   size_t len;
   union Arg a;
};

extern union Arg *Arg1,*Arg2;

#endif

