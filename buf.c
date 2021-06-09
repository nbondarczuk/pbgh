/**************************************************************************************************
 *                                                                                          
 * MODULE: BUF
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 15.03.98                                              
 *
 **************************************************************************************************
 */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pbgh.h"
#include "gen.h"
#include "buf.h"

#define timm_sprintf sprintf

static char *SCCS_VERSION = "1.0";

void fovdPrintVerInfoBuf (void)
{
  printf ("%s\t\t%s\n", __FILE__, SCCS_VERSION);
}

int foiBuf_Init(tostBuf *ppstBuf)
{
  ppstBuf->soiMaxLen = 0;
  ppstBuf->soiStrLen = 0;
  ppstBuf->soiFree = 0;
  ppstBuf->spchBuf = (char *)NULL;

  return 1;
}

int foiBuf_New(tostBuf *ppstBuf, int poiLen)
{
  if ((ppstBuf->spchBuf = (char *)malloc(poiLen * sizeof(char))) == NULL)
    {
      return -1;
    }
  
  ppstBuf->soiMaxLen = poiLen;
  ppstBuf->soiStrLen = 0;
  ppstBuf->soiFree = poiLen;
  
  return poiLen;
}

void fovdBuf_CopyEtisStr(char *pachzDest, char *pachzSou, int poiMax)
{
  int i, j;

  for (i = 0, j = 0; i < poiMax && pachzSou[i] != '\0'; i++)
    {
      if (pachzSou[i] == '\'' || pachzSou[i] == '\+' || pachzSou[i] == '\?')
        {
          pachzDest[j++] = '\?';
        }

      pachzDest[j++] = pachzSou[i];               
    }

  pachzDest[j] = '\0';
}

void fovdBuf_UpdateEtisStr(char *pachzSou)
{
  int i, j;
  char pachzDest[256];
  
  for (i = 0, j = 0; i < strlen(pachzSou); i++)
    {
      if (pachzSou[i] == '\'' || pachzSou[i] == '\+' || pachzSou[i] == '\?')
        {
          pachzDest[j++] = '\?';
        }

      pachzDest[j++] = pachzSou[i];               
    }

  pachzDest[j] = '\0';
  strcpy(pachzSou, pachzDest);
}

  

int foiBuf_Append(tostBuf *ppstBuf, char *pachzStr, int n)
{
  char *ptr;  
  char *lpchzStr;
  
#ifdef DEBUG  
  fprintf(stderr, "Buf_Append[%d, %d]: [%d]: %s\n", ppstBuf->soiStrLen, ppstBuf->soiMaxLen, n, pachzStr);
#endif

  if (ppstBuf->soiStrLen + n > ppstBuf->soiMaxLen)
    {
      /*
       * Reallocation
       */

      if ((ptr = (char *)malloc(2 * ppstBuf->soiMaxLen)) == NULL)
        {
          return -1;
        }

      memcpy((char *)ptr, (char *)ppstBuf->spchBuf, ppstBuf->soiMaxLen);
      free(ppstBuf->spchBuf);

      ppstBuf->spchBuf = ptr;
      ppstBuf->soiMaxLen = 2 * ppstBuf->soiMaxLen;
      ppstBuf->soiFree = ppstBuf->soiFree + ppstBuf->soiMaxLen;
    }

  memcpy(ppstBuf->spchBuf + ppstBuf->soiStrLen, pachzStr, n);
  ppstBuf->soiFree = ppstBuf->soiFree - n;
  ppstBuf->soiStrLen = ppstBuf->soiStrLen + n;
  ppstBuf->spchBuf[ppstBuf->soiStrLen] = '\0';

  return ppstBuf->soiStrLen;
}

/*
 * UNB segment
 */

int foiBuf_UNB_Sprintf(tostBuf *ppstBuf, struct s_unb_seg *p_unb)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG    
  fprintf(stderr, "UNB\n");
#endif
  n = timm_sprintf (lachTmpBuf, 
                    "UNB+%s:%s+%s+%s+%s:%s+%s'\n",
                    p_unb->v_0001, p_unb->v_0002, p_unb->v_0004, p_unb->v_0010,
                                                     p_unb->v_0017, p_unb->v_0019, p_unb->v_0020);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * UNH segment
 */

int foiBuf_UNH_Sprintf(tostBuf *ppstBuf, struct s_unh_seg *p_unh)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "UNH\n");
#endif
  n = timm_sprintf(lachTmpBuf,"UNH+%s+%s:%s:%s:%s:%s'\n",
                   p_unh->v_0062,p_unh->v_0065,p_unh->v_0052,p_unh->v_0054,
                   p_unh->v_0051,p_unh->v_0057);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * BGM segment
 */

int foiBuf_BGM_Sprintf(tostBuf *ppstBuf, struct s_bgm_seg *p_bgm)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "BGM\n");
#endif
  n = timm_sprintf(lachTmpBuf, "BGM+%s:::%s+%s+%s' \n",
                   p_bgm->v_1001,p_bgm->v_1000,p_bgm->v_1004,p_bgm->v_1225);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * DTM segment
 */

int foiBuf_DTM_Sprintf(tostBuf *ppstBuf, struct s_dtm_seg *p_dtm)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "DTM\n");
#endif
  n = timm_sprintf(lachTmpBuf,"DTM+%s:%s:%s'\n",
                   p_dtm->v_2005,p_dtm->v_2380,p_dtm->v_2379);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * FTX
 */

int foiBuf_FTX_Sprintf(tostBuf *ppstBuf, struct s_ftx_seg *p_ftx)
{
  int n;
  char lachzEtisStr[8][142];

  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "FTX\n");
#endif
  
  fovdBuf_CopyEtisStr(lachzEtisStr[0], p_ftx->v_4451, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[1], p_ftx->v_4441, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[2], p_ftx->v_4440, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[3], p_ftx->v_4440a, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[4], p_ftx->v_4440b, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[5], p_ftx->v_4440c, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[6], p_ftx->v_4440d, 142);
  fovdBuf_CopyEtisStr(lachzEtisStr[7], p_ftx->v_4440e, 142);

  /*
  n = timm_sprintf(lachTmpBuf,"FTX+%s++%s+%s:%s:%s:%s:%s'\n",
                   p_ftx->v_4451, p_ftx->v_4441,p_ftx->v_4440, p_ftx->v_4440a,
                   p_ftx->v_4440b, p_ftx->v_4440c, p_ftx->v_4440d);
  */

  n = timm_sprintf(lachTmpBuf,"FTX+%s++%s+%s:%s:%s:%s:%s:%s'\n",
                   lachzEtisStr[0],
                   lachzEtisStr[1],
                   lachzEtisStr[2],
                   lachzEtisStr[3],
                   lachzEtisStr[4],
                   lachzEtisStr[5],
                   lachzEtisStr[6],
                   lachzEtisStr[7]);

  if (n <= 0)
    {
      return n;
    }
  
  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * RFF
 */

int foiBuf_RFF_Sprintf(tostBuf *ppstBuf, struct s_rff_seg *p_rff)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "RFF\n");
#endif
  n = timm_sprintf(lachTmpBuf, "RFF+%s:%s'\n", p_rff->v_1153,p_rff->v_1154);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * NAD
 */

int foiBuf_NAD_Sprintf(tostBuf *ppstBuf, struct s_nad_seg *p_nad)
{
  int n;
  char lachzEtisStr[7][142];

  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "NAD\n");
#endif
  if (!strcmp(p_nad->v_3035, "II"))
    {
      fovdBuf_CopyEtisStr(lachzEtisStr[0], p_nad->v_3035, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[1], p_nad->v_3036, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[2], p_nad->v_3036a, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[3], p_nad->v_3036b, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[4], p_nad->v_3036c, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[5], p_nad->v_3036d, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[6], p_nad->v_3251, 142);

      /*
      n = timm_sprintf(lachTmpBuf, "NAD+%s+++%s:%s:%s:%s++%s++%s'\n",
                       p_nad->v_3035,  
                       p_nad->v_3036,  p_nad->v_3036a, p_nad->v_3036b, p_nad->v_3036c, p_nad->v_3036d,
                       p_nad->v_3251);
      */

      n = timm_sprintf(lachTmpBuf, "NAD+%s+++%s:%s:%s:%s++%s++%s'\n",
                       lachzEtisStr[0],
                       lachzEtisStr[1],
                       lachzEtisStr[2],
                       lachzEtisStr[3],
                       lachzEtisStr[4],
                       lachzEtisStr[5],
                       lachzEtisStr[6]);
    }
  else
    {
      fovdBuf_CopyEtisStr(lachzEtisStr[0], p_nad->v_3035, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[1], p_nad->v_3036, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[2], p_nad->v_3036a, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[3], p_nad->v_3036b, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[4], p_nad->v_3036c, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[5], p_nad->v_3036d, 142);
      fovdBuf_CopyEtisStr(lachzEtisStr[6], p_nad->v_3164, 142);

      /*
        n = timm_sprintf(lachTmpBuf, "NAD+%s+++%s:%s:%s:%s:%s:++%s'\n",
                       p_nad->v_3035,  
                       p_nad->v_3036,  p_nad->v_3036a, p_nad->v_3036b, p_nad->v_3036c, p_nad->v_3036d,
                       p_nad->v_3164);
      */

      n = timm_sprintf(lachTmpBuf, "NAD+%s+++%s:%s:%s:%s:%s:++%s'\n",
                       lachzEtisStr[0],
                       lachzEtisStr[1],
                       lachzEtisStr[2],
                       lachzEtisStr[3],
                       lachzEtisStr[4],
                       lachzEtisStr[5],
                       lachzEtisStr[6]);

      
    }
 
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * FII
 */

int foiBuf_FII_Sprintf(tostBuf *ppstBuf, struct s_fii_seg *p_fii)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "FII\n");
#endif
  n = timm_sprintf (lachTmpBuf, "FII+%s+%s:%s:%s+%s::::::%s:%s+'\n",
                    p_fii->v_3035,p_fii->v_3194,p_fii->v_3192,p_fii->v_3192a,
                    p_fii->v_3433,p_fii->v_3432,p_fii->v_3436);  
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * DOC
 */

int foiBuf_DOC_Sprintf(tostBuf *ppstBuf, struct s_doc_seg *p_doc)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "DOC\n");
#endif
  n = timm_sprintf (lachTmpBuf, "DOC+%s+:::%s'\n", p_doc->v_1001, p_doc->v_3453);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * CTA
 */

int foiBuf_CTA_Sprintf(tostBuf *ppstBuf, struct s_cta_seg *p_cta)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "CTA\n");
#endif
  n = timm_sprintf (lachTmpBuf, "CTA+%s+%s:%s'\n", p_cta->v_3139,p_cta->v_3413, p_cta->v_3412);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * COM
 */

int foiBuf_COM_Sprintf(tostBuf *ppstBuf, struct s_com_seg *p_com)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "COM\n");
#endif
  n = timm_sprintf(lachTmpBuf, "COM+%s:%s'\n", p_com->v_3148, p_com->v_3155);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * TAX
 */

int foiBuf_TAX_Sprintf(tostBuf *ppstBuf, struct s_tax_seg *p_tax)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "TAX\n");
#endif
  n = timm_sprintf (lachTmpBuf, "TAX+%s+%s:::%s+++:::%s+%s'\n",
                    p_tax->v_5283,p_tax->v_5153, p_tax->v_5152,
                    p_tax->v_5278, p_tax->v_5305);  
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * CUX
 */

int foiBuf_CUX_Sprintf(tostBuf *ppstBuf, struct s_cux_seg *p_cux)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "CUX\n");
#endif
  n = timm_sprintf(lachTmpBuf, "CUX+%s:%s:'\n",
                   p_cux->v_6347, p_cux->v_6345);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * PAT
 */

int foiBuf_PAT_Sprintf(tostBuf *ppstBuf, struct s_pat_seg *p_pat)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "PAT\n");
#endif
  n = timm_sprintf (lachTmpBuf, "PAT+%s++%s:%s:%s:%s'\n",
                    p_pat->v_4279,p_pat->v_2475, p_pat->v_2009,p_pat->v_2151, p_pat->v_2152);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * LIN
 */

int foiBuf_LIN_Sprintf(tostBuf *ppstBuf, struct s_lin_seg *p_lin)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "LIN\n");
#endif
  n = timm_sprintf(lachTmpBuf, "LIN+%s++%s:%s++%s'\n",
                   p_lin->v_1082,p_lin->v_7140, p_lin->v_7143,p_lin->v_1222);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * PIA
 */

int foiBuf_PIA_Sprintf(tostBuf *ppstBuf, struct s_pia_seg *p_pia)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "PIA\n");
#endif
  n = timm_sprintf (lachTmpBuf, "PIA+%s+%s'\n",
                    p_pia->v_4347,p_pia->v_7140);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * IMD
 */

int foiBuf_IMD_Sprintf(tostBuf *ppstBuf, struct s_imd_seg *p_imd)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "IMD\n");
#endif
  n = timm_sprintf (lachTmpBuf, "IMD++%s+%s:::%s:%s'\n",
                    p_imd->v_7081,p_imd->v_7009,p_imd->v_7008,p_imd->v_7008a);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * QTY
 */

int foiBuf_QTY_Sprintf(tostBuf *ppstBuf, struct s_qty_seg *p_qty)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "QTY\n");
#endif
  n = timm_sprintf (lachTmpBuf, "QTY+%s:%s:%s'\n",
                    p_qty->v_6063,p_qty->v_6060,p_qty->v_6411);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * MOA
 */

int foiBuf_MOA_Sprintf(tostBuf *ppstBuf, struct s_moa_seg *p_moa)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "MOA\n");
#endif
  n = timm_sprintf (lachTmpBuf, "MOA+%s:%s:%s:%s:%s'\n",
                    p_moa->v_5025,p_moa->v_5004, p_moa->v_6345,
                    p_moa->v_6343, p_moa->v_4405);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * PRI
 */

int foiBuf_PRI_Sprintf(tostBuf *ppstBuf, struct s_pri_seg *p_pri)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "PRI\n");
#endif
  n = timm_sprintf (lachTmpBuf, "PRI+%s:%s:%s'\n", p_pri->v_5125,p_pri->v_5118, p_pri->v_5375);
  if (n <= 0)
    {
      return n;
    }

#ifdef DEBUG  
  fprintf(stderr, "OK\n");
#endif

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * LOC
 */

int foiBuf_LOC_Sprintf(tostBuf *ppstBuf, struct s_loc_seg *p_loc)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "LOC\n");
#endif
  n = timm_sprintf (lachTmpBuf, "LOC+%s++%s:::'\n", p_loc->v_3227, p_loc->v_3223);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * ALC
 */

int foiBuf_ALC_Sprintf(tostBuf *ppstBuf, struct s_alc_seg *p_alc)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "ALC\n");
#endif
  n = timm_sprintf(lachTmpBuf, "ALC+%s'\n", p_alc->v_5463);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * PCD
 */

int foiBuf_PCD_Sprintf(tostBuf *ppstBuf, struct s_pcd_seg *p_pcd)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "PCD\n");
#endif
  n = timm_sprintf(lachTmpBuf, "PCD+%s:%s'\n", p_pcd->v_5245,p_pcd->v_5482);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * XCD
 */

int foiBuf_XCD_Sprintf(tostBuf *ppstBuf, struct s_xcd_seg *p_xcd)
{
  int n;
  char lachzEtisStr[2][128];

  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG    
  fprintf(stderr, "XCD\n");
#endif


  fovdBuf_CopyEtisStr(lachzEtisStr[0], p_xcd->v_X045, 35);
  fovdBuf_CopyEtisStr(lachzEtisStr[1], p_xcd->v_X048, 85);
  
  n = timm_sprintf (lachTmpBuf, 
                    "XCD+"
                    "%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+"
                    "%s+"
                    "%s+%s+"
                    "%s+%s+%s+"
                    "%s+%s+%s+"
                    "%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+%s+"
#ifdef US_TIMM_LAYOUT
                    "%s:%s:%s+"
                    "%s:%s:%s+"
                    "%s+%s+%s+%s+%s+%s+"
#else
                    "::+::+0++++0+0+"
#endif
                    "%s:%s:%s:%s:%s:%s:%s+"
                    "%s:%s:%s:%s+"
                    "%s+%s'\n",
                    
                    p_xcd->v_X001, 
                    p_xcd->v_X002, 
                    p_xcd->v_X003, 
                    p_xcd->v_X004, 
                    p_xcd->v_X005, 
                    p_xcd->v_X006, 
                    p_xcd->v_X007, 
                    p_xcd->v_X008, 
                    p_xcd->v_X009, 
                    p_xcd->v_X010, 
                    
                    p_xcd->v_6411,
                    
                    p_xcd->v_X011, 
                    p_xcd->v_X012, 
                    
                    p_xcd->v_5004c, 
                    p_xcd->v_6345c, 
                    p_xcd->v_6343c, 
                    
                    p_xcd->v_5004, 
                    p_xcd->v_6345, 
                    p_xcd->v_6343, 
                    
                    p_xcd->v_X013, 
                    p_xcd->v_X014, 
                    p_xcd->v_X015,
                    p_xcd->v_X016, 
                    p_xcd->v_X017, 
                    p_xcd->v_X018, 
                    p_xcd->v_X019, 
                    p_xcd->v_X020, 
                    p_xcd->v_X021,
                    p_xcd->v_X022,
                    p_xcd->v_X023, 
                    p_xcd->v_X024, 
                    p_xcd->v_X025, 
                    p_xcd->v_X026,
                    p_xcd->v_X027, 
                    p_xcd->v_X028,
                    p_xcd->v_X029,
#ifdef US_TIMM_LAYOUT
                    p_xcd->v_X030, p_xcd->v_X031, p_xcd->v_X032,
                    p_xcd->v_X033, p_xcd->v_X034, p_xcd->v_X035,
                    p_xcd->v_X036,
                    p_xcd->v_X037,
                    p_xcd->v_X038,
                    p_xcd->v_X039,
                    p_xcd->v_X040,
                    p_xcd->v_X041,
#endif
                    p_xcd->v_5004a, p_xcd->v_6345a, p_xcd->v_6343a, 
                    p_xcd->v_5004b, p_xcd->v_6345b, p_xcd->v_6343b, p_xcd->v_X042, 
                    p_xcd->v_X043, p_xcd->v_X044, 
                    lachzEtisStr[0],
                    p_xcd->v_X046, 
                    p_xcd->v_X047,
                    lachzEtisStr[1]);
  
  if (n <= 0)
    {
      return n;
    }
  
  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * UNS
 */

int foiBuf_UNS_Sprintf(tostBuf *ppstBuf, struct s_uns_seg *p_uns)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "UNS\n");
#endif
  n = timm_sprintf (lachTmpBuf, "UNS+%s'\n",p_uns->v_0081);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * CNT
 */

int foiBuf_CNT_Sprintf(tostBuf *ppstBuf, struct s_cnt_seg *p_cnt)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "CNT\n");
#endif
  n = timm_sprintf (lachTmpBuf, "CNT+%s:%s:%s'\n", p_cnt->v_6069,p_cnt->v_6066,p_cnt->v_6411);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * UNT
 */

int foiBuf_UNT_Sprintf(tostBuf *ppstBuf, struct s_unt_seg *p_unt)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "UNT\n");
#endif
  n = timm_sprintf (lachTmpBuf, "UNT+%s+%s'\n", p_unt->v_0074,p_unt->v_0062);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

/*
 * UNZ
 */

int foiBuf_UNZ_Sprintf(tostBuf *ppstBuf, struct s_unz_seg *p_unz)
{
  int n;
  char lachTmpBuf[BUF_LEN];
#ifdef DEBUG  
  fprintf(stderr, "UNZ\n");
#endif
  n = timm_sprintf (lachTmpBuf, "UNZ+%s+%s'\n", p_unz->v_0036, p_unz->v_0020);
  if (n <= 0)
    {
      return n;
    }

  return foiBuf_Append(ppstBuf, lachTmpBuf, n);
}

