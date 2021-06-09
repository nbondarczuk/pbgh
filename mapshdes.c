/**************************************************************************************************
 *                                                                                          
 * MODULE: SHDES2DES
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 30.04.98                                              
 *
 **************************************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "pbgh.h"
#include "protos.h"
#include "parser.h"

#include "gen.h"
#include "fnmatch.h"
#include "mp.h"

#if 0	/* just for version.sh */
static char *SCCS_VERSION = "4.0";
#endif

extern stMPUZNTAB *pstMPUZNTAB;
extern long glZNCount;

extern stMPUTTTAB *pstMPUTTTAB;
extern long glTTCount;

extern stMPUTMTAB *pstMPUTMTAB;
extern long glTMCount;

extern stMPUSPTAB *pstMPUSPTAB;
extern long glSPCount;

extern stMPUSNTAB *pstMPUSNTAB;
extern long glSNCount;

extern tostMPUSNTAB *gpstMPUSNTAB;
extern long golMPUSNTAB_Length; 

/****************************************************************************
 * FUNCTION: fpsnzMapTZShdes2Des
 *
 * DESCRIPTION: Mapping of TZ short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char *fpsnzMapZNShdes2Des(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glZNCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUZNTAB[i].szShdes))
        {
          return pstMPUZNTAB[i].szDes;
        }
    }

  return NULL;
}

long folMapZNShdes2Code(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glZNCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUZNTAB[i].szShdes))
        {
          return pstMPUZNTAB[i].lCode;
        }
    }

  return NULL;
}

char *fpsnzMapZNCode2Shdes(long polCode)
{
  int i;

  for (i = 0; i < glZNCount; i++)
    {
      if (polCode == pstMPUZNTAB[i].lCode)
        {
          return pstMPUZNTAB[i].szShdes;
        }
    }

  return NULL;
}

/****************************************************************************
 * FUNCTION: fpsnzMapTTShdes2Des
 *
 * DESCRIPTION: Mapping of TT short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char *fpsnzMapTTShdes2Des(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glTTCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUTTTAB[i].szShdes))
        {
          return pstMPUTTTAB[i].szDes;
        }
    }

  return NULL;
}

long folMapTTShdes2Code(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glTTCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUTTTAB[i].szShdes))
        {
          return pstMPUTTTAB[i].lCode;
        }
    }

  return NULL;
}

char *fpsnzMapTTCode2Shdes(long polCode)
{
  int i;

  for (i = 0; i < glZNCount; i++)
    {
      if (polCode == pstMPUTTTAB[i].lCode)
        {
          return pstMPUTTTAB[i].szShdes;
        }
    }

  return NULL;
}


/****************************************************************************
 * FUNCTION: fpsnzMapTMShdes2Des
 *
 * DESCRIPTION: Mapping of TM short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char *fpsnzMapTMShdes2Des(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glTMCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUTMTAB[i].szShdes))
        {
          return pstMPUTMTAB[i].szDes;
        }
    }

  return NULL;
}

long folMapTMShdes2Code(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glTMCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUTMTAB[i].szShdes))
        {
          return pstMPUTMTAB[i].lCode;
        }
    }

  return NULL;
}

char *fpsnzMapTMCode2Shdes(long polCode)
{
  int i;

  for (i = 0; i < glTMCount; i++)
    {
      if (polCode == pstMPUTMTAB[i].lCode)
        {
          return pstMPUTMTAB[i].szShdes;
        }
    }

  return NULL;
}

/****************************************************************************
 * FUNCTION: fpsnzMapSPShdes2Des
 *
 * DESCRIPTION: Mapping of TZ short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char *fpsnzMapSPShdes2Des(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glSPCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUSPTAB[i].szShdes))
        {
          return pstMPUSPTAB[i].szDes;
        }
    }

  return NULL;
}

long folMapSPShdes2Code(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glSPCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUSPTAB[i].szShdes))
        {
          return pstMPUSPTAB[i].lCode;
        }
    }

  return NULL;
}

char *fpsnzMapSPCode2Shdes(long polCode)
{
  int i;

  for (i = 0; i < glSPCount; i++)
    {
      if (polCode == pstMPUSPTAB[i].lCode)
        {
          return pstMPUSPTAB[i].szShdes;
        }
    }

  return NULL;
}


/****************************************************************************
 * FUNCTION: fpsnzMapSNShdes2Des
 *
 * DESCRIPTION: Mapping of SN short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char *fpsnzMapSNShdes2Des(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glSNCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUSNTAB[i].szShdes))
        {
          return pstMPUSNTAB[i].szDes;
        }
    }

  return NULL;
}

long folMapSNShdes2Code(char *pasnzShdes)
{
  int i;

  for (i = 0; i < glSNCount; i++)
    {
      if (EQ(pasnzShdes, pstMPUSNTAB[i].szShdes))
        {
          return pstMPUSNTAB[i].lCode;
        }
    }

  return NULL;
}

char *fpsnzMapSNCode2Shdes(long polCode)
{
  int i;

  for (i = 0; i < glSNCount; i++)
    {
      if (polCode == pstMPUSNTAB[i].lCode)
        {
          return pstMPUSNTAB[i].szShdes;
        }
    }

  return NULL;
}

/****************************************************************************
 * FUNCTION: fochMapSNShdes2SClass
 *
 * DESCRIPTION: Mapping of SN short description to full description
 *              Returned is pointer to string in global table.
 *
 ***************************************************************************/

char fochMapSNShdes2SClass(char *pasnzShdes)
{
  int i;
  char lochNetSnInd = '\0';

  for (i = 0; i < golMPUSNTAB_Length; i++)
    {
      if (!strcmp(gpstMPUSNTAB[i].sachzSHDES, pasnzShdes))
        {
          lochNetSnInd = gpstMPUSNTAB[i].sochNETSNIND; 
          break;
        }
    }
  
  return lochNetSnInd;
}




