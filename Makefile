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
INCLUDES= -I. -I./include

PROCINC=INCLUDE=./include

# COMPILER FLAGS GIVEN BELOW
COMPILERFLAGS= -g3 -DOSF1 -DA_OSF -fsigned-char -fwritable-strings -assume noaligned_objects
CPPFLAGS := $(CPPFLAGS) -I./include
CFLAGS=$(COMPILERFLAGS) -I./include -DTEST
FFLAGS=$(COMPILERFLAGS)
PFLAGS=$(COMPILERFLAGS)

.SUFFIXES: .o .c

PC_SRC 	= bgh_esql.pc bgh_dnl.pc bgh_dealer.pc
PC_OBJ 	= $(BGH_PC_SRC:.pc=.o)

# SOURCES GIVEN BELOW
SRCS= $(PC_SRC) \
	g22.c \
	rtx_list.c \
	rtx_tree.c \
	map.c \
	mc.c \
	g22_copy.c \
	mapshdes.c \
	mc_split.c \
	split.c \
	main.c \
	bscslink.c \
	proc.c \
	bch_charge.c \
	parser.c \
	printer.c \
 	timm.c \
	mp.c \
	buf.c


# OBJECTS GIVEN BELOW
OBJ= $(PC_OBJ) \
	g22.o \
	rtx_list.o \
	rtx_tree.o \
	map.o \
	mc.o \
	g22_copy.o \
	mapshdes.o \
	mc_split.o \
	split.o \
	main.o \
	bscslink.o \
	proc.o \
	bch_charge.o \
	parser.o\
 	printer.o \
 	timm.o \
	mp.o \
	buf.o

all:	pbgh

pbgh: version.h $(OBJ) 
	$(PROC) $(PROCFLAGS) DEFINE=PRODUCT iname=db.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DPRODUCT -c db.c
	$(PROC) $(PROCFLAGS) DEFINE=PRODUCT iname=rtx.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DPRODUCT -c rtx.c
	$(PROC) $(PROCFLAGS) DEFINE=PRODUCT iname=bscs.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DPRODUCT -c bscs.c
	$(CC) $(INCLUDES) -g3 -o pbgh $(OBJ) db.o rtx.o bscs.o -L$(LIBHOME) $(PROLDLIBS)

pbgh_test: version.h $(OBJ) 
	$(PROC) $(PROCFLAGS) DEFINE=TEST iname=db.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DTEST -c db.c
	$(PROC) $(PROCFLAGS) DEFINE=TEST iname=rtx.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DTEST -c rtx.c
	$(PROC) $(PROCFLAGS) DEFINE=TEST iname=bscs.pc
	$(CC) $(INCLUDES) $(CFLAGS) -DTEST -c bscs.c
	$(CC) $(INCLUDES) -o pbgh_test $(OBJ) db.o rtx.o bscs.o -L$(LIBHOME) $(PROLDLIBS)


atom: pbgh_test
	atom pbgh_test -tool third

clean::
	$(RM) $(OBJS) pbgh pbgh_test core *~ rtx.c db.c bscs.c *.o *.3log *.third sqlnet.log

touchsrcs:
	touch $(SRCS)

tar: clean
	tar -cvf $(HOME)/source/pbgh.tar .
	compress -f $(HOME)/source/pbgh.tar

shipment: pbgh
	strip pbgh
	cp pbgh /bscs/binEDS

include $(MPDE_MKINC)/make.misc
