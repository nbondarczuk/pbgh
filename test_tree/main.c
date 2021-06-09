#include <stdio.h>
#include "gen.h"
#include "rtx.h"
#include "rtx_tree.h"

#define N 14

tostRTXTree sostRTXTree;

main()
{
  toenBool loenStatus;
  tostRTX lostRTX, *lpstRTX;
  tostRTXArray *lpstRTXArray;
  char lasnzKey[64];
  int i;
  char *lasnzKeys[] = 
  {
    "1.2.3", 
    "2.2.3", 
    "2.2.3", 
    "3.3.3", 
    "0.2.3", 
    "4.2.3", 
    "2.2.3", 
    "2.2.3", 
    "2.2.3", 
    "2.2.3", 
    "2.2.3", 
    "2.2.3", 
    "2.2.3", 
    "1.2.3"
  };
  
  loenStatus = foenRTXTree_Init(&sostRTXTree);
  if (loenStatus != TRUE)
    {
      perror("RTXTree.Init");
      exit(1);
    }

  for (i = 0; i < N; i++)
    {
      strcpy(lasnzKey, lasnzKeys[i]);

      lpstRTX = (tostRTX *)malloc(sizeof(tostRTX));
      if (lpstRTX == NULL)
        {
          perror("malloc");
          exit(1);          
        }

      lpstRTX->rtx_sqn = i;
      lpstRTX->rtx_lfnr = i * 2;
      lpstRTX->rtx_sub_lfnr = i * 4;

      fprintf(stderr, "Key: %s, Val: %ld\n", lasnzKey, i);
      loenStatus= foenRTXTree_Insert(&sostRTXTree, lasnzKey, lpstRTX);
      if (loenStatus != TRUE)
        {
          perror("RTXTree.Insert");
          exit(1);
        }
    }

  strcpy(lasnzKey, "2.2.3");
  lpstRTXArray = fpstRTXTree_Search(&sostRTXTree, lasnzKey);
  if (lpstRTXArray == NULL)
    {
      fprintf(stderr, "No key: %s\n", lasnzKey);
    }
  else
    {
      printf("Key: %s\n", lasnzKey);
      for (i = 0; i < lpstRTXArray->soiSize; i++)
        {
          lpstRTX = lpstRTXArray->spstArray[i];
          if (lpstRTX != NULL)
            {
              printf("RTX: %08ld %08ld %08ld\n", lpstRTX->rtx_sqn, lpstRTX->rtx_lfnr, lpstRTX->rtx_sub_lfnr);
            }
          else
            {
              printf("RTX: NULL\n");
            }
        }
    }

  loenStatus = foenRTXTree_Print(&sostRTXTree);
  if (loenStatus != TRUE)
    {
      perror("RTXTree.Print");
      exit(1);
    }
  
  loenStatus = foenRTXTree_Destroy(&sostRTXTree);
  if (loenStatus != TRUE)
    {
      perror("RTXTree.Destroy");
      exit(1);
    }
}
