#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include "pbgh.h"
#include "protos.h"
#include "parser.h"
#include "gen.h"
#include "timm.h"
#include "rtx.h"
#include "version.h"

static char *SCCS_VERSION = "1.0";

#define COMMIT_INTERVAL 5
#define ERROR_EXIT(rc, str) {printf("ERROR[%08d]: %s\n", rc, str); foiDB_Rollback(); exit(rc);}

#define BSCS_USER "BCH"
#define RTX_USER "BCH"

#define MODULE "pbgh"
#define VERSION "2.0"
#define RELEASE "1"
#define LEVEL "1"

/*
 * global
 */

toenBool goenVerbose;
toenBool goenTestOnly;
int goiCustomersNo;
int goiCustomerId;
int goiTryCreateEnclosure;
int goiCommitInterval;

stPBGHGLOB gostPBGHEnv;


/*
 * BSCSLink
 */

extern char *fpchzBSCSLink_GetPassword(char *);
extern char *fpchzBSCSLink_GetConnectString();

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

/*
 * DB
 */

extern int foiDB_CursorOpen(int);
extern int foiDB_CursorClose();
extern int foiDB_LoadTimmStr(TYPEID, char **);          
extern int foiDB_UpdateDocumentAll(char *);
extern int foiDB_Commit();
extern int foiDB_Rollback();
extern int foiDB_Open(char *, char *, char *);

/*
 * BSCS
 */

extern int foiBSCS_Open(char *, char *, char *);
extern int foiBSCS_GetMPUSNTAB();
extern int foiBSCS_GetGlobalUnitPrice();
extern int foiBSCS_GetMPUPUTAB();
extern char *fpchzBSCS_GetRTXServerName();
extern int foiBSCS_GetWorkDir();

extern int foiBSCS_GetTTDes();
extern int foiBSCS_GetZNDes();
extern int foiBSCS_GetTMDes();
extern int foiBSCS_GetSPDes();
extern int foiBSCS_GetSNDes();
extern int foiBSCS_GetMPULKTMM();
extern int foiBSCS_GetMPULKRIM();
extern int foiBSCS_GetMPULKEVM();

/*
 * RTX
 */

extern int foiRTX_Open(char *, char *, char *);
extern int foiRTX_GetCustomerRTX(int, char *, char *);
extern int foiRTX_GetCustomerContractRTX(int, int, char *, char *);
extern int foiRTX_FreeRTX();

/*
 * Proc
 */

extern toenBool foenProc_ProcessTimmSumSheet(char *, char **, char *);

/*
 * Main local functions
 */

toenBool foenMain_FileLoadTimmStr(char *, TYPEID, char **);
toenBool foenMain_OpenAll();
toenBool foenMain_LoadEnv();
void fovdPrintHelp();

/*
 * Version info handling
 */

void fovdPrintVerInfoMain (void)
{
  printf ("%s\t\t\t%s\n", __FILE__, SCCS_VERSION);
}

static void fovdPrintVersionInfo (void)
{  
  int i;

  printf ("Build: %s, %s\n\n", __DATE__, __TIME__);
  
  printf ("Component:\t%s\n", MODULE);
  printf ("Version:\t%s\n", VERSION);
  printf ("Release:\t%s\n", RELEASE);
  printf ("Level:\t\t%s\n\n", LEVEL);

  /*
  printf ("CMVC VERSIONS OF FILES\n\n");
  printf ("File:\t\t\tVersion:\n");

  fovdPrintVerInfoMain ();
  fovdPrintVerInfoRtx ();
  fovdPrintVerInfoBscs ();
  fovdPrintVerInfoDb ();
  fovdPrintVerInfoProc ();
  fovdPrintVerInfoParser ();
  fovdPrintVerInfoPrinter ();
  fovdPrintVerInfoTimm ();
  fovdPrintVerInfoBscslink ();
  fovdPrintVerInfoMp ();
  fovdPrintVerInfoBchCharge ();
  */

  printf("\n\n");
  
  for (i = 0; i < sizeof(mpszEnvironment); i++)
    {
      if (mpszEnvironment[i][0] == '\0')
        {
          break;
        }
      else
        {
          printf("%s\n", mpszEnvironment[i]);
        }
    }

  printf("\n\n");
}

/*
 * Main
 */

main(int argc, char **argv) 
{
  char *ppchzInTimmStr = NULL, *ppchzOutTimmStr = NULL;
  toenBool loenStatus;
  toenBool loenProcessFile;
  int rc = 0, loiDocumentNo;
  char *lpchzInputFileName, *lpchzOutputFileName;
  time_t lostDocStartTime, lostDocEndTime;
  time_t lostJobStartTime, lostJobEndTime;
  
  /*
   * Default environment
   */

  goiCommitInterval = COMMIT_INTERVAL;
  lpchzInputFileName = NULL;
  lpchzOutputFileName = NULL;
  loenProcessFile = FALSE;
  goenVerbose = FALSE;
  goenTestOnly = FALSE;
  goiCustomersNo = 1;
  goiCustomerId = -1;
  goiTryCreateEnclosure = 0;
  strcpy(gostPBGHEnv.szBaseDirectory, "");

  /*
   * Mode of work
   */

  while ((argc > 1) && (argv[1][0] == '-'))
    {
      switch (argv[1][1])
        {
        case 'i':
          fovdPrintVersionInfo ();
          exit(0);

        case 'a':          
          goiCustomersNo = -1;
          goiCustomerId = -1;
          break;

        case 'n':
          goiCustomersNo = atoi(argv[2]);
          goiCustomerId = -1;
          argv++;
          argc--;
          break;

        case 'b':
          
          /*
           * WORK
           */

          strcpy(gostPBGHEnv.szBaseDirectory, argv[2]);

          /*
           * LOG
           */
          
          strcpy(gostPBGHEnv.szLogDir, argv[2]);
          strcat(gostPBGHEnv.szLogDir, "/LOG");

          /*
           * TMP
           */
          
          strcpy(gostPBGHEnv.szTmpDir, argv[2]);
          strcat(gostPBGHEnv.szTmpDir, "/TMP");

          argv++;
          argc--;
          break;

        case 'c':
          goiCustomerId = atoi(argv[2]);
          goiCustomersNo = 1;
          argv++;
          argc--;
          break;
          
        case 'v':
          goenVerbose = TRUE;
          break;

        case 't':
          goenTestOnly = TRUE;
          break;
          
        case 'o':
          lpchzOutputFileName = argv[2];
          argv++;
          argc--;
          break;
          
        case 'f':
          loenProcessFile = TRUE;
          lpchzInputFileName = argv[2];
          loenStatus = foenMain_FileLoadTimmStr(lpchzInputFileName, SUM_TYPE, &ppchzInTimmStr);
          if (loenStatus == FALSE)
            {
              ERROR_EXIT(0, "foenFileLoadTimmStr");
            }
          argv++;
          argc--;
          break;
          
        case 'E':
          goiTryCreateEnclosure = 1;
          break;

        default:
          fovdPrintHelp();
        }

      argv++;
      argc--;      
    }

  if (argc > 1)
    {
      fovdPrintHelp();
    }


  /*
   * Connect 
   */

  loenStatus = foenMain_OpenAll();
  if (loenStatus == FALSE)
    {      
      ERROR_EXIT(0, "foenOpenAllConnections");
    }

  /*
   * Load environment
   */

  loenStatus = foenMain_LoadEnv();
  if (loenStatus == FALSE)
    {      
      ERROR_EXIT(0, "foenLoadEnv");
    }

  if (loenProcessFile == TRUE)
    {
      /*
       * File processing
       */
      
      loenStatus = foenProc_ProcessTimmSumSheet(ppchzInTimmStr, &ppchzOutTimmStr, lpchzOutputFileName);
      if (loenStatus == FALSE)
        {
          ERROR_EXIT(0, "foenProcessTimmSumSheet");
        }

      free(ppchzInTimmStr);      
      free(ppchzOutTimmStr);      
    }
  else
    {
      /*
       * Process all documents with type SUM_TYPE  
       */
      
      loiDocumentNo = 1;
      lostJobStartTime = time(NULL);
      
      rc = foiDB_CursorOpen(goiCustomerId);
      if (rc != 0)
        {
          ERROR_EXIT(rc, "foiDB_CursorOpen");
        }

      while (1)
        {

          lostDocStartTime = time(NULL);

          /*
           * Next document from opended cursor
           */
          
          rc = foiDB_LoadTimmStr(SUM_TYPE, &ppchzInTimmStr);          
          if (rc == ESQL_NOMOREDATA)
            {
              /*
               * Last document in a batch job
               */

              break;
            }
          else if (rc != 0)
            {
              /*
               * Some error
               */

              ERROR_EXIT(rc, "loiDB_LoadTimmStr");              
            }
          else
            {
              /*
               * Process next SUMSHEET
               */

              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "Processing SUM with DOC size: %d\n", strlen(ppchzInTimmStr));
                }
              
              loenStatus = foenProc_ProcessTimmSumSheet(ppchzInTimmStr, &ppchzOutTimmStr, NULL);
              if (loenStatus == FALSE)
                {
                  ERROR_EXIT(0, "foenMain_ProcessTimmSumSheet");
                }
              
              /*
               * SUMSHEET is already processed
               */
              
              if (goenVerbose == TRUE)
                {
                  fprintf(stderr, "Updating DOCUMENT_ALL\n");
                }

              rc = foiDB_UpdateDocumentAll(ppchzOutTimmStr);
              if (rc != 0)
                {
                  ERROR_EXIT(rc, "foiDB_UpdateDocumentAll");
                }

              /*
               * Free memory used by TIMM string
               */ 

              free(ppchzInTimmStr);
              ppchzInTimmStr = NULL;

              free(ppchzOutTimmStr);
              ppchzOutTimmStr = NULL;

              /*
               * Timing
               */

              lostDocEndTime = time(NULL);
              fprintf(stderr, "TIME: %4ld sec.\n", lostDocEndTime - lostDocStartTime);
            }
          
          if ((loiDocumentNo++ % goiCommitInterval) == 0)
            {
              /*
               * Do we commit ?
               */

              rc = foiDB_Commit();
              if (rc != 0)
                {
                  ERROR_EXIT(rc, "foiDB_Commit");
                }
              else
                {
                  fprintf(stderr, "Commit\n");                    
                }
            }

          if (goiCustomersNo != -1)
            {
              /*
               * We have a given number of customers to be processed
               */

              if (goiCustomersNo == 1)
                {
                  /*
                   * The last one
                   */
                  
                  break;
                }
              else
                {
                  /*
                   * One customer was processed
                   */

                  goiCustomersNo--;
                }
            }
        }
      
      rc = foiDB_CursorClose();
      if (rc != 0)
        {
          ERROR_EXIT(rc, "foiDB_CursorClose");
        }
      
      if (goenTestOnly != TRUE)
        {
          rc = foiDB_Accept();
          if (rc != 0)
            {
              ERROR_EXIT(rc, "foiDB_Accept");
            }
        }

      rc = foiDB_Commit();
      if (rc != 0)
        {
          ERROR_EXIT(rc, "foiDB_Commit");
        }

      lostJobEndTime = time(NULL);

      fprintf(stderr, "\nMessages processed: %d\n", loiDocumentNo - 1);
      fprintf(stderr, "Processing time   : %d sec.\n\n", lostJobEndTime - lostJobStartTime);
    }

  /*
   * Main exit code
   */
  
  exit(0);
}

/*
 * Help
 */

void fovdPrintHelp()
{
  fprintf(stderr, "OPTIONS: pbgh -f <FILE> - load SumSheet from file\n");
  fprintf(stderr, "         pbgh -o <FILE> - save processed SumSheet to file\n");
  fprintf(stderr, "         pbgh -v        - verbose mode - stderr reporting\n");
  fprintf(stderr, "         pbgh -t        - test mode - no update of DOCUMENT_ALL table\n");
  fprintf(stderr, "         pbgh -n <NO>   - customers number\n");
  fprintf(stderr, "         pbgh -c <NO>   - customer id\n");
  fprintf(stderr, "         pbgh -a        - process all SumSheets from BSCS DB using RTX DB\n");
  fprintf(stderr, "         pbgh -b <DIR>  - configure base directory\n");

  exit(1);
}

/*
 * Use file as a source of TIMM message
 */

toenBool foenMain_FileLoadTimmStr(char *ppchzFileName, TYPEID poenTypeId, char **ppchzTimmStr)
{
  struct stat stStat;
  FILE *filInput;
  int c, i;
  
  if (lstat(ppchzFileName, &stStat) < 0)
    {
      return FALSE;
    }

  if((*ppchzTimmStr = (char *)malloc((stStat.st_size + 1) * sizeof(char))) == NULL)
    { 
      return FALSE;
    }

  if ((filInput = fopen(ppchzFileName, "r")) == NULL)
    {
      return FALSE;
    }

  i = 0;
  while ((c = getc(filInput)) != EOF)
    {
      if (c != '\n')
        {
          (*ppchzTimmStr)[i++] = c;
        }
    }
  
  (*ppchzTimmStr)[i] = '\0';
  
  return TRUE;
}  

/*
 * Create connection for each DB access module
 */

toenBool foenMain_OpenAll()
{
  int rc;
  char *lpchzPassword, *lpchzConnectString;

  /*
   * BSCS DB 
   */

  lpchzPassword = fpchzBSCSLink_GetPassword(BSCS_USER);
  if (lpchzPassword == NULL)
    {
      fprintf(stderr, "Can't get password\n");
      return FALSE;
    }

  lpchzConnectString = fpchzBSCSLink_GetConnectString();
  if (lpchzConnectString == NULL)
    {
      fprintf(stderr, "Can't get connect string\n");
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Using: %s/%s@%s\n", BSCS_USER, lpchzPassword, lpchzConnectString);
    }

  rc = foiBSCS_Open(BSCS_USER, lpchzPassword, lpchzConnectString);
  if (rc != 0)
    {
      fprintf(stderr, "Can't connect to the BSCS DB\n");
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Connected BSCS\n");
    }

  rc = foiDB_Open(BSCS_USER, lpchzPassword, lpchzConnectString);
  if (rc != 0)
    {
      printf("Can't connect to the TIMM DB\n");
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Connected TIMM DB\n");
    }

  /*
   * RTX DB
   */
  
  lpchzPassword = fpchzBSCSLink_GetPassword(RTX_USER);
  if (lpchzPassword == NULL)
    {
      return FALSE;
    }
  
  lpchzConnectString = fpchzBSCS_GetRTXServerName();
  if (lpchzConnectString == NULL)
    {
      return FALSE;
    }
  
  rc = foiRTX_Open(BSCS_USER, lpchzPassword, lpchzConnectString);
  if (rc != 0)
    {
      printf("Can't connect to the RTX DB\n");
      return FALSE;
    }

  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Connected RTX\n");
    }

  return TRUE;
}

/*
 * Load table from BSCS module
 */

toenBool foenMain_LoadEnv()
{
  int rc;

  if (strlen(gostPBGHEnv.szBaseDirectory) == 0)
    {
      /*
       * Use config from DB
       */

      rc = foiBSCS_GetWorkDir();
      if (rc != 0)
        {
          printf("Can't configure WORK directory\n");
          return FALSE;
        }
    }
  else
    {
      if (goenVerbose == TRUE)
        {
          fprintf(stderr, "WORK: %s\n", gostPBGHEnv.szBaseDirectory);
          fprintf(stderr, "LOG : %s\n", gostPBGHEnv.szLogDir);
          fprintf(stderr, "TMP : %s\n", gostPBGHEnv.szTmpDir);
        }
    }

  rc = foiBSCS_GetMPUPUTAB();
  if (rc != 0)
    {
      printf("Can't load MPUPUTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUPUTAB\n");
    }

  rc = foiBSCS_GetTTDes();
  if (rc != 0)
    {
      printf("Can't load MPUTTTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUTTTAB\n");
    }


  rc = foiBSCS_GetZNDes();
  if (rc != 0)
    {
      printf("Can't load MPUZNTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUZNTAB\n");
    }


  rc = foiBSCS_GetSPDes();
  if (rc != 0)
    {
      printf("Can't load MPUSPTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUSPTAB\n");
    }


  rc = foiBSCS_GetSNDes();
  if (rc != 0)
    {
      printf("Can't load MPUSNTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUSNTAB\n");
    }


  rc = foiBSCS_GetTMDes();
  if (rc != 0)
    {
      printf("Can't load MPUTMTAB\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUTMTAB\n");
    }


  rc = foiBSCS_GetMPUSNTAB(); 
  if (rc != 0)
    {
      printf("Can't load MPUSNTAB\n");
      return FALSE;
    }

  rc = foiBSCS_GetGlobalUnitPrice(); 
  if (rc != 0)
    {
      printf("Can't load unit price\n");
      return FALSE;
    }

  rc = foiBSCS_GetMPUFFTAB();
  if (rc != 0)
    {
      printf("Can't load Micro Cells\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPUFFTAB\n");
    }


  rc = foiBSCS_GetMPULKTMM();
  if (rc != 0)
    {
      printf("Can't load MPULKTMM\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPULKTMM\n");
    }


  rc = foiBSCS_GetMPULKRIM();
  if (rc != 0)
    {
      printf("Can't load MPULKRIM\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPULKRIM\n");
    }


  rc = foiBSCS_GetMPULKEVM();
  if (rc != 0)
    {
      printf("Can't load MPULKEVM\n");
      return FALSE;
    }
  if (goenVerbose == TRUE)
    {
      fprintf(stderr, "Loaded MPULKEVM\n");
    }

  return TRUE;
}



