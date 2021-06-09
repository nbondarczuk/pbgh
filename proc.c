#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pbgh.h"
#include "protos.h"
#include "parser.h"
#include "gen.h"
#include "timm.h"
#include "rtx.h"


static char *SCCS_VERSION = "1.0";


void fovdPrintVerInfoProc (void)
{
  printf ("%s\t\t\t%s\n", __FILE__, SCCS_VERSION);
}


#define COMMIT_INTERVAL 5
#define ERROR_EXIT(rc, str) {printf("ERROR[%08d]: %s\n", rc, str); exit(rc);}

/*
 * global
 */

extern toenBool goenVerbose;
extern toenBool goenTestOnly;
extern int goiCustomersNo;

/*
 * Parser
 */

struct s_TimmInter *fpstParser_Parse(char *);

/*
 * Printer
 */

toenBool foenPrinter_Sprintf(char **, struct s_TimmInter *);
toenBool foenPrinter_Printf(struct s_TimmInter *);
toenBool foenPrinter_Fprintf(FILE *, struct s_TimmInter *);  

/*
 * Timm
 */

toenBool foenTimm_LoadBillingInterval(struct s_TimmInter *, char *, char *); 
toenBool foenTimm_ProcessUsageSegments(struct s_TimmInter *, int, int);
int foiTimm_CountSubscribers(struct s_TimmInter *);
int foiTimm_CountSubscriberContracts(struct s_TimmInter *, int);
int foiTimm_LoadContractNo(struct s_TimmInter *, int, int);
int foiTimm_FindSubscriberCode(struct s_TimmInter, int, char *);

/*
 * DB
 */

extern int foiDB_CursorOpen();
extern int foiDB_CursorClose();
extern int foiDB_LoadTimmStr(TYPEID, char **);          
extern int foiDB_UpdateDocumentAll(char *);
extern int foiDB_Commit();
extern int foiDB_Open(char *, char *, char *);
extern int foiDB_CreateEnclosure(char *);

/*
 * BSCS
 */

extern int foiBSCS_Open(char *, char *, char *);
extern int foiBSCS_GetMPUSNTAB();
extern int foiBSCS_GetGlobalUnitPrice();
extern int foiBSCS_GetMPUPUTAB();
extern long foilBSCS_FindCustomerId(char *);

/*
 * RTX
 */

extern int foiRTX_Open(char *, char *, char *);
extern int foiRTX_GetCustomerRTX(int, char *, char *);
extern int foiRTX_GetCustomerContractRTX(int, int, char *, char *);
extern int foiRTX_Free();
extern int foiRTX_Init();


/*
 * Process buffer with TIMM message then reallocate space 
 * and print it to the buffer. 
 */

toenBool foenProc_ProcessTimmSumSheet(char *ppchzInTimmStr, char **ppchzOutTimmStr, char *pachzOutputFileName)
{
  int loiSubsNo, loiSub, loiContsNo, loiCont, n, rc, loiContractNo, loiCustomerId;  
  char lachzStart[7], lachzEnd[7];
  struct s_TimmInter *pstTimmInter;    
  toenBool loenStatus;
  FILE *lpfilOutputFile;
  long *lailCustomerId;
  char lasnzCustcode[64];

  /*
   * Process TIMM interchange
   */
  
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Parsing SUM\n");      
      fflush(stderr);
    }


  if ((pstTimmInter = fpstParser_Parse(ppchzInTimmStr)) == NULL)
    {
      return FALSE;
    }

  /*
   * TIMM interchange is parsed 
   */

  loenStatus = foenTimm_LoadBillingInterval(pstTimmInter, lachzStart, lachzEnd); 
  if (loenStatus == FALSE)
    {          
      return FALSE;
    }

  loiCustomerId = atoi(pstTimmInter->unb->v_0010);  

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Payment Responsible Customer: %d\n", loiCustomerId);
      fprintf(stderr, "Billing interval: [%s, %s]\n", lachzStart, lachzEnd); 
    }
  else
    {                      
      fprintf(stderr, "Customer: %08d, TIMM size: %8d, ", loiCustomerId, strlen(ppchzInTimmStr));
    }
    
  /* 
   * For each subscriber do
   */
  
  loiSubsNo = foiTimm_CountSubscribers(pstTimmInter);
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Subscribers: %d\n", loiSubsNo);      
    }

  for (loiSub = 0; loiSub < loiSubsNo; loiSub++)
    {
      /*
       * Load RTX records for a subscriber
       */
      
      loenStatus = foenTimm_FindSubscriberCode(pstTimmInter, loiSub, lasnzCustcode);
      if (loenStatus != TRUE)
        {
          fprintf(stderr, "No custcode found\n");
          return FALSE;
        }

      if (lasnzCustcode[0] != '1')
        {
          loiCustomerId = foilBSCS_FindCustomerId(lasnzCustcode);
          if (loiCustomerId == 0)
            {
              fprintf(stderr, "Can't map custcode: %s to customer_id\n", lasnzCustcode);
              return FALSE;
            }
      
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "Subscriber[%d].Customer_id: %08d\n", loiSub, loiCustomerId);
            }
        }

      /*
       * for each contract of a subscriber
       */
      
      loiContsNo = foiTimm_CountSubscriberContracts(pstTimmInter, loiSub);
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Subscriber: %d, Contracts: %d\n", loiSub, loiContsNo);
        }
      
      if (loiContsNo > 0)
        {

          /*
           * Init Data for RTX segment list
           */
          
          rc = foiRTX_Init();
          if (rc != 0)
            {
              return FALSE;
            }
          
          /*
           * For this customer load all RTX records
           */
          
          rc = foiRTX_GetCustomerRTX(loiCustomerId, lachzStart, lachzEnd);
          if (rc != 0)
            {
              fprintf(stderr, "Can't load RTX for customer: %ld\n", loiCustomerId);
              return FALSE;
            }
          
          for (loiCont = 0; loiCont < loiContsNo; loiCont++)
            {
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "Loading contract no: %d\n", loiCont);
                }
              
              loiContractNo = foiTimm_LoadContractNo(pstTimmInter, loiSub, loiCont);
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "Contract: %d\n", loiContractNo);
                }
              
              /*
               * Process list of segments
               */
              
              loenStatus = foenTimm_ProcessUsageSegments(pstTimmInter, loiSub, loiCont);
              if (loenStatus == FALSE)
                {
                  return FALSE;
                }

              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "Contract processed\n");
                }

            }

          /*
           * Free RTX segment list
           */
          
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "RTX table freeing\n");
            }
          
          rc = foiRTX_Free();
          if (rc != 0)
            {
              return FALSE;
            }
          
          if (goenVerbose == TRUE)
            {
              fprintf(stderr, "RTX table is empty\n");
            }
        }
    }

  if (pachzOutputFileName != NULL)
    {
      /*
       * If single file is being processed
       */
      
      lpfilOutputFile = fopen(pachzOutputFileName, "w");
      if (lpfilOutputFile == NULL)
        {
          return FALSE;
        }

      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Writing TIMM message to file: %s\n", pachzOutputFileName);
        }

      loenStatus = foenPrinter_Fprintf(lpfilOutputFile, pstTimmInter);  
      if (loenStatus == FALSE)
        {
          return FALSE;
        }      
      
      fclose(lpfilOutputFile);        
    }
  else
    {
      /*
       * Batch processing mode
       */
      
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Printing processed TIMM message\n");
        }
      
      loenStatus = foenPrinter_Sprintf(ppchzOutTimmStr, pstTimmInter);  
      if (loenStatus == FALSE)
        {
          return FALSE;
        }

      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "Processed TIMM message printed\n");
        }
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Freeing TIMM message\n");
    }
  
  free_timm (pstTimmInter);

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "TIMM message free\n");
    }

  return TRUE;
}

