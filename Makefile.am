SUBDIRS = tpproto test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtpproto.pc 

EXTRA_DIST = base.dox index.dox Doxyfile autogen.sh m4/ax_boost_base.m4 \
	ms/ax_boost_signals.m4

doc: Doxyfile
	doxygen Doxyfile
