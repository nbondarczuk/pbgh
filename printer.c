/**************************************************************************************************
 *                                                                                          
 * MODULE: PRINTER
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

static char *SCCS_VERSION = "1.0";

void fovdPrintVerInfoPrinter (void)
{
  printf ("%s\t\t%s\n", __FILE__, SCCS_VERSION);
}

#define TMP_DIR "/WORK/TMP"

/********************************************************************/
/*    Version of the Parser - insert new ones in the array here     */
/********************************************************************/
extern int aiParsVersion[];

char *bgh_input;      /* input string */
char *bgh_input_ptr;  /* current position in the input string */
long bgh_input_lim;   /* end of data */

/* One (global) pointer on pointer and a simple pointer on every
 * segment and group to be processed from the timmessge
 * (The 'strange' pointer on pointer is always set to the position
 * where the next element of this type is to be attached at
 * The simple pointer is mainly used for simplify the refrencing
 */

extern int SegNr;
extern char *SegName;

static struct s_alc_seg **pp_alc,*p_alc; /* pointers on struct that can hold  */
                                         /* thats able to hold the info of an */
                                         /* ALC Segment. (see bgh_data.h)     */
static struct s_bgm_seg **pp_bgm,*p_bgm;
static struct s_cnt_seg **pp_cnt,*p_cnt;
static struct s_cta_seg **pp_cta,*p_cta;
static struct s_com_seg **pp_com,*p_com;
static struct s_cux_seg **pp_cux,*p_cux;
static struct s_doc_seg **pp_doc,*p_doc;
static struct s_dtm_seg **pp_dtm,*p_dtm;
static struct s_fii_seg **pp_fii,*p_fii;
static struct s_ftx_seg **pp_ftx,*p_ftx;
static struct s_imd_seg **pp_imd,*p_imd;
static struct s_lin_seg **pp_lin,*p_lin;
static struct s_loc_seg **pp_loc,*p_loc;
static struct s_nad_seg **pp_nad,*p_nad;
static struct s_moa_seg **pp_moa,*p_moa;
static struct s_pat_seg **pp_pat,*p_pat;
static struct s_pcd_seg **pp_pcd,*p_pcd;
static struct s_pia_seg **pp_pia,*p_pia;
static struct s_pri_seg **pp_pri,*p_pri;
static struct s_qty_seg **pp_qty,*p_qty;
static struct s_rff_seg **pp_rff,*p_rff;
static struct s_tax_seg **pp_tax,*p_tax;
static struct s_unh_seg **pp_unh,*p_unh;
static struct s_unb_seg **pp_unb,*p_unb;
static struct s_uns_seg **pp_uns,*p_uns;
static struct s_unt_seg **pp_unt,*p_unt;
static struct s_unz_seg **pp_unz,*p_unz;
static struct s_xcd_seg **pp_xcd,*p_xcd;
static struct s_group_1 **pp_g1,*p_g1;
static struct s_group_2 **pp_g2,*p_g2;
static struct s_group_3 **pp_g3,*p_g3;
static struct s_group_4 **pp_g4,*p_g4;
static struct s_group_5 **pp_g5,*p_g5;
static struct s_group_6 **pp_g6,*p_g6;
static struct s_group_7 **pp_g7,*p_g7;
static struct s_group_8 **pp_g8,*p_g8;
static struct s_group_22 **pp_g22,*p_g22;
static struct s_group_23 **pp_g23,*p_g23;
static struct s_group_25 **pp_g25,*p_g25;
static struct s_group_26 **pp_g26,*p_g26;
static struct s_group_30 **pp_g30,*p_g30;
static struct s_group_31 **pp_g31,*p_g31;
static struct s_group_35 **pp_g35,*p_g35;
static struct s_group_37 **pp_g37,*p_g37;
static struct s_group_38 **pp_g38,*p_g38;
static struct s_group_40 **pp_g40,*p_g40;
static struct s_group_45 **pp_g45,*p_g45;
static struct s_group_47 **pp_g47,*p_g47;
static struct s_group_99 **pp_g99,*p_g99;

static struct s_timm_message **pp_timm,*p_timm; /* There can be more than one timm  */
                                                /*message embedded in a so called  */
                                                /* interchange.                     */

static struct s_TimmInter *p_inter;  /*=&interchange;*/

/* One temporary fixpoint for every segment in the timmessage
 * In reality only used for segments that start groups
 * For example:  LIN Segement in Group22 or CTA in Group 5
 * (I have to read and to fill the Segment befor i know that i have
 * to allocate a new Group22 where the LIN Segemnt is then attached
 * at. (In the meantime its attached at hold.tmp_lin.)
 */

struct anker{
  struct s_alc_seg *tmp_alc;
  struct s_bgm_seg *tmp_bgm;
  struct s_cnt_seg *tmp_cnt;
  struct s_cta_seg *tmp_cta;
  struct s_com_seg *tmp_com;
  struct s_cux_seg *tmp_cux;
  struct s_doc_seg *tmp_doc;
  struct s_dtm_seg *tmp_dtm;
  struct s_fii_seg *tmp_fii;
  struct s_ftx_seg *tmp_ftx;
  struct s_imd_seg *tmp_imd;
  struct s_lin_seg *tmp_lin;
  struct s_loc_seg *tmp_loc;
  struct s_nad_seg *tmp_nad;
  struct s_moa_seg *tmp_moa;
  struct s_pat_seg *tmp_pat;
  struct s_pia_seg *tmp_pia;
  struct s_pcd_seg *tmp_pcd;
  struct s_pri_seg *tmp_pri;
  struct s_qty_seg *tmp_qty;
  struct s_rff_seg *tmp_rff;
  struct s_tax_seg *tmp_tax;
  struct s_unb_seg *tmp_unb;
  struct s_unh_seg *tmp_unh;
  struct s_uns_seg *tmp_uns;
  struct s_unt_seg *tmp_unt;
  struct s_unz_seg *tmp_unz;
  struct s_xcd_seg *tmp_xcd;
} hold;

/* Since the timmnorm defines a maximum number for every item in the     */
/* timmessage we have to count them while attaching and eventually       */
/* generate a warning or error,if the real_number exceeds the max_number */

int alc_maxanz,alc_realanz;
int bgm_maxanz,bgm_realanz;
int com_maxanz,com_realanz;
int cnt_maxanz,cnt_realanz;
int cta_maxanz,cta_realanz;
int cux_maxanz,cux_realanz;
int doc_maxanz,doc_realanz;
int dtm_maxanz,dtm_realanz;
int fii_maxanz,fii_realanz;
int ftx_maxanz,ftx_realanz;
int imd_maxanz,imd_realanz;
int lin_maxanz,lin_realanz;
int loc_maxanz,loc_realanz;
int nad_maxanz,nad_realanz;
int moa_maxanz,moa_realanz;
int pat_maxanz,pat_realanz;
int pia_maxanz,pia_realanz;
int pcd_maxanz,pcd_realanz;
int pri_maxanz,pri_realanz;
int qty_maxanz,qty_realanz;
int rff_maxanz,rff_realanz;
int tax_maxanz,tax_realanz;
int unb_maxanz,unb_realanz;
int unh_maxanz,unh_realanz;
int uns_maxanz,uns_realanz;
int unt_maxanz,unt_realanz;
int unz_maxanz,unz_realanz;
int xcd_maxanz,xcd_realanz;
int g_1_maxanz,g_1_realanz;
int g_2_maxanz,g_2_realanz;
int g_3_maxanz,g_3_realanz;
int g_4_maxanz,g_4_realanz;
int g_5_maxanz,g_5_realanz;
int g_6_maxanz,g_6_realanz;
int g_7_maxanz,g_7_realanz;
int g_8_maxanz,g_8_realanz;
int g_22_maxanz,g_22_realanz;
int g_23_maxanz,g_23_realanz;
int g_25_maxanz,g_25_realanz;
int g_26_maxanz,g_26_realanz;
int g_30_maxanz,g_30_realanz;
int g_31_maxanz,g_31_realanz;
int g_35_maxanz,g_35_realanz;
int g_37_maxanz,g_37_realanz;
int g_38_maxanz,g_38_realanz;
int g_40_maxanz,g_40_realanz;
int g_45_maxanz,g_45_realanz;
int g_47_maxanz,g_47_realanz;
int g_99_maxanz,g_99_realanz;

int foiBuf_Sprintf(tostBuf *ppstBuf, struct s_TimmInter *p_inter_print)
{
  int i, j, k, l, n, rc;
  
  p_unb=p_inter_print->unb;    
  rc = foiBuf_UNB_Sprintf(ppstBuf, p_unb);
  if (rc <= 0)
    {
      return -1;
    }
  
  for(p_timm = p_inter_print->timm; p_timm; p_timm = p_timm->timm_next) 
    {
      p_unh=p_timm->unh;
      rc = foiBuf_UNH_Sprintf(ppstBuf, p_unh);
      if (rc <= 0)
        {
          return -1;
        }
      
      p_bgm=p_timm->bgm;
      rc = foiBuf_BGM_Sprintf(ppstBuf, p_bgm);
      if (rc <= 0)
        {
          return -1;
        }
            
      for (i=1, p_dtm=p_timm->dtm; p_dtm; p_dtm=p_dtm->dtm_next, i++) 
        {
          rc = foiBuf_DTM_Sprintf(ppstBuf, p_dtm);
          if (rc <= 0)
            {
              return -1;
            }
        }
      
      for (i=1, p_ftx=p_timm->ftx; p_ftx; p_ftx=p_ftx->ftx_next, i++) 
        {
          rc = foiBuf_FTX_Sprintf(ppstBuf, p_ftx);
          if (rc <= 0)
            {
              return -1;
            }
        }
      
      for (i=1, p_g1=p_timm->g_1; p_g1; p_g1=p_g1->g_1_next, i++)   
        {
          p_rff = p_g1->rff;
          rc = foiBuf_RFF_Sprintf(ppstBuf, p_rff);
          if (rc <= 0)
            {
              return -1;
            }          
        }

      for (i=1, p_g2=p_timm->g_2; p_g2; p_g2=p_g2->g_2_next, i++) 
        {
          p_nad = p_g2->nad;
          p_fii = p_g2->fii;
          p_g3  = p_g2->g_3;
          p_g4  = p_g2->g_4;
          p_g5  = p_g2->g_5;
          
          rc = foiBuf_NAD_Sprintf(ppstBuf, p_nad);
          if (rc <= 0)
            {
              return -1;
            }          
          
          for (j=1;p_fii;p_fii=p_fii->fii_next, j++) 
            {
              rc = foiBuf_FII_Sprintf(ppstBuf, p_fii);
              if (rc <= 0)
                {
                  return -1;
                }          
            }
          
          for (j=1;p_g3;p_g3=p_g3->g_3_next,j++) 
            {
              p_rff = p_g3->rff;
              rc = foiBuf_RFF_Sprintf(ppstBuf, p_rff);
              if (rc <= 0)
                {
                  return -1;
                }          
            }
          
          for (j=1; p_g4; p_g4=p_g4->g_4_next, j++) 
            {
              p_doc = p_g4->doc;
              rc = foiBuf_DOC_Sprintf(ppstBuf, p_doc);
              if (rc <= 0)
                {
                  return -1;
                }                        
            }
          
          for (j=1; p_g5; p_g5=p_g5->g_5_next, j++) 
            {
              p_cta = p_g5->cta;
              rc = foiBuf_CTA_Sprintf(ppstBuf, p_cta);
              if (rc <= 0)
                {
                  return -1;
                }                        
              
              for (k=1, p_com=p_g5->com; p_com;p_com=p_com->com_next, k++) 
                {
                  rc = foiBuf_COM_Sprintf(ppstBuf, p_com);
                  if (rc <= 0)
                    {
                      return -1;
                    }                                          
                }
            }
        }

      for (i=1, p_g6=p_timm->g_6;p_g6; p_g6=p_g6->g_6_next, i++) 
        {
          p_tax = p_g6->tax;
          rc = foiBuf_TAX_Sprintf(ppstBuf, p_tax);
          if (rc <= 0)
            {
              return -1;
            }                                          
        }

      for (i=1,p_g7=p_timm->g_7;p_g7;p_g7=p_g7->g_7_next,i++) 
        {
          p_cux = p_g7->cux;
          rc = foiBuf_CUX_Sprintf(ppstBuf, p_cux);
          if (rc <= 0)
            {
              return -1;
            }                                          
        }

    for (i=1, p_g8=p_timm->g_8; p_g8; p_g8=p_g8->g_8_next, i++) 
      {
        p_pat = p_g8->pat;
        rc = foiBuf_PAT_Sprintf(ppstBuf, p_pat);
        if (rc <= 0)
          {
            return -1;
          }                                          
        
        for (j=1, p_dtm=p_g8->dtm; p_dtm; p_dtm=p_dtm->dtm_next, j++) 
          {
            rc = foiBuf_DTM_Sprintf(ppstBuf, p_dtm);
            if (rc <= 0)
              {
                return -1;
              }                                          
          }
      }

    for (i=1, p_g22=p_timm->g_22; p_g22; p_g22=p_g22->g_22_next, i++) 
      {
        p_lin = p_g22->lin;
        p_pia = p_g22->pia;
        p_imd = p_g22->imd;
        p_qty = p_g22->qty;
        p_dtm = p_g22->dtm;
        p_ftx = p_g22->ftx;
        p_g23 = p_g22->g_23;
        p_g25 = p_g22->g_25;
        p_g26 = p_g22->g_26;
        p_g30 = p_g22->g_30;
        p_g31 = p_g22->g_31;
        p_g35 = p_g22->g_35;
        p_g99 = p_g22->g_99;
        
        rc = foiBuf_LIN_Sprintf(ppstBuf, p_lin);
        if (rc <= 0)
          {
            return -1;
          }                                          

        for (j=1; p_pia; p_pia=p_pia->pia_next, j++) 
          {
            rc = foiBuf_PIA_Sprintf(ppstBuf, p_pia);
            if (rc <= 0)
              {
                return -1;
              }                                          
          }

        for (j=1; p_imd; p_imd=p_imd->imd_next, j++) 
          {
            rc = foiBuf_IMD_Sprintf(ppstBuf, p_imd);
            if (rc <= 0)
              {
                return -1;
              }                                          
          }

        for (j=1;p_qty;p_qty=p_qty->qty_next, j++) 
          {
            rc = foiBuf_QTY_Sprintf(ppstBuf, p_qty);
            if (rc <= 0)
              {
                return -1;
              }                                                      
          }

        for (j=1; p_dtm; p_dtm=p_dtm->dtm_next, j++) 
          {
            rc = foiBuf_DTM_Sprintf(ppstBuf, p_dtm);
            if (rc <= 0)
              {
                return -1;
              }                                                      
          }
        
        for (j=1;p_ftx;p_ftx=p_ftx->ftx_next,j++) 
          {
            rc = foiBuf_FTX_Sprintf(ppstBuf, p_ftx);
            if (rc <= 0)
              {
                return -1;
              }                                                      
          }
        
        for (j=1; p_g23; p_g23=p_g23->g_23_next, j++) 
          {
            /*p_moa = p_g23->moa;*/
            for (k=1, p_moa = p_g23->moa;p_moa; p_moa=p_moa->moa_next, k++) 
              {
                rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
                if (rc <= 0)
                  {
                    return -1;
                  }                                                                      
              }
          }

        for (j=1; p_g25; p_g25=p_g25->g_25_next, j++) 
          {
            p_pri = p_g25->pri;
            if (p_pri)
              {
                rc = foiBuf_PRI_Sprintf(ppstBuf, p_pri);
                if (rc <= 0)
                  {
                    return -1;
                  }        
              }

          }
        
        for (j=1;p_g26;p_g26=p_g26->g_26_next, j++) 
          {
            p_rff = p_g26->rff;
            rc = foiBuf_RFF_Sprintf(ppstBuf, p_rff);
            if (rc <= 0)
              {
                return -1;
              }                                                                      
          }
        
        for (j=1;p_g30;p_g30=p_g30->g_30_next, j++) 
          {
            p_tax = p_g30->tax;
            rc = foiBuf_TAX_Sprintf(ppstBuf, p_tax);
            if (rc <= 0)
              {
                return -1;
              }                                                                      

            for (k=1,p_moa=p_g30->moa;p_moa;p_moa=p_moa->moa_next, k++) 
              {
                rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
                if (rc <= 0)
                  {
                    return -1;
                  }                                                                      
              }
          }

        for (j=1;p_g31;p_g31=p_g31->g_31_next, j++) 
          {
            p_nad = p_g31->nad;
            rc = foiBuf_NAD_Sprintf(ppstBuf, p_nad);
            if (rc <= 0)
              {
                return -1;
              }                                                                      
                      
            for (k=1, p_loc=p_g31->loc; p_loc; p_loc=p_loc->loc_next, k++) 
              {
                rc = foiBuf_LOC_Sprintf(ppstBuf, p_loc);
                if (rc <= 0)
                  {
                    return -1;
                  }                                                                      
              }
          }

      for (j=1; p_g35; p_g35=p_g35->g_35_next, j++) 
        {
          k=1;
          p_alc = p_g35->alc;
          p_g37 = p_g35->g_37;
          p_g38 = p_g35->g_38;
          p_g40 = p_g35->g_40;

          rc = foiBuf_ALC_Sprintf(ppstBuf, p_alc);
          if (rc <= 0)
            {
              return -1;
            }                                                                      
          
          if(p_g37 != NULL) 
            {
              p_pcd = p_g37->pcd;
            } 
          else 
            {
              p_pcd = NULL;
            }
          if (p_pcd) 
            {
              rc = foiBuf_PCD_Sprintf(ppstBuf, p_pcd);
              if (rc <= 0)
                {
                  return -1;
                }                                                                                    
            }

          for (k=1;p_g38;p_g38=p_g38->g_38_next, k++) 
            {
              /* p_moa=p_g38->moa;*/
              for (l=1, p_moa=p_g38->moa;p_moa; p_moa=p_moa->moa_next, l++) 
                {
                  rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
                  if (rc <= 0)
                    {
                      return -1;
                    }                                                                      
                }
            }

          for (k=1;p_g40;p_g40=p_g40->g_40_next, k++) 
            {
              p_tax=p_g40->tax;
              rc = foiBuf_TAX_Sprintf(ppstBuf, p_tax);
              if (rc <= 0)
                {
                  return -1;
                }                                                                      
              
              for (l=1,p_moa=p_g40->moa;p_moa;p_moa=p_moa->moa_next, l++) 
                {
                  rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
                  if (rc <= 0)
                    {
                      return -1;
                    }                                                                                        
                }
            }
        }

      for (j=1;p_g99;p_g99=p_g99->g_99_next, j++) 
        {
          p_xcd = p_g99->xcd;
          rc = foiBuf_XCD_Sprintf(ppstBuf, p_xcd);
          if (rc <= 0)
            {
              return -1;
            }                                                                                                  
        }      
      }
    
    p_uns=p_timm->uns;
    rc = foiBuf_UNS_Sprintf(ppstBuf, p_uns);
    if (rc <= 0)
      {
        return -1;
      }                                                                                                      

    for (i=1, p_cnt=p_timm->cnt; p_cnt; p_cnt=p_cnt->cnt_next, i++) 
      {
        rc = foiBuf_CNT_Sprintf(ppstBuf, p_cnt);
        if (rc <= 0)
          {
            return -1;
          }
      }
    
    for (i=1, p_g45=p_timm->g_45; p_g45; p_g45=p_g45->g_45_next, i++) 
      {
        p_moa = p_g45->moa;
        for (j=1, p_moa = p_g45->moa; p_moa; p_moa=p_moa->moa_next, j++) 
          {
            rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
            if (rc <= 0)
              {
                return -1;
              }
          }
      }
    
    for (i=1, p_g47=p_timm->g_47;p_g47; p_g47=p_g47->g_47_next, i++) 
      {
        p_tax = p_g47->tax;
        rc = foiBuf_TAX_Sprintf(ppstBuf, p_tax);
        if (rc <= 0)
          {
            return -1;
          }
        
        for (j=1, p_moa=p_g47->moa; p_moa; p_moa=p_moa->moa_next, j++) 
          {
            rc = foiBuf_MOA_Sprintf(ppstBuf, p_moa);
            if (rc <= 0)
              {
                return -1;
              }
          }
      }

    p_unt=p_timm->unt;
    rc = foiBuf_UNT_Sprintf(ppstBuf, p_unt);
    if (rc <= 0)
      {
        return -1;
      }
        
    } /* of interchange */
  
  p_unz=p_inter_print->unz;
  rc = foiBuf_UNZ_Sprintf(ppstBuf, p_unz);
  if (rc <= 0)
    {
      return -1;
    }
  
  return 1;
}

toenBool foenPrinter_Sprintf(char **ppchzTmpStr, struct s_TimmInter *ppstTimmInt)
{
  char *ppchzTimmStr;
  tostBuf postTmpBuf;
  int rc, i;

#ifdef _DEBUG_
  fprintf(stderr, "Printer_Sprintf: New buffer\n");
#endif
  rc = foiBuf_New(&postTmpBuf, 4048);
  if (rc == -1)
    {
      return FALSE;
    }

#ifdef _DEBUG_
  fprintf(stderr, "Printer_Fprintf: Buf_Sprintf\n");
#endif
  rc = foiBuf_Sprintf(&postTmpBuf, ppstTimmInt);
  if (rc == -1)
    {
      return FALSE;
    }

  *ppchzTmpStr = postTmpBuf.spchBuf;

#ifdef ___DEBUG_    
  for (i = 0; i < strlen(*ppchzTmpStr); i++)
    {
      putc((*ppchzTmpStr)[i], stderr);
    }
#endif

  return TRUE;
}

toenBool foenPrinter_Fprintf(FILE *ppfilOut, struct s_TimmInter *ppstTimmInt)
{
  char *ppchzTimmStr;
  tostBuf postTmpBuf;
  int rc, i;
  toenBool loenStatus;

#ifdef _DEBUG_
  fprintf(stderr, "Printer_Fprintf: Printing to file\n");
#endif
  loenStatus = foenPrinter_Sprintf(&ppchzTimmStr, ppstTimmInt);
  if (loenStatus == FALSE)
    {
      return FALSE;
    }

  fputs(ppchzTimmStr, ppfilOut);

  return TRUE;
}

toenBool foenPrinter_Printf(struct s_TimmInter *ppstTimmInt)
{
  return foenPrinter_Fprintf(stdout, ppstTimmInt);
}

void print_g22(struct s_group_22 *p_g22)
{
  int i, j, k, l;

  p_lin = p_g22->lin;
  p_pia = p_g22->pia;
  p_imd = p_g22->imd;
  p_qty = p_g22->qty;
  p_dtm = p_g22->dtm;
  p_ftx = p_g22->ftx;
  p_g23  = p_g22->g_23;
  p_g25  = p_g22->g_25;
  p_g26  = p_g22->g_26;
  p_g30  = p_g22->g_30;
  p_g31  = p_g22->g_31;
  p_g35  = p_g22->g_35;
  p_g99  = p_g22->g_99;

  fprintf(stderr,"G22:\n");

  fprintf (stderr, "LIN+%s++%s:%s++%s'\n", p_lin->v_1082,p_lin->v_7140, p_lin->v_7143,p_lin->v_1222);
  for (j=1;p_pia;p_pia=p_pia->pia_next, j++) 
    {
      fprintf (stderr, "PIA+%s+%s'\n", p_pia->v_4347, p_pia->v_7140);
    }

  for (j=1;p_imd;p_imd=p_imd->imd_next, j++) 
    {
      fprintf (stderr, "IMD++%s+%s:::%s:%s'\n", p_imd->v_7081,p_imd->v_7009,p_imd->v_7008,p_imd->v_7008a);
    }

  for (j=1;p_qty;p_qty=p_qty->qty_next, j++) 
    {
      fprintf (stderr, "QTY+%s:%s:%s'\n", p_qty->v_6063,p_qty->v_6060,p_qty->v_6411);
    }

  for (j=1;p_dtm;p_dtm=p_dtm->dtm_next, j++) 
    {
      printf ("DTM+%s:%s:%s'\n", p_dtm->v_2005,p_dtm->v_2380,p_dtm->v_2379);
    }

  for (j=1;p_ftx;p_ftx=p_ftx->ftx_next,j++) 
    {
      fprintf (stderr, "FTX+%s++%s+%s:%s:%s:%s:%s:%s'\n", 
              p_ftx->v_4451,p_ftx->v_4441,p_ftx->v_4440,
              p_ftx->v_4440a,p_ftx->v_4440b,p_ftx->v_4440c,p_ftx->v_4440d,p_ftx->v_4440e);
    }

  for (j=1;p_g23;p_g23=p_g23->g_23_next, j++) 
    {
      /*p_moa = p_g23->moa;*/
      for (k=1, p_moa = p_g23->moa;p_moa; p_moa=p_moa->moa_next, k++) 
        {
          fprintf (stderr, "MOA+%s:%s:%s:%s:%s'\n", p_moa->v_5025,p_moa->v_5004, p_moa->v_6345, p_moa->v_6343, p_moa->v_4405);
        }
    }

  for (j=1;p_g25;p_g25=p_g25->g_25_next, j++) 
    {
      p_pri = p_g25->pri;
      fprintf (stderr, "PRI+%s:%s:%s'\n", p_pri->v_5125,p_pri->v_5118, p_pri->v_5375);
    }

  for (j=1;p_g26;p_g26=p_g26->g_26_next, j++) 
    {
      p_rff = p_g26->rff;
      printf ("RFF+%s:%s'\n", p_rff->v_1153,p_rff->v_1154);
    }

  for (j=1;p_g30;p_g30=p_g30->g_30_next, j++) 
    {
      p_tax = p_g30->tax;
      fprintf (stderr, "TAX+%s+%s:::%s+++:::%s+%s'\n",
              p_tax->v_5283,p_tax->v_5153, p_tax->v_5152,p_tax->v_5278, p_tax->v_5305);
      for (k=1,p_moa=p_g30->moa;p_moa;p_moa=p_moa->moa_next, k++) 
        {
          fprintf (stderr, "MOA+%s:%s:%s:%s:%s'\n",
                  p_moa->v_5025,p_moa->v_5004, p_moa->v_6345,
                  p_moa->v_6343, p_moa->v_4405);
        }
    }

  for (j=1;p_g31;p_g31=p_g31->g_31_next, j++) 
    {
      p_nad = p_g31->nad;
      printf("NAD+%s+::::+%s:%s:%s:%s:%s+::+%s+%s+%s'\n",
             p_nad->v_3035,  p_nad->v_3036,  p_nad->v_3036a,
             p_nad->v_3036b, p_nad->v_3036c, p_nad->v_3036d,
             p_nad->v_3164,  p_nad->v_3251,  p_nad->v_3207);
      
      for (k=1,p_loc=p_g31->loc;p_loc;p_loc=p_loc->loc_next, k++) 
        {
          printf ("LOC+%s+:::+%s:::%s+:::+'\n",
                  p_loc->v_3227, p_loc->v_3223, p_loc->v_3222);
        }
    }

  for (j=1;p_g35;p_g35=p_g35->g_35_next, j++) 
    {
      k=1;
      p_alc = p_g35->alc;
      p_g37 = p_g35->g_37;
      p_g38 = p_g35->g_38;
      p_g40 = p_g35->g_40;
      printf("ALC+%s'\n", p_alc->v_5463);

      if(p_g37 != NULL) 
        {
          p_pcd = p_g37->pcd;
        } 
      else 
        {
          p_pcd = NULL;
        }

      if (p_pcd) 
        {
          printf("PCD+%s:%s'\n", p_pcd->v_5245,p_pcd->v_5482);
        }

      for (k=1;p_g38;p_g38=p_g38->g_38_next, k++) 
        {
          /* p_moa=p_g38->moa;*/
          for (l=1, p_moa=p_g38->moa;p_moa; p_moa=p_moa->moa_next, l++) 
            {
              printf ("MOA+%s:%s:%s:%s:%s'\n",
                      p_moa->v_5025,p_moa->v_5004, p_moa->v_6345,
                      p_moa->v_6343, p_moa->v_4405);
            }
        }

      for (k=1;p_g40;p_g40=p_g40->g_40_next, k++) 
        {
          p_tax=p_g40->tax;
          printf ("TAX+%s+%s:::%s+++:::%s+%s'\n",
                  p_tax->v_5283,p_tax->v_5153, p_tax->v_5152,
                  p_tax->v_5278, p_tax->v_5305);
          
          for (l=1,p_moa=p_g40->moa;p_moa;p_moa=p_moa->moa_next, l++) 
            {
              printf ("MOA+%s:%s:%s:%s:%s'\n",
                      p_moa->v_5025,p_moa->v_5004, p_moa->v_6345,
                      p_moa->v_6343, p_moa->v_4405);
            }
        }
    }

  for (j=1;p_g99;p_g99=p_g99->g_99_next, j++) 
    {
      p_xcd = p_g99->xcd;
      printf ("XCD+%s+%s+%s+%s+%s+%s+%s+%s+%s+"
              "5004c=%s+6345c=%s+6343c=%s+"
              "%s+%s+%s+%s+%s+%s+%s+%s+%s+"
              "%s+%s+%s+%s+%s+%s+%s+%s+"
              "%s:%s:%s:%s:%s:%s+"
              "%s:%s:%s:%s+%s+%s'\n",
              p_xcd->v_X001, p_xcd->v_X002, p_xcd->v_X003, 
              p_xcd->v_X004, p_xcd->v_X005, p_xcd->v_X006, 
              p_xcd->v_X006a, p_xcd->v_6411, p_xcd->v_X007, 
              p_xcd->v_5004c, p_xcd->v_6345c, p_xcd->v_6343c,      
              p_xcd->v_5004, p_xcd->v_6345, p_xcd->v_6343, 
              p_xcd->v_X008, p_xcd->v_X009, p_xcd->v_X009a, 
              p_xcd->v_X010, p_xcd->v_X011, p_xcd->v_X012, 
              p_xcd->v_X013, p_xcd->v_X014, p_xcd->v_X015,
              p_xcd->v_X016, p_xcd->v_X017, p_xcd->v_X017a, 
              p_xcd->v_X018, p_xcd->v_X019, 
              p_xcd->v_5004a, p_xcd->v_6345a, p_xcd->v_6343a, 
              p_xcd->v_5004b, p_xcd->v_6345b, p_xcd->v_6343b, 
              p_xcd->v_X020, p_xcd->v_X021, p_xcd->v_X023, 
              p_xcd->v_X024a, p_xcd->v_X025, p_xcd->v_X026);
    }
}
