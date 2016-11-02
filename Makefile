# Makefile -- build template for the logger library.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Wed Nov  2 14:36:42 CET 2016
# Copyright: (C) 2016 LUIS COLORADO <luiscoloradourcola@gmail.com>  All rights reserved.
#

PACKAGE=logger

RM				?= rm -f
INSTALL			?= install

logger_MAJOR	?= 0
logger_MINOR	?= 1
logger_DEVLIB	?= lib$(PACKAGE).so
logger_SONAME   ?= $(logger_DEVLIB).$(logger_MAJOR)
logger_LIB      ?= $(logger_SONAME).$(logger_MINOR)
logger_VERSION  ?= $(logger_MAJOR).$(logger_MINOR)
logger_OBJS     ?= logger.o
TOCLEAN			+= $(logger_OBJS)

targets			=  $(logger_LIB)
TOCLEAN         += $(targets)

.PHONY: all clean install uninstall

all: $(targets)
clean:
	$(RM) $(TOCLEAN)
install: $(targets)
uninstall:

$(logger_LIB): $(logger_OBJS)
	$(CC) $(LDFLAGS) -o $@ -shared -Wl,-soname=$(logger_SONAME) $(logger_OBJS)
