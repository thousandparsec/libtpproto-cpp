AC_INIT([libtpproto-cpp],[0.1.9],[lee@thousandparsec.net])
AC_CONFIG_SRCDIR(tpproto/framecodec.cpp)

AC_CANONICAL_TARGET([])

AM_INIT_AUTOMAKE(libtpproto-cpp, 0.1.9)
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

CXXFLAGS="-g -Wall"


AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_LANG_CPLUSPLUS
AC_REQUIRE_CPP

AC_HEADER_STDC



AC_CHECK_LIB(pthread, pthread_create,,AC_MSG_ERROR(libtpproto-cpp requires pthread library))
#check headers, functions, etc

AC_C_BIGENDIAN

dnl Test for IPv6 socket addresses
dnl "Borrowed" from skstream


AC_MSG_CHECKING([for IPv6 socket address])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
                #include <sys/socket.h>
                #include <netinet/in.h>]], [[
    sockaddr_in6 sin6;
    sin6.sin6_family = AF_INET6;
]])],[
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_IPV6, 1, true if we have sockaddr_in6 and AF_INET6)
],[
   AC_MSG_RESULT(no)
])

#check for boost
AX_BOOST_BASE([1.31.1])
AX_BOOST_SIGNALS

PKG_CHECK_MODULES(libgnutls, gnutls >= 1.2.10,
  [
    AC_DEFINE(HAVE_LIBGNUTLS, 1, Define to 1 if libgnutls is available)
    AM_CONDITIONAL(GNUTLS, true)
  ],
  [
    AM_CONDITIONAL(GNUTLS, false)
  ]
)
AC_SUBST([libgnutls_LIBS])
AC_SUBST([libgnutls_CFLAGS])

AC_CONFIG_FILES([
libtpproto.pc
Makefile
tpproto/Makefile
test/Makefile
]) 

AC_OUTPUT
