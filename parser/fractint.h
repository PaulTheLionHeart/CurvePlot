/* FRACTINT.H - common structures and values for the FRACTINT routines */

#ifndef FRACTINT_H
#define FRACTINT_H

#ifndef WIN32
typedef BYTE BOOLEAN;
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifndef C6
#define _fastcall       /* _fastcall is a Microsoft C6.00 extension */
#endif

#define MAXPARAMS 10            /* maximum number of parameters */
/*
 * Note: because non-MSDOS machines store structures differently, we have
 * to do special processing of the fractal_info structure in loadfile.c.
 * Make sure changes to the structure here get reflected there.
 */
#define FRACTAL_INFO_SIZE sizeof(FRACTAL_INFO)
#define ITEMNAMELEN 18   /* max length of names in .frm/.l/.ifs/.fc */
#define NUMGENES 21

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define MAXCALCWORK 12

#define MAX_JUMPS 200  /* size of JUMP_CONTROL array */

typedef struct frm_jmpptrs_st {
   int      JumpOpPtr;
   int      JumpLodPtr;
   int      JumpStoPtr;
} JUMP_PTRS_ST;

typedef struct frm_jump_st {
   int      type;
   JUMP_PTRS_ST ptrs;
   int      DestJumpIndex;
} JUMP_CONTROL_ST;

typedef char FCODE;
typedef FCODE * PFCODE;

#endif