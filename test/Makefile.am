TESTS = simpletest nettest.sh 
check_PROGRAMS = simpletest nettest

INCLUDES = -I$(top_srcdir)/src

simpletest_SOURCES = simpletest.cpp testsocket.h testsocket.cpp

simpletest_LDADD= \
    $(top_srcdir)/tpproto/libtpproto.la


nettest_SOURCES = nettest.cpp

nettest_LDADD= \
    $(top_srcdir)/tpproto/libtpproto.la



