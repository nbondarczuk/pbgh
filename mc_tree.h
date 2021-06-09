typedef struct tostMCNode
{
  long soilContractCode;
  long soilMCCode;
  double soflScale;
  struct tostMCNode *spstLeft, *spstRight;
}
tostMCNode;

typedef struct tostMCTree
{
  struct tostMCNode *spstMCNode;
}
tostMCTree;

toenBool foenMCTree_Init();
toenBool foenMCTree_Delete();
toenBool foenMCTree_Insert(long, long, double);
int foiMCTree_Count(long);
toenBool foenMCTree_Splay(long);
toenBool foenMCTree_Get(long, long *, double *);
toenBool foenMCTree_GetNext(long, long *, double *);
