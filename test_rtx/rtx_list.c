#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pbgh.h"
#include "bgh_esql.h"
#include "rtx.h"
#include "gen.h"
#include "rtx_tree.h"
#include "rtx_list.h"

static toenBool foenRTXSegment_Delete(tostRTXSegment *);
static tostRTXSegment *fpstRTXSegment_New(int, tostRTXSegment *);

/*
 *
 * DATATYPE: RTXList
 *
 */

/*
 * METHOD: Init
 *
 */

toenBool foenRTXList_Init(tostRTXList *ppstRTXList)
{
  ppstRTXList->spstRTXSegment = NULL;

  return TRUE;
}

/*
 * METHOD: NewSegment
 *
 */

tostRTX **fpstRTXList_NewSegment(tostRTXList *ppstRTXList, int poiSize)
{
  tostRTXSegment *lpstRTXSegment;
  
  lpstRTXSegment = fpstRTXSegment_New(poiSize, ppstRTXList->spstRTXSegment);
  if (lpstRTXSegment == NULL)
    {
      return NULL;
    }
  
  ppstRTXList->spstRTXSegment = lpstRTXSegment;
  
  return lpstRTXSegment->spstRTXArray;
}

/*
 * METHOD: Delete
 *
 */

toenBool foenRTXList_Delete(tostRTXList *ppstRTXList)
{
  tostRTXSegment *lpstRTXSegment;
  toenBool loenStatus;
  int i;
  
  lpstRTXSegment = ppstRTXList->spstRTXSegment;
  while (lpstRTXSegment)
    {
      loenStatus = foenRTXSegment_Delete(lpstRTXSegment);
      if (loenStatus != TRUE)
        {
          return FALSE;
        }

      lpstRTXSegment = lpstRTXSegment->spstRTXSegmentNext;
    }

  return TRUE;
}

/*
 *
 * DATATYPE: RTXSegment
 *
 */

/*
 * METHOD: Delete
 *
 */

static toenBool foenRTXSegment_Delete(tostRTXSegment *ppstRTXSegment)
{
  tostRTX **lpstRTX;
  int i;

  lpstRTX = ppstRTXSegment->spstRTXArray;
  for (i = 0; i < ppstRTXSegment->soiSize; i++)
    {
      free(lpstRTX[i]);
    }
      
  free(ppstRTXSegment->spstRTXArray);
  
  return TRUE;
}

/*
 * METHOD: New
 *
 */

static tostRTXSegment *fpstRTXSegment_New(int poiSize, tostRTXSegment *ppstRTXSegmentNext)
{
  tostRTX **lpstRTX;
  tostRTXSegment *lpstRTXSegment;
  int i;

  lpstRTX = (tostRTX **)malloc(sizeof(tostRTX *) * poiSize);
  if (lpstRTX == NULL)
    {
      return FALSE;
    }

  for (i = 0; i < poiSize; i++)
    {
      lpstRTX[i] = (tostRTX *)malloc(sizeof(tostRTX));
      if (lpstRTX[i] == NULL)
        {
          return NULL;
        }
    }

  lpstRTXSegment = (tostRTXSegment *)malloc(sizeof(tostRTXSegment));
  if (lpstRTXSegment == NULL)
    {
      return FALSE;
    }

  lpstRTXSegment->spstRTXArray = lpstRTX;
  lpstRTXSegment->spstRTXSegmentNext = ppstRTXSegmentNext;
  lpstRTXSegment->soiSize = poiSize;

  return lpstRTXSegment;
}
