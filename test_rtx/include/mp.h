/**************************************************************************************************
 *                                                                                          
 * MODULE: MP
 *                                                                                          
 * AUTHOR: N.Bondarczuk                                                
 * 
 * CREATION DATE : 15.03.98                                              
 *
 **************************************************************************************************
 */


#define PRINT_RI(s) { printf("%f\t%f\t%ld\t%f\t%d\t%f\n", s.sodIL, s.sodIR, s.solIM, s.sodIC, s.solCalls, s.sodVal); }
  
#define FILL_RI(s, a1, a2, a3, a4) {\
    s.sodIL = a1; s.sodIR = a2; s.solIM = a3; s.sodIC = a4; s.solCalls = 0; s.sodVal = 0.0; }
                                                                                              


/*
 * MPUTMTAB
 */

typedef struct 
{
  long solIndex;
  long solTMCODE;        /* Tariff Model Code */
  long solVSCODE;        /* Tariff Model Version */
  char sachzVSDATE[7];   /* Tariff Model Effective Date */
  long solPLCODE;        
  char sachzPLMNNAME[31];
  char sachzDES[31]; 
  char sachzSHDES[6];

} tostMPUTMTAB;

/*
 * MPUSNTAB x MPULKNXC
 */

typedef struct 
{
  long solIndex;
  long solSNCODE;        /* Service Code */
  char sachzDES[31];
  char sachzSHDES[6];
  char sochSNIND;        /* Network Service Indicator: Y - Network, N - VAS */

  char sochNETSNIND;     /* Network Subtype U=Usage, E=Event from MPULKNXC */
  long solSCCODE;        /* Service Class (Market) from  MPULKNXC */

} tostMPUSNTAB; 

/*
 * MPUSPTAB
 */

typedef struct
{
  long solIndex;
  long solSPCODE;        /* Service Package Code */
  char sachzDES[31];
  char sachzSHDES[6];
  long solSPTYPE;        /* Package used for EC/IR */
  
} tostMPUSPTAB;

/*
 * MPUZNTAB
 */

typedef struct
{
  long solIndex;
  long solZNCODE;        /* Tariff zone Code */ 
  char sachzDES[31];
  char sachzSHDES[6];
  
} tostMPUZNTAB;

/*
 * MPUTTTAB
 */

typedef struct 
{
  long solIndex;
  long solTTCODE;        /* Tariff Time Code */
  char sachzDES[31];
  char sachzSHDES[6];

} tostMPUTTTAB;

/*
 * MPULKTMM
 */

typedef struct 
{
  long solIndex;
  long solTMCODE;        /* Tariff Model Code */ 
  long solVSCODE;        /* Tariff Model Varsion Code */
  char sachzVSDATE[7];   /* Tariff Model Varsion Effective Date */
  long solSPCODE;        /* Service Package Code */ 
  long solSNCODE;        /* Service code */
  char sochUSGIND;       /* Usage Indicator: U - Usage, E - Event */
  char sochRATEIND;      /* Rate Indicator */
  long solUPCODE;        /* Usage Package Code */
  long solRICODE;        /* Rating Interval Package Code */
  long solRICODE_INBOUND;/* Rating Interval Inbound Package Code */
  long solEVCODE;        /* Event Package Code */
  long solEGCODE;        /* EG Package Code */ 
  long solFUCODE;        /* FUoM Package Code */
  long solPPCODE;        /* Price Plan Package Code */

} tostMPULKTMM;

/*
 * MPURITAB
 */

typedef struct 
{
  long solIndex;
  long solRICODE;        /* Rating Interval Code */
  char sachzDES[31];
  char sachzSHDES[6];
  
} tostMPURITAB;

/*
 * Rating Interval from MPULKRIM
 */

typedef struct
{
  double sodIL;         /* Interval Length in UoM */
  double sodIR;         /* Interval Rounding in UoM */
  long solIM;           /* Interval Multiplicator */
  double sodIC;         /* number of Clicks */
  long solCalls;        /* number of intervals taken into account */
  double sodVal;        /* value */

} tostRatingInterval; 


#define PRINT_UI_INFO(ui) printf("%d.%d.%d.%d.%c.%d.%d = %s.%d.%s.%s.%c.%s.%s\n", \
                                 ui->solTMCODE, ui->solVSCODE,\
                                 ui->solSPCODE, ui->solSNCODE,\
                                 ui->sochType, ui->solTTCODE,\
                                 ui->solZNCODE,\
                                 ui->sachzTMSHDES, ui->solVSCODE, ui->sachzSPSHDES,\
                                 ui->sachzSNSHDES, ui->sochType,\
                                 ui->sachzTTSHDES, ui->sachzZNSHDES);

typedef struct
{
  long solTMCODE;        /* Tariff Model Code */ 
  char sachzTMSHDES[6];  /* Tariff Model Shdes */ 
  long solVSCODE;        /* Tariff Model Varsion Code */
  long solSPCODE;        /* Service Package Code */ 
  char sachzSPSHDES[6];  /* Service Package Shdes */ 
  long solSNCODE;        /* Service Code */ 
  char sachzSNSHDES[6];  /* Service Shdes*/
  char sochType;
  
  long solRICODE;
  long solRIVERSION;
  long solEGLCODE;
  long solEGLVERSION;

  long solTTCODE;
  char sachzTTSHDES[6];  /* */
  long solZNCODE;
  char sachzZNSHDES[6];  /* */

} tostUsageItem;

/*
 * MPULKRIM
 */

typedef struct
{
  long solIndex;
  long solRICODE;        /* Rating Interval Code */
  long solVSCODE;        /* Rating Interval Version */
  char sachzVSDATE[7];   /* Rating Interval Version Effective Date */

  long solGVCODE;        /* Tariff Zone Package Code */
  long solGVVSCODE;      /* Tariff Zone Package Version Code */
  long solZNCODE;        /* Tariff Zone Code */

  long solTWCODE;        /* Tariff Time Package Code */
  long solTWVSCODE;      /* Tariff Time Package Version Code */
  long solTTCODE;        /* Tariff Time Code */
  
  char sochTYPEIND;      /* Type Indicator : A - Air, L - Land, C - CF, R - Roaming leg */
  long solUMCODE;        /* Unit Measaure Code */

  tostRatingInterval sastRI[10];
  
} tostMPULKRIMM;

/*
 * MPUEVTAB
 */

typedef struct
{
  long solIndex;
  long solEVCODE;        /* Event Code */
  char sachzDES[31];
  char sachzSHDES[6];

} tostMPUEVTAB;

/*
 * MPULKEVM 
 */

typedef struct
{
  long solIndex;
  long solEVCODE;        /* Event Charge Package Code */
  long solVSCODE;        /* Event Charge Package Version */
  char sachzVSDATE[7];   /* Event Package Effective Date */

  long solTWCODE;        /* Tariff Time Package Code */
  long solTWVSCODE;      /* Tariff Time Package Version Code */
  long solTTCODE;        /* Tariff Time Code */

  long solUMCODE;        /* UofM Code */

  double sodEV_REG;     /* Price of Registration */
  double sodEV_ACT;     /* Price of Activation */
  double sodEV_DEA;     /* Price of Deactivation */
  double sodEV_INT;     /* Price of Interrogation */
  double sodEV_INV;     /* Price of Invocation */
  double sodEV_ERA;     /* Price of Erasure */
  double sodEV_PWC;     /* Price of Password Change */

} tostMPULKEVM;

typedef struct
{
  double sodEV_REG;     /* Price of Registration */
  long   solEV_REG_Count;
  double sodEV_ACT;     /* Price of Activation */
  long   solEV_ACT_Count;
  double sodEV_DEA;     /* Price of Deactivation */
  long   solEV_DEA_Count;
  double sodEV_INT;     /* Price of Interrogation */
  long   solEV_INT_Count;
  double sodEV_INV;     /* Price of Invocation */
  long   solEV_INV_Count;
  double sodEV_ERA;     /* Price of Erasure */
  long   solEV_ERA_Count;
  double sodEV_PWC;     /* Price of Password Change */  
  long   solEV_PWC_Count;

} tostEventPrice;

/*
 * MPUPUTAB
 */

typedef struct {
  long solIndex;
  char sachzSHDES[6];
  long solTMCODE;
  char sachzVSDATE[7];
  char sachzVSENDDATE[7];
  double soflPPUIC;
  double soflPPUAIR;
} tostMPUPUTAB;


/*
 * MPSUMTAB
 */

typedef struct
{
  long solUMCODE;
  char sachzDES[31];
  char sachzSHDES[6];
  char sochSPLITIND;
  char sochUMTYPE;
  char sochUMOPER;
  long solUMFACTOR;
  long solUMTHRESH;
  
} tostMPSUMTAB;

/*
 * MPUFFTAB
 */

typedef struct
{
  long lIndex;
  long lCode;
  long lCoId;
  char szDes[21];
  char szShdes[7];
  double fScaleFactor;

} stMPUFFTAB;



/*
 * MPULKTMM
 */

typedef struct
{
  long soilIndex;

  long soilTMCODE;
  long soilVSCODE;
  char sasnzVSDATE[7];
  long soilSPCODE;
  long soilSNCODE;
  char sochUSGIND;
  char sochRATEIND;

  long soilRICODE;
  long soilEVCODE;
  long soilEGCODE;
  long soilFUCODE;
  long soilPPCODE;

} stMPULKTMM;


/*
 * MPULKRIM
 */

typedef struct
{
  long soilIndex;

  long soilRICODE;
  long soilVSCODE;
  char sasnzVSDATE[7];
  long soilGVCODE;
  long soilGVVSCODE;
  long soilZNCODE;
  long soilTWCODE;
  long soilTWVSCODE;
  long soilTTCODE;
  char sochTYPEIND;
  char sochRNDIND;
  char sochSPLITIND;
  long soilUMCODE;
  
  double soflIL01;
  double soflIR01;
  int soiIM01;
  double soflIC01;

  double soflIL02;
  double soflIR02;
  int soiIM02;
  double soflIC02;

  double soflIL03;
  double soflIR03;
  int soiIM03;
  double soflIC03;

  double soflIL04;
  double soflIR04;
  int soiIM04;
  double soflIC04;

  double soflIL05;
  double soflIR05;
  int soiIM05;
  double soflIC05;

  double soflIL06;
  double soflIR06;
  int soiIM06;
  double soflIC06;

  double soflIL07;
  double soflIR07;
  int soiIM07;
  double soflIC07;

  double soflIL08;
  double soflIR08;
  int soiIM08;
  double soflIC08;

  double soflIL09;
  double soflIR09;
  int soiIM09;
  double soflIC09;

  double soflIL10;
  double soflIR10;
  int soiIM10;
  double soflIC10;

} stMPULKRIM;

/*
 * MPULKEVM
 */

typedef struct
{
  long soilIndex;

  long soilEVCODE;
  long soilVSCODE;
  char sasnzVSDATE[7];
  long soilTWCODE;
  long soilTWVSCODE;
  long soilTTCODE;
  long soilUMCODE;
  
  double soflEV_REG;
  double soflEV_ACT;
  double soflEV_DEA;
  double soflEV_INT;
  double soflEV_INV;
  double soflEV_ERA;
  double soflEV_PWC;

} stMPULKEVM;

