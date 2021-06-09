/**************************************************************************/
/*  MODULE : data generator                                               */
/*                                                                        */
/*  AUTHOR : N.Bondarczuk                                                 */
/*                                                                        */
/*  CREATION DATE : 25.10.97                                              */
/*                                                                        */
/*  DESCRIPTION : Generates output                                        */
/*                                                                        */
/**************************************************************************/

#include <stdio.h>

#include "pbgh.h"

#if 0	/* just for version.sh */
static char *SCCS_VERSION = "1.1";
#endif


#define EQ(a, b) !strcmp(a, b)
#define MAX_BUFFER 512
#define ERROR -1
#define EOS '\0'
#define DOT '.'

#define NOT(a) ((a) == FALSE)

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum toenBool {FALSE = 0, TRUE = 1} toenBool;

#define MAX_LINE 256
#define MAX_FILE 256
