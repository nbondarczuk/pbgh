#define RTX_ARRAY_SIZE 8

typedef struct tostRTXArray
{
  tostRTX **spstArray;
  int soiMaxSize;
  int soiSize;
}
tostRTXArray;

typedef struct tostRTXNode
{
  char *spsnzKey;
  struct tostRTXArray *spstRTXArray;
  struct tostRTXNode *spstLeft, *spstRight;
}
tostRTXNode;

typedef struct
{
  tostRTXNode *spstRTXNode;
}
tostRTXTree;

toenBool foenRTXTree_Init(tostRTXTree *);
toenBool foenRTXTree_Destroy(tostRTXTree *);
toenBool foenRTXTree_Insert(tostRTXTree *, char *, tostRTX *);
tostRTXArray *fpstRTXTree_Search(tostRTXTree *, char *);
toenBool foenRTXTree_Print(tostRTXTree *);



