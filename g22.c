/**************************************************************************************************
 *                                                                                          
 * MODULE: G22
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
#include "g22.h"



/*
typedef struct
{
  char sasnzArticleNumber[36];
  char sasnzItemId[36];
  char sasnzServiceDes[76];
  char sasnzServiceShdes[6];  
  int soiCallsNumber;
  int soiCallsTime;
  double soflAmount;
  double soflFullAmount;
  double soflIntervalLength;
  int soiIntervalsNumber;
  double soflIntervalPrice;
}
tostSumUsage;
*/

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

extern   void print_g22(struct s_group_22 *);

/*
 * METHOD: Load
 *
 */

toenBool foenG22_Load(struct s_group_22 *ppstG22, tostSumUsage *ppstSumUsage)
{
  static char lasnzStr[128];
  toenBool loenStatus;

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      print_g22(ppstG22);
    }
#endif

  /*
   * LIN
   */

  loenStatus = foenLIN_Load(ppstG22->lin, ppstSumUsage->sasnzArticleNumber);
  if (loenStatus != TRUE)
    {
      return FALSE;
    }
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Load G22.LIN: %s\n",  ppstSumUsage->sasnzArticleNumber);
    }
#endif
  /*
   * PIA
   */

  loenStatus = foenPIA_Load(ppstG22->pia, ppstSumUsage->sasnzItemId);
  if (loenStatus != TRUE)
    {
      return FALSE;
    }
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Load G22.PIA: %s\n",  ppstSumUsage->sasnzItemId);
    }
#endif


  /*
   * IMD
   */

  loenStatus = foenIMD_Load(ppstG22->imd, "SN", ppstSumUsage->sasnzServiceShdes, ppstSumUsage->sasnzServiceDes);
  if (loenStatus != TRUE)
    {
      return FALSE;
    }
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Load G22.IMD: %s:%s:%s\n","SN", ppstSumUsage->sasnzServiceShdes, ppstSumUsage->sasnzServiceDes);
    }
#endif  

  /*
   * QTY
   */

  loenStatus = foenQTY_Load(ppstG22->qty, "107", "UNI", lasnzStr);
  if (loenStatus != TRUE)
    {
      return FALSE;
    }
  
  ppstSumUsage->soiCallsNumber = atoi(lasnzStr);
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Load G22.QTY: %s:%s:%s\n",  "107", "UNI", lasnzStr);
    }
#endif

  loenStatus = foenQTY_Load(ppstG22->qty, "997", "SEC", lasnzStr);
  if (loenStatus == TRUE)
    {
      ppstSumUsage->soiCallsTime = atoi(lasnzStr);
#ifdef DEBUG
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Load G22.QTY: %s:%s:%s\n", "997", "SEC", lasnzStr);
        }
#endif
    }

  loenStatus = foenQTY_Load(ppstG22->qty, "998", "INT", lasnzStr);
  if (loenStatus == TRUE)
    {
      ppstSumUsage->soiIntervalsNumber = atoi(lasnzStr);
#ifdef DEBUG
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Load G22.QTY: %s:%s:%s\n", "998", "INT", lasnzStr);
        }
#endif
    }

  loenStatus = foenQTY_Load(ppstG22->qty, "999", "SEC", lasnzStr);
  if (loenStatus == TRUE)
    {
      ppstSumUsage->soflIntervalLength = atoi(lasnzStr);
#ifdef DEBUG
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Load G22.QTY: %s:%s:%s\n", "999", "SEC", lasnzStr);
        }
#endif
    }

  /*
   * MOA
   */

  loenStatus = foenG23_MOA_Load(ppstG22->g_23, "125", lasnzStr);
  if (loenStatus != TRUE)
    {
      return FALSE;
    }
  
  sscanf(lasnzStr, "%lf", &(ppstSumUsage->soflAmount));
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Load G22.MOA: %s:%s\n", "125", lasnzStr);
    }
#endif

  loenStatus = foenG23_MOA_Load(ppstG22->g_23, "126", lasnzStr);
  if (loenStatus == TRUE)
    {
      sscanf(lasnzStr, "%lf", &(ppstSumUsage->soflFullAmount));
#ifdef DEBUG
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Load G22.MOA: %s:%s\n", "126", lasnzStr);
        }
#endif
    }

  /*
   * PRI
   */

  loenStatus = foenG25_PRI_Load(ppstG22->g_25, "INT", lasnzStr);
  if (loenStatus == TRUE)
    {
      sscanf(lasnzStr, "%lf", &(ppstSumUsage->soflIntervalPrice));
#ifdef DEBUG
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Load G22.PRI: %s:%s\n", "INT", lasnzStr);
        }
#endif
    }

  if (goenVerbose == TRUE)
    {

      fprintf(stderr, "Loaded Article     : %s\n",  ppstSumUsage->sasnzArticleNumber);
      fprintf(stderr, "       Item        : %s\n",  ppstSumUsage->sasnzItemId);
      fprintf(stderr, "       Calls       : %d\n",  ppstSumUsage->soiCallsNumber);
      fprintf(stderr, "       Time        : %d\n",  ppstSumUsage->soiCallsTime);
      fprintf(stderr, "       Amount      : %lf\n", ppstSumUsage->soflAmount);
      fprintf(stderr, "       Full amount : %lf\n", ppstSumUsage->soflFullAmount);
      fprintf(stderr, "       Interval    : %lf\n", ppstSumUsage->soflIntervalLength);
      fprintf(stderr, "       Intervals no: %d\n",  ppstSumUsage->soiIntervalsNumber);
      fprintf(stderr, "       Interval pr.: %lf\n", ppstSumUsage->soflIntervalPrice);
    }

  return TRUE;
}

/*
 * METHOD: New
 *
 */

struct s_group_22 *fpstG22_New(tostSumUsage *ppstSumUsage)
{
  struct s_group_22 *lpstG22;
  struct s_group_23 *lpstG23;
  struct s_group_25 *lpstG25;
  struct s_moa_seg *lpstMOA;
  struct s_qty_seg *lpstQTY;
  struct s_pri_seg *lpstPRI;
  struct s_lin_seg *lpstLIN;
  struct s_pia_seg *lpstPIA;
  struct s_imd_seg *lpstIMD;

  /*
   * G22
   */

  lpstG22 = (struct s_group_22 *)talloc(sizeof(struct s_group_22));
  if (lpstG22 == NULL)
    {
      return NULL;
    }

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New G22\n");
    }
#endif

  /*
   * LIN block
   */

  lpstLIN = fpstLIN_New("0", ppstSumUsage->sasnzArticleNumber, "04");
  if (lpstLIN == NULL)
    {
      fprintf(stderr, "fpstLIN_New\n");
      return NULL;
    }
  
  lpstG22->lin = lpstLIN;
  
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New LIN: %s:%s:%s\n", lpstLIN->v_1082, lpstLIN->v_7140, lpstLIN->v_1222);
    }
#endif

  /*
   * PIA block
   */

  lpstPIA = fpstPIA_New(ppstSumUsage->sasnzItemId);
  if (lpstPIA == NULL)
    {
      fprintf(stderr, "fpstPIA_New\n");
      return NULL;
    }

  lpstG22->pia = lpstPIA;

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New PIA: %s\n", lpstPIA->v_7140);
    }
#endif

  /*
   * IMD seq
   */

  lpstIMD = fpstIMD_New("SN", ppstSumUsage->sasnzServiceShdes, ppstSumUsage->sasnzServiceDes);
  if (lpstIMD == NULL)
    {
      fprintf(stderr, "fpstIMD_New\n");
      return NULL;
    }

  lpstG22->imd = lpstIMD;

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New IMD: %s:%s\n", lpstIMD->v_7008, lpstIMD->v_7008a);
    }
#endif

  /*
   * QTY seq
   */

  lpstG22->qty = NULL;
  
  lpstQTY = fpstQTY_NewInt("107", ppstSumUsage->soiCallsNumber, "UNI");
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "fpstQTY_New: 107\n");
      return NULL;
    }
  
  lpstQTY->qty_next = lpstG22->qty;
  lpstG22->qty = lpstQTY;

  lpstQTY = fpstQTY_NewInt("997", ppstSumUsage->soiCallsTime, "SEC");
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "fpstQTY_New: 997\n");
      return NULL;
    }

  lpstQTY->qty_next = lpstG22->qty;
  lpstG22->qty = lpstQTY;

  lpstQTY = fpstQTY_NewInt("998", ppstSumUsage->soiIntervalsNumber, "INT");
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "fpstQTY_New: 998\n");
      return NULL;
    }

  lpstQTY->qty_next = lpstG22->qty;
  lpstG22->qty = lpstQTY;

  lpstQTY = fpstQTY_NewDouble("999", ppstSumUsage->soflIntervalLength, "SEC");
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "fpstQTY_New: 999\n");
      return NULL;
    }

  lpstQTY->qty_next = lpstG22->qty;
  lpstG22->qty = lpstQTY;

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New QTY\n");
    }
#endif


  /*
   * DTM seq
   */

  lpstG22->dtm = NULL;

  /*
   * FTX seq
   */

  lpstG22->ftx = NULL;

  /*
   * G23 seq
   */
    

  lpstMOA = fpstMOA_New("125", ppstSumUsage->soflAmount, "PLN", "998", "9");
  if (lpstMOA == NULL)
    {
      fprintf(stderr, "fpstMOA_New: 125\n");
      return NULL;
    }

  lpstG23 = fpstG23_New(lpstMOA);
  if (lpstG23 == NULL)
    {
      fprintf(stderr, "fpstG23_New\n");
      return NULL;
    }
  
  lpstG23->g_23_next = NULL;
  lpstG22->g_23 = lpstG23;
  
  lpstMOA = fpstMOA_New("126", ppstSumUsage->soflFullAmount, "PLN", "998", "9");
  if (lpstMOA == NULL)
    {
      fprintf(stderr, "fpstMOA_New: 126\n");
      return NULL;
    }

  lpstG23 = fpstG23_New(lpstMOA);
  if (lpstG23 == NULL)
    {
      fprintf(stderr, "fpstG23_New\n");
      return NULL;
    }

  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;
  
#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New G23.MOA\n");
    }
#endif
  
  /*
   * G25 seq
   */

  lpstPRI = fpstPRI_New("INT", ppstSumUsage->soflIntervalPrice, "PLN");
  if (lpstPRI == NULL)
    {
      fprintf(stderr, "fpstPRI_New: INT\n");
      return NULL;
    }

  lpstG25 = fpstG25_New(lpstPRI);
  if (lpstG25 == NULL)
    {
      fprintf(stderr, "fpstG25_New\n");
      return NULL;
    }

  lpstG25->g_25_next = NULL;
  lpstG22->g_25 = lpstG25;

#ifdef DEBUG
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New G25.PRI\n");
    }
#endif
  
  lpstG22->g_26 = NULL;
  lpstG22->g_30 = NULL;
  lpstG22->g_31 = NULL;
  lpstG22->g_35 = NULL;
  lpstG22->g_99 = NULL;

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "New G22 segment created\n");
      print_g22(lpstG22);
    }

  return lpstG22;
}

/*
 * METHOD: Free
 *
 */


void  fovdPIA_Free(struct s_pia_seg *);
void  fovdIMD_Free(struct s_imd_seg *);
void  fovdQTY_Free(struct s_qty_seg *);
void  fovdDTM_Free(struct s_dtm_seg *);
void  fovdFTX_Free(struct s_ftx_seg *);
void  fovdG23_Free(struct s_group_23 *);
void  fovdG25_Free(struct s_group_25 *);

void fovdG22_Free(struct s_group_22 *ppstG22)
{  
  free(ppstG22->lin);
  fovdPIA_Free(ppstG22->pia);
  fovdIMD_Free(ppstG22->imd);
  fovdQTY_Free(ppstG22->qty);
  fovdDTM_Free(ppstG22->dtm);
  fovdFTX_Free(ppstG22->ftx);
  fovdG23_Free(ppstG22->g_23);
  fovdG25_Free(ppstG22->g_25);

  free(ppstG22);
}

void fovdFTX_Free(struct s_ftx_seg *ppstFTX)
{
  struct s_ftx_seg *lpstFTX, *lpstTmpFTX;

  lpstFTX = ppstFTX;
  while (lpstFTX)
    {
      lpstTmpFTX = lpstFTX;      
      lpstFTX = lpstFTX->ftx_next;
      free(lpstTmpFTX);
    }
}

void fovdDTM_Free(struct s_dtm_seg *ppstDTM)
{
  struct s_dtm_seg *lpstDTM, *lpstTmpDTM;

  lpstDTM = ppstDTM;
  while (lpstDTM)
    {
      lpstTmpDTM = lpstDTM;      
      lpstDTM = lpstDTM->dtm_next;
      free(lpstTmpDTM);
    }
}

void fovdQTY_Free(struct s_qty_seg *ppstQTY)
{
  struct s_qty_seg *lpstQTY, *lpstTmpQTY;

  lpstQTY = ppstQTY;
  while (lpstQTY)
    {
      lpstTmpQTY = lpstQTY;      
      lpstQTY = lpstQTY->qty_next;
      free(lpstTmpQTY);
    }
}

void fovdPIA_Free(struct s_pia_seg *ppstPIA)
{
  struct s_pia_seg *lpstPIA, *lpstTmpPIA;

  lpstPIA = ppstPIA;
  while (lpstPIA)
    {
      lpstTmpPIA = lpstPIA;      
      lpstPIA = lpstPIA->pia_next;
      free(lpstTmpPIA);
    }
}

void fovdG23_Free(struct s_group_23 *ppstG23)
{
  struct s_group_23 *lpstG23;

  lpstG23 = ppstG23;
  while (lpstG23)
    {
      if (lpstG23->moa != NULL)
        {
          free(lpstG23->moa);
        }
      
      lpstG23 = lpstG23->g_23_next;
    }
}

/*
 * G23
 */

struct s_group_23 *fpstG23_New(struct s_moa_seg *ppstMoa)
{
  struct s_group_23 *lpstG23;  

  lpstG23 = (struct s_group_23 *)talloc(sizeof(struct s_group_23));
  if (lpstG23 == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }

  lpstG23->moa = ppstMoa;
  lpstG23->g_23_next = NULL;

  return lpstG23;
}

toenBool foenG23_MOA_Load(struct s_group_23 *ppstG23, char *pasnzType, char *pasnzStr)
{
  struct s_group_23 *lpstG23;  
  struct s_moa_seg *lpstMOA;  

  lpstG23 = ppstG23;
  while(lpstG23)
    {
      lpstMOA = lpstG23->moa;
      if (EQ(lpstMOA->v_5025, pasnzType))
        {
          strncpy(pasnzStr, lpstMOA->v_5004, 18);
          return TRUE;
        }

      lpstG23 = lpstG23->g_23_next;
    }

  return FALSE;
}

/*
 * G25
 */

struct s_group_25 *fpstG25_New(struct s_pri_seg *ppstPri)
{
  struct s_group_25 *lpstG25;  

  lpstG25 = (struct s_group_25 *)talloc(sizeof(struct s_group_25));
  if (lpstG25 == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }

  lpstG25->pri = ppstPri;
  lpstG25->g_25_next = NULL;

  return lpstG25;
}

/*
 * MOA
 */

/*
struct s_moa_seg
{
  struct s_moa_seg *moa_next;
  m_char           v_5025[4];
  m_char           v_5004[19];
  m_char           v_6345[4];
  m_char           v_6343[4];
  m_char           v_4405[4];
};
*/

#define SET_MOA(s, id, val, ccode, cur, type) \
strncpy(s->v_5025, id, 3); \
strcpy(s->v_5004, val, 18); \
strcpy(s->v_6345, ccode, 3); \
strcpy(s->v_6343, cur, 3); \
strcpy(s->v_4405, type, 3); \

struct s_moa_seg *fpstMOA_New(char *ppsnzId, double pofFlatValue, char *ppsnzCur, char *ppsnzCCode, char *ppsnzType)
{
  struct s_moa_seg *lpstMoa = NULL;
  static char lasnzStr[32];

  lpstMoa = (struct s_moa_seg *)talloc(sizeof(struct s_moa_seg));
  if (lpstMoa == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      

  sprintf(lasnzStr, "%lf", pofFlatValue);
  SET_MOA(lpstMoa, ppsnzId, lasnzStr, ppsnzCur, ppsnzCCode, ppsnzType);
  lpstMoa->moa_next = NULL;

  return lpstMoa;
}

/*
 * QTY
 */

/*
struct s_qty_seg
{
  struct s_qty_seg *qty_next;
  m_char           v_6063[4];
  m_char           v_6060[16];
  m_char           v_6411[4];
};
*/

#define SET_QTY(s, a, b, c) \
strncpy(s->v_6063, a, 3); \
strncpy(s->v_6060, b, 15); \
strncpy(s->v_6411, c, 3);

struct s_qty_seg *fpstQTY_NewDouble(char *ppsnzType, double poflVal, char *ppsnzId)
{
  struct s_qty_seg *lpstQTY;
  char lasnzStr[16];

  lpstQTY = (struct s_qty_seg *)talloc(sizeof(struct s_qty_seg));
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      
  
  sprintf(lasnzStr, "%lf", poflVal);
  SET_QTY(lpstQTY, ppsnzType, lasnzStr, ppsnzId);  
  lpstQTY->qty_next = NULL;

  return lpstQTY;
}

struct s_qty_seg *fpstQTY_NewInt(char *ppsnzType, int poiVal, char *ppsnzId)
{
  struct s_qty_seg *lpstQTY;
  char lasnzStr[16];
  
  lpstQTY = (struct s_qty_seg *)talloc(sizeof(struct s_qty_seg));
  if (lpstQTY == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      
  
  sprintf(lasnzStr, "%d", poiVal);
  SET_QTY(lpstQTY, ppsnzType, lasnzStr, ppsnzId);  
  lpstQTY->qty_next = NULL;
  
  return lpstQTY;
}

toenBool foenQTY_Load(struct s_qty_seg *ppstQTY, char *pasnzType, char *pasnzUnit, char *pasnzStr)
{
  struct s_qty_seg *lpstQTY;

  lpstQTY = ppstQTY;
  while (lpstQTY)
    {
      if (EQ(lpstQTY->v_6063, pasnzType) && EQ(lpstQTY->v_6411, pasnzUnit))
        {
          strncpy(pasnzStr, lpstQTY->v_6060, 15);
          return TRUE;
        }

      lpstQTY = lpstQTY->qty_next;
    }

  return FALSE;
}

/*
 * PIA
 */

/*
struct s_pia_seg
{
  struct s_pia_seg *pia_next;
  m_char	   v_4347[4];
  m_char	   v_7140[36];
};
*/

#define SET_PIA(t, a, b) \
strncpy(t->v_4347, a, 3); \
strncpy(t->v_7140, b, 35);

struct s_pia_seg *fpstPIA_New(char *pasnzItemNo) 
{
  struct s_pia_seg *lpstPIA;

  lpstPIA = (struct s_pia_seg *)talloc(sizeof(struct s_pia_seg));
  if (lpstPIA == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      

  SET_PIA(lpstPIA, "1", pasnzItemNo);
  lpstPIA->pia_next = NULL;

  return lpstPIA;
}

toenBool foenPIA_Load(struct s_pia_seg *ppstPIA, char *pasnzStr)
{
  strncpy(pasnzStr, ppstPIA->v_7140, 35);
 
  return TRUE;
}

/*
 * IMD
 */

/*
struct s_imd_seg
{
  struct s_imd_seg *imd_next;
  m_char           v_7081[4];
  m_char           v_7009[8];
  m_char           v_7008[76],
                   v_7008a[76];
};
*/

#define SET_IMD(t, a, b, c, d) \
strncpy(t->v_7081, a, 3); \
strncpy(t->v_7009, b, 7); \
strncpy(t->v_7008, c, 75); \
strncpy(t->v_7008a, d, 75);

struct s_imd_seg *fpstIMD_New(char *pasnzType, char *pasnzShdes, char *pasnzDes) 
{
  struct s_imd_seg *lpstIMD;

  lpstIMD = (struct s_imd_seg *)talloc(sizeof(struct s_imd_seg));
  if (lpstIMD == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      
  
  SET_IMD(lpstIMD, "8", pasnzType, pasnzShdes, pasnzDes); 
  lpstIMD->imd_next = NULL;
  
  return lpstIMD;
}

toenBool foenIMD_Load(struct s_imd_seg *ppstIMD, char *pasnzType, char *pasnzShdes, char *pasnzDes)
{
  strncpy(pasnzShdes, ppstIMD->v_7008, 75);
  strncpy(pasnzDes, ppstIMD->v_7008a, 75);
  
  return TRUE;
}

void fovdIMD_Free(struct s_imd_seg *ppstIMD)
{
  struct s_imd_seg *lpstIMD, *lpstTmpIMD;

  lpstIMD = ppstIMD;
  while (lpstIMD)
    {
      lpstTmpIMD = lpstIMD;      
      lpstIMD = lpstIMD->imd_next;
      free(lpstTmpIMD);
    }
}


/*
 * LIN
 */

/*
struct s_lin_seg
{
  m_char           v_1082[7];
  m_char           v_7140[36];
  m_char           v_7143[4];
  m_char           v_1222[3];
};
*/


#define SET_LIN(t, a, b) \
strncpy(t->v_1082, a, 6); \
strncpy(t->v_7140, b, 35); \
strncpy(t->v_7143, "SA", 3); \
strncpy(t->v_1222, "04", 3);

struct s_lin_seg *fpstLIN_New(char *pasnzNumber, char *pasnzItem, char *pasnzLevel)
{
  struct s_lin_seg *lpstLIN;

  lpstLIN = (struct s_lin_seg *)talloc(sizeof(struct s_lin_seg));
  if (lpstLIN == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }

  SET_LIN(lpstLIN, pasnzNumber, pasnzItem);
  
  return lpstLIN;
}


toenBool foenLIN_Load(struct s_lin_seg *ppstLIN, char *pasnzStr)
{
  strncpy(pasnzStr, ppstLIN->v_7140, 35);

  return TRUE;
}


/*
 * PRI
 */

/*
struct s_pri_seg
{
  m_char           v_5125[4];
  m_char           v_5118[16];
  m_char           v_5375[4];
};
*/

#define SET_PRI(t, a, b, c) \
strcpy(t->v_5125, a, 3); \
strcpy(t->v_5118, b, 15); \
strcpy(t->v_5375, c, 3)

struct s_pri_seg *fpstPRI_New(char *pasnzType, double soflIntervalPrice, char *pasnzCType)
{
  struct s_pri_seg *lpstPRI;
  static char lasnzStr[32];

  lpstPRI = (struct s_pri_seg *)talloc(sizeof(struct s_pri_seg));
  if (lpstPRI == NULL)
    {
      fprintf(stderr, "talloc\n");
      return NULL;
    }      

  sprintf(lasnzStr, "%lf", soflIntervalPrice);
  SET_PRI(lpstPRI, pasnzType, lasnzStr, pasnzCType);
  
  return lpstPRI;
}

toenBool foenG25_PRI_Load(struct s_group_25 *ppstG25, char *pasnzType, char *pasnzStr)
{
  if (ppstG25->pri != NULL)
    {
      strncpy(pasnzStr, ppstG25->pri->v_5118, 15);
      return TRUE;
    }
  
  return FALSE;
}

void fovdG25_Free(struct s_group_25 *ppstG25)
{
  struct s_group_25 *lpstG25;

  lpstG25 = ppstG25;
  while (lpstG25)
    {
      if (lpstG25->pri != NULL)
        {
          free(lpstG25->pri);
        }
      
      lpstG25 = lpstG25->g_25_next;
    }
}


toenBool foenQTY_Set(struct s_group_22 *ppstG22, char *pasnzType, char *pasnzId,  double poflVal)
{
  struct s_qty_seg *lpstQTY, *lpstNewQTY;
 
  lpstQTY = ppstG22->qty; 
  while (lpstQTY)
    {
      if (lpstQTY != NULL && EQ(lpstQTY->v_6063, pasnzType) && EQ(lpstQTY->v_6411, pasnzId))
        {
          if (!strcmp(pasnzType, "107") || !(strcmp(pasnzType, "998")))
            {
              sprintf(lpstQTY->v_6060, "%d", (int)poflVal);
            }
          else
            {
              sprintf(lpstQTY->v_6060, "%lf", poflVal);
            }
          
          return TRUE;
        }
      
      lpstQTY = lpstQTY->qty_next;
    }

  /*
   * Not found
   */
  
  lpstNewQTY = fpstQTY_NewDouble(pasnzType, poflVal, pasnzId);
  
  /*
   * Link QTY block
   */
  
  lpstNewQTY->qty_next = ppstG22->qty; 
  ppstG22->qty = lpstNewQTY;
  
  return TRUE;
}

toenBool foenQTY_Get(struct s_group_22 *ppstG22, char *pasnzType, char *pasnzId, double *poflVal)
{
  struct s_qty_seg *lpstQTY;

  lpstQTY = ppstG22->qty; 
  while (lpstQTY)
    {
      if (lpstQTY != NULL && EQ(lpstQTY->v_6063, pasnzType) && EQ(lpstQTY->v_6411, pasnzId))
        {
          *poflVal = atof(lpstQTY->v_6060);
          return TRUE;
        }
      
      lpstQTY = lpstQTY->qty_next;
    }

  return FALSE;
}










