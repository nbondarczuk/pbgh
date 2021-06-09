#include <string.h>
#include "gen.h"
#include "rtx.h"
#include "rtx_tree.h"

/*
 * Local methods
 */

static tostRTXArray *fpstRTXArray_New();
static toenBool foenRTXArray_Destroy(tostRTXArray *);
static tostRTXNode *fpstRTXNode_New(char *);
static toenBool foenRTXNode_Destroy(tostRTXNode *);
static tostRTXNode *foenRTXNode_Insert(tostRTXNode *, char *, tostRTX *);
static tostRTXArray *fpstRTXNode_Search(tostRTXNode *, char *);
static tostRTXArray *fpstRTXArray_Insert(tostRTXArray *, tostRTX *);
static toenBool foenRTXNode_Print(tostRTXNode *, int);
static toenBool foenRTXArray_Print(tostRTXArray *, int);

static toenBool foenRTXArray_Print(tostRTXArray *ppstRTXArray, int poiLevel)
{
  int i;
  tostRTX *lpstRTX;

  for (i = 0; i < poiLevel; i++) {fprintf(stderr, " "); }
  fprintf(stderr, "Size    : %d\n", ppstRTXArray->soiSize);

  for (i = 0; i < poiLevel; i++){fprintf(stderr, " ");}
  fprintf(stderr, "Max Size: %d\n", ppstRTXArray->soiMaxSize);

  /*
  for (i = 0; i < ppstRTXArray->soiSize; i++)
    {
      for (i = 0; i < poiLevel; i++){fprintf(stderr, " ");}
      lpstRTX = ppstRTXArray->spstArray[i];
      if (lpstRTX != NULL)
        {
          printf("RTX: %08ld %08ld %08ld\n", lpstRTX->rtx_sqn, lpstRTX->rtx_lfnr, lpstRTX->rtx_sub_lfnr);
        }
      else
        {
          printf("RTX: NULL\n");
        }
    }
  */

  return TRUE;
}

static tostRTXArray *fpstRTXArray_Insert(tostRTXArray *ppstRTXArray, tostRTX *ppstRTX)
{
  tostRTXArray *lpstRTXArray;
  int i;
  tostRTX **lppstRTX;

  lpstRTXArray = ppstRTXArray;

  if (lpstRTXArray->soiSize == lpstRTXArray->soiMaxSize)
    {
      fprintf(stderr, "Growing Size: %d -> %d\n", lpstRTXArray->soiSize, lpstRTXArray->soiSize * 2 );
      lpstRTXArray->soiMaxSize = lpstRTXArray->soiMaxSize * 2;
      lppstRTX = (tostRTX **)malloc(sizeof(tostRTX *) * lpstRTXArray->soiMaxSize);
      if (lppstRTX == NULL)
        {
          return NULL;
        }

      memcpy(lppstRTX, lpstRTXArray->spstArray, sizeof(tostRTX *) * lpstRTXArray->soiSize);
      free(lpstRTXArray->spstArray);
      lpstRTXArray->spstArray = lppstRTX;
    }
  
  i = lpstRTXArray->soiSize;
  lpstRTXArray->spstArray[i] = ppstRTX;
  lpstRTXArray->soiSize++;

  return lpstRTXArray;
}

static tostRTXArray *fpstRTXArray_New()
{
  tostRTXArray *lpstRTXArray;
  tostRTX *lpstRTX;
  int i;
  
  /*
   * Alloc memory
   */

  lpstRTXArray = (tostRTXArray *)malloc(sizeof(tostRTXArray));
  if (lpstRTXArray == NULL)
    {
      return NULL;
    }

  /*
   * Init structure
   */

  lpstRTXArray->spstArray = (tostRTX **)malloc(RTX_ARRAY_SIZE * sizeof(tostRTX *));
  if (lpstRTXArray->spstArray == NULL)
    {
      return NULL;
    }
  
  for (i = 0; i < RTX_ARRAY_SIZE; i++)
    {
      lpstRTXArray->spstArray[i] = NULL;
    }

  lpstRTXArray->soiMaxSize = RTX_ARRAY_SIZE;
  lpstRTXArray->soiSize = 0;

  return lpstRTXArray;
}

static toenBool foenRTXArray_Destroy(tostRTXArray *ppstRTXArray)
{
  free(ppstRTXArray->spstArray);
  free(ppstRTXArray);
  
  return TRUE;
}


static toenBool foenRTXNode_Destroy(tostRTXNode *ppstRTXNode)
{
  toenBool loenStatus = TRUE;

  if (ppstRTXNode->spstLeft != NULL)
    {
      loenStatus = foenRTXNode_Destroy(ppstRTXNode->spstLeft);
    }

  if (loenStatus == TRUE && ppstRTXNode->spstRight != NULL)
    {
      loenStatus = foenRTXNode_Destroy(ppstRTXNode->spstRight);
    }

  if (loenStatus == TRUE)
    {
      loenStatus = foenRTXArray_Destroy(ppstRTXNode->spstRTXArray);
      free(ppstRTXNode->spsnzKey);
    }

  return loenStatus;
}

static tostRTXNode *fpstRTXNode_New(char *pasnzKey)
{
  tostRTXNode *lpstRTXNode;

  lpstRTXNode = (tostRTXNode *)malloc(sizeof(tostRTXNode));
  if (lpstRTXNode == NULL)
    {
      return NULL;
    }
  
  strcpy(lpstRTXNode->spsnzKey, pasnzKey);
  lpstRTXNode->spstRTXArray = NULL;
  lpstRTXNode->spstLeft = NULL;
  lpstRTXNode->spstRight = NULL;

  return lpstRTXNode;
}

static tostRTXNode *fpstRTXNode_Insert(tostRTXNode *ppstRTXNode, char *pasnzKey, tostRTX *ppstRTX)
{
  int n;
  tostRTXNode *lpstRTXNode;

  if (ppstRTXNode == NULL)
    {
      fprintf(stderr, "New key: %s\n", pasnzKey);
      lpstRTXNode = fpstRTXNode_New(pasnzKey);
      if (lpstRTXNode == NULL)
        {
          return NULL;
        }
      lpstRTXNode->spstRTXArray = fpstRTXArray_New();
      lpstRTXNode->spstRTXArray = fpstRTXArray_Insert(lpstRTXNode->spstRTXArray, ppstRTX);
    }
  else
    {
      lpstRTXNode = ppstRTXNode;
      n = strcmp(pasnzKey, lpstRTXNode->spsnzKey);
      if (n < 0)
        {
          lpstRTXNode->spstLeft = fpstRTXNode_Insert(lpstRTXNode->spstLeft, pasnzKey, ppstRTX);
        }
      else if (n > 0)
        {
          lpstRTXNode->spstRight = fpstRTXNode_Insert(lpstRTXNode->spstRight, pasnzKey, ppstRTX);
        }
      else
        {
          lpstRTXNode->spstRTXArray = fpstRTXArray_Insert(lpstRTXNode->spstRTXArray, ppstRTX);
        }
    }

  return lpstRTXNode;
}

static toenBool foenRTXNode_Print(tostRTXNode *ppstRTXNode, int poiLevel)
{
  toenBool loenStatus;
  int i;

  for (i = 0; i < poiLevel; i++)
    {
      fprintf(stderr, " ");
    }
  
  fprintf(stderr, "Key[%02d] : %s\n", strlen(ppstRTXNode->spsnzKey), ppstRTXNode->spsnzKey);
  loenStatus = foenRTXArray_Print(ppstRTXNode->spstRTXArray, poiLevel);
  
  if (loenStatus == TRUE && ppstRTXNode->spstLeft != NULL)
    {
      loenStatus = foenRTXNode_Print(ppstRTXNode->spstLeft, poiLevel + 1);
    }

  if (loenStatus == TRUE && ppstRTXNode->spstRight != NULL)
    {
      loenStatus = foenRTXNode_Print(ppstRTXNode->spstRight, poiLevel + 1);
    }
  
  return loenStatus;
}

tostRTXArray *fpstRTXNode_Search(tostRTXNode *ppstRTXNode, char *pasnzKey)
{
  int n;

  n = strcmp(pasnzKey, ppstRTXNode->spsnzKey);
  if (n < 0)
    {
      if (ppstRTXNode->spstLeft != NULL)
        {
          return fpstRTXNode_Search(ppstRTXNode->spstLeft, pasnzKey);
        }
    }
  else if (n > 0)
    {
      if (ppstRTXNode->spstRight != NULL)
        {
          return fpstRTXNode_Search(ppstRTXNode->spstRight, pasnzKey);
        }
    }
  else
    {
      return ppstRTXNode->spstRTXArray;
    }

  /*
   * Nothing found
   */

  return NULL;
}

/*
 * Exported methods
 */

/*
 * METHOD: Init
 *
 */

static int soiRTXAccessNo = 0;

toenBool foenRTXTree_Init(tostRTXTree *ppstRTXTree)
{
  toenBool loenStatus;

  if (soiRTXAccessNo++ > 0)
    {
      loenStatus = foenRTXTree_Destroy(ppstRTXTree);
    }
  else
    {
      loenStatus = TRUE;
    }

  if (loenStatus == TRUE)
    {
      ppstRTXTree->spstRTXNode = NULL;
    }

  return loenStatus;
}

/*
 * METHOD: Destroy
 *
 */

toenBool foenRTXTree_Destroy(tostRTXTree *ppstRTXTree)
{
  if (ppstRTXTree->spstRTXNode != NULL)
    {
      return foenRTXNode_Destroy(ppstRTXTree->spstRTXNode);
    }
  
  return TRUE;
}

/*
 * METHOD: Insert
 *
 */

toenBool foenRTXTree_Insert(tostRTXTree *ppstRTXTree, char *pasnzKey, tostRTX *ppstRTX)
{
  ppstRTXTree->spstRTXNode = fpstRTXNode_Insert(ppstRTXTree->spstRTXNode, pasnzKey, ppstRTX);
  return TRUE;
}

/*
 * METHOD: Search
 *
 */

tostRTXArray *fpstRTXTree_Search(tostRTXTree *ppstRTXTree, char *pasnzKey)
{
  return fpstRTXNode_Search(ppstRTXTree->spstRTXNode, pasnzKey);
}

/*
 * METHOD: Print
 *
 */

toenBool foenRTXTree_Print(tostRTXTree *ppstRTXTree)
{
  return foenRTXNode_Print(ppstRTXTree->spstRTXNode, 0);
}



