#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "gen.h"

static char *SCCS_VERSION = "1.0";


void fovdPrintVerInfoBscslink (void)
{
  printf ("%s\t\t%s\n", __FILE__, SCCS_VERSION);
}


static char *fpchzGetToken(char *, int);
static int foiGetLine(FILE *, char *, int);

char *fpchzBSCSLink_GetPassword(char *pachzUser)
{
  int rc = 0;
  static char sachzPassword[64];
  FILE *lpfilPasswd;
  char lachzLine[128];
  char *lpchzVal, *lpchzPtr;

  lpchzVal = getenv("BSCS_PASSWD");
  if (lpchzVal == NULL)
    {
      fprintf(stderr, "Can't get BSCS_PASSWD variable\n");
      return NULL;
    }

  lpfilPasswd = fopen(lpchzVal, "r");
  if (lpfilPasswd == NULL)
    {
      fprintf(stderr, "Can't open file with BSCS passwords\n");
      return NULL;
    }

  lpchzPtr = NULL;
  
  do {
    rc = foiGetLine(lpfilPasswd, lachzLine, 128);
    if (rc > 0)
      {
        lpchzPtr = fpchzGetToken(lachzLine, 0);
        
        if (!strcmp(lpchzPtr, pachzUser))
          {
            lpchzPtr = fpchzGetToken(lachzLine, 1);
            strncpy(sachzPassword, lpchzPtr, 64);
            lpchzPtr = sachzPassword;            
            break;
          }
      }
  } while (rc != -1);

  fclose(lpfilPasswd);

  return lpchzPtr;
}

char *fpchzBSCSLink_GetConnectString()
{
  int rc = 0;
  static char sachzConnectString[64];
  char *lpchzVal;
  
  lpchzVal = getenv("BSCSDB");
  if (lpchzVal == NULL)
    {
      return NULL;
    }
  
  strncpy(sachzConnectString, lpchzVal, 64);

  return sachzConnectString;
}

char *fpchzGetToken(char *pochzField, int poiFieldNo)
{
  static char lpchzOutput[MAX_BUFFER];
  char *lpchzToken;
  char lpchzField[MAX_BUFFER];
  int loiTokenNo;
  int loiFound;
  
  strncpy(lpchzField, pochzField, MAX_BUFFER);
  loiTokenNo = 0;

  loiFound = TRUE;
  lpchzToken = strtok(lpchzField, " \t");
  if (poiFieldNo == 0)
    {
      strncpy(lpchzOutput, lpchzToken, MAX_BUFFER);
      return lpchzOutput;
    }

  while (loiTokenNo < poiFieldNo)
    {
      lpchzToken = strtok(NULL, " \t");
      if (lpchzToken == NULL)
        {
          lpchzOutput[0] = '\0';
          loiFound = FALSE;
          break;
        }
      loiTokenNo++;
    }
  
  if (loiFound)
    {
      strncpy(lpchzOutput, lpchzToken, MAX_BUFFER);
    }

  return lpchzOutput;
}


static int foiGetLine(FILE *pofilFile, char *pachzLine, int poiMaxLineLen)
{
  int c, i;

  i = 0;
  while ((c = getc(pofilFile)) != EOF)
    {
      if (c == '#')
        {
          /*
           * Eat all characters until the end of line
           */

          while ((c = getc(pofilFile)) != EOF)
            {
              if (c == '\n')
                {
                  /*
                   * End of line found
                   */
                  
                  break;
                }
            }
        }

      if (i < poiMaxLineLen && c != '\n' && c != '#' && c != EOF)
        {
          /*
           * We may accept char in a line if line buffer is not full,  
           * if this is not end of line and if not EOF found
           */

          pachzLine[i++] = c;
        }
      else if (c == '\n' || c == EOF || c == poiMaxLineLen)
        {
          /*
           * This is an end of line
           */

          break;
        }
      else
        {
          /*
           * We are in a line of text
           */
        }
    }

  /*
   * If no eol found then read all characters that were left
   * until eol found or eof
   */
  
  if (c != '\n')
    {
      while (c != EOF && (c = getc(pofilFile)) != '\n') {}
    }

  pachzLine[i] = '\0';

  if (c == EOF)
    {
      return -1;
    }

  return strlen(pachzLine);
}


