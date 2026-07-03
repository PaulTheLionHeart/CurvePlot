  /* see Fractint.c for a description of the "include"  hierarchy */
//#include "port.h"
#include "prototyp.h"

/* start of string literals cleanup */
char s_atan[]    = "atan";
char s_real[]    = "real";
char s_imag[]    = "imag";
char s_sin [] =             "sin";
char s_sinh [] =            "sinh";
char s_cos [] =             "cos";
char s_cosh [] =            "cosh";
char s_sqr [] =             "sqr";
char s_log [] =             "log";
char s_exp [] =             "exp";
char s_abs [] =             "abs";
char s_conj [] =            "conj";
char s_fn1 [] =             "fn1";
char s_fn2 [] =             "fn2";
char s_fn3 [] =             "fn3";
char s_fn4 [] =             "fn4";
char s_flip [] =            "flip";
char s_floor [] =           "floor";
char s_ceil [] =            "ceil";
char s_trunc [] =           "trunc";
char s_round [] =           "round";
char s_tan [] =             "tan";
char s_tanh [] =            "tanh";
char s_cotan [] =           "cotan";
char s_cotanh [] =          "cotanh";
char s_cosxx [] =           "cosxx";
char s_srand [] =           "srand";
char s_recip [] =           "recip";
char s_ident [] =           "ident";
char s_zero [] =            "zero";
char s_one  [] =            "one";
char s_asin [] =            "asin";
char s_asinh [] =           "asinh";
char s_acos [] =            "acos";
char s_acosh [] =           "acosh";
char s_atanh [] =           "atanh";
char s_cabs [] =            "cabs";
char s_sqrt [] =            "sqrt";
//char s_ismand [] =          "ismand";

//  The following Complex function routines added by Tim Wegner November 1994.

#define Sqrtz(z,rz) (*(rz) = ComplexSqrtFloat((z).x, (z).y))

/* rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)} */
void Arcsinz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX tempz1,tempz2;

  FPUcplxmul( &z, &z, &tempz1);
  tempz1.x = 1 - tempz1.x; tempz1.y = -tempz1.y;  /* tempz1 = 1 - tempz1 */
  Sqrtz( tempz1, &tempz1);

  tempz2.x = -z.y; tempz2.y = z.x;                /* tempz2 = i*z  */
  tempz1.x += tempz2.x;  tempz1.y += tempz2.y;    /* tempz1 += tempz2 */
  FPUcplxlog( &tempz1, &tempz1);
  rz->x = tempz1.y;  rz->y = -tempz1.x;           /* rz = (-i)*tempz1 */
}   /* end. Arcsinz */


/* rz=Arccos(z)=-i*Log{z+sqrt(z*z-1)} */
void Arccosz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX temp;

  FPUcplxmul( &z, &z, &temp);
  temp.x -= 1;                                 /* temp = temp - 1 */
  Sqrtz( temp, &temp);

  temp.x += z.x; temp.y += z.y;                /* temp = z + temp */

  FPUcplxlog( &temp, &temp);
  rz->x = temp.y;  rz->y = -temp.x;              /* rz = (-i)*tempz1 */
}   /* end. Arccosz */

void Arcsinhz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX temp;

  FPUcplxmul( &z, &z, &temp);
  temp.x += 1;                                 /* temp = temp + 1 */
  Sqrtz( temp, &temp);
  temp.x += z.x; temp.y += z.y;                /* temp = z + temp */
  FPUcplxlog( &temp, rz);
}  /* end. Arcsinhz */

/* rz=Arccosh(z)=Log(z+sqrt(z*z-1)} */
void Arccoshz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX tempz;
  FPUcplxmul( &z, &z, &tempz);
  tempz.x -= 1;                              /* tempz = tempz - 1 */
  Sqrtz( tempz, &tempz);
  tempz.x = z.x + tempz.x; tempz.y = z.y + tempz.y;  /* tempz = z + tempz */
  FPUcplxlog( &tempz, rz);
}   /* end. Arccoshz */

/* rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)} */
void Arctanhz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX temp0,temp1,temp2;

  if( z.x == 0.0){
    rz->x = 0;
    rz->y = atan( z.y);
    return;
  }
  else{
    if( fabs(z.x) == 1.0 && z.y == 0.0){
      return;
    }
    else if( fabs( z.x) < 1.0 && z.y == 0.0){
      rz->x = log((1+z.x)/(1-z.x))/2;
      rz->y = 0;
      return;
    }
    else{
      temp0.x = 1 + z.x; temp0.y = z.y;             /* temp0 = 1 + z */
      temp1.x = 1 - z.x; temp1.y = -z.y;            /* temp1 = 1 - z */
      FPUcplxdiv( &temp0, &temp1, &temp2);
      FPUcplxlog( &temp2, &temp2);
      rz->x = .5*temp2.x; rz->y = .5*temp2.y;       /* rz = .5*temp2 */
      return;
    }
  }
}   /* end. Arctanhz */

/* rz=Arctan(z)=i/2*Log{(1-i*z)/(1+i*z)} */
void Arctanz(_CMPLX z,_CMPLX *rz)
{
  _CMPLX temp0,temp1,temp2,temp3;
  if( z.x == 0.0 && z.y == 0.0)
    rz->x = rz->y = 0;
  else if( z.x != 0.0 && z.y == 0.0){
    rz->x = atan( z.x);
    rz->y = 0;
  }
  else if( z.x == 0.0 && z.y != 0.0){
    temp0.x = z.y;  temp0.y = 0.0;
    Arctanhz( temp0, &temp0);
    rz->x = -temp0.y; rz->y = temp0.x;              /* i*temp0 */
  }
  else if( z.x != 0.0 && z.y != 0.0){

    temp0.x = -z.y; temp0.y = z.x;                  /* i*z */
    temp1.x = 1 - temp0.x; temp1.y = -temp0.y;      /* temp1 = 1 - temp0 */
    temp2.x = 1 + temp0.x; temp2.y = temp0.y;       /* temp2 = 1 + temp0 */

    FPUcplxdiv( &temp1, &temp2, &temp3);
    FPUcplxlog( &temp3, &temp3);
    rz->x = -temp3.y*.5; rz->y = .5*temp3.x;           /* .5*i*temp0 */
  }
}   /* end. Arctanz */

_CMPLX ComplexSqrtFloat(double x, double y)
{
   double mag;
   double theta;
   _CMPLX  result;

   if(x == 0.0 && y == 0.0)
      result.x = result.y = 0.0;
   else
   {
      mag   = sqrt(sqrt(x*x + y*y));
      theta = atan2(y, x) / 2;
      FPUsincos(&theta, &result.y, &result.x);
      result.x *= mag;
      result.y *= mag;
   }
   return result;
}



