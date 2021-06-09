/**************************************************************************************************
 *                                                                                          
 * MODULE: MP
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 08.01.98                                              
 *
 **************************************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "gen.h"
#include "protos.h"
#include "parser.h"
#include "strutl.h"
#include "timm.h"
#include "mp.h"
#include "bch_charge.h"
#include "rtx.h"
#include "rtx_tree.h"

static char *SCCS_VERSION = "1.0";

void fovdPrintVerInfoMp (void)
{
  printf ("%s\t\t\t%s\n", __FILE__, SCCS_VERSION);
}

#define FMOA "%.2lf"

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

extern tostRTX *gpstRTX;
extern long goilRTX_Length;

extern tostRTXTree gostRTXTree;

extern tostMPUSNTAB *gpstMPUSNTAB;
extern long golMPUSNTAB_Length; 

double foflMap_GetUP(char, long);
extern   print_g22(struct s_group_22 *);

toenBool foenMpG22AddUsagePrice(struct s_group_22 *ppstG22, 
                                long poilContractId, tostUsageItem *ppstUI, tostRatingInterval *pastRI)
{
  toenBool loenStatus;
  int i, j, k;
  double loflVolume, loflAmount, up;
  struct s_imd_seg *lpstImd;
  struct s_moa_seg *lpstMoa;
  struct s_qty_seg *lpstQty;
  struct s_pri_seg *lpstPri;
  struct s_group_23 *lpstG23, *lpstCurrentG23;
  struct s_group_25 *lpstG25;
  char lasnzKey[64];
  tostRTXArray *lpstRTXArray;
  tostRTX *lpstRTX;

  if (ppstUI->sochType == 'I')
    {
      ppstUI->sochType = 'L';
    }

  if (goenVerbose == TRUE)
    {
      if (ppstUI->sachzTTSHDES[0] != '\0')
        {
          fprintf(stderr, "Call category: %ld.%ld.%ld.%ld.%c.%ld.%ld.%ld.%ld\n", 
                  ppstUI->solTMCODE, ppstUI->solVSCODE, ppstUI->solSPCODE, ppstUI->solSNCODE,
                  ppstUI->sochType,
                  ppstUI->solRIVERSION,
                  ppstUI->solEGLVERSION,
                  ppstUI->solTTCODE,
                  ppstUI->solZNCODE);
        }
      else
        {
          fprintf(stderr, "Call category: %ld.%ld.%ld.%ld.%c.%ld.%ld.*.*\n", 
                  ppstUI->solTMCODE, ppstUI->solVSCODE, ppstUI->solSPCODE, ppstUI->solSNCODE,
                  ppstUI->sochType,
                  ppstUI->solRIVERSION,
                  ppstUI->solEGLVERSION);                  
        }
    }

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
          ppstUI->solTMCODE, 
          ppstUI->solVSCODE, 
          ppstUI->solSNCODE,
          ppstUI->sochType,
          ppstUI->solRIVERSION,
          ppstUI->solEGLVERSION,
          ppstUI->solTTCODE,
          ppstUI->solZNCODE);

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Looking for the key: %s\n", lasnzKey);
    }

  lpstRTXArray = fpstRTXTree_Search(&gostRTXTree, lasnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "Can't find the key: %s\n", lasnzKey);
      return FALSE;
    }
  
  loflVolume = 0.0;
  loflAmount = 0.0;

  for (i = 0; i < lpstRTXArray->soiSize; i++)
    {
      lpstRTX = lpstRTXArray->spstArray[i];
      loflVolume = loflVolume + lpstRTX->rounded_volume;
      loflAmount = loflAmount + lpstRTX->rated_flat_amount;      
    }

  if(goenVerbose == TRUE) {fprintf(stderr, "CALLS: %d\n", i);} 
  if(goenVerbose == TRUE) {fprintf(stderr, "SUM(rounded_volume): %f\n", loflVolume);} 
  if(goenVerbose == TRUE) {fprintf(stderr, "SUM(rated_flat_amount): %f\n", loflAmount);} 

  /*
   * New QTY segment : number of seconds (after rounding)
   */ 

  lpstQty = fpstNewFloatQuantity("997", "SEC", loflVolume); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }
  
  /* 
   * New QTY segment : number of intervals 
   */

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Interval length: %lf\n", pastRI[9].sodIL);
    }

  if (pastRI[9].sodIL == 0.0)
    {
      lpstQty = fpstNewLongQuantity("998", "INT", (long)(loflVolume / 60.0));
    }
  else
    {
      lpstQty = fpstNewLongQuantity("998", "INT", (long)(loflVolume / pastRI[9].sodIL));
    }
  
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }
  
  /*
   * New QTY segment : length of interval in seconds
   */ 

  lpstQty = fpstNewFloatQuantity("999", "SEC", pastRI[9].sodIL);
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New PRI segment : price of interval
   */

  up = foflMap_GetUP(ppstUI->sochType, ppstUI->solTMCODE);
  
  lpstPri = (struct s_pri_seg *) malloc(sizeof(struct s_pri_seg));
  if (lpstMoa == NULL)
    {
      return FALSE;
    }
  
  strcpy(lpstPri->v_5125, "INT");
  
  sprintf(lpstPri->v_5118, "%lf", pastRI[9].sodIC * up);
  strcpy(lpstPri->v_5375, "PLN");

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  lpstG25 = (struct s_group_25 *) malloc(sizeof(struct s_group_25));
  if (lpstG25 == NULL)
    {
      return FALSE;
    }
  
  lpstG25->pri = lpstPri;
  lpstG25->g_25_next = ppstG22->g_25;
  ppstG22->g_25 = lpstG25;

  /*
   * New MOA+333 segment in group G23
   */

  lpstMoa = (struct s_moa_seg *) malloc(sizeof(struct s_moa_seg));
  if (lpstMoa == NULL)
    {
      return FALSE;
    }
  
  strcpy(lpstMoa->v_5025, "126");
  sprintf(lpstMoa->v_5004, FMOA, loflAmount);
  strcpy(lpstMoa->v_6345, "PLN");
  strcpy(lpstMoa->v_6343, "985");
  strcpy(lpstMoa->v_4405, "9");
  lpstMoa->moa_next = NULL;

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: MOA:%s:%s:%s:%s:%s\n", 
              lpstMoa->v_5025, lpstMoa->v_5004, lpstMoa->v_6345, lpstMoa->v_6343, lpstMoa->v_4405);
    }

  lpstG23 = (struct s_group_23 *) malloc(sizeof(struct s_group_23));
  if (lpstG23 == NULL)
    {
      return FALSE;
    }
  lpstG23->g_23_next = NULL;
  lpstG23->moa = lpstMoa;

  lpstCurrentG23 = ppstG22->g_23;
  while (lpstCurrentG23->g_23_next != NULL)
    {
      lpstCurrentG23 = lpstCurrentG23->g_23_next;
    }  

  lpstCurrentG23->g_23_next = lpstG23;  

  if (goenVerbose == TRUE)
    {
      print_g22(ppstG22);
    }

  return TRUE;
}


toenBool foenMpMatchUsageRTX_TTTZ(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE &&
      postRTX->rtx_type == postUI->sochType &&
      postRTX->ttcode == postUI->solTTCODE &&
      postRTX->tzcode == postUI->solZNCODE &&
      postRTX->rp_version == postUI->solRIVERSION &&
      postRTX->eg_version == postUI->solEGLVERSION)
    {
      return TRUE;
    }

  return FALSE;
}

toenBool foenMpMatchUsageRTX_NoTTTZ(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE &&
      postRTX->rtx_type == postUI->sochType &&
      postRTX->rp_version == postUI->solRIVERSION)
    {
      return TRUE;
    }

  return FALSE;
}

/*
toenBool foenMpMatchUsageRTX_TTTZ(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE &&
      postRTX->rtx_type == postUI->sochType &&
      postRTX->ttcode == postUI->solTTCODE &&
      postRTX->tzcode == postUI->solZNCODE)
    {
      return TRUE;
    }

  return FALSE;
}

toenBool foenMpMatchUsageRTX_NoTTTZ(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE &&
      postRTX->rtx_type == postUI->sochType)
    {
      return TRUE;
    }

  return FALSE;
}
*/

toenBool foenMpG22AddEventPrice(struct s_group_22 *ppstG22, 
                                long poilContractId, tostUsageItem *ppstUI, tostEventPrice *ppstEP)
{
  toenBool loenStatus;
  int i, j, k;
  double loflVolume, loflAmount, up;
  struct s_imd_seg *lpstImd;
  struct s_moa_seg *lpstMoa;
  struct s_qty_seg *lpstQty;
  struct s_pri_seg *lpstPri;
  struct s_group_23 *lpstG23, *lpstCurrentG23;
  struct s_group_25 *lpstG25;
  char lasnzKey[64];
  tostRTXArray *lpstRTXArray;
  tostRTX *lpstRTX;
  
  ppstEP->solEV_REG_Count = 0;
  ppstEP->solEV_ACT_Count = 0;
  ppstEP->solEV_DEA_Count = 0;
  ppstEP->solEV_INT_Count = 0;
  ppstEP->solEV_INV_Count = 0;
  ppstEP->solEV_ERA_Count = 0;
  ppstEP->solEV_PWC_Count = 0;
  
  sprintf(lasnzKey, "%ld.%ld.%ld.%ld.%c.%ld.%ld.%ld.%ld",
          poilContractId,
          ppstUI->solTMCODE, 
          ppstUI->solVSCODE, 
          ppstUI->solSNCODE,
          ppstUI->sochType,
          ppstUI->solRIVERSION,
          ppstUI->solEGLVERSION,
          ppstUI->solTTCODE,
          ppstUI->solZNCODE);

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Looking for the key: %s\n", lasnzKey);
    }

  lpstRTXArray = fpstRTXTree_Search(&gostRTXTree, lasnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "Can't find the key: %s\n", lasnzKey);
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Category found with key: %s\n", lasnzKey);
      fprintf(stderr, "Number of events in the category: %d\n",  lpstRTXArray->soiSize);
    }
  
  loflVolume = 0;
  loflAmount = 0;

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Counting\n");
    }

  for (i = 0; i < lpstRTXArray->soiSize; i++)
    {
      lpstRTX = lpstRTXArray->spstArray[i];
      switch(lpstRTX->action_code)
        {
        case 'R': ppstEP->solEV_REG_Count++; break;
        case 'A': ppstEP->solEV_ACT_Count++; break;
        case 'E': ppstEP->solEV_ERA_Count++; break;
        case 'D': ppstEP->solEV_DEA_Count++; break;
        case 'Q': ppstEP->solEV_INT_Count++; break;
        case 'I': ppstEP->solEV_INV_Count++; break;
        case 'P': ppstEP->solEV_PWC_Count++; break;
        }

      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Vol: %lf, Amount: %lf\n", lpstRTX->rounded_volume, lpstRTX->rated_flat_amount);
        }
      
      loflVolume = loflVolume + lpstRTX->rounded_volume;
      loflAmount = loflAmount + lpstRTX->rated_flat_amount;      
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "OK\n");
    }

  if(goenVerbose == TRUE) {fprintf(stderr, "EVENTS: %d\n", i);} 
  if(goenVerbose == TRUE) {fprintf(stderr, "SUM(rounded_volume): %lf\n", loflVolume);} 
  if(goenVerbose == TRUE) {fprintf(stderr, "SUM(rated_flat_amount): %lf\n", loflAmount);} 


  /*
   * New QTY segment : number of events REG
   */ 

  lpstQty = fpstNewLongQuantity("990", "REG", ppstEP->solEV_REG_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events ACT
   */ 

  lpstQty = fpstNewLongQuantity("991", "ACT", ppstEP->solEV_ACT_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events ERA
   */ 

  lpstQty = fpstNewLongQuantity("992", "ERA", ppstEP->solEV_ERA_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events DEA
   */ 

  lpstQty = fpstNewLongQuantity("993", "DEA", ppstEP->solEV_DEA_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events INT
   */ 

  lpstQty = fpstNewLongQuantity("994", "INT", ppstEP->solEV_INT_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events INV
   */ 

  lpstQty = fpstNewLongQuantity("995", "INV", ppstEP->solEV_INV_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }

  /*
   * New QTY segment : number of events PWC
   */ 

  lpstQty = fpstNewLongQuantity("996", "PWC", ppstEP->solEV_PWC_Count); 
  if (lpstQty == NULL)
    {
      return FALSE;
    }
  
  fovdUpdateQuantityList(&(ppstG22->qty), &lpstQty); 

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: QTY:%s:%s:%s\n", lpstQty->v_6063, lpstQty->v_6060, lpstQty->v_6411);
    }


  /*
   * PRI
   */
  
  up = foflMap_GetUP(ppstUI->sochType, ppstUI->solTMCODE);
 
  /*
   * New PRI segment : price of event REG
   */

  lpstPri = fpstNewFloatPrice("REG", ppstEP->sodEV_REG * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", 
              lpstPri->v_5125, 
              lpstPri->v_5118, 
              lpstPri->v_5375);
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }
  
  /*
   * New PRI segment : price of event ACT 
   */

  lpstPri = fpstNewFloatPrice("ACT", ppstEP->sodEV_ACT * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  /*
   * New PRI segment : price of event DEA
   */

  lpstPri = fpstNewFloatPrice("DEA", ppstEP->sodEV_DEA * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  /*
   * New PRI segment : price of event INT
   */

  lpstPri = fpstNewFloatPrice("INT", ppstEP->sodEV_INT * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  /*
   * New PRI segment : price of event INV
   */

  lpstPri = fpstNewFloatPrice("INV", ppstEP->sodEV_INV * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  /*
   * New PRI segment : price of event ERA
   */

  lpstPri = fpstNewFloatPrice("ERA", ppstEP->sodEV_ERA * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  /*
   * New PRI segment : price of event PWC
   */

  lpstPri = fpstNewFloatPrice("PWC", ppstEP->sodEV_PWC * up, "PLN");
  if (lpstPri == NULL)
    {
      return FALSE;
    }

  lpstG25 = fpstUpdateG25List(&(ppstG22->g_25), &lpstPri); 
  if (lpstG25 == NULL)
    {
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New: PRI:%s:%s:%s\n", lpstPri->v_5125, lpstPri->v_5118, lpstPri->v_5375);
    }

  if (goenVerbose == TRUE)
    {
      print_g22(ppstG22);
    }

  return TRUE;
}

toenBool foenMpMatchEventRTX_TT(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->call_type == 3 &&
      postRTX->rtx_type == postUI->sochType &&
      postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE &&
      postRTX->ttcode == postUI->solTTCODE)
    {
      return TRUE;
    }

  return FALSE;
}

toenBool foenMpMatchEventRTX_NoTT(tostRTX *postRTX, tostUsageItem *postUI)
{
  if (postRTX->call_type == 3 &&
      postRTX->rtx_type == postUI->sochType &&
      postRTX->tmcode == postUI->solTMCODE &&
      postRTX->tm_version == postUI->solVSCODE &&
      postRTX->sp_code == postUI->solSPCODE &&
      postRTX->sncode == postUI->solSNCODE)
    {
      return TRUE;
    }

  return FALSE;
}


