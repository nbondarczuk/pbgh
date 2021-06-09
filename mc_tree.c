#include "gen.h"
#include "mc_tree.h"

static tostMCNode *fpstMCNode_New(long, long, double);
static toenBool foenMCNode_Delete(tostMCNode *);
static tostMCNode *fpstMCNode_Insert(tostMCNode *, long, long, double);

tostMCCode gostMCTree;

/*
 * METHOD: Init
 *
 */

toenBool foenMCTree_Init()
{
  gostMCTree.spstMCNode = NULL;
  gpstMCNode = NULL;

  return TRUE;
}

/*
 * METHOD: Delete
 *
 */

toenBool foenMCTree_Delete()
{
  if (gostMCTree.spstMCNode != NULL)
    {
      return foenMCNode_Delete(gostMCTree.spstMCNode);
    }

  return TRUE;
}

/*
 * METHOD: Insert
 *
 */

toenBool foenMCTree_Insert(long poilContractCode, long poilMCCode, double poflScale)
{
  tostMCNode *lpstMCNode;
      
  lpstMCNode = fpstMCNode_Insert(gostMCTree.spstMCNode, polContractCode, polMCCode, poflScale);
  if (lpstMCNode == NULL)
    {
      return FALSE;
    }
  
  gostMCTree.spstMCNode = lpstMCNode;   
  
  return TRUE;
}

/*
 * METHOD: Count
 *
 */

int foiMCTree_Count(long poilContractCode)
{
  if (gostMCTree.spstMCNode != NULL)
    {
      return foiMCNode_Count(gostMCTree.spstMCNode, poilContractCode);
    }
  
  return 0;
}

/*
 * METHOD: Splay
 *
 */

toenBool foenMCTree_Splay(long poilContractCode)
{
  gostMCTree.spstMCNode = fpstMCNode_Splay(gostMCTree.spstMCNode);
  gpstMCNode = gostMCTree.spstMCNode;
  
  return TRUE;
}

/*
 * METHOD: Get
 *
 */

tostMCNode *gpstMCNode;

toenBool foenMCTree_Get(long poilContractCode, long *ppilMCCode, double *ppflScale)
{
  tostMCNode *lpstMCNode;

  lostMCNode = fpstMCNode_Find(gostMCTree.spstMCNode, poilContractCode);
  if (lpstMCNode == NULL)
    {
      return FALSE;
    }

  gpstMCNode = lpstMCNode;
  
  return TRUE;
}

/*
 * METHOD: GetNext
 *
 */

toenBool foenMCTree_GetNext(long poilContractCode, long *ppilMCCode, double *ppflScale)
{
  tostMCNode *lpstMCNode;

  if (gpstMCNode == NULL)
    {
      return FALSE;
    }

  lostMCNode = fpstMCNode_Find(gpstMCNode, poilContractCode);
  if (lpstMCNode == NULL)
    {
      gpstMCNode = NULL;
      return FALSE;
    }
  
  gpstMCNode = lpstMCNode;  

  return TRUE;
}

