toenBool foenTIMMTypeCheck(struct s_TimmInter *, char *);
struct s_imd_seg *fpstFindItemDescription(struct s_imd_seg *, char *);
struct s_qty_seg *fpstFindQuantity(struct s_qty_seg *, char *, char *);
struct s_moa_seg *fpstFindPaymentSegment(struct s_group_23 *, char *);
struct s_group_2 *fpstFindPartyBlock(struct s_group_2 *, char *); 
struct s_moa_seg *fpstFindPaymentSegment(struct s_group_23 *, char *); 
struct s_imd_seg *fpstFindItemDescription(struct s_imd_seg *, char *);
struct s_rff_seg *fpstFindReference(struct s_group_3 *, char *); 
struct s_qty_seg *fpstFindQuantity(struct s_qty_seg *, char *, char *); 
struct s_xcd_seg *fpstFindXCDSegment(struct s_group_99 *, int);
char *fpchzGetField(int, char *);
struct s_moa_seg *fpstFindMainPaymentSegment(struct s_group_45 *, char *);
struct s_rff_seg *fpstFindItemReference(struct s_group_26 *, char *);
int foiCountSubscribers(struct s_TimmInter *);
int foiCountSubscriberContracts(struct s_TimmInter *, int); 
struct s_group_22 *lpstFindSubscriberSegment(struct s_group_22 *, int);
struct s_group_22 *lpstFindSubscriberContractSegment(struct s_group_22 *, int);
int foiCountSimCallRecords(struct s_TimmInter *, int, int); 
struct s_xcd_seg *lpstFindSimCallRecord(struct s_TimmInter *, int, int, int); 
toenBool foenProcessUsageSegments(struct s_TimmInter *, int, int);
char *fpchzGetField(int, char *);

struct s_qty_seg *fpstNewFloatQuantity(char *, char *, double); 
struct s_qty_seg *fpstNewLongQuantity(char *, char *, long); 
void fovdUpdateQuantityList(struct s_qty_seg **, struct s_qty_seg **); 

struct s_pri_seg *fpstNewFloatPrice(char *, double, char *);
struct s_group_25 *fpstUpdateG25List(struct s_group_25 **, struct s_pri_seg **); 
