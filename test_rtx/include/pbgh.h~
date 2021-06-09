/*******************************************************************************
 * LH-Specification GmbH 1995.
 *
 * All rights reserved.
 * Copying of this software or parts of this software is a violation of German
 * and International laws and will be prosecuted.
 *
 * Project  :   BGH
 *
 * File     :   bgh.h
 * Created  :   Feb. 1996
 * Author(s):   B. Michler
 *
 * Modified :
 * 26.06.96	B. Michler	additional parameter of type stDBCOL * for
 * 				database access routines
 * 14.08.96	B. Michler	added parameter for passing the document type
 *				to process
 *
 *******************************************************************************/

#ifndef BGH_H
#define BGH_H

#if 0 /* just for version.sh */
static char *SCCS_VERSION = "1.45";
#endif


#include "parser.h"            /* needed for typedef */

/* Defines                      */
#define COMMON_HEADER_FILE "header.ps"
#define LOCAL_CURRENCY "PLN"

/* take care of lowercase define for BSCS4 */
#ifdef bscs4
#  define BSCS4		1
#endif


#define FALSE  	0
#define TRUE	1

#define UINT	unsigned int
#define ULONG	unsigned long
#define BYTE	unsigned char

#ifndef PATH_MAX                /* ensure that PATH_MAX is set */
#define PATH_MAX    1024
#endif
#define BGHSTRLEN   32          /* standard string in BGH is 32 bytes long  */
#define DEFTIMMLEN  2048        /* starting length for TIMM-message         */

#define DEFNRINVOICES   1
#define DEFNOCUSTID	-1	/* no customer id was set 		    */

/* --- filenames --- */
#define BGH_LOGFILE "./bgh.log"


/* Enumerations                 */

typedef enum {                  /* document type    */
  BAL_TYPE = 0,
  INV_TYPE = 1,
  ITB_TYPE = 2,
  SUM_TYPE = 3,
  ROA_TYPE = 4,
  LGN_TYPE = 5,
  INV_DCH  = 6,
  ITM_DCH  = 7,
  DNL_DWH  = 8,
  WLL_DWH  = 9,
  INV_IR   = 10,
  INV_EC   = 11,
  TYPLAST			/* the last one */
} TYPEID;

typedef enum {                  /* customer-process type    */
  SINGLE   = 0,
  MULTIPLE = 1,
  CUR2ND   = 2                /* 2nd cursor */ 
} TYPECUST;

/* logging levels, the last has to be LOG_MAX */
typedef enum {
  LOG_DEBUG = 1,          /* logs everything, for debugging purposes */
  LOG_TIMM,
  LOG_CUSTOMER,
  LOG_NORMAL,
  LOG_SELDOM,
  LOG_MAX
} LOGLEVEL;

/* enumerations for error handling */
enum ERROR_TYPES {
  UNDEFINED = 0, 
  NORMAL, 
  WARNING, 
  CRITICAL
};

enum {
  NO_PARALLEL_PROC = 0,
  NR_OF_PROC_MIN   = 2,
  NR_OF_PROC_MAX   = 50
};


/* CAUTION CAUTION CAUTION CAUTION CAUTION CAUTION CAUTION CAUTION CAUTION 	*/
/* if an error is added to the following table, be sure to add a corres-	*/
/* ponding line to 'bgherrh.c' !!						*/
enum ERROR_NUMBERS {
  NO_ERROR = 0,
  
  ERRH_SIGINT   	= 2,      /* signals */
  ERRH_SIGILL   	= 4,
  ERRH_SIGABORT 	= 6,
  ERRH_SIGFPE   	= 8,
  ERRH_SIGSEGV  	= 11,
  ERRH_SIGTERM  	= 15,

  MAIN_NOTIMM   	= 100,	   /* no TIMM message */
  MAIN_INVTYPE,           	   /* invalid TIMM type */
  MAIN_PARAMERR,          	   /* error in passed parameter */
  MAIN_LOGLEVERR,         	   /* invalid loglevel */
  MAIN_NOFNAME,           	   /* filename missing although option needs one */
  MAIN_UNKNOWNOPT,        	   /* unknown option */
  MAIN_VERERROR,          	   /* wrong version of BGH */
  MAIN_FORKERR,	 	         /* cannot fork process */
  MAIN_NR_OF_PROC_ERR,      	/* wrong number of processes */

  PROC_DBOPENERR 	= 200,	/* cannot open database */
  PROC_INVERR,     	      /* error reading invoice */
  PROC_TIMMVERERR,        	/* wrong version of TIMM message */
  PROC_NOTIMMVER,         	/* TIMM version not found in TIMM message */
  PROC_NOBASENAME,        	/* no basename for file input of TIMM */
  PROC_MULTITIMM,         	/* more TIMMs than expected in database */
  
  PARSE_MALLOC 	= 300,	   /* no memory for malloc */
  PARSE_NOPOINTER,        	/* pointer not set */
  PARSE_NOATTACH,         	/* Attach-point for segment not set */
  PARSE_MANDMISSING,      	/* mandatory element missing */
  PARSE_ELEMTOOLONG,      	/* element is too long */
  PARSE_ILLEGALELEM,      	/* element not allowed a this position */
  PARSE_TOOMUCHELEM,      	/* too much elements in a repetition */

  ACC_NOPASSWORD	= 400,	/* no database-password for BGH */
  ACC_NOCONNECTSTRING,    	/* no database-connect-string for BGH */
  ACC_NOBASEDIR,       	/* no base-directory info  */
  ACC_SECCURSOR_ILLUSAGE, 	/* try to close 2nd cursor with illegal usage */
  ACC_NOVERSIONCHECK,		/* no version check to be done */
  ACC_NOPROCPROG,		/* no bill image processing program */

  ESQL_ERROR		= 500,	/* error in esql execution */
  ESQL_DBCONNECT,         	/* cannnot connect database */
  ESQL_DBDISCONNECT,      	/* cannnot disconnect database */
  ESQL_OPENMAINCURSOR,    	/* error by open from main cursor */
  ESQL_OPENSECONDCURSOR,  	/* error by open from second cursor */
  ESQL_OPENSECMAINCLOSE,  	/* try to open second cursor with main closed */
  ESQL_OPENTESTCURSOR,    	/* error by open from test cursor */
  ESQL_OPENBALANCECURSOR,     /* error by open from balance cursor */
  ESQL_CLOSEMAINCURSOR,   	/* error by close from main cursor */
  ESQL_CLOSESECONDCURSOR, 	/* error by close from second cursor */
  ESQL_CLOSETESTCURSOR,   	/* error by close from test cursor */
  ESQL_CLOSEBALANCECURSOR,    /* error by close from balance cursor */
  ESQL_FETCHCURSOR,       	/* error by fetching for main(second) cursor */
  ESQL_FETCHTESTCURSOR,   	/* error by fetching for test cursor */
  ESQL_SECCURSOR_ILLUSAGE,	/* try to manipulate second cursor with illegal usage */
  ESQL_ILLCUSTPROC,       	/* illegal customer-process-usage for main cursor */
  ESQL_SELECT,            	/* error in SQL-SELECT statement */
  ESQL_SELVERS,           	/* error in SQL-SELECT statement version-number */
  ESQL_CHECKBATCH,           	/* error in SQL-SELECT statement version-check  */
  ESQL_SELBASEDIR,          	/* error in SQL-SELECT statement base directory */
  ESQL_NULLBASEDIR,          	/* NULL detected in database for base directory */
  ESQL_SELPROCPROG,          	/* error in SQL-SELECT statement bill-images-process-program */
  ESQL_NULLPROCPROG,         	/* NULL detected in database for bill-images-process-program */
  ESQL_SELCOUNT,          	/* error in SQL-SELECT statement count(*) Xs */
  ESQL_UPDATE,            	/* error in SQL-UPDATE statement */
  ESQL_UPDATEMOREASONE,   	/* update more as a record */
  ESQL_RESETX,            	/* error in SQL-UPDATE statement reset Xs */
  ESQL_NOX,               	/* nothing to reset */
  ESQL_COMMIT,            	/* error in SQL-COMMIT statement */
  ESQL_COMMIT_DBCLOSED,   	/* commit with closed database */
  ESQL_ROLLBACK,          	/* error in SQL-ROLLBACK statement */
  ESQL_ROLLBACK_DBCLOSED, 	/* rollback with closed database */
  ESQL_ALLOC,             	/* no memory for malloc */
  ESQL_REALLOC,           	/* no memory for realloc */
  ESQL_NOMOREDATA,        	/* no more data to read from database */
  ESQL_DOCSIZENULL,       	/* readed document has size zero */
  ESQL_BILLINSERT,        	/* insert into table bill_images */
  ESQL_BILL_ILLTYPE,      	/* illegal timm-type for bill_image */
  ESQL_BILL_DBCLOSED,     	/* insert bill_image with closed database */
  ESQL_BILL_NULL,         	/* nothing to insert, no image-information */
  ESQL_INV_COLUMN,		   /* invalid column name */
  ESQL_SET_NLS_LANG,       /* error during set up of NLS_LANG for sessiog */

  FILE_MALLOC		= 600,	/* no memory for malloc */
  FILE_OPEN,              	/* file open failed */
  FILE_CLOSE,             	/* file close failed */
  FILE_WRITE,             	/* writing to file failed */
  FILE_INVALIDNAME,       	/* name was not from invoice */
  FILE_PATHLENGTH,        	/* path was too long for given buffer */
  FILE_NOMATCH,           	/* no match for wildcard search */
  FILE_OPENDIR,           	/* open on directory failed */
  FILE_NOACCESS,		/* no access to directory */

  FORM_LINETOOSHORT	= 700,	/* truncated line in layout-file */
  FORM_LISTERROR,         	/* error in list */
  FORM_CANNOTOPEN,        	/* error opening layout-file */
  FORM_NOCLOSE,           	/* error closing layout-file */
  FORM_OUTMEM,            	/* no memory from malloc */
  FORM_LINEERR,           	/* error in line */
  
  LAY_NULLPOINTER	= 800,	/* wrong NULL-pointer in paField */
  LAY_NOMETAHEAD,		/* file 'metahead.ps' not found */
  LAY_INEXPCOUNT,		/* count was not expected in vdInitLayout */
  LAY_MALLOC,			/* no memory from malloc */
  LAY_INITLINE,		/* line was not correct initialized */
  LAY_YPOSOFFPAGE,		/* element should be printed off pageend */
  LAY_XPOSOFFPAGE,		/* element should be printed off pageend */
  LAY_FROMTOOBIG,		/* 'from' element too big */
  LAY_TOFROM,			/* 'to' smaller than 'from' */
  LAY_TOTOOBIG,		/* 'to' element too big */
  LAY_TOUNDEFINED,		/* 'to' element undefined (may not be 0) */
  LAY_NOBLOCKSIZE,		/* no blocksize in put_next_line */

    /* Errors from post parser (data collection unit) */
    PREP_UNDEF_DOCTYPE	= 900,
    PREP_UNDEF_LIN_7140,
    PREP_UNDEF_PIA_7140,
    PREP_UNDEF_PIA_7140_PpU,
    PREP_UNDEF_PIA_7140_CHARGE_TYPE,
    PREP_ILL_PIA_7140,
    PREP_NO_IMD_WITH_VPLMN_CODE,
    PREP_NO_IMD_SERVICE_CODE_ID,
    PREP_MALLOC_FAILED,
    PREP_ILL_MALLOC_REQUEST,
    PREP_IMD_NO_SERVICE_DESCR,
    PREP_PRI_NO_PRICE,
    PREP_MOA_NO_NETAMOUNT,
    PREP_MOA_NO_GROSAMOUNT,
    PREP_MOA_NO_VAT_FOR_ITEM,
    PREP_NO_TAX_RATE,
    PREP_PIA_NO_ADD_INFO,
    PREP_NO_QTY,
    PREP_NAD_NO_5035,
    PREP_UNDEF_DOC_LANG,
    PREP_FIXED_UNDEF,
    PREP_FIXED_WRONG_VERSION,
    PREP_FIXED_FILE_MISSING,
    PREP_G23_MOA_NO_THRESHOLD,
    PREP_AMNT_LESS_THAN_THRESHOLD,
    PREP_XCD_UNDEF_CALL_DEST,
    PREP_XCD_UNDEF_CALL_TYPE,
    PREP_G45_NO_ROUNDING_DIFF,
    PREP_MOA_NO_SUM_TO_PAY,
    PREP_MOA_NO_INV_TAX_SUM,
    PREP_MOA_NO_NETTO_INV_SUM,
    PREP_MOA_NO_INV_SUM_AMOUNT,
    PREP_G3_NO_SENDER_VAT_NO,
    PREP_FII_NO_SENDER_POST_NO,
    PREP_NO_SENDER_FAX_NO,
    PREP_NO_SENDER_TELEX_NO,
    PREP_NO_SENDER_PHONE_NO,
    PREP_NO_SENDER_ADDRESS,
    PREP_NO_SENDER_PHONE_FAX_TELEX,
    PREP_NO_CUSTOMER_ADDRESS,
    PREP_NO_CUSTOMER_NUMBER,
    PREP_NO_INVOICE_NUMBER,
    PREP_NO_INVOICE_DATE,
    PREP_NO_BILLING_PERIOD,
    PREP_NO_RECEIVER_ADDRESS,
    PREP_G45_NO_MOA,
    PREP_NO_G45,
    PREP_G22_G23_NO_MOA,
    PREP_INVALID_G22_SORTED,
    PREP_UNDEF_NESTING_LEVEL,
    PREP_NO_G22,
    PREP_NO_G22_G26_RFF,
    PREP_NO_G22_G31_NAD,
    PREP_LIN_ILL_NEST_IND,
    PREP_NO_G22_LIN,
    PREP_NO_G22_IMD,
    PREP_NO_G22_MOA,
    PREP_UNDEF_IMD_CHARGE_TYPE,
    PREP_NO_G23_MOA,
    PREP_RFF_NO_CUSTOMER_CODE,
    PREP_IMD_NO_SIM_NUMBER,
    PREP_IMD_NO_MSISDN_NUMBER,
    PREP_NO_XCDS,
    PREP_ITB_NO_CALL_REC_INFO,
    PREP_NO_ROAMING_INFO,
    PREP_NO_VPLMN_NAME,
    PREP_NO_FOREIGN_TAX,
    PREP_NO_SUM_INCL_TAX,
    PREP_NO_TOTAL_USAGE_AMOUNT,
    PREP_NO_TOTAL_SURCHARGE_AMOUNT,
    PREP_NO_TAX_NUMBER,
    PREP_LIN_LEVEL_ERROR,
    PREP_BALANCE_ERROR,

    BILL_NO_OUTPUT_GENERATED = 1000,

    ERR_END		    	/* the last element... */
};

/* Local typedefs               */
typedef int BOOL;

typedef TIMM_INTERCHANGE stTIMM;

typedef TIMM_MESSAGE stTMSG;


/* interface structure for database access to document_all and bill_images */
typedef struct {
  long		lCustId;		/* Customer ID */
  long		lContId;		/* Contract ID */
  long		lTypeId;		/* Type ID */
  long		lCopies;		/* no of copies for BILL_IMAGES */
  char		szCsLevel[3];		/* CSLEVEL */
  char		szBillIns[101];		/* Bill inserts */
} stDBCOL;

#ifdef BSCS4
/* structures for new DB tables */

/* structure for DOC_TYPES */
typedef struct {
  long		lTypeId;		/* TYPE_ID */
  char		cGenThisMsg;		/* GEN_THIS_MSG */
  stTIMM	*stTimm;		/* Timm interchange */
  stDBCOL	stCol;			/* info structure */
} stDOCTYPES;

/* structure for IMG_TYPES */
typedef struct {
  long		lTypeId;		/* TYPE_ID */
  char		szShdes[6];		/* TYPE_SHDES */
  long		lDefCopies;		/* DEFAULT_COPIES */
} stIMGTYPES;

/* structure for IMG_TYPES_LEVEL */
typedef struct {
  long		lTypeId;		/* TYPE_ID */
  char		szCslevel[3];		/* CSLEVEL */
  long		lCopies;		/* COPIES */
  int		aiDocTyp[TYPLAST+1]; 	/* array of doc_types needed */
  int		BchItb: 1;		/* 1 => needs Itemized TIMM */
} stIMGTYPESLV;

/* structure for IMG_LNK_DOC */
typedef struct {
  long		lDocType;		/* DOC_TYPE_ID */
  long		lImgType;		/* IMG_TYPE_ID */
} stIMGLNKDOC;

/* structure for CUST_IMAGES */
typedef struct {
  long		lCustId;		/* CUSTOMER_ID */
  long		lTypeId;		/* TYPE_ID */
  long		lCopies;		/* COPIES */
} stCUSTIMAGES;

#endif

typedef struct {
  long lIndex;
  char szShdes[7];
  char szPlmnName[13];
  char szCountry[21];
} stVPLMN;

typedef struct {
  char szPrefix[21];
  char szDest[31];
  char szZone[11];
  int  lIndex;
  char *szRegexp;
} stDest;

typedef struct {
  int  lIndex;
  long lCustId;
  long lTaxTypeId;
  char szExpirationDate[7];
} stTaxExempt;

typedef struct {
  long lIndex;
  long lCode;
  char szDes[31];
  char szShdes[6];
} stMPUTTTAB;

typedef struct {
  int  lIndex;
  long lCode;
  char szDes[31];
  char szShdes[6];
} stMPUZNTAB;

typedef struct {
  int lIndex;
  long lCode;
  char szDes[31];
  char szShdes[6];
} stMPUSNTAB;

typedef struct {
  int lIndex;
  long lCode;
  long lVersion;
  char szDes[31];
  char szShdes[6];
} stMPUTMTAB;

typedef struct {
  int lIndex;
  long lCode;
  char szDes[31];
  char szShdes[6];
} stMPUSPTAB;

typedef enum {XGF, TEX} GEN_TYPE;

/* structure with global information */
typedef struct {
  BOOL      bDebugMode;                /* debug style */
  char      szBaseDirectory[PATH_MAX]; /* origin point for used directories */
  char		szLogDir[PATH_MAX];	      /* log file directory */
  char      szTmpDir[PATH_MAX];
} stPBGHGLOB;


/* Function macros              */
#define macErrorMessage(a,b,c) fovdErrorMessage(__FILE__,__LINE__,(enum ERROR_NUMBERS)(a),(b),(c))

/* definition of macro ASSERT */
#ifdef DEBUG

# define _myassert(ex)	{if(!(ex)){(void)fprintf(stderr,"ASSERT (%s) failed in file \"%s\", line %d\n", #ex, __FILE__, __LINE__);exit(1);}}

#  define ASSERT(par)	_myassert (par)
#else
#  define ASSERT(par)
#endif


#endif /* ifndef BGH_H */

#define SPECIAL_NUMBER_PREFIX "501000000*"
#define SPECIAL_NUMBER_PREFIX_INDEX 9



