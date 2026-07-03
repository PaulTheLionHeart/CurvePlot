#ifndef EXTERNS_H
#define EXTERNS_H

#ifndef DEBUG
#define DEBUG 1
#endif

#define LDBL	double
#define U16	unsigned short
#define U8	unsigned char
#define DBL_MIN	2.2250738585072014e-308

extern int                   debugflag;

extern char                  s_abs[];
extern char                  s_and[];
extern char                  s_acos[];
extern char                  s_acosh[];
extern char                  s_asin[];
extern char                  s_asinh[];
extern char                  s_atan[];
extern char                  s_atanh[];
extern char                  s_cabs[];
extern char                  s_ceil[];
extern char                  s_conj[];
extern char                  s_cosh[];
extern char                  s_cosxx[];
extern char                  s_cos[];
extern char                  s_cotanh[];
extern char                  s_cotan[];
extern char                  s_exp[];
extern char                  s_flip[];
extern char                  s_float[];
extern char                  s_floor[];
extern char                  s_fmod[];
extern char                  s_fn1[];
extern char                  s_fn2[];
extern char                  s_fn3[];
extern char                  s_fn4[];
extern char                  s_ident[];
extern char                  s_imag[];
extern char                  s_log[];
extern char                  s_real[];
extern char                  s_recip[];
extern char                  s_round[];
extern char                  s_sinh[];
extern char                  s_sin[];
extern char                  s_sqr[];
extern char                  s_sqrt[];
extern char                  s_srand[];
extern char                  s_sum[];
extern char                  s_tanh[];
extern char                  s_tan[];
extern char                  s_trunc[];
extern long                  total_formula_mem;
extern BYTE                  used_extra;
extern int                   xdots;
extern int                   ydots;
#endif