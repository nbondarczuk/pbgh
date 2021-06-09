include $(MPDE_MKINC)/make.rules

LDSTRING=
PRODUCT_LIBHOME=
MAKEFILE=Makefile
PROCPLSFLAGS= sqlcheck=full userid=$(USERID) dbms=v6_char
PROCPPFLAGS= code=cpp include=/usr/include include=/usr/include/cxx
USERID=developer/developer
INCLUDE=$(I_SYM). $(PRECOMPPUBLIC)

INC_DIR=./include

# Rule to compile any program (specify EXE= and OBJS= on command line)

# C COMPILER GIVEN BELOW
CC=cc 

# LIBRARY NAMES GIVEN BELOW
LIBRARIES= 

# LINKER GIVEN BELOW
LD=$(CC)

# LINKER FLAGS GIVEN BELOW
LDFLAGS= -L$(ORACLE_HOME)/lib

# DEFINES
AR=ar
ARFLAGS=rv
CP=cp
RM=rm -f

# INCLUDE SEARCH PATH GIVEN BELOW
INCLUDES= -I. -I.. -I../include

PROCINC=INCLUDE=../include

# COMPILER FLAGS GIVEN BELOW
COMPILERFLAGS= -g3 -DOSF1 -DA_OSF -fsigned-char -fwritable-strings -assume noaligned_objects
CPPFLAGS := $(CPPFLAGS) -I./include -DTEST
CFLAGS=$(COMPILERFLAGS) -I./include -I.. -DTEST
FFLAGS=$(COMPILERFLAGS)
PFLAGS=$(COMPILERFLAGS)

.SUFFIXES: .o .c

PC_SRC 	= bgh_esql.pc bgh_dnl.pc bgh_dealer.pc
PC_OBJ 	= $(BGH_PC_SRC:.pc=.o)

# SOURCES GIVEN BELOW
SRCS= $(PC_SRC) \
	rtx_list.c \
	rtx_tree.c \
	main.c


# OBJECTS GIVEN BELOW
OBJ= $(PC_OBJ) \
	rtx_list.o \
	rtx_tree.o \
	main.o

all:	rtx

rtx: version.h $(OBJ) 
	$(PROC) $(PROCFLAGS) DEFINE=TEST INCLUDE=. INCLUDE=./include INCLUDE=/usr/include iname=rtx.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DTEST -I/usr/include -c rtx.c
	$(CC) $(INCLUDES) -o rtx $(OBJ) rtx.o -L$(LIBHOME) $(PROLDLIBS)

atom: rtx
	atom rtx -tool third


clean::
	$(RM) $(OBJS) rtx core *~ rtx.c db.c bscs.c *.o pbgh libc.so.pbgh.third pbgh.3log pbgh.third sqlnet.log

touchsrcs:
	touch $(SRCS)

tar: clean
	tar -cvf $(HOME)/shipment/pbgh.tar .
	compress -f $(HOME)/shipment/pbgh.tar

shipment: pbgh
	strip pbgh
	cp pbgh /bscs/binEDS

include $(MPDE_MKINC)/make.misc
