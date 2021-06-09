/**************************************************************************************************
 *                                                                                          
 * MODULE: TIMM
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 08.01.98                                              
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

#define FMOA "%.2lf"

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

extern toenBool foenMpFillRatingIntervals(long, tostUsageItem *, tostRatingInterval *);
extern toenBool foenMpAddUnitPrice(struct s_group_22 *, long, tostUsageItem *, tostRatingInterval *);


static char *SCCS_VERSION = "1.0";


void fovdPrintVerInfoTimm (void)
{
  printf ("%s\t\t\t%s\n", __FILE__, SCCS_VERSION);
}



toenBool foenTIMMTypeCheck(struct s_TimmInter *pstTimmInter, char *pachzType)
{
  return EQ(pstTimmInter->timm->bgm->v_1000, pachzType);
}

char *fpchzGetField(int, char *);
struct s_imd_seg *fpstFindItemDescription(struct s_imd_seg *, char *);
struct s_qty_seg *fpstFindQuantity(struct s_qty_seg *, char *, char *);
struct s_moa_seg *fpstFindPaymentSegment(struct s_group_23 *, char *);
struct s_group_2 *fpstFindPartyBlock(struct s_group_2 *ppstG2, char *ppchzLabel) 
{
  struct s_group_2 *lpstG2;
  struct s_nad_seg *nad;

  lpstG2 = ppstG2;
  while (lpstG2) 
    {
      nad = lpstG2->nad;
      if (EQ(nad->v_3035, ppchzLabel))
        {
          return lpstG2;
        }
      lpstG2 = lpstG2->g_2_next;
    }
  
  return NULL;
}

struct s_moa_seg *fpstFindMainPaymentSegment(struct s_group_45 *ppstG45, char *ppchzType)
{
  struct s_moa_seg *lpstMoa;
  struct s_group_45 *lpstG45;
  
  lpstG45 = ppstG45;
  while (lpstG45)
    {
      lpstMoa = lpstG45->moa;
      if (EQ(lpstMoa->v_5025, ppchzType))
        {
          return lpstMoa;
        }
      lpstG45 = lpstG45->g_45_next;
    }

  return NULL;
}

struct s_moa_seg *fpstFindPaymentSegment(struct s_group_23 *ppstG23, char *ppchzType) 
{
  struct s_moa_seg *lpstMoa;
  struct s_group_23 *lpstG23;
  
  lpstG23 = ppstG23;
  while (lpstG23)
    {
      lpstMoa = lpstG23->moa;
      if (EQ(lpstMoa->v_5025, ppchzType))
        {
          return lpstMoa;
        }
      lpstG23 = lpstG23->g_23_next;
    }

  return NULL;
}

struct s_pri_seg *fpstNewFloatPrice(char *pachzTag, double poflValue, char *pachzCurrency)
{
  struct s_pri_seg *lpstPri;
  char lpchzBuf[16];

  lpstPri = (struct s_pri_seg *) malloc(sizeof(struct s_pri_seg));
  if (lpstPri == NULL)
    {      
      return NULL;
    }
  
  sprintf(lpchzBuf, FMOA, poflValue);
  strcpy(lpstPri->v_5125, pachzTag);
  strcpy(lpstPri->v_5118, lpchzBuf);
  strcpy(lpstPri->v_5375, pachzCurrency);

  return lpstPri;
}

struct s_group_25 *fpstUpdateG25List(struct s_group_25 **ppstG25, struct s_pri_seg **ppstNewPri) 
{
  struct s_group_25 *lpstG25, *lpstPrevG25;
  struct s_pri_seg *lpstPri;

  if (*ppstG25 == NULL)
    {
      lpstG25 = (struct s_group_25 *) malloc(sizeof(struct s_group_25));
      if (lpstG25 == NULL)
        {
          return NULL;
        }

      lpstG25->pri = *ppstNewPri;
      lpstG25->g_25_next = NULL;
      *ppstG25 = lpstG25;
      return lpstG25;
    }
  else
    {
      lpstG25 = *ppstG25;
      lpstPrevG25 = lpstG25;
      while (lpstG25)
        {
          lpstPri = lpstG25->pri;
          if (lpstPri == NULL)
            {
              return NULL;
            }

          if (EQ(lpstPri->v_5125, (*ppstNewPri)->v_5125))  
            {
              /*
               * Update container G25 
               */

              free(lpstG25->pri);
              lpstG25->pri = *ppstNewPri;              
              return lpstG25; 
            }

          lpstPrevG25 = lpstG25;
          lpstG25 = lpstG25->g_25_next;
        } 
      
      /*
       * Append new container with item pri
       */

      lpstG25 = (struct s_group_25 *) malloc(sizeof(struct s_group_25));
      if (lpstG25 == NULL)
        {
          return NULL;
        }

      lpstG25->pri = *ppstNewPri;
      lpstG25->g_25_next = NULL;
      lpstPrevG25->g_25_next = lpstG25;
    }

  return lpstG25;
}


struct s_imd_seg *fpstFindItemDescription(struct s_imd_seg *ppstImd, char *ppszDescr)
{
  struct s_imd_seg *lpstImd;

  lpstImd = ppstImd;
  while (lpstImd) 
    {
      if (EQ(lpstImd->v_7009, ppszDescr))
        {
          return lpstImd;
        }
      lpstImd = lpstImd->imd_next;
    }

  return NULL;
}

struct s_rff_seg *fpstFindReference(struct s_group_3 *ppstG3, char *ppszType) 
{
  struct s_group_3 *lpstG3;  
  struct s_rff_seg *lpstRff;

  lpstG3 = ppstG3;
  while (lpstG3) 
    {
      lpstRff = lpstG3->rff;
      if (EQ(lpstRff->v_1153, ppszType))
        {
          return lpstRff;
        }

      lpstG3 = lpstG3->g_3_next;
    }

  return NULL;
}


struct s_rff_seg *fpstFindItemReference(struct s_group_26 *ppstG26, char *ppszType)
{
  struct s_group_26 *lpstG26;  
  struct s_rff_seg *lpstRff;

  lpstG26 = ppstG26;
  while (lpstG26) 
    {
      lpstRff = lpstG26->rff;
      if (EQ(lpstRff->v_1153, ppszType))
        {
          return lpstRff;
        }

      lpstG26 = lpstG26->g_26_next;
    }

  return NULL;
}


struct s_qty_seg *fpstFindQuantity(struct s_qty_seg *ppstQty, char *ppchzDetails, char *ppchzUnitType) 
{
  struct s_qty_seg *lpstQty;

  lpstQty = ppstQty;
  while (lpstQty) 
    {
      if (EQ(lpstQty->v_6063, ppchzDetails) && EQ(lpstQty->v_6411, ppchzUnitType))
        {
          return lpstQty;
        }
      lpstQty = lpstQty->qty_next;
    }

  return NULL;
}

struct s_qty_seg *fpstNewFloatQuantity(char *ppchzDetails, char *ppchzUnitType, double poflVal) 
{
  struct s_qty_seg *lpstQty;

  lpstQty = (struct s_qty_seg *) malloc(sizeof(struct s_qty_seg));
  if (lpstQty == NULL)
    {
      return NULL;
    }

  strcpy(lpstQty->v_6063, ppchzDetails);
  sprintf(lpstQty->v_6060, FMOA, poflVal);
  strcpy(lpstQty->v_6411, ppchzUnitType);
  lpstQty->qty_next = NULL;

  return lpstQty;
}

struct s_qty_seg *fpstNewLongQuantity(char *ppchzDetails, char *ppchzUnitType, long polVal) 
{
  struct s_qty_seg *lpstQty;

  lpstQty = (struct s_qty_seg *) malloc(sizeof(struct s_qty_seg));
  if (lpstQty == NULL)
    {
      return NULL;
    }

  strcpy(lpstQty->v_6063, ppchzDetails);
  sprintf(lpstQty->v_6060, "%ld", polVal);
  strcpy(lpstQty->v_6411, ppchzUnitType);
  lpstQty->qty_next = NULL;

  return lpstQty;
}

void fovdUpdateQuantityList(struct s_qty_seg **ppstQty, struct s_qty_seg **ppstNewQty) 
{
  struct s_qty_seg *lpstQty, *lpstPrevQty;

  if (*ppstQty == NULL)
    {
      *ppstQty = *ppstQty;
    }
  else
    {
      lpstQty = *ppstQty;
      lpstPrevQty = lpstQty;
      while (lpstQty)
        {
          if (EQ(lpstQty->v_6063, (*ppstNewQty)->v_6063))  
            {
              /*
               * Update
               */

              (*ppstNewQty)->qty_next = lpstQty->qty_next; 
              lpstPrevQty->qty_next = *ppstNewQty;              
              free(lpstQty);
              return; 
            }
          
          lpstPrevQty = lpstQty;
          lpstQty = lpstQty->qty_next;
        } 
      
      /*
       * Append
       */
      
      lpstPrevQty->qty_next = *ppstNewQty; 
    }
}

char *fpchzGetField(int poiFieldNo, char *pochzField)
{
  static char lpchzOutput[MAX_BUFFER];
  char *lpchzToken;
  char lpchzField[MAX_BUFFER];
  int loiTokenNo;
  int loiFound;
  
  strncpy(lpchzField, pochzField, MAX_BUFFER);
  loiTokenNo = 0;

  loiFound = TRUE;
  lpchzToken = strtok(lpchzField, ".");
  if (poiFieldNo == 0)
    {
      strncpy(lpchzOutput, lpchzToken, MAX_BUFFER);
      return lpchzOutput;
    }

  while (loiTokenNo < poiFieldNo)
    {
      lpchzToken = strtok(NULL, ".");
      if (lpchzToken == NULL)
        {
          lpchzOutput[0] = '\0';
          loiFound = FALSE;
          break;
        }
      loiTokenNo++;
    }
  
  if (loiFound)
    {
      strncpy(lpchzOutput, lpchzToken, MAX_BUFFER);
    }

  return lpchzOutput;
}


struct s_xcd_seg *fpstFindXCDSegment(struct s_group_99 *ppstG99, int poiIndex)
{
  struct s_xcd_seg *lpstXcd;
  struct s_group_99 *lpstG99;
  int loiIndex;

  loiIndex = 0;
  lpstG99 = ppstG99;
  while (lpstG99) 
    {
      if (loiIndex == poiIndex) 
        {
          return lpstG99->xcd;
        }
      else
        {
          loiIndex++;
        }
      lpstG99 = lpstG99->g_99_next;
    }

  return NULL;
}


/**************************************************************************************************
 *                                                                                          
 * FUNCTION :  foiTimm_CountSubscribers             
 *                                                                                          
 * LOCAL FUNCTIONS USED:                                                                    
 *                                                                                          
 * EXTERNAL FUNCTIONS USED:                                                                 
 * MODULE: SUM_FETCH                                                                        
 *                                                                                          
 **************************************************************************************************
 */

int foiTimm_CountSubscribers(struct s_TimmInter *spstSumSheet) 
{
  int loiSubscribersNo;
  struct s_group_22 *g_22;             
  struct s_lin_seg *lin;

  g_22 = spstSumSheet->timm->g_22;
  loiSubscribersNo = 0;
  while (g_22) 
    {
      lin = g_22->lin;
      if (lin && lin->v_1222 && EQ(lin->v_1222, "01"))
        {
          loiSubscribersNo++;
        }
      g_22 = g_22->g_22_next;
    }  

  return loiSubscribersNo;
}



/**************************************************************************************************
 *                                                                                          
 * FUNCTION :  foiCountSubscriberContracts             
 *                                                                                          
 * LOCAL FUNCTIONS USED:                                                                    
 * struct s_group_22 *lpstFindSubscriberSegment(struct s_group_22 *, int);  
 *                                                                                          
 * EXTERNAL FUNCTIONS USED:                                                                 
 * MODULE: SUM_FETCH                                                                        
 *                                                                                          
 **************************************************************************************************
 */

int foiTimm_CountSubscriberContracts(struct s_TimmInter *spstSumSheet, int poiSubscriberNo) 
{
  struct s_group_22 *g_22;
  struct s_lin_seg *lin;  
  int loiContractsNo;

  g_22 = lpstFindSubscriberSegment(spstSumSheet->timm->g_22, poiSubscriberNo);  
  if (g_22 == NULL)
    {
      return 0;
    }
  
  loiContractsNo = 0;
  g_22 = g_22->g_22_next;
  while (g_22) 
    {
      lin = g_22->lin;
      if (lin && lin->v_1222 && EQ(lin->v_1222, "01")) 
        {
          break;
        }
      if (lin && lin->v_1222 && EQ(lin->v_1222, "02") && lin->v_7140 && NOT(EQ(lin->v_7140, "O")))
        {
          loiContractsNo++;
        }
      g_22 = g_22->g_22_next;
    }  
  
  return loiContractsNo;
}


struct s_group_22 *lpstFindSubscriberSegment(struct s_group_22 *ppstG22, int poiSegmentNo)
{
  struct s_group_22 *lpstG22;
  struct s_lin_seg *lpstLin;
  struct s_imd_seg *lpstImd;
  int loiSegmentNo;

  lpstG22 = ppstG22;
  loiSegmentNo = 0;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
      if (lpstLin && lpstLin->v_1222 && EQ(lpstLin->v_1222, "01"))
        {
          if (poiSegmentNo == loiSegmentNo)
            {
              return lpstG22;
            }
          else
            {
              loiSegmentNo++;
            }
        }

      lpstG22 = lpstG22->g_22_next;      
    }

  return NULL; 
}

struct s_group_22 *lpstFindSubscriberContractSegment(struct s_group_22 *ppstG22, int poiContractNo)
{
  struct s_group_22 *lpstG22;
  struct s_lin_seg *lpstLin;
  struct s_imd_seg *lpstImd;
  int loiContractNo;

  lpstG22 = ppstG22;
  loiContractNo = 0;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
      if (lpstLin && lpstLin->v_1222 && EQ(lpstLin->v_1222, "02") && NOT(EQ(lpstLin->v_7140, "O")))
        {
          if (poiContractNo == loiContractNo)
            {
              return lpstG22;
            }
          loiContractNo++;
        }
      if (lpstLin && lpstLin->v_1222 && EQ(lpstLin->v_1222, "01"))
        {
          return NULL;
        }

      lpstG22 = lpstG22->g_22_next;      
    }

  return NULL; 
}


/**************************************************************************************************
 *                                                                                          
 * FUNCTION :  foiCountSimCallRecords
 *                                                                                          
 * LOCAL FUNCTIONS USED:                                                                    
 * struct s_group_22 *lpstFindPaymentSegment(struct s_moa_seg *, char *);
 * struct s_group_22 *lpstFindChargeSegment(struct s_group_22, toenRecordType); 
 *
 * EXTERNAL FUNCTIONS USED:                                                                 
 * MODULE:
 *                                                                                          
 **************************************************************************************************
 */

int foiCountSimCallRecords(struct s_TimmInter *ppstSumSheet, int poiSubscriberNo, int poiContractNo) 
{
  struct s_group_22 *lpstG22;
  struct s_group_99 *lpstG99;  
  struct s_moa_seg *lpstMoa;
  struct s_lin_seg *lpstLin;
  struct s_xcd_seg *lpstXcd;
  int loiIndex;

  /*
   * G22 segment on level 02
   */

  lpstG22 = lpstFindSubscriberSegment(ppstSumSheet->timm->g_22, poiSubscriberNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }

  /*
   * G22 segment on level 02
   */
  
  lpstG22 = lpstFindSubscriberContractSegment(lpstG22->g_22_next, poiContractNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }


  /*
   * G22 segment on level 03 with 
   * - IMD block empty 
   * - MOA+903 in group G23
   * - RFF+IC in group G23
   * - G99 not empty
   * Let's find any block with G99 not equal NULL
   */

  lpstG99 = NULL;
  lpstG22 = lpstG22->g_22_next;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
      
      /*
       * Next contract or customer
       */
      if (EQ(lpstLin->v_1222, "02") || EQ(lpstLin->v_1222, "01"))
        {
          lpstG99 = NULL;
          break;
        }

      /*
       * Not broken loop so we are in a contract
       */
      if (lpstG22->g_99)
        {
          lpstG99 = lpstG22->g_99;
          break;
        }
      lpstG22 = lpstG22->g_22_next;
    }
  
  /*
   * Is it a correct G99 segment that was previously found for a given contract ?
   */

  if (lpstG99 != NULL)
    {
      loiIndex = 0;
      while (lpstG99) 
        {
          loiIndex++;
          lpstG99 = lpstG99->g_99_next;
        }
    }
  else
    {
      loiIndex = 0;
    }

  return loiIndex;
}

struct s_xcd_seg *lpstFindSimCallRecord(struct s_TimmInter *ppstSumSheet, int poiSubscriberNo, int poiContractNo, int poiCallNo) 
{

  struct s_group_22 *lpstG22;
  struct s_group_99 *lpstG99;  
  struct s_moa_seg *lpstMoa;
  struct s_lin_seg *lpstLin;
  struct s_xcd_seg *lpstXcd;
  int loiIndex;

  /*
   * G22 segment on level 02
   */


  lpstG22 = lpstFindSubscriberSegment(ppstSumSheet->timm->g_22, poiSubscriberNo);
  if (lpstG22 == NULL)
    {
      return NULL;
    }

  /*
   * G22 segment on level 02
   */
  
  lpstG22 = lpstFindSubscriberContractSegment(lpstG22->g_22_next, poiContractNo);
  if (lpstG22 == NULL)
    {
      return NULL;
    }

  /*
   * G22 segment on level 03 with 
   * - IMD block empty 
   * - MOA+903 in group G23
   * - RFF+IC in group G23
   * - G99 not empty
   * Let's find any block with G99 not equal NULL
   */

  lpstG99 = NULL;
  lpstG22 = lpstG22->g_22_next;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
      
      /*
       * Next contract or customer
       */
      if (EQ(lpstLin->v_1222, "02") || EQ(lpstLin->v_1222, "01"))
        {
          lpstG99 = NULL;
          break;
        }

      /*
       * Not broken loop so we are in a contract
       */
      if (lpstG22->g_99)
        {
          lpstG99 = lpstG22->g_99;
          break;
        }
      lpstG22 = lpstG22->g_22_next;
    }
  
  /*
   * Is it a correct G99 segment that was previously found for a given contract ?
   */

  if (lpstG99 != NULL)
    {
      loiIndex = 0;
      while (lpstG99) 
        {
          if (loiIndex == poiCallNo)
            {
              return lpstG99->xcd;
            }
          else
            {
              loiIndex++;
              lpstG99 = lpstG99->g_99_next;
            }
        }
    }

  return NULL;
}

int foiTimm_LoadContractNo(struct s_TimmInter *ppstSumSheet, int poiSubscriberNo, int poiContractNo)
{
  struct s_group_22 *lpstG22;
  struct s_group_99 *lpstG99;  
  struct s_moa_seg *lpstMoa;
  struct s_lin_seg *lpstLin;
  struct s_pia_seg *lpstPia;
  struct s_xcd_seg *lpstXcd;
  struct s_imd_seg *lpstImd;

  /*
   * G22 segment on level 02
   */

  lpstG22 = lpstFindSubscriberSegment(ppstSumSheet->timm->g_22, poiSubscriberNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }

  /*
   * G22 segment on level 02
   */
  
  lpstG22 = lpstFindSubscriberContractSegment(lpstG22->g_22_next, poiContractNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }

  lpstImd = fpstFindItemDescription(lpstG22->imd, "CO");  
  if (lpstImd == NULL)
    {
      return -1;
    }
  
  return atoi(lpstImd->v_7008a);
}

extern tostRTX *gpstRTX;
extern long goilRTX_Length;

extern toenBool foenTimmFillRatingIntervals(struct s_group_22 *, tostUsageItem *, tostRatingInterval *);
extern toenBool foenFindFlatPPU(tostUsageItem *, tostRatingInterval *);
extern toenBool foenIsEGLPUsed(char *ppchzItemNo);
extern void free_g22(struct s_group_22 *);
extern struct s_group_22 *fpstSplitUsage(long, struct s_group_22 *, struct s_group_22 *, struct s_group_22 *, char *, char *);

extern struct s_group_22 *fpstMCSplit_Usage(
                                            long,
                                            struct s_group_22 *,
                                            struct s_group_22 *, struct s_group_22 *, 
                                            char *, char *
                                            );

extern char fochMapSNShdes2SClass(char *);

toenBool foenTimm_ProcessUsageSegments(struct s_TimmInter *ppstSumSheet, int poiSubscriberNo, int poiContractNo)
{
  struct s_group_99 *lpstG99;  
  struct s_moa_seg *lpstMoa;
  struct s_lin_seg *lpstLin;
  struct s_pia_seg *lpstPia;
  struct s_imd_seg *lpstImd;
  struct s_xcd_seg *lpstXcd;
  int loiIndex, i;
  long loilContractId;
  char lochNetSnInd;
  char *lpchzArticleNo, *lpchzItemNo;
  toenBool loenStatus;
  tostRatingInterval lastRI[10];
  tostUsageItem lostUsageItem;
  tostEventPrice lostEP;
  struct s_group_22 *lpstG22, *lpstPrevG22, *lpstNewG22;

  /*
   * G22 segment on level 02
   */

  lpstG22 = lpstFindSubscriberSegment(ppstSumSheet->timm->g_22, poiSubscriberNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }

  /*
   * G22 segment on level 02
   */
  
  lpstG22 = lpstFindSubscriberContractSegment(lpstG22->g_22_next, poiContractNo);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }

  lpstImd = fpstFindItemDescription(lpstG22->imd, "CO");  
  if (lpstImd == NULL)
    {
      return FALSE;
    }
  
  loilContractId = atol(lpstImd->v_7008a);
  
  /*
   * G22 segment on level 03 with 
   * - IMD block not empty with U at the end
   */

  lpstPrevG22 = lpstG22;
  lpstG22 = lpstG22->g_22_next;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
     
      /*
       * Next contract or customer
       */

      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "LIN+%s+%s\n", lpstLin->v_1082, lpstLin->v_1222);
        }

      if (EQ(lpstLin->v_1222, "02") || EQ(lpstLin->v_1222, "01"))
        {
          break;
        }
      
      /*
       * Not broken loop so we are in a contract on level 04
       */

      if (EQ(lpstLin->v_1222, "04"))
        {
          lpchzArticleNo = lpstLin->v_7140; 
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "\n[%s] LIN.C212-7140: %s\n", lpstLin->v_1082, lpchzArticleNo);
            }

          if (lpchzArticleNo[strlen(lpchzArticleNo) - 1] == 'U'
#ifndef TEST              
              && lpstLin->v_7143[1] == 'A'
#endif
              )              
            {                      
              lpstPia = lpstG22->pia;
              lpchzItemNo = lpstPia->v_7140;
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "[%s] PIA.C212-7140: %s\n", lpstLin->v_1082, lpchzItemNo);
                }
              
              /*
               * Recode Article and Item
               */
              
              loenStatus = foenRecodeCharge(lpchzArticleNo, lpchzItemNo, &lostUsageItem,  lastRI, &lostEP);
              if (loenStatus == FALSE)
                {
                  printf("Can't recode charge\n");
                  return FALSE;
                }                              

              lochNetSnInd = fochMapSNShdes2SClass(lostUsageItem.sachzSNSHDES);
              switch (lpchzItemNo[2])
                {
                case 'F':
                  
                  /*
                   * FLAT rating model handling if no EGLP defined
                   */
                  
                  if (foenIsEGLPUsed(lpchzItemNo) == FALSE && lochNetSnInd == 'U')
                    {
                      if (goenVerbose == TRUE)
                        {
                          fprintf(stderr, "No EGLP used, splitting G22\n");
                        }
                      
                      /*
                       * Touch Processed indicator in G22 LIN block
                       */
                      
                      lpstG22->lin->v_7143[1] = 'P';
                      if (goenVerbose == TRUE)
                        {
                          fprintf(stderr, "G22 item signed\n");
                        }
                      
                      
                      lpstNewG22 = fpstSplitUsage(
                                                  loilContractId,
                                                  lpstG22, lpstPrevG22, lpstG22->g_22_next, 
                                                  lpchzArticleNo, lpchzItemNo
                                                  );
                      if (lpstNewG22 == NULL)
                        {
                          return FALSE;
                        }
                      
                      lpstG22 = lpstNewG22;
                      
                      lpstLin = lpstG22->lin;
                      lpchzArticleNo = lpstLin->v_7140; 
                      
                      lpstPia = lpstG22->pia;
                      lpchzItemNo = lpstPia->v_7140;
                      
                      if (goenVerbose == TRUE)
                        {
                          fprintf(stderr, "New [%s] PIA.C212-7140: %s\n", lpstLin->v_1082, lpchzItemNo);
                        }
                      

                      /*
                       * Recode Article and Item
                       */
                      
                      loenStatus = foenRecodeCharge(lpchzArticleNo, lpchzItemNo, &lostUsageItem,  lastRI, &lostEP);
                      if (loenStatus == FALSE)
                        {
                          printf("Can't recode charge\n");
                          return FALSE;
                        }                              
                    }
                  
                                  
                  /*
                   * Change whole group G22 for EVENTS or USAGE 
                   */            
                  
                  switch(lochNetSnInd)
                    {
                    case 'E':
                      
                      /*
                       * Unit price for event service
                       */

                      loenStatus = foenMpG22AddEventPrice(lpstG22, loilContractId, &lostUsageItem, &lostEP);
                      if (loenStatus == FALSE)
                        {
                          printf("Can't find event price\n");
                          return FALSE;
                        }
                      
                      break;
                      
                    case 'U':
                      
                      /*
                       * Unit price for usage
                       */

                      loenStatus = foenMpG22AddUsagePrice(lpstG22, loilContractId, &lostUsageItem, lastRI);
                      if (loenStatus == FALSE)
                        {
                          printf("Can't find usage price\n");
                          return FALSE;
                        }

                      /*
                       * Split G22 item by MC (if any found) using scaling factor
                       */
                      
                      lpstNewG22 = fpstMCSplit_Usage(loilContractId, lpstG22, lpstPrevG22, lpstG22->g_22_next, lpchzArticleNo, lpchzItemNo); 
                      if (lpstNewG22 == NULL)
                        {
                          printf("Can't split by MC usage\n");
                          return FALSE;
                        }                 

                      lpstG22 = lpstNewG22;
                      
                      if (goenVerbose == TRUE)
                        {
                          fprintf(stderr, "G22 item splitted\n");
                        }

                      break;
                      
                    default:
                      printf("Can't process service type: %c\n", lochNetSnInd);
                      return FALSE;
                    }
                                    
                  break;
                  
                default:
                  printf("No support for rating model: %c\n", lpchzItemNo[2]);
                  break;
                }
            }
        }

      /*
       * Next block G22
       */
          
      lpstPrevG22 = lpstG22;
      lpstG22 = lpstG22->g_22_next;
    } /* while(lpstG22) */
       
  /* 
   *   OK 
   */

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "G22 list processed\n");
    }
      
  return TRUE;
}


toenBool foenTimmFillRatingIntervals(struct s_group_22 *ppstG22, tostUsageItem *ppstUI, tostRatingInterval *pastRI)
{
  struct s_group_22 *lpstG22;
  struct s_group_99 *lpstG99;  
  struct s_moa_seg *lpstMoa;
  struct s_lin_seg *lpstLin;
  struct s_xcd_seg *lpstXcd;
  toenBool loenStatus;
  int i;

  PRINT_UI_INFO(ppstUI); 

  for (i = 0; i < 10; i++)
    {
      PRINT_RI(pastRI[i]);
    }

  /*
   * G22 segment on level 03 with 
   * - IMD block empty 
   * - MOA+903 in group G23
   * - RFF+IC in group G23
   * - G99 not empty
   * Let's find any block with G99 not equal NULL
   */

  lpstG99 = NULL;
  lpstG22 = ppstG22->g_22_next;
  while (lpstG22)
    {
      lpstLin = lpstG22->lin;
      
      /*
       * Next contract or customer
       */
      if (EQ(lpstLin->v_1222, "02") || EQ(lpstLin->v_1222, "01"))
        {
          lpstG99 = NULL;
          break;
        }

      /*
       * Not broken loop so we are in a contract
       */
      if (lpstG22->g_99)
        {
          lpstG99 = lpstG22->g_99;
          break;
        }
      lpstG22 = lpstG22->g_22_next;
    }
  
  while (lpstG99) 
    {          
      lpstXcd = lpstG99->xcd;
      

      lpstG99 = lpstG99->g_99_next;          
    }

  return TRUE;
}


toenBool foenTimm_LoadBillingInterval(struct s_TimmInter *ppstSumSheet, char *pachzStart, char *pachzEnd) 
{
  toenBool loenIsFound;
  struct s_dtm_seg *lpstDtm;  
  
  /*
   * Start
   */

  lpstDtm = ppstSumSheet->timm->dtm;  
  loenIsFound = FALSE;
  while (lpstDtm) 
    {
      if (EQ(lpstDtm->v_2005, "167")) 
        {
          strncpy(pachzStart, lpstDtm->v_2380, 7);
          loenIsFound = TRUE;
          break;
        }
      else
        {
          lpstDtm = lpstDtm->dtm_next;
        }
    }

  if (loenIsFound == FALSE)
    {
      return FALSE;
    }
  
  /*
   * End
   */

  lpstDtm = ppstSumSheet->timm->dtm;  
  loenIsFound = FALSE;
  while (lpstDtm) 
    {
      if (EQ(lpstDtm->v_2005, "168")) 
        {
          strncpy(pachzEnd, lpstDtm->v_2380, 7);
          loenIsFound = TRUE;
          break;
        }
      else
        {
          lpstDtm = lpstDtm->dtm_next;
        }
    }

  return loenIsFound;
}

toenBool foenTimm_FindSubscriberCode(struct s_TimmInter *ppstSumSheet, int poiSub, char *pasnzCustcode)
{
  struct s_group_22 *lpstG22;
  struct s_rff_seg *lpstRff;

  /*
   * G22 segment on level 02
   */

  lpstG22 = lpstFindSubscriberSegment(ppstSumSheet->timm->g_22, poiSub);
  if (lpstG22 == NULL)
    {
      return FALSE;
    }
  
  /*
   * Custcode of subscriber
   */

  lpstRff = fpstFindItemReference(lpstG22->g_26, "IT");  
  if (lpstRff == NULL)
    {
      return FALSE;
    }
  
  strcpy(pasnzCustcode, lpstRff->v_1154);
  return TRUE;
}

toenBool foenLoadCustomerAccountNo(struct s_TimmInter *ppstSumSheet, char *pachzAccountNo) 
{
  toenBool loenIsFound;
  struct s_group_2 *lpstG2;
  struct s_group_3 *lpstG3;
  struct s_nad_seg *lpstNad;
  struct s_rff_seg *lpstRff;

  lpstG2 = ppstSumSheet->timm->g_2;
  loenIsFound = FALSE;
  while(lpstG2) 
    {
      lpstNad = lpstG2->nad;
      if (EQ(lpstNad->v_3035, "IV")) 
        {
          loenIsFound = TRUE;
          break;
        }
      else
        {
          lpstG2 = lpstG2->g_2_next;
        }
    }

  if (loenIsFound == FALSE)
    {
      return FALSE;
    }

  lpstG3 = lpstG2->g_3;
  while (lpstG3) 
    {
      lpstRff = lpstG3->rff;
      if (EQ(lpstRff->v_1153, "IT")) 
        {
          strncpy(pachzAccountNo, lpstRff->v_1154, 64); 
          loenIsFound = TRUE;
          break;
        }
      else
        {
          lpstG3 = lpstG3->g_3_next;
        }
    }

  return loenIsFound;
}


