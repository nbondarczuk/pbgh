/**************************************************************************************************
 *                                                                                          
 * MODULE: MAP
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 15.05.98                                              
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
#include "rtx.h"

static char *SCCS_VERSION = "1.0";

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

/*
 * Mapping with MPULKTMM
 */

extern stMPULKTMM *pstMPULKTMM;
extern long glTMMCount;

extern long folMapTMShdes2Code(char *);
extern long folMapSNShdes2Code(char *);
extern long folMapSPShdes2Code(char *);

#define MATCH_TMM(s, tm, tmver, sp, sn) \
s.soilTMCODE == tm && \
s.soilVSCODE == tmver && \
s.soilSPCODE == sp && \
s.soilSNCODE == sn 

long foilMap_Service2RICode(
                            char *pasnzTmShdes,
                            char *pasnzTmVer,
                            char *pasnzSpShdes,
                            char *pasnzSnShdes,
                            tostUsageItem *ppstUI
                            )
{
  long loilTM, loilTMVer, loilSP, loilSN, i;
  
  loilTM = folMapTMShdes2Code(pasnzTmShdes);
  loilTMVer = atol(pasnzTmVer);
  loilSP = folMapSPShdes2Code(pasnzSpShdes);
  loilSN = folMapSNShdes2Code(pasnzSnShdes);
  
  for (i = 0; i < glTMMCount; i++)
    {
      if (MATCH_TMM(pstMPULKTMM[i], loilTM, loilTMVer, loilSP, loilSN))
        {
          ppstUI->solTMCODE = loilTM;
          ppstUI->solSPCODE = loilSP;
          ppstUI->solSNCODE = loilSN;

          return pstMPULKTMM[i].soilRICODE;
        }
    }
  
  return -1;
}

long foilMap_Service2EvCode(
                            char *pasnzTmShdes,
                            char *pasnzTmVer,
                            char *pasnzSpShdes,
                            char *pasnzSnShdes,
                            tostUsageItem *ppstUI
                            )
{
  long loilTM, loilTMVer, loilSP, loilSN, i;
  
  loilTM = folMapTMShdes2Code(pasnzTmShdes);
  loilTMVer = atol(pasnzTmVer);
  loilSP = folMapSPShdes2Code(pasnzSpShdes);
  loilSN = folMapSNShdes2Code(pasnzSnShdes);
  
  for (i = 0; i < glTMMCount; i++)
    {
      if (MATCH_TMM(pstMPULKTMM[i], loilTM, loilTMVer, loilSP, loilSN))
        {
          ppstUI->solTMCODE = loilTM;
          ppstUI->solSPCODE = loilSP;
          ppstUI->solSNCODE = loilSN;

          return pstMPULKTMM[i].soilEVCODE;
        }
    }
  
  return -1;
}

/*
 * Mapping with MPULKRIM
 */

extern stMPULKRIM *pstMPULKRIM;
extern long glRIMCount;

#define MATCH_RI(st, type, ri, river, zn, tt) \
st.sochTYPEIND == type && \
st.soilRICODE == ri && \
st.soilVSCODE == river && \
st.soilZNCODE == zn && \
st.soilTTCODE == tt 

#define MATCH_RINOZNTT(st, type, ri, river) \
st.sochTYPEIND == type && \
st.soilRICODE == ri && \
st.soilVSCODE == river 

toenBool foenMap_FindRI(
                        char *pasnzTypeind,
                        long poilRICode, 
                        char *pasnzRIVer,
                        char *pasnzTTShdes,
                        char *pasnzZNShdes,
                        tostRatingInterval *ppstRI,
                        tostUsageItem *ppstUI
                        )
{
  long loilRIVer, loilZN, loilTT, i;
  
  loilRIVer = atol(pasnzRIVer);

  if (pasnzTTShdes == NULL && pasnzZNShdes == NULL)
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "MPULKRIM: Looking for: %c.%ld.%ld\n",
                  pasnzTypeind[0],
                  poilRICode, 
                  loilRIVer);
        }

      ppstUI->solZNCODE = loilZN;
      ppstUI->solTTCODE = loilTT;          
      
      for (i = 0; i < glRIMCount; i++)
        {
          if (MATCH_RINOZNTT(pstMPULKRIM[i], pasnzTypeind[0], poilRICode, loilRIVer))
            {
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, 
                          "MPULKRIM: Found: [IL:%8.2lf\tIR:%8.2lf\tIM:%3ld\tIC:%8.2lf]\n",
                          pstMPULKRIM[i].soflIL01,
                          pstMPULKRIM[i].soflIR01,
                          pstMPULKRIM[i].soiIM01,
                          pstMPULKRIM[i].soflIC01
                          );
                }
              
              ppstRI[0].sodIL = pstMPULKRIM[i].soflIL01;
              ppstRI[0].sodIR = pstMPULKRIM[i].soflIR01;
              ppstRI[0].solIM = pstMPULKRIM[i].soiIM01;
              ppstRI[0].sodIC = pstMPULKRIM[i].soflIC01;          
              
              ppstRI[1].sodIL = pstMPULKRIM[i].soflIL02;
              ppstRI[1].sodIR = pstMPULKRIM[i].soflIR02;
              ppstRI[1].solIM = pstMPULKRIM[i].soiIM02;
              ppstRI[1].sodIC = pstMPULKRIM[i].soflIC02;          
              
              ppstRI[2].sodIL = pstMPULKRIM[i].soflIL03;
              ppstRI[2].sodIR = pstMPULKRIM[i].soflIR03;
              ppstRI[2].solIM = pstMPULKRIM[i].soiIM03;
              ppstRI[2].sodIC = pstMPULKRIM[i].soflIC03;          
              
              ppstRI[3].sodIL = pstMPULKRIM[i].soflIL04;
              ppstRI[3].sodIR = pstMPULKRIM[i].soflIR04;
              ppstRI[3].solIM = pstMPULKRIM[i].soiIM04;
              ppstRI[3].sodIC = pstMPULKRIM[i].soflIC04;          
              
              ppstRI[4].sodIL = pstMPULKRIM[i].soflIL05;
              ppstRI[4].sodIR = pstMPULKRIM[i].soflIR05;
              ppstRI[4].solIM = pstMPULKRIM[i].soiIM05;
              ppstRI[4].sodIC = pstMPULKRIM[i].soflIC05;          
              
              ppstRI[5].sodIL = pstMPULKRIM[i].soflIL06;
              ppstRI[5].sodIR = pstMPULKRIM[i].soflIR06;
              ppstRI[5].solIM = pstMPULKRIM[i].soiIM06;
              ppstRI[5].sodIC = pstMPULKRIM[i].soflIC06;          
              
              ppstRI[6].sodIL = pstMPULKRIM[i].soflIL07;
              ppstRI[6].sodIR = pstMPULKRIM[i].soflIR07;
              ppstRI[6].solIM = pstMPULKRIM[i].soiIM07;
              ppstRI[6].sodIC = pstMPULKRIM[i].soflIC07;          
              
              ppstRI[7].sodIL = pstMPULKRIM[i].soflIL08;
              ppstRI[7].sodIR = pstMPULKRIM[i].soflIR08;
              ppstRI[7].solIM = pstMPULKRIM[i].soiIM08;
              ppstRI[7].sodIC = pstMPULKRIM[i].soflIC08;          
              
              ppstRI[8].sodIL = pstMPULKRIM[i].soflIL09;
              ppstRI[8].sodIR = pstMPULKRIM[i].soflIR09;
              ppstRI[8].solIM = pstMPULKRIM[i].soiIM09;
              ppstRI[8].sodIC = pstMPULKRIM[i].soflIC09;          
              
              ppstRI[9].sodIL = pstMPULKRIM[i].soflIL10;
              ppstRI[9].sodIR = pstMPULKRIM[i].soflIR10;
              ppstRI[9].solIM = pstMPULKRIM[i].soiIM10;
              ppstRI[9].sodIC = pstMPULKRIM[i].soflIC10;                    
          
              return TRUE;
            }
        }
    }
  else
    {
      loilZN = folMapZNShdes2Code(pasnzZNShdes);
      loilTT = folMapTTShdes2Code(pasnzTTShdes);      
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "MPULKRIM: Looking for: %c.%ld.%ld.%ld.%ld\n",
                  pasnzTypeind[0],
                  poilRICode, 
                  loilRIVer,
                  loilTT,                   
                  loilZN);
        }

      ppstUI->solZNCODE = loilZN;
      ppstUI->solTTCODE = loilTT;          
      
      for (i = 0; i < glRIMCount; i++)
        {
          if (MATCH_RI(pstMPULKRIM[i], pasnzTypeind[0], poilRICode, loilRIVer, loilZN, loilTT))
            {
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, 
                          "MPULKRIM: Found: [IL:%.2lf, IR:%.2lf, IM:%ld, IC:%.2lf]\n",
                          pstMPULKRIM[i].soflIL01,
                          pstMPULKRIM[i].soflIR01,
                          pstMPULKRIM[i].soiIM01,
                          pstMPULKRIM[i].soflIC01
                          );
                }
              
              ppstRI[0].sodIL = pstMPULKRIM[i].soflIL01;
              ppstRI[0].sodIR = pstMPULKRIM[i].soflIR01;
              ppstRI[0].solIM = pstMPULKRIM[i].soiIM01;
              ppstRI[0].sodIC = pstMPULKRIM[i].soflIC01;          
              
              ppstRI[1].sodIL = pstMPULKRIM[i].soflIL02;
              ppstRI[1].sodIR = pstMPULKRIM[i].soflIR02;
              ppstRI[1].solIM = pstMPULKRIM[i].soiIM02;
              ppstRI[1].sodIC = pstMPULKRIM[i].soflIC02;          
              
              ppstRI[2].sodIL = pstMPULKRIM[i].soflIL03;
              ppstRI[2].sodIR = pstMPULKRIM[i].soflIR03;
              ppstRI[2].solIM = pstMPULKRIM[i].soiIM03;
              ppstRI[2].sodIC = pstMPULKRIM[i].soflIC03;          
              
              ppstRI[3].sodIL = pstMPULKRIM[i].soflIL04;
              ppstRI[3].sodIR = pstMPULKRIM[i].soflIR04;
              ppstRI[3].solIM = pstMPULKRIM[i].soiIM04;
              ppstRI[3].sodIC = pstMPULKRIM[i].soflIC04;          
              
              ppstRI[4].sodIL = pstMPULKRIM[i].soflIL05;
              ppstRI[4].sodIR = pstMPULKRIM[i].soflIR05;
              ppstRI[4].solIM = pstMPULKRIM[i].soiIM05;
              ppstRI[4].sodIC = pstMPULKRIM[i].soflIC05;          
              
              ppstRI[5].sodIL = pstMPULKRIM[i].soflIL06;
              ppstRI[5].sodIR = pstMPULKRIM[i].soflIR06;
              ppstRI[5].solIM = pstMPULKRIM[i].soiIM06;
              ppstRI[5].sodIC = pstMPULKRIM[i].soflIC06;          
              
              ppstRI[6].sodIL = pstMPULKRIM[i].soflIL07;
              ppstRI[6].sodIR = pstMPULKRIM[i].soflIR07;
              ppstRI[6].solIM = pstMPULKRIM[i].soiIM07;
              ppstRI[6].sodIC = pstMPULKRIM[i].soflIC07;          
              
              ppstRI[7].sodIL = pstMPULKRIM[i].soflIL08;
              ppstRI[7].sodIR = pstMPULKRIM[i].soflIR08;
              ppstRI[7].solIM = pstMPULKRIM[i].soiIM08;
              ppstRI[7].sodIC = pstMPULKRIM[i].soflIC08;          
              
              ppstRI[8].sodIL = pstMPULKRIM[i].soflIL09;
              ppstRI[8].sodIR = pstMPULKRIM[i].soflIR09;
              ppstRI[8].solIM = pstMPULKRIM[i].soiIM09;
              ppstRI[8].sodIC = pstMPULKRIM[i].soflIC09;          
              
              ppstRI[9].sodIL = pstMPULKRIM[i].soflIL10;
              ppstRI[9].sodIR = pstMPULKRIM[i].soflIR10;
              ppstRI[9].solIM = pstMPULKRIM[i].soiIM10;
              ppstRI[9].sodIC = pstMPULKRIM[i].soflIC10;          
                    
              return TRUE;
            }
        }
    }

  ppstRI[0].sodIL = 60.0;
  ppstRI[0].sodIR = 6.0;
  ppstRI[0].solIM = 1;
  ppstRI[0].sodIC = 0;

  ppstRI[1].solIM = 0;
  ppstRI[2].solIM = 0;
  ppstRI[3].solIM = 0;
  ppstRI[4].solIM = 0;
  ppstRI[5].solIM = 0;
  ppstRI[6].solIM = 0;
  ppstRI[7].solIM = 0;
  ppstRI[8].solIM = 0;

  ppstRI[9].sodIL = 60.0;
  ppstRI[9].sodIR = 6.0;
  ppstRI[9].solIM = 999;
  ppstRI[9].sodIC = 0;
                
  return TRUE;
}

extern stMPULKEVM *pstMPULKEVM;
extern long glEVMCount;

#define MATCH_EVM(s, evcode, evver, tt) \
s.soilEVCODE == evcode && \
s.soilVSCODE == evver && \
s.soilTTCODE == tt

#define MATCH_EVMNOTT(s, evcode, evver) \
s.soilEVCODE == evcode && \
s.soilVSCODE == evver 


toenBool foenMap_FindEv(
                        long poilEvCode, 
                        char *pasnzEvVer,
                        char *pasnzTTShdes,                        
                        tostEventPrice *ppstEP,
                        tostUsageItem *ppstUI
                        )
{
  long loilEvVer, loilTT, i;
  
  loilEvVer = atol(pasnzEvVer);  
       
  if (pasnzTTShdes != NULL)
    {
      loilTT = folMapTTShdes2Code(pasnzTTShdes);
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "MPULKEVM: Looking for: %ld.%ld.%ld\n",
                  poilEvCode, 
                  loilEvVer,
                  loilTT);
        }
      
      for (i = 0; i < glEVMCount; i++)
        {
          if (MATCH_EVM(pstMPULKEVM[i], poilEvCode, loilEvVer, loilTT))
            {
              ppstEP->sodEV_REG = pstMPULKEVM[i].soflEV_REG; 
              ppstEP->sodEV_ACT = pstMPULKEVM[i].soflEV_ACT;
              ppstEP->sodEV_DEA = pstMPULKEVM[i].soflEV_DEA;
              ppstEP->sodEV_INT = pstMPULKEVM[i].soflEV_INT;
              ppstEP->sodEV_INV = pstMPULKEVM[i].soflEV_INV;
              ppstEP->sodEV_ERA = pstMPULKEVM[i].soflEV_ERA;
              ppstEP->sodEV_PWC = pstMPULKEVM[i].soflEV_PWC;
              ppstUI->solTTCODE = loilTT;          

              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "MPULKEVM: Found: %ld.%ld\n", poilEvCode, loilEvVer);
                }
              
              return TRUE;
            }
        }
    }
  else
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "MPULKEVM: Looking for: %ld.%ld\n", poilEvCode, loilEvVer);
        }
  
      for (i = 0; i < glEVMCount; i++)
        {
          if (MATCH_EVMNOTT(pstMPULKEVM[i], poilEvCode, loilEvVer))
            {
              ppstEP->sodEV_REG = pstMPULKEVM[i].soflEV_REG; 
              ppstEP->sodEV_ACT = pstMPULKEVM[i].soflEV_ACT;
              ppstEP->sodEV_DEA = pstMPULKEVM[i].soflEV_DEA;
              ppstEP->sodEV_INT = pstMPULKEVM[i].soflEV_INT;
              ppstEP->sodEV_INV = pstMPULKEVM[i].soflEV_INV;
              ppstEP->sodEV_ERA = pstMPULKEVM[i].soflEV_ERA;
              ppstEP->sodEV_PWC = pstMPULKEVM[i].soflEV_PWC;

              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "MPULKEVM: Found: %ld.%ld\n", poilEvCode, loilEvVer);
                }
              
              return TRUE;
            }
        }
    }
  
  return FALSE;
}

extern long golMPUPUTAB_Length;
extern tostMPUPUTAB *gpstMPUPUTAB;

double foflMap_GetUP(char pochType, long poilTm)
{
  int i;
  double loflUP = 0.01;

  /*
  for (i = 0; i < golMPUPUTAB_Length; i++)
    {
      if (gpstMPUPUTAB[i].solTMCODE == poilTm || poilTm <= 0)
        {
          break;
        }
    }
  
  switch (pochType)
    {
    case 'L':
    case 'I':
      loflUP = gpstMPUPUTAB[i].soflPPUIC;
      break;
    case 'A':
      loflUP = gpstMPUPUTAB[i].soflPPUAIR;
      break;
    }
  */

  return loflUP;
}

