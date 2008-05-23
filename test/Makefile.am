TESTS = nettest.sh 
check_PROGRAMS = nettest

EXTRA_DIST = nettest.sh

INCLUDES = -I$(top_srcdir)/src


nettest_SOURCES = nettest.cpp downloadprintvisitor.h downloadprintvisitor.cpp \
    testgamestatelistener.h testgamestatelistener.cpp nettest.h

nettest_LDADD= \
    $(top_srcdir)/tpproto/libtpproto.la



