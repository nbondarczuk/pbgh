/**************************************************************************************************
 *                                                                                          
 * MODULE: TIMM_COPY
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

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

extern   print_g22(struct s_group_22 *);

static struct s_lin_seg *fpstCopyLIN(struct s_lin_seg *); 
static struct s_pia_seg *fpstCopyPIA(struct s_pia_seg *, char *); 
static struct s_imd_seg *fpstCopyIMD(struct s_imd_seg *); 
static struct s_ftx_seg *fpstCopyFTX(struct s_ftx_seg *); 
extern  struct s_moa_seg *fpstNewMOA(char *, double, char *, char *, char *);
extern  struct s_group_23 *fpstNewG23(struct s_moa_seg *);
static struct s_pri_seg *fpstCopyPRI(struct s_pri_seg *, double);
extern  struct s_group_25 *fpstNewG25(struct s_pri_seg *);


static struct s_qty_seg *fpstQTY_Copy(struct s_qty_seg *);
static struct s_qty_seg *fpstQTY_CopySeq(struct s_qty_seg *);

static toenBool foenQTY_Set(struct s_group_22 *, char *, char *,  double);
static toenBool foenQTY_Get(struct s_group_22 *, char *, char *,  double *);

struct s_group_22 *fpstCopyG22(
                               struct s_group_22 *ppstG22,
                               char *ppsnzTZShdes, char *ppsnzTTShdes, int poiCalls, double pofFlatValue
                               ) 
{
  struct s_group_22 *lpstG22;
  struct s_group_23 *lpstG23;
  struct s_moa_seg *lpstMoa;
  char lasnzStr[128];

  lpstG22 = (struct s_group_22 *)talloc(sizeof(struct s_group_22));
  if (lpstG22 == NULL)
    {
      return NULL;
    }

  lpstG22->lin = NULL;
  lpstG22->pia = NULL;
  lpstG22->imd = NULL;
  lpstG22->qty = NULL;
  lpstG22->dtm = NULL;
  lpstG22->ftx = NULL;
  lpstG22->g_23 = NULL;
  lpstG22->g_25 = NULL;
  lpstG22->g_26 = NULL;
  lpstG22->g_30 = NULL;
  lpstG22->g_31 = NULL;
  lpstG22->g_35 = NULL;
  lpstG22->g_99 = NULL;

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy LIN\n"); }            
  lpstG22->lin = fpstCopyLIN(ppstG22->lin);

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy PIA\n"); }            

  if (ppsnzTZShdes == NULL && ppsnzTTShdes == NULL)
    {
      lasnzStr[0] = '\0';
    }
  else if (ppsnzTTShdes == NULL)
    {
      sprintf(lasnzStr,".%s", ppsnzTZShdes);
    }
  else
    {
      sprintf(lasnzStr,".%s.%s.%s", "0", ppsnzTTShdes, ppsnzTZShdes);
    }

  lpstG22->pia = fpstCopyPIA(ppstG22->pia, lasnzStr);

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy IMD\n"); }            
  lpstG22->imd = fpstCopyIMD(ppstG22->imd);

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy QTY\n"); }            
  lpstG22->qty = fpstQTY_NewInt("107", poiCalls, "UNI");

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy FTX\n"); }            
  lpstG22->ftx = fpstCopyFTX(ppstG22->ftx);

  lpstG22->g_23 = NULL;

  if (goenVerbose == TRUE) { fprintf(stderr, "New MOA\n"); }            
  lpstMoa = fpstMOA_New("125", pofFlatValue, "985", "PLN", "19");
  lpstG23 = fpstG23_New(lpstMoa);
  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;

  if (goenVerbose == TRUE) { fprintf(stderr, "New MOA\n"); }            
  lpstMoa = fpstMOA_New("125", pofFlatValue, "985", "PLN", "9");
  lpstG23 = fpstG23_New(lpstMoa);
  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;

  if (goenVerbose == TRUE) 
    { 
      fprintf(stderr, "Segment G22 created.\n");            
      print_g22(lpstG22);
    }

  return lpstG22;
}

/*
struct s_lin_seg
{
  m_char           v_1082[7];
  m_char           v_7140[36];
  m_char           v_7143[4];
  m_char           v_1222[3];
};
*/

#define COPY_LIN(t, s) \
strcpy(t->v_1082, s->v_1082); \
strcpy(t->v_7140, s->v_7140); \
strcpy(t->v_7143, s->v_7143); \
strcpy(t->v_1222, s->v_1222);

#define PRINT_LIN(s) 

static struct s_lin_seg *fpstCopyLIN(struct s_lin_seg *ppstLin) 
{
  struct s_lin_seg *lpstNewLin = NULL;

  lpstNewLin = (struct s_lin_seg *)talloc(sizeof(struct s_lin_seg));
  if (lpstNewLin == NULL)
    {
      return NULL;
    }
  
  COPY_LIN(lpstNewLin, ppstLin);
  /*
  fprintf(stderr, "LIN: %s %s %s %s\n", lpstNewLin->v_1082, lpstNewLin->v_7140, lpstNewLin->v_7143, lpstNewLin->v_1222);  
  */
  return lpstNewLin;
}

/*
struct s_pia_seg
{
  struct s_pia_seg *pia_next;
  m_char	   v_4347[4];
  m_char	   v_7140[36];
};
*/

#define COPY_PIA(t, s) \
strcpy(t->v_4347, s->v_4347); \
strcpy(t->v_7140, s->v_7140);

#define PRINT_PIA(s) 

static struct s_pia_seg *fpstCopyPIA(struct s_pia_seg *ppstPia, char  *ppsnzStr) 
{
  struct s_pia_seg *lpstNewPia = NULL, *lpstPia;

  lpstPia = ppstPia;
  lpstNewPia = (struct s_pia_seg *)talloc(sizeof(struct s_pia_seg));
  if (lpstNewPia == NULL)
    {
      return NULL;
    }      
  
  COPY_PIA(lpstNewPia, lpstPia);

  strcat(lpstNewPia->v_7140, ppsnzStr);

  lpstNewPia->pia_next = NULL;
  
  return lpstNewPia;
}

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

#define COPY_IMD(t, s) \
strcpy(t->v_7081, s->v_7081); \
strcpy(t->v_7009, s->v_7009); \
strcpy(t->v_7008, s->v_7008); \
strcpy(t->v_7008a, s->v_7008a);

#define PRINT_IMD(s) 

static struct s_imd_seg *fpstCopyIMD(struct s_imd_seg *ppstImd) 
{
  struct s_imd_seg *lpstNewImd = NULL, *lpstImd;

  lpstImd = ppstImd;
  while (lpstImd)
    {
      lpstNewImd = (struct s_imd_seg *)talloc(sizeof(struct s_imd_seg));
      if (lpstNewImd == NULL)
        {
          return NULL;
        }      
      
      COPY_IMD(lpstNewImd, lpstImd);
      /*
      fprintf(stderr, "IMD: %s %s %s %s\n", lpstNewImd->v_7081, lpstNewImd->v_7009, lpstNewImd->v_7008, lpstNewImd->v_7008a);
      */
      lpstNewImd->imd_next = NULL;
      lpstImd = lpstImd->imd_next;
    }

  return lpstNewImd;
}

/*

struct s_ftx_seg
{
  struct s_ftx_seg *ftx_next;
  m_char           v_4451[4];
#if UNUSED
  m_char           v_4453[4];
#endif
  m_char           v_4441[4];
  m_char           v_4440[71],
                   v_4440a[71],
                   v_4440b[71],
                   v_4440c[71],
                   v_4440d[71];
};

 */

#define COPY_FTX(t, s) \
strcpy(t->v_4451, s->v_4451); \
strcpy(t->v_4441, s->v_4441); \
strcpy(t->v_4440, s->v_4440); \
strcpy(t->v_4440a, s->v_4440a); \
strcpy(t->v_4440b, s->v_4440b); \
strcpy(t->v_4440c, s->v_4440c); \
strcpy(t->v_4440d, s->v_4440d); \
strcpy(t->v_4440e, s->v_4440e); \

static struct s_ftx_seg *fpstCopyFTX(struct s_ftx_seg *ppstFtx) 
{
  struct s_ftx_seg *lpstNewFtx = NULL, *lpstFtx;

  lpstFtx = ppstFtx;
  while (lpstFtx)
    {
      lpstNewFtx = (struct s_ftx_seg *)talloc(sizeof(struct s_ftx_seg));
      if (lpstNewFtx == NULL)
        {
          return NULL;
        }      
      
      COPY_FTX(lpstNewFtx, lpstFtx);
      lpstNewFtx->ftx_next = NULL;

      lpstFtx = lpstFtx->ftx_next;
    }

  return lpstNewFtx;
}


#define COPY_QTY(s, t) \
strcpy(s->v_6063, t->v_6063); \
strcpy(s->v_6060, t->v_6060); \
strcpy(s->v_6411, t->v_6411);

static struct s_qty_seg *fpstQTY_Copy(struct s_qty_seg *ppstQty)
{
  struct s_qty_seg *lpstNewQty = NULL;
  
  lpstNewQty = (struct s_qty_seg *)talloc(sizeof(struct s_qty_seg));
  if (lpstNewQty == NULL)
    {
      return NULL;
    }      

  COPY_QTY(lpstNewQty, ppstQty);  
  /*
  fprintf(stderr, "QTY: %s %s %s\n", lpstNewQty->v_6063, lpstNewQty->v_6060, lpstNewQty->v_6411);
  */
  lpstNewQty->qty_next = NULL;

  return lpstNewQty;
}

static struct s_qty_seg *fpstQTY_CopySeq(struct s_qty_seg *ppstQTY)
{
  struct s_qty_seg *lpstNewQTY, *lpstSourceQTY, *lpstTargetQTY, *lpstQTY;

  lpstSourceQTY = ppstQTY;
  lpstTargetQTY = NULL;
  while (lpstSourceQTY)
    {
      if (goenVerbose == TRUE) { fprintf(stderr, "Copy QTY\n"); }            
      lpstNewQTY = fpstQTY_Copy(lpstSourceQTY);
      lpstNewQTY->qty_next = lpstTargetQTY;
      lpstTargetQTY = lpstNewQTY;

      lpstSourceQTY = lpstSourceQTY->qty_next;
    }

  return lpstTargetQTY;
}


struct s_group_22 *fpstCopyMCG22(
                                 struct s_group_22 *ppstG22,
                                 char *ppsnzTZShdes, char *ppsnzTTShdes, int poiCalls, double pofFlatValue,
                                 double pofScale
                                 ) 
{
  struct s_group_22 *lpstG22;
  struct s_group_23 *lpstG23;
  struct s_group_25 *lpstG25, *lpstSourceG25, *lpstTargetG25, *lpstNewG25;
  struct s_qty_seg *lpstQty, *lpstNewQty, *lpstTargetQty;
  struct s_pri_seg *lpstPri;
  struct s_moa_seg *lpstMoa;
  char lasnzStr[128];
  double loflVal;

  lpstG22 = (struct s_group_22 *)talloc(sizeof(struct s_group_22));
  if (lpstG22 == NULL)
    {
      return NULL;
    }

  lpstG22->lin = NULL;
  lpstG22->pia = NULL;
  lpstG22->imd = NULL;
  lpstG22->qty = NULL;
  lpstG22->dtm = NULL;
  lpstG22->ftx = NULL;
  lpstG22->g_23 = NULL;
  lpstG22->g_25 = NULL;
  lpstG22->g_26 = NULL;
  lpstG22->g_30 = NULL;
  lpstG22->g_31 = NULL;
  lpstG22->g_35 = NULL;
  lpstG22->g_99 = NULL;

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy LIN\n"); }            
  lpstG22->lin = fpstCopyLIN(ppstG22->lin);

  /*
   * PIA
   */

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy PIA\n"); }            

  if (ppsnzTZShdes == NULL && ppsnzTTShdes == NULL)
    {
      lasnzStr[0] = '\0';
    }
  else if (ppsnzTTShdes == NULL)
    {
      sprintf(lasnzStr,".%s", ppsnzTZShdes);
    }
  else
    {
      sprintf(lasnzStr,".%s.%s.%s", "0", ppsnzTTShdes, ppsnzTZShdes);
    }

  lpstG22->pia = fpstCopyPIA(ppstG22->pia, lasnzStr);

  /*
   * IMD
   */

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy IMD seq\n"); }            
  lpstG22->imd = fpstCopyIMD(ppstG22->imd);

  /*
   * QTY
   */

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy QTY seq\n"); }            

  if (goenVerbose == TRUE) { fprintf(stderr, "Get QTY : %lf\n", poiCalls); }            
  foenQTY_Set(lpstG22, "107", "UNI", (double)poiCalls);

  foenQTY_Get(ppstG22, "997", "SEC", &loflVal);
  if (goenVerbose == TRUE) { fprintf(stderr, "Get QTY : %lf\n", loflVal); }            
  foenQTY_Set(lpstG22, "997", "SEC", loflVal);

  foenQTY_Get(ppstG22, "998", "INT", &loflVal);
  if (goenVerbose == TRUE) { fprintf(stderr, "Get QTY : %lf\n", loflVal); }            
  foenQTY_Set(lpstG22, "998", "INT", loflVal);

  foenQTY_Get(ppstG22, "999", "SEC", &loflVal);
  if (goenVerbose == TRUE) { fprintf(stderr, "Get QTY : %lf\n", loflVal); }            
  foenQTY_Set(lpstG22, "999", "SEC", loflVal);

  /*
   * FTX
   */

  if (goenVerbose == TRUE) { fprintf(stderr, "Copy FTX\n"); }            
  lpstG22->ftx = fpstCopyFTX(ppstG22->ftx);

  /*
   * G23
   */

  lpstG22->g_23 = NULL;

  if (goenVerbose == TRUE) { fprintf(stderr, "New MOA\n"); }            
  lpstMoa = fpstMOA_New("125", pofFlatValue, "985", "PLN", "19");
  lpstG23 = fpstG23_New(lpstMoa);
  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;

  if (goenVerbose == TRUE) { fprintf(stderr, "New MOA\n"); }            
  lpstMoa = fpstMOA_New("125", pofFlatValue, "985", "PLN", "9");
  lpstG23 = fpstG23_New(lpstMoa);
  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;

  if (goenVerbose == TRUE) { fprintf(stderr, "New MOA\n"); }            
  lpstMoa = fpstMOA_New("126", pofFlatValue, "985", "PLN", "9");
  lpstG23 = fpstG23_New(lpstMoa);
  lpstG23->g_23_next = lpstG22->g_23;
  lpstG22->g_23 = lpstG23;

  /*
   * G25
   */

  lpstG22->g_25 = NULL;

  if (goenVerbose == TRUE) { fprintf(stderr, "New PRI seq in G25\nScale: %lf\n", pofScale); }            

  lpstSourceG25 = ppstG22->g_25;
  lpstTargetG25 = NULL;
  while (lpstSourceG25)
    {
      lpstPri = fpstCopyPRI(lpstSourceG25->pri, pofScale);
      lpstNewG25 = fpstG25_New(lpstPri);
      lpstNewG25->g_25_next = lpstTargetG25;
      lpstTargetG25 = lpstNewG25;

      lpstSourceG25 = lpstSourceG25->g_25_next;
    }
  
  lpstG22->g_25 = lpstTargetG25;

  if (goenVerbose == TRUE) 
    { 
      fprintf(stderr, "Segment G22 created.\n");                  
      print_g22(lpstG22);
    }

  return lpstG22;
}

/*
struct s_pri_seg
{
  m_char           v_5125[4];
  m_char           v_5118[16];
  m_char           v_5375[4];
};
*/

#define COPY_PRI(t, s) \
strcpy(t->v_5125, s->v_5125); \
strcpy(t->v_5118, s->v_5118); \
strcpy(t->v_5375, s->v_5375);

static struct s_pri_seg *fpstCopyPRI(struct s_pri_seg *ppstPri, double pofScale)
{
  struct s_pri_seg *lpstNewPri = NULL;
  double lofPrice;


  lpstNewPri = (struct s_pri_seg *)talloc(sizeof(struct s_pri_seg));
  if (lpstNewPri == NULL)
    {
      return NULL;
    }      

  COPY_PRI(lpstNewPri, ppstPri);  

  sscanf(lpstNewPri->v_5118, "%lf", &lofPrice);
  sprintf(lpstNewPri->v_5118, "%.2lf", lofPrice * pofScale);

  return lpstNewPri;
}

struct s_group_22 *fpstG22_Copy(struct s_group_22 *ppstG22)
{
  struct s_group_22 *lpstG22;

  lpstG22 = (struct s_group_22 *)talloc(sizeof(struct s_group_22));
  if (lpstG22 == NULL)
    {
      return NULL;
    }

  lpstG22->lin = NULL;
  lpstG22->pia = NULL;
  lpstG22->imd = NULL;
  lpstG22->qty = NULL;
  lpstG22->dtm = NULL;
  lpstG22->ftx = NULL;
  lpstG22->g_23 = NULL;
  lpstG22->g_25 = NULL;
  lpstG22->g_26 = NULL;
  lpstG22->g_30 = NULL;
  lpstG22->g_31 = NULL;
  lpstG22->g_35 = NULL;
  lpstG22->g_99 = NULL;

  return lpstG22;
}

/*
struct s_qty_seg
{
  struct s_qty_seg *qty_next;
  m_char           v_6063[4];
  m_char           v_6060[16];
  m_char           v_6411[4];
};
*/

/*
 * QTY+107
 */

static toenBool foenQTY_Set(struct s_group_22 *ppstG22, char *pasnzType, char *pasnzId,  double poflVal)
{
  struct s_qty_seg *lpstQTY, *lpstNewQTY;
  char lasnzStr[32];

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
              sprintf(lpstQTY->v_6060, "%.2lf", poflVal);
            }

          return TRUE;
        }
      
      lpstQTY = lpstQTY->qty_next;
    }

  /*
   * Not found
   */

  lpstNewQTY = fpstQTY_NewDouble(pasnzType, poflVal, pasnzId);
  if (lpstNewQTY == NULL)
    {
      return FALSE;
    }
  
  /*
   * Link QTY block
   */
  
  lpstNewQTY->qty_next = ppstG22->qty; 
  ppstG22->qty = lpstNewQTY;
  
  return TRUE;
}

static toenBool foenQTY_Get(struct s_group_22 *ppstG22, char *pasnzType, char *pasnzId, double *ppflVal)
{
  struct s_qty_seg *lpstQTY, *lpstNewQTY;

  lpstQTY = ppstG22->qty; 
  while (lpstQTY)
    {
      if (lpstQTY != NULL && EQ(lpstQTY->v_6063, pasnzType) && EQ(lpstQTY->v_6411, pasnzId))
        {
          sscanf(lpstQTY->v_6060, "%lf",  ppflVal);
          return TRUE;
        }
      
      lpstQTY = lpstQTY->qty_next;
    }

  return FALSE;
}

