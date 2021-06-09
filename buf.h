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

#define BUF_LEN 1024
#define DOUBLE_BUF_LEN 2048

typedef struct
{
  int  soiMaxLen;
  int  soiStrLen;
  int  soiFree;
  char *spchBuf;
}
tostBuf;

int foiBuf_TimmCharRecode(char *, int, char **);
int foiBuf_Init(tostBuf *);
int foiBuf_New(tostBuf *, int);
int foiBuf_Append(tostBuf *, char *, int);

int foiBuf_UNB_Sprintf(tostBuf *, struct s_unb_seg *);
int foiBuf_UNH_Sprintf(tostBuf *, struct s_unh_seg *);
int foiBuf_BGM_Sprintf(tostBuf *, struct s_bgm_seg *);
int foiBuf_DTM_Sprintf(tostBuf *, struct s_dtm_seg *);
int foiBuf_FTX_Sprintf(tostBuf *, struct s_ftx_seg *);
int foiBuf_RFF_Sprintf(tostBuf *, struct s_rff_seg *);
int foiBuf_NAD_Sprintf(tostBuf *, struct s_nad_seg *);
int foiBuf_FII_Sprintf(tostBuf *, struct s_fii_seg *);
int foiBuf_DOC_Sprintf(tostBuf *, struct s_doc_seg *);
int foiBuf_CTA_Sprintf(tostBuf *, struct s_cta_seg *);
int foiBuf_COM_Sprintf(tostBuf *, struct s_com_seg *);
int foiBuf_TAX_Sprintf(tostBuf *, struct s_tax_seg *);
int foiBuf_CUX_Sprintf(tostBuf *, struct s_cux_seg *);
int foiBuf_PAT_Sprintf(tostBuf *, struct s_pat_seg *);
int foiBuf_LIN_Sprintf(tostBuf *, struct s_lin_seg *);
int foiBuf_PIA_Sprintf(tostBuf *, struct s_pia_seg *);
int foiBuf_IMD_Sprintf(tostBuf *, struct s_imd_seg *);
int foiBuf_QTY_Sprintf(tostBuf *, struct s_qty_seg *);
int foiBuf_MOA_Sprintf(tostBuf *, struct s_moa_seg *);
int foiBuf_PRI_Sprintf(tostBuf *, struct s_pri_seg *);
int foiBuf_LOC_Sprintf(tostBuf *, struct s_loc_seg *);
int foiBuf_ALC_Sprintf(tostBuf *, struct s_alc_seg *);
int foiBuf_PCD_Sprintf(tostBuf *, struct s_pcd_seg *);
int foiBuf_XCD_Sprintf(tostBuf *, struct s_xcd_seg *);
int foiBuf_UNS_Sprintf(tostBuf *, struct s_uns_seg *);
int foiBuf_CNT_Sprintf(tostBuf *, struct s_cnt_seg *);
int foiBuf_UNT_Sprintf(tostBuf *, struct s_unt_seg *);
int foiBuf_UNZ_Sprintf(tostBuf *, struct s_unz_seg *);
