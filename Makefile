# Makefile -- build template for the logger library.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Wed Nov  2 14:36:42 CET 2016
# Copyright: (C) 2016 LUIS COLORADO <luiscoloradourcola@gmail.com>  All rights reserved.
#

PACKAGE=logger

prefix			?= $(HOME)
incdir			?= $(prefix)/include
libdir			?= $(prefix)/lib
etcdir			?= $(prefix)/etc
datadir			?= $(prefix)/share/$(PACKAGE)

SUBDIRS			?= avl_c lists

RM				?= rm -f
INSTALL			?= install $(OWNER)
OWNER			?= -o `id -u` -g `id -g`
DPERM			?= -m 0755
FPERM			?= -m 0644
XPERM			?= -m 0711
CFLAGS			?= -g -O0
PIC_CFLAGS		?= -fPIC

logger_MAJOR	?= 0
logger_MINOR	?= 1
logger_DEVLIB	?= lib$(PACKAGE).so
logger_SONAME   ?= $(logger_DEVLIB).$(logger_MAJOR)
logger_LIB      ?= $(logger_SONAME).$(logger_MINOR)
logger_VERSION  ?= $(logger_MAJOR).$(logger_MINOR)
logger_OBJS     ?= logger.pic_o
TOCLEAN			+= $(logger_OBJS)

includes		+= logger.h
targets			=  $(mod_targets) $(logger_LIB)
mod_targets     =  logg_sample.so
test_targets	+= test_logger
TOCLEAN         += $(targets) $(test_targets)

PHONYS			?= all clean install uninstall test
.SUFFIXES: .pic_o
.PHONY: $(PHONYS) subdirs

all:: $(targets)

clean::
	$(RM) $(TOCLEAN)

install:: $(targets)
	-@for i in $(incdir) $(libdir); \
	do \
		echo $(INSTALL) $(DPERM) -d $$i; \
		$(INSTALL) $(DPERM) -d $$i; \
	done;
	-@for i in $(includes); \
	do \
		echo $(INSTALL) $(FPERM) $$i $(incdir); \
		$(INSTALL) $(FPERM) $$i $(incdir); \
	done;
	-@for i in $(targets); \
	do \
		echo $(INSTALL) $(FPERM) $$i $(libdir); \
		$(INSTALL) $(FPERM) $$i $(libdir); \
	done;
	ln -fs $(logger_LIB) $(libdir)/$(logger_SONAME)
	ln -fs $(logger_SONAME) $(libdir)/$(logger_DEVLIB)

uninstall::
	-@for i in $(includes); \
	do \
		echo $(RM) $(incdir)/$$i; \
		$(RM) $(incdir)/$$i; \
	done;
	-@for i in $(targets); \
	do \
		echo $(RM) $(libdir)/$$i; \
		$(RM) $(libdir)/$$i; \
	done; 
	$(RM) $(libdir)/$(logger_SONAME)
	$(RM) $(libdir)/$(logger_DEVLIB)

test:: $(test_targets)
	@for i in $(test_targets); \
	do \
		echo $$i; \
		$$i; \
	done

depend:
	gcc -MM *.c >.depend

.c.pic_o:
	$(CC) $(CFLAGS) -c $(PIC_CFLAGS) -o $@ $<

$(logger_LIB): $(logger_OBJS)
	$(CC) $(LDFLAGS) -o $@ -shared -Wl,-soname=$(logger_SONAME) $(logger_OBJS)

test_logger_objs = test.o $(logger_LIB) avl_c/libavl.so
test_logger_libs = -ldl
test_logger: $(test_logger_objs) 
	$(CC) $(LDFLAGS) -o $@ $(test_logger_objs) $(test_logger_libs)
TOCLEAN += test.o

# We use the linker here, so we don't include /usr/lib/crt0.o in the output, which
# already has _init() and _fini() functions, to allow for _init(), _fini() processing.
logg_sample.so_objs = $(logger_LIB) sample.pic_o avl_c/libavl.so
logg_sample.so_libs =
logg_sample.so: $(logg_sample.so_objs)
	$(LD) $(LDFLAGS) -o $@ -shared $(logg_sample.so_objs) $(logg_sample.so_libs) -lc
TOCLEAN += sample.pic_o

avl_c/libavl.so:
	$(MAKE) -C avl_c all

-include .depend
