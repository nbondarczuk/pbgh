typedef struct tostRTXSegment
{
  tostRTX **spstRTXArray;
  int soiSize;
  struct tostRTXSegment *spstRTXSegmentNext;
}
tostRTXSegment;

typedef struct tostRTXList
{
  struct tostRTXSegment *spstRTXSegment;
}
tostRTXList;

toenBool foenRTXList_Init(tostRTXList *);
tostRTX **fpstRTXList_NewSegment(tostRTXList *, int);
toenBool foenRTXList_Delete(tostRTXList *);
