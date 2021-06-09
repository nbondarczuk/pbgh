/**************************************************************************************************
 *                                                                                          
 * MODULE: MC
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
#include "mc_tree.h"

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

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

extern stMPUFFTAB *pstMPUFFTAB;
extern long glFFCount;

extern tostRTXTree gostRTXTree;
extern void fovdPrintRTX(tostRTX *);

toenBool foenMC_Init()
{  
  return TRUE;
}

toenBool foenMC_Insert(long poilMCId, long poilConteractId, double poflScale)
{
  return TRUE;
}

toenBool foenMC_Splay(long poilContractId)
{  
  return TRUE;
}

int foiMC_MicroCellCount(long poilContractId)
{
  int i, n;

  n = 0;
  for (i = 0; i < glFFCount; i++)
    {
      if (pstMPUFFTAB[i].lCoId == poilContractId)
        {
          n++;
        }
    }

  return n;
}

double foflMC_MicroCellGetScale(long poilContractId, int poiMCSeqNo)
{
  int i, n;

  n = 0;
  for (i = 0; i < glFFCount; i++)
    {
      if (pstMPUFFTAB[i].lCoId == poilContractId)
        {
          return (1.0 - pstMPUFFTAB[i].fScaleFactor);
        }
    }

  return -1;
}

int foiMC_MicroCellGet(long poilContractId, int poiMCSeqNo)
{
  int i, n;

  n = 0;
  for (i = 0; i < glFFCount; i++)
    {
      if (pstMPUFFTAB[i].lCoId == poilContractId)
        {
          return pstMPUFFTAB[i].lCode;
        }
    }

  return -1;
}

toenBool foenMC_InternalCalls(
                              long poilContractId,
                              long poilMCCode,
                              char *spsnzKey,
                              int *ppiCalls, double *ppflFlatValue, int *ppiCallsTime
                              )
{
  int i;
  tostRTXArray *lpstRTXArray;
  tostRTX *lpstRTX;

  *ppiCalls = 0;
  *ppflFlatValue = 0.0;
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC Internal: Looking for the key: %s\n", spsnzKey);
    }

  lpstRTXArray = fpstRTXTree_Search(&gostRTXTree, spsnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "F&F MC Internal: Can't find the key: %s\n", spsnzKey);
      return FALSE;
    }
  
  for (i = 0; i < lpstRTXArray->soiSize; i++)
    {
      lpstRTX = lpstRTXArray->spstArray[i];      
      if (goenVerbose == TRUE)
        {
          fovdPrintRTX(lpstRTX);
        }

      if (lpstRTX->r_p_mcind = 1 && lpstRTX->r_p_mccode == poilMCCode)
        {
          *ppiCallsTime = *ppiCallsTime + lpstRTX->rounded_volume;
          *ppflFlatValue = *ppflFlatValue + lpstRTX->rated_flat_amount;
          *ppiCalls = *ppiCalls + 1;
        }
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC Internal: Found in MC: %ld internal calls: %d\n", poilMCCode, *ppiCalls);
    }

  return TRUE;
}

toenBool foenMC_ExternalCalls(
                              long poilContractId,
                              char *spsnzKey,
                              int *ppiCalls, double *ppflFlatValue, int *ppiCallsTime
                              )
{
  int i;
  tostRTXArray *lpstRTXArray;
  tostRTX *lpstRTX;

  *ppiCalls = 0;
  *ppflFlatValue = 0.0;
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC External: Looking for the key: %s\n", spsnzKey);
    }
  
  lpstRTXArray = fpstRTXTree_Search(&gostRTXTree, spsnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "F&F MC External: Can't find the key: %s\n", spsnzKey);
      return FALSE;
    }
  
  for (i = 0; i < lpstRTXArray->soiSize; i++)
    {
      lpstRTX = lpstRTXArray->spstArray[i];
      if (goenVerbose == TRUE)
        {
          fovdPrintRTX(lpstRTX);
        }
      
      if (lpstRTX->r_p_mcind != 1)
        {
          *ppiCallsTime = *ppiCallsTime + lpstRTX->rounded_volume;
          *ppflFlatValue = *ppflFlatValue + lpstRTX->rated_flat_amount;
          *ppiCalls =  *ppiCalls + 1;
        }
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC External: Found MC external calls: %d\n", *ppiCalls);
    }

  return TRUE;
}

  

/*
 * METHOD: MakeKey
 *
 */

toenBool foenMC_MakeKey(long poilContractId, char *ppsnzArticleNo, char *ppsnzItemNo, char *ppsnzKey)
{
  long loilTMCode, loilTMVer, loilSPCode, loilSNCode, loilTTCode, loilZNCode, loilRIMVer, loilEGLVer, loilMCCode;
  char *lpsnzTTShdes, *lpsnzZNShdes;  
  static char lasnzZNShdes[16], lasnzTTShdes[16]; 
  char *lpsnzShdes;
  char lochUsageType;

  /*
   * Tariff Model
   */

  lpsnzShdes = fpchzGetField(0, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }

  loilTMCode = folMapTMShdes2Code(lpsnzShdes);

  /*
   * Tariff Model Version
   */

  lpsnzShdes = fpchzGetField(1, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }
  
  loilTMVer = atol(lpsnzShdes);

  /*
   * Service Package
   */

  lpsnzShdes = fpchzGetField(2, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }

  loilSPCode = folMapSPShdes2Code(lpsnzShdes);

  /*
   * Service Name
   */

  lpsnzShdes = fpchzGetField(3, ppsnzArticleNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
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
      return FALSE;
    }

  loilRIMVer = atol(lpsnzShdes);

  /*
   * EGL
   */

  lpsnzShdes = fpchzGetField(3, ppsnzItemNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }

  loilEGLVer = atol(lpsnzShdes);

  /*
   * Tariff Time
   */

  lpsnzShdes = fpchzGetField(4, ppsnzItemNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }

  strcpy(lasnzTTShdes, lpsnzShdes); 
  loilTTCode = folMapTTShdes2Code(lpsnzShdes);

  /*
   * Tariff Zone
   */

  lpsnzShdes = fpchzGetField(5, ppsnzItemNo);
  if (lpsnzShdes == NULL)
    {
      return FALSE;
    }

  strcpy(lasnzZNShdes, lpsnzShdes); 
  loilZNCode = folMapZNShdes2Code(lpsnzShdes);

  sprintf(ppsnzKey, "%ld.%ld.%ld.%ld.%c.%ld.%ld.%ld.%ld",
          poilContractId,
          loilTMCode, loilTMVer, loilSNCode, lochUsageType,
          loilRIMVer, loilEGLVer,
          loilTTCode, loilZNCode);

  return TRUE;
}


