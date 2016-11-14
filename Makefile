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

logger_MAJOR	?= 0
logger_MINOR	?= 1
logger_DEVLIB	?= lib$(PACKAGE).so
logger_SONAME   ?= $(logger_DEVLIB).$(logger_MAJOR)
logger_LIB      ?= $(logger_SONAME).$(logger_MINOR)
logger_VERSION  ?= $(logger_MAJOR).$(logger_MINOR)
logger_OBJS     ?= logger.pic_o
TOCLEAN			+= $(logger_OBJS)

includes		+= logger.h
targets			=  $(logger_LIB)
TOCLEAN         += $(targets)

PHONYS			?= all clean install uninstall
.SUFFIXES: .pic_o
.PHONY: $(PHONYS) subdirs

all:: $(targets)

$(PHONYS)::
	@for i in $(SUBDIRS); \
	do \
		echo $(MAKE) -C $$i $@; \
		$(MAKE) -C $$i $@; \
	done

clean::
	$(RM) $(TOCLEAN)
install:: $(targets)
	-@for i in $(incdir) $(libdir); \
	do \
		echo $(INSTALL) $(DPERM) -d $$i; \
		$(INSTALL) $(DPERM) -d $$i; \
	done; \
	for i in $(includes); \
	do \
		echo $(INSTALL) $(FPERM) $$i $(incdir); \
		$(INSTALL) $(FPERM) $$i $(incdir); \
	done; \
	for i in $(targets); \
	do \
		echo $(INSTALL) $(FPERM) $$i $(libdir); \
		$(INSTALL) $(FPERM) $$i $(libdir); \
	done;
uninstall::
	-@for i in $(includes); \
	do \
		echo $(RM) $(incdir)/$$i; \
		$(RM) $(incdir)/$$i; \
	done; \
	for i in $(targets); \
	do \
		echo $(RM) $(libdir)/$$i; \
		$(RM) $(libdir)/$$i; \
	done;

.c.pic_o:
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

$(logger_LIB): $(logger_OBJS)
	$(CC) $(LDFLAGS) -o $@ -shared -Wl,-soname=$(logger_SONAME) $(logger_OBJS)
