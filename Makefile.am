SUBDIRS = tpproto test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libtpproto.pc 

EXTRA_DIST = base.dox index.dox Doxyfile debian

doc: Doxyfile
	doxygen Doxyfile