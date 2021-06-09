/**************************************************************************************************
 *                                                                                          
 * MODULE: SPLIT_CAT
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 09.05.98                                              
 *
 **************************************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "pbgh.h"
#include "protos.h"
#include "parser.h"

#include "gen.h"
#include "strutl.h"
#include "timm.h"
#include "mp.h"
#include "bch_charge.h"
#include "rtx.h"
#include "rtx_tree.h"


/*
 * EXTERN variables
 */

extern long folMapZNShdes2Code(char *);
extern long folMapTTShdes2Code(char *);
extern long folMapTMShdes2Code(char *);
extern long folMapSPShdes2Code(char *);
extern long folMapSNShdes2Code(char *);

extern char *fpsnzMapZNCode2Shdes(long);
extern char *fpsnzMapTTCode2Shdes(long);
extern char *fpsnzMapTMCode2Shdes(long);
extern char *fpsnzMapSPCode2Shdes(long);
extern char *fpsnzMapSNCode2Shdes(long);

extern struct s_group_22 *fpstCopyG22(struct s_group_22 *, char *, char *, int, double); 

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

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

extern tostRTX *gpstRTX;
extern long goilRTX_Length;

extern tostRTXTree gostRTXTree;

extern void print_g22(struct s_group_22 *);

/*
 * DATATYPE: PairSeq
 */

#define PAIR_SEQ_SIZE 16

typedef struct tostPairSeq
{
  long *soilFirst;
  long *soilSecond;
  int soiMaxSize;
  int soiSize;
}
tostPairSeq;

static toenBool foenPairSeq_Init(tostPairSeq *);
static toenBool foenPairSeq_Delete(tostPairSeq *);
static toenBool foenPairSeq_Add(tostPairSeq *, long, long);
static int foiPairSeq_Size(tostPairSeq *);
static long foilPairSeq_First(tostPairSeq *, int);
static long foilPairSeq_Second(tostPairSeq *, int);

extern void fovdPrintRTX(tostRTX *);

/***********************************************************************
 
 * FUNCTION: fpstSplitUsage

 * DESCRIPTION: For a given G22 item with Usage find it's fartition
                into TT and TZ if o EGLP was used by BCH in order 
                to create this item. 

 ***********************************************************************/

struct s_group_22 *fpstSplitUsage(
                                  long poilContractId,
                                  struct s_group_22 *ppstG22, 
                                  struct s_group_22 *ppstPrevG22, struct s_group_22 *ppstNextG22, 
                                  char *ppsnzArticleNo, char *ppsnzItemNo
                                  )
{
  struct s_group_22 *lpstG22, *lpstFirstG22, *lpstNewG22;
  long loilTMCode, loilTMVer, loilSPCode, loilSNCode, loilTTCode, loilZNCode, loilRIMVer, loilEGLVer;
  char lochUsageType;
  char *lpsnzShdes;
  char *lpsnzTTShdes, *lpsnzZNShdes;
  char lasnzKey[64];
  tostRTXArray *lpstRTXArray;
  double loflFlatValue;
  int i, j, loiSegs;
  toenBool loenStatus;
  int loiCalls, loiSize;
  tostRTX *lpstRTX;
  tostPairSeq lostPairSeq;

  /*
   * Tariff Model
   */

  lpsnzShdes = fpchzGetField(0, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return NULL;
    }

  loilTMCode = folMapTMShdes2Code(lpsnzShdes);

  /*
   * Tariff Model Version
   */

  lpsnzShdes = fpchzGetField(1, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return NULL;
    }
  
  loilTMVer = atol(lpsnzShdes);

  /*
   * Service Package
   */

  lpsnzShdes = fpchzGetField(2, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return NULL;
    }

  loilSPCode = folMapSPShdes2Code(lpsnzShdes);

  /*
   * Service Name
   */

  lpsnzShdes = fpchzGetField(3, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return NULL;
    }

  loilSNCode = folMapSNShdes2Code(lpsnzShdes);

  /*
   * Usage type : A, I -> L, C, R
   */

  if (ppsnzItemNo[0] == 'I')
    {
      lochUsageType = 'L';
    }
  else
    {
      lochUsageType = ppsnzItemNo[0];
    }

  /*
   * RIM Version
   */

  lpsnzShdes = fpchzGetField(2, ppsnzItemNo);
  if (lpsnzShdes == NULL)
    {
      return NULL;
    }

  loilRIMVer = atol(lpsnzShdes);

  /*
   * Find:
   * poilContractId,
   * postUI->solTMCODE, 
   * postUI->solVSCODE, 
   * postUI->solSNCODE,
   * postUI->sochType,
   * postUI->solRIVERSION,
   * postUI->solEGLVERSION,
   * postUI->solTTCODE,
   * postUI->solZNCODE,
   */

  sprintf(lasnzKey, "%ld.%ld.%ld.%ld.%c.%ld.%ld.%ld.%ld",
          poilContractId,
          loilTMCode, loilTMVer, loilSNCode, lochUsageType,
          loilRIMVer, 0,
          0, 0);

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Split G22: Looking for the key: %s\n", lasnzKey);
    }

  lpstRTXArray = fpstRTXTree_Search(&gostRTXTree, lasnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "Can't find the key: %s\n", lasnzKey);
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Split G22: Found calls: %d\n", lpstRTXArray->soiSize);
    }

  /*
   * Pair Seq of TT x ZN Init
   */

  loenStatus = foenPairSeq_Init(&lostPairSeq);
  if (loenStatus != TRUE)
    {
      return NULL;
    }
  
  /*
   * Filling sequences with unique codes of TT, ZN
   */

  for (i = 0; i < lpstRTXArray->soiSize; i++)
    {
      lpstRTX = lpstRTXArray->spstArray[i];
      if (goenVerbose == TRUE)
        {
          fovdPrintRTX(lpstRTX);
        }

      loenStatus = foenPairSeq_Add(&lostPairSeq, lpstRTX->ttcode, lpstRTX->tzcode);      
      if (loenStatus == FALSE)
        {
          return NULL;
        }
    }

  /*
   * Size of sequences for iteration
   */

  lpstG22 = NULL;
  loiSize = foiPairSeq_Size(&lostPairSeq);
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Split G22: Category set size: %d\n", loiSize);
    }

  loiSegs = 0;
  for (i = 0; i < loiSize; i++)
    {
      loilTTCode = foilPairSeq_First(&lostPairSeq, i);
      loilZNCode = foilPairSeq_Second(&lostPairSeq, i);          

      /*
       * Count number of calls in category extended with TT x ZN
       */
      
      loiCalls = 0;
      loflFlatValue = 0.0;
      for (j = 0; j < lpstRTXArray->soiSize; j++)
        {
          lpstRTX = lpstRTXArray->spstArray[j];
          if (lpstRTX->tzcode == loilZNCode && lpstRTX->ttcode == loilTTCode)
            {
              loflFlatValue = loflFlatValue + lpstRTX->rated_flat_amount;
              loiCalls++;
            }
        }

      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Split G22: Category: %ld, %ld calls no: %d of value: %lf\n", 
                  loilZNCode, loilTTCode, loiCalls, loflFlatValue);
        }

      if (loiCalls > 0)
        {
          /*
           * New category has to be created
           */
          
          lpsnzZNShdes = fpsnzMapZNCode2Shdes(loilZNCode);      
          lpsnzTTShdes = fpsnzMapTTCode2Shdes(loilTTCode);      
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "Split G22: New category with: %s %s\n", lpsnzTTShdes, lpsnzZNShdes);
            }
          
          lpstNewG22 = fpstCopyG22(ppstG22, lpsnzZNShdes, lpsnzTTShdes, loiCalls, loflFlatValue); 
          if (lpstNewG22 == NULL)
            {
              return NULL;
            }
          
          /*
           * Append G22 item to list of G22 items
           */
          
          loiSegs++;
          lpstNewG22->g_22_next = lpstG22;
          lpstG22 = lpstNewG22;
        }
    }
  
  loenStatus = foenPairSeq_Delete(&lostPairSeq);
  if (loenStatus != TRUE)
    {
      return NULL;
    }
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Split G22: New G22 segments: %d\n", loiSegs);
    }
  

  if (loiSegs > 0)
    {
      /*
       * New sequence of G22 segments was created
       */

      lpstFirstG22 = lpstG22;
      ppstPrevG22->g_22_next = lpstG22;
      while (lpstG22->g_22_next != NULL)
        {
          lpstG22 = lpstG22->g_22_next;
        }
      
      lpstG22->g_22_next = ppstNextG22;
    }
  else
    {
      return ppstG22;
    }

  /*
   * Return linked sequence of G22 blocks
   */

  return lpstFirstG22;
}

/*
 * DATATYPE: PairSeq
 *
 */

/*
 * METHOD: Init
 *
 */

static toenBool foenPairSeq_Init(tostPairSeq *ppstPairSeq) 
{
  ppstPairSeq->soilFirst = (long *)malloc(sizeof(long) * PAIR_SEQ_SIZE);
  if (ppstPairSeq->soilFirst == NULL)
    {
      return FALSE;
    }

  ppstPairSeq->soilSecond = (long *)malloc(sizeof(long) * PAIR_SEQ_SIZE);
  if (ppstPairSeq->soilSecond == NULL)
    {
      return FALSE;
    }

  ppstPairSeq->soiMaxSize = PAIR_SEQ_SIZE;
  ppstPairSeq->soiSize = 0;

  return TRUE;
}

/*
 * METHOD: Delete
 *
 */

static toenBool foenPairSeq_Delete(tostPairSeq *ppstPairSeq)
{
  free(ppstPairSeq->soilFirst);
  free(ppstPairSeq->soilSecond);

  return TRUE;
}

/*
 * METHOD: Add
 *
 */

static toenBool foenPairSeq_Add(tostPairSeq *ppstPairSeq, long poilFirst, long poilSecond) 
{
  int i;
  long *loilPtr;

  /*
   * Linear search for a pair
   */
  
  for (i = 0; i < ppstPairSeq->soiSize; i++)
    {
      if (ppstPairSeq->soilFirst[i] == poilFirst && ppstPairSeq->soilSecond[i] == poilSecond)
        {
          return TRUE;
        }
    }

  /*
   * Nothing was found
   */
  
  if (ppstPairSeq->soiSize == ppstPairSeq->soiMaxSize)
    {
      /*
       * Realloc memory
       */

      ppstPairSeq->soiMaxSize = ppstPairSeq->soiMaxSize * 2;

      /*
       * First seq
       */

      loilPtr = (long *)malloc(sizeof(long) * ppstPairSeq->soiMaxSize);
      if (loilPtr == NULL)
        {
          return FALSE;
        }
      
      memcpy(loilPtr, ppstPairSeq->soilFirst, ppstPairSeq->soiSize * sizeof(long));
      free(ppstPairSeq->soilFirst);
      ppstPairSeq->soilFirst = loilPtr;

      /*
       * Second seq
       */

      loilPtr = (long *)malloc(sizeof(long) * ppstPairSeq->soiMaxSize);
      if (loilPtr == NULL)
        {
          return FALSE;
        }
      
      memcpy(loilPtr, ppstPairSeq->soilSecond, ppstPairSeq->soiSize * sizeof(long));      
      free(ppstPairSeq->soilSecond);
      ppstPairSeq->soilSecond = loilPtr;
    }
  
  /*
   * Add element on the last position
   */

  ppstPairSeq->soilFirst[ppstPairSeq->soiSize] = poilFirst;
  ppstPairSeq->soilSecond[ppstPairSeq->soiSize] = poilSecond;
  
  ppstPairSeq->soiSize++;

  return TRUE;
}

/*
 * METHOD: Size
 *
 */

static int foiPairSeq_Size(tostPairSeq *ppstPairSeq) 
{
  return ppstPairSeq->soiSize;
}

/*
 * METHOD: First
 *
 */

static long foilPairSeq_First(tostPairSeq *ppstPairSeq, int poiIndex) 
{
  return ppstPairSeq->soilFirst[poiIndex];
}

/*
 * METHOD: Second
 *
 */

static long foilPairSeq_Second(tostPairSeq *ppstPairSeq, int poiIndex) 
{
  return ppstPairSeq->soilSecond[poiIndex];
}

  

