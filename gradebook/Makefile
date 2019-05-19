###########################################################################
# FILE        : Makefile
# AUTHOR      : (C) Copyright 2006 by Peter C. Chapin
# SUBJECT     : Makefile for my grade book server project.
#
###########################################################################

CPPFLAGS = -g -Wall
OBJS     = cgiread.o gfile.o grades.o grecord.o helper.o

grades.cgi:	$(OBJS)
	g++ -g -o grades.cgi $(OBJS)

cgiread.o:	cgiread.cpp cgiread.h

gfile.o:	gfile.cpp gfile.h

grades.o:	grades.cpp cgiread.h gfile.h grecord.h helper.h

grecord.o:	grecord.cpp grecord.h

helper.o:	helper.cpp gfile.h helper.h
