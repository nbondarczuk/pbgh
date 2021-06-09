/**************************************************************************************************
 *                                                                                          
 * MODULE: BCH_CHARGE
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 15.03.98                                              
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


extern int fnGetTTTZRatingInterval(
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   tostRatingInterval *, 
                                   tostUsageItem *
                                   );


extern int fnGetNoTTTZRatingInterval(
                                     char *,
                                     char *,
                                     char *,
                                     char *,
                                     char *,
                                     char *,
                                     tostRatingInterval *, 
                                     tostUsageItem *
                                     );

extern int fnGetEventPrice(
                           char *,
                           char *,
                           char *,
                           char *,
                           char *,
                           char *,
                           char *,
                           tostEventPrice *, 
                           tostUsageItem *
                           );

extern int fnGetNoEGLEventPrice(
                                char *,
                                char *,
                                char *,
                                char *,
                                char *,
                                char *,
                                tostEventPrice *, 
                                tostUsageItem *
                                );

extern long foilMap_Service2RICode(
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   tostUsageItem *                                    
                                   );

extern long foilMap_Service2EvCode(
                                   char *,
                                   char *,
                                   char *,
                                   char *,
                                   tostUsageItem *                                    
                                   );

extern toenBool foenMap_FindRI(
                               char *,
                               long, 
                               char *,
                               char *,
                               char *,
                               tostRatingInterval *, 
                               tostUsageItem *
                               );


extern toenBool foenMap_FindEv(
                               long,
                               char *,
                               char *,
                               tostEventPrice *,
                               tostUsageItem *
                               );


extern tostMPUSNTAB *gpstMPUSNTAB;
extern long golMPUSNTAB_Length; 
extern int foiCompareMPSNTABRecord(const void *, const void *);

void fovdPrintVerInfoBchCharge (void)
{
  printf ("%s\t\t%s\n", __FILE__, SCCS_VERSION);
}

toenBool foenIsEGLPUsed(char *ppchzItemNo)
{
  char *lpchzField, *lpchzDes;
  char lachzTtShdes[16];
  char lachzTzShdes[16];

  /*
   * Tariff Zone
   */

  lpchzField = fpchzGetField(4, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTtShdes, lpchzField, 16);
    }

  /*
   * Tariff Time
   */

  lpchzField = fpchzGetField(5, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTzShdes, lpchzField, 16);
    }

  if (lachzTtShdes[0] != '\0' && lachzTzShdes[0] != '\0')
    {
      return TRUE;
    }

  return FALSE;
}  


toenBool foenRecodeCharge(char *ppchzArticleNo, char *ppchzItemNo, 
                          tostUsageItem *ppstUI, 
                          tostRatingInterval **ppstRI, 
                          tostEventPrice *ppstEP)
{
  int rc, i;
  tostMPUSNTAB *ptr;
  char *lpchzField, *lpchzDes;
  char lachzTmShdes [16];
  char lachzTmVer[16];
  char lachzSpShdes[16];
  char lachzSnShdes[16];
  char lachzTypeind[16];
  char lachzRimVer [16];
  char lachzEGLVer [16];
  char lachzTtShdes[16];
  char lachzTzShdes[16];

  char lochNetSnInd;
  long loilRICode, loilEvCode;
  toenBool loenStatus;

  memset(ppstUI, 0, sizeof(tostUsageItem));
  memset(ppstRI, 0, sizeof(tostRatingInterval));
  memset(ppstEP, 0, sizeof(tostEventPrice));

  /*
   * Tariff Model
   */

  lpchzField = fpchzGetField(0, ppchzArticleNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTmShdes, lpchzField, 16);
    }

  lpchzField = fpchzGetField(1, ppchzArticleNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTmVer, lpchzField, 16);
    }
  
  /*
   * Service Package
   */

  lpchzField = fpchzGetField(2, ppchzArticleNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzSpShdes, lpchzField, 16);
    }


  /*
   * Service 
   */

  lpchzField = fpchzGetField(3, ppchzArticleNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzSnShdes, lpchzField, 16);
    }

  /*
   * Type of connection: A, I, C, R
   */

  lpchzField = fpchzGetField(0, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTypeind, lpchzField , 16);
      if (lpchzField[0] == 'I')
        {
          lachzTypeind[0] = 'L';
        }
    }

  /*
   * Type of rating: F, R, B, S
   */

  lpchzField = fpchzGetField(1, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      
    }
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Rating model: %s\n", lpchzField);                              
    }

 
  /*
   * Rating Interval Version
   */

  lpchzField = fpchzGetField(2, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {      
      strncpy(lachzRimVer, lpchzField, 16);
    }

  /*
   * Extended GL Package Version
   */

  lpchzField = fpchzGetField(3, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzEGLVer, lpchzField, 16);
    }


  /*
   * Tariff Zone
   */

  lpchzField = fpchzGetField(4, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTtShdes, lpchzField, 16);
    }

  /*
   * Tariff Time
   */

  lpchzField = fpchzGetField(5, ppchzItemNo);
  if (lpchzField == NULL)
    {
      return FALSE;
    }
  else
    {
      strncpy(lachzTzShdes, lpchzField, 16);
    }

  lochNetSnInd = '\0';
  for (i = 0; i < golMPUSNTAB_Length; i++)
    {
      if (!strcmp(gpstMPUSNTAB[i].sachzSHDES, lachzSnShdes))
        {
          lochNetSnInd = gpstMPUSNTAB[i].sochNETSNIND; 
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "Service Class: %c\n", lochNetSnInd);
            }
          
          break;
        }
    }
  
  strncpy(ppstUI->sachzTMSHDES, lachzTmShdes, 5);
  ppstUI->solVSCODE = atoi(lachzTmVer);
  strncpy(ppstUI->sachzSPSHDES, lachzSpShdes, 5);
  strncpy(ppstUI->sachzSNSHDES, lachzSnShdes, 5);
  ppstUI->sochType = lachzTypeind[0];
  
  ppstUI->solRIVERSION = atol(lachzRimVer);
  if (lachzEGLVer[0] != '\0')
    {
      ppstUI->solEGLVERSION = atol(lachzEGLVer);
    }
  else
    {
      ppstUI->solEGLVERSION = 0;
    }
  strncpy(ppstUI->sachzTTSHDES, lachzTtShdes, 5);
  strncpy(ppstUI->sachzZNSHDES, lachzTzShdes, 5);

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "TM  : %s\n", ppstUI->sachzTMSHDES);
      fprintf(stderr, "VER : %ld\n", ppstUI->solVSCODE);
      fprintf(stderr, "SP  : %s\n", ppstUI->sachzSPSHDES);
      fprintf(stderr, "SN  : %s\n", ppstUI->sachzSNSHDES);
      fprintf(stderr, "TYPE: %c\n", ppstUI->sochType);
      fprintf(stderr, "RIV : %ld\n", ppstUI->solRIVERSION);
      fprintf(stderr, "EGV : %ld\n", ppstUI->solEGLVERSION);
      fprintf(stderr, "TT  : %s\n", ppstUI->sachzTTSHDES);
      fprintf(stderr, "ZN  : %s\n", ppstUI->sachzZNSHDES);
    }

  switch(lochNetSnInd)
    {
    case 'U':
      
      /*
       * Find RICODE for Usage
       */
      
      loilRICode = foilMap_Service2RICode(lachzTmShdes, lachzTmVer, lachzSpShdes, lachzSnShdes, ppstUI);
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "RICODE[%s, %s, %s, %s]: %ld\n", 
                  lachzTmShdes, lachzTmVer, lachzSpShdes, lachzSnShdes, 
                  loilRICode);      
        }
      
      /*
       * Find usage price
       */

      if (lachzTtShdes[0] != '\0' && lachzTzShdes[0] != '\0')
        {
          /*
           * TT and ZN are defined
           */
          
          loenStatus =  foenMap_FindRI(lachzTypeind, loilRICode, lachzRimVer, lachzTtShdes, lachzTzShdes, ppstRI, ppstUI);
        }
      else
        {
          /*
           * No TT and ZN defined
           */

          loenStatus =  foenMap_FindRI(lachzTypeind, loilRICode, lachzRimVer, NULL, NULL, ppstRI, ppstUI);
        }

      if (goenVerbose == TRUE && loenStatus == TRUE)
        {
          fprintf(stderr, "Rating Interval loaded\n"); 
        }      
      
      /*
       * Some error occured during loading information from MPULKRIM
       */

      if (loenStatus == FALSE)
        {
          fprintf(stderr, "Error loading RI for Usage\n");
          return FALSE;
        }
      
      break;

    case 'E':

      /*
       * Find EVCODE for Event
       */

      loilEvCode = foilMap_Service2EvCode(lachzTmShdes, lachzTmVer, lachzSpShdes, lachzSnShdes, ppstUI);
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "EVCODE[%s, %s, %s, %s]: %ld\n", 
                  lachzTmShdes, lachzTmVer, lachzSpShdes, lachzSnShdes, 
                  loilEvCode);      
        }
      
      /*
       * Find event price
       */

      if (lachzTtShdes[0] != '\0')
        {
          /*
           * TT is defined
           */          
          
          loenStatus = foenMap_FindEv(loilEvCode, lachzRimVer, lachzTtShdes, ppstEP, ppstUI);
        }
      else
        {
          /*
           * No TT defined
           */

          loenStatus = foenMap_FindEv(loilEvCode, lachzRimVer, NULL, ppstEP, ppstUI);
        }
        
      if (goenVerbose == TRUE && loenStatus == TRUE)
        {
          fprintf(stderr, "Event Price loaded\n"); 
        }
      
      if (loenStatus == FALSE)
        {
          fprintf(stderr, "Error loading RI for Event\n");
          return FALSE;
        }

      break;

    default:
      fprintf(stderr, "Error loading RI: bad service type\n");
      return FALSE;

    }
  
  return TRUE;
}


toenBool foenFindFlatPPU(tostUsageItem *postUsageItem, tostRatingInterval *pastRI)
{
  return TRUE;
}




