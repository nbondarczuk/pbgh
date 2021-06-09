#include <stdio.h>
#include "pbgh.h"
#include "gen.h"

toenBool goenVerbose;
toenBool goenTestOnly;
int goiCustomersNo;

extern int foiRTX_Open(char *, char *, char *);
extern int foiRTX_GetCustomerRTX(long, char *, char *);

int main(int argn, char *argv[]) 
{
  int rc;

  rc = foiRTX_Open("BCH", "SYSADM", "");
  if (rc != 0)
    {
      perror("Open");
      exit(1);
    }

  rc = foiRTX_GetCustomerRTX(atol(argv[1]), argv[2], argv[3]);
  if (rc != 0)
    {
      perror("Get");
      exit(1);
    }

  return 0;
}
