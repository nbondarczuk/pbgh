typedef struct
{
  char sasnzArticleNumber[36];
  char sasnzItemId[36];
  char sasnzServiceDes[76];
  char sasnzServiceShdes[6];  
  int soiCallsNumber;
  int soiCallsTime;
  double soflAmount;
  double soflFullAmount;
  double soflIntervalLength;
  int soiIntervalsNumber;
  double soflIntervalPrice;
}
tostSumUsage;

toenBool foenG22_Load(struct s_group_22 *, tostSumUsage *);
struct s_group_22 *fpstG22_New(tostSumUsage *);
struct s_lin_seg *fpstLIN_New(char *, char *, char *);
struct s_pia_seg *fpstPIA_New(char *);
struct s_imd_seg *fpstIMD_New(char *, char *, char *);
struct s_group_23 *fpstG23_New(struct s_moa_seg *);
struct s_group_25 *fpstG25_New(struct s_pri_seg *);


struct s_qty_seg *fpstQTY_NewDouble(char *, double, char *);
struct s_qty_seg *fpstQTY_NewInt(char *, int, char *);

struct s_moa_seg *fpstMOA_New(char *, double, char *, char *, char *);

toenBool foenLIN_Load(struct s_lin_seg *, char *);
toenBool foenPIA_Load(struct s_pia_seg *, char *);
toenBool foenIMD_Load(struct s_imd_seg *, char *, char *, char *);

struct s_pri_seg *fpstPRI_New(char *, double, char *);

toenBool foenQTY_Load(struct s_qty_seg *, char *, char *, char *);

toenBool foenG23_MOA_Load(struct s_group_23 *, char *, char *);


#define talloc(a) calloc(1, a)
