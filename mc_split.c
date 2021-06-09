/**************************************************************************************************
 *                                                                                          
 * MODULE: MC_SPLIT
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
#include "g22.h"

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

extern void print_g22(struct s_group_22 *);
extern void free_g22(struct s_group_22 *);

void fovdG22_Free(struct s_group_22 *);

extern int foiMC_MicroCellCount(long);
extern int foiMC_MicroCellGet(long, int);
extern double foflMC_MicroCellGetScale(long, int);
extern toenBool foenMC_Splay(long);
extern toenBool foenMC_InternalCalls(long, long, char *, int *, double *, int *);
extern toenBool foenMC_ExternalCalls(long, char *, int *, double *, int *);
extern toenBool foenMC_MakeKey(long, char *, char *, char *);

/*
 * METHOD: SplitUsage
 *
 */

struct s_group_22 *fpstMCSplit_Usage(
                                     long poilContractId,
                                     struct s_group_22 *ppstG22, 
                                     struct s_group_22 *ppstPrevG22, struct s_group_22 *ppstNextG22, 
                                     char *ppsnzArticleNo, char *ppsnzItemNo
                                     )
{
  struct s_group_22 *lpstG22, *lpstFirstG22, *lpstNewG22;
  char lochUsageType;
  char *lpsnzShdes;
  static char lasnzKey[64];
  int i, j, loiBlocks, n, loiCalls, loiCallsTime;
  toenBool loenStatus;
  double loflFlatValue, loflScale;
  static char lasnzStr[32];
  static tostSumUsage lostSumUsage;
  static char lasnzMCCode[16];
  long loilMCCode;
  
  loenStatus = foenMC_MakeKey(poilContractId, ppsnzArticleNo, ppsnzItemNo, lasnzKey);
  if (loenStatus == FALSE)
    {
      return NULL;
    }

  /*
   * Handle F&F MC
   */

  loiBlocks = 0;
  lpstG22 = NULL;
  lpstNewG22 = NULL;

  n = foiMC_MicroCellCount(poilContractId);
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC found: %d\n", n);
    }

  if (n == 0)
    {
      return ppstG22;
    }

  for (i = 0; i < n; i++)
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "F&F MC no %d\n", i);
        }

      loilMCCode = foiMC_MicroCellGet(poilContractId, i);
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "F&F MC code: %ld\n", loilMCCode);
        }
      
      loenStatus = foenMC_InternalCalls(poilContractId, loilMCCode, lasnzKey, &loiCalls, &loflFlatValue, &loiCallsTime);
      if (loenStatus == FALSE)
        {
          return NULL;
        }                         
      
      if (loiCalls > 0)
        {
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC processing\n");
            }
          
          /*
           * New G22 item
           */
          
          sprintf(lasnzMCCode, ".%ld", loilMCCode);              
          loflScale = foflMC_MicroCellGetScale(poilContractId, i);
          
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC loading G22 group for MCCode: %s\n", lasnzMCCode);
            }
          
          memset(&lostSumUsage, 0, sizeof(tostSumUsage));
          loenStatus = foenG22_Load(ppstG22, &lostSumUsage);
          if (loenStatus != TRUE)
            {
              return NULL;
            }
          
          lostSumUsage.soiCallsNumber = loiCalls;
          lostSumUsage.soiCallsTime = loiCallsTime;                
          lostSumUsage.soflFullAmount = loflFlatValue;
          lostSumUsage.soflAmount = loflFlatValue;

          sprintf(lasnzMCCode, ".%ld", loilMCCode);
          strcat(lostSumUsage.sasnzItemId, lasnzMCCode);
          
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC Internal Calls: %d\n", loiCalls);
              fprintf(stderr, "               ItemId: %s\n", lostSumUsage.sasnzItemId);
              fprintf(stderr, "          Basic Price: %lf\n", lostSumUsage.soflIntervalPrice);
              fprintf(stderr, "                Scale: %lf\n", loflScale);
              fprintf(stderr, "             MC Price: %lf\n", lostSumUsage.soflIntervalPrice * loflScale);
              fprintf(stderr, "                 Time: %d\n", loiCallsTime);
              fprintf(stderr, "           FullAmount: %lf\n", loflFlatValue);
            }

          lostSumUsage.soflIntervalPrice = lostSumUsage.soflIntervalPrice * loflScale;
          
          lpstNewG22 = fpstG22_New(&lostSumUsage);
          if (lpstNewG22 == NULL)
            {
              return NULL;
            }                    

          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F Micro Cell new G22 group created\n");
              print_g22(lpstNewG22);
            }
          
          /*
           * Number of new blocks increases by one
           */
      
          loiBlocks++;
                    
          /*
           * Append G22 item to list of G22 items
           */
          
          lpstNewG22->g_22_next = lpstG22;
          lpstG22 = lpstNewG22;          
        }
      else
        {
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC no calls\n");
            }
        }
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC G22 blocks created: %d\n", loiBlocks);
    }

  if (loiBlocks == 0)
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "F&F MC no B22 blocks created\n");
        }
      
      return ppstG22;
    }
  else
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "F&F MC creating G22 with external calls\n");
        }
      
      loenStatus = foenMC_ExternalCalls(poilContractId, lasnzKey, &loiCalls, &loflFlatValue, &loiCallsTime);
      if (loenStatus == FALSE)
        {
          return NULL;
        }                         
      
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "F&F MC found external calls no: %d\n", loiCalls);
        }
      
      if (loiCalls > 0)
        {
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC creating G22 block\n");
            }
          
          loenStatus = foenG22_Load(ppstG22, &lostSumUsage);
          if (loenStatus != TRUE)
            {
              return NULL;
            }
          
          lostSumUsage.soiCallsNumber = loiCalls;
          lostSumUsage.soflFullAmount = loflFlatValue;
          lostSumUsage.soflAmount = loflFlatValue;
          lostSumUsage.soiCallsTime = loiCallsTime;

          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "F&F MC External calls: %d\n", loiCalls);
              fprintf(stderr, "               ItemId: %s\n", lostSumUsage.sasnzItemId);
              fprintf(stderr, "          Basic Price: %lf\n", lostSumUsage.soflIntervalPrice);
              fprintf(stderr, "                Scale: %lf\n", 1.0);
              fprintf(stderr, "             MC Price: %lf\n", lostSumUsage.soflIntervalPrice);
              fprintf(stderr, "                 Time: %d\n", loiCallsTime);
              fprintf(stderr, "           FullAmount: %lf\n", loflFlatValue);
            }
          
          lpstNewG22 = fpstG22_New(&lostSumUsage);
          if (lpstNewG22 == NULL)
            {
              return NULL;
            }          
          
          lpstNewG22->g_22_next = lpstG22;
          lpstG22 = lpstNewG22;                            
        }
    }

  /*
   * New sequence of G22 segments was created
   */
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "F&F MC linking new G22 blocks list\n");
    }

  lpstFirstG22 = lpstG22;
  ppstPrevG22->g_22_next = lpstG22;
  while (lpstG22->g_22_next != NULL)
    {
      lpstG22 = lpstG22->g_22_next;
    }
      
  lpstG22->g_22_next = ppstNextG22;

  /*
   * free memory from ppstG22 segment
   */

  fovdG22_Free(ppstG22);

  /*
   * Return linked sequence of G22 blocks
   */

  return lpstG22;
}



