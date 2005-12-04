

lib_LTLIBRARIES = libtpproto.la

libtpproto_la_LIBADD = $(LIBGNUTLS_LIBS)
libtpproto_la_LDFLAGS = -version-info 1:0:0
COMMONSOURCES = framecodec.cpp tpsocket.cpp frame.cpp buffer.cpp \
    connect.cpp okframe.cpp failframe.cpp login.cpp tcpsocket.cpp sequence.cpp\
    getobjectbyid.cpp vector3d.cpp getobjectbypos.cpp object.cpp \
    universe.cpp galaxy.cpp starsystem.cpp planet.cpp fleet.cpp \
    board.cpp getboard.cpp getmessage.cpp message.cpp gettime.cpp timeremaining.cpp \
    removemessage.cpp order.cpp getorder.cpp removeorder.cpp \
    orderdesc.cpp getorderdesc.cpp orderparameter.cpp spacecoord.cpp timeparameter.cpp \
    objectparameter.cpp listparameter.cpp playerparameter.cpp \
    spacecoordrel.cpp rangeparameter.cpp stringparameter.cpp silentlogger.cpp \
    framefactory.cpp redirect.cpp probeorder.cpp getresourcedesc.cpp getplayer.cpp \
    getcategory.cpp getdesign.cpp getcomponent.cpp getproperty.cpp removecategory.cpp \
    removedesign.cpp ping.cpp getfeatures.cpp getidsequence.cpp getboardidslist.cpp \
    getobjectidslist.cpp getordertypeslist.cpp getresourcetypeslist.cpp \
    getcategoryidslist.cpp getdesignidslist.cpp getcomponentidslist.cpp getpropertyidslist.cpp \
    idsequence.cpp objectidslist.cpp ordertypeslist.cpp boardidslist.cpp resourcetypeslist.cpp \
    categoryidslist.cpp designidslist.cpp componentidslist.cpp propertyidslist.cpp \
    featuresframe.cpp player.cpp resourcedesc.cpp category.cpp design.cpp component.cpp \
    property.cpp addcategory.cpp adddesign.cpp modifydesign.cpp getobjectidsbypos.cpp \
    getobjectidsbycontainer.cpp framebuilder.cpp protocollayer.cpp gamelayer.cpp
libtpprotodir = $(includedir)/tpproto
COMMONHEADERS = framecodec.h frame.h tpsocket.h frame.h buffer.h \
    connect.h okframe.h failframe.h login.h tcpsocket.h sequence.h \
    getobjects.h getobjectbyid.h vector3d.h getobjectbypos.h object.h \
    objectvisitor.h universe.h galaxy.h starsystem.h planet.h fleet.h \
    board.h getboard.h getmessage.h message.h gettime.h timeremaining.h \
    removemessage.h asyncframelistener.h order.h getorder.h removeorder.h \
    orderdesc.h getorderdesc.h orderparameter.h spacecoord.h timeparameter.h \
    objectparameter.h listparameter.h orderparametervisitor.h playerparameter.h \
    spacecoordrel.h rangeparameter.h stringparameter.h logger.h silentlogger.h \
    framefactory.h redirect.h probeorder.h getresourcedesc.h getplayer.h \
    getcategory.h getdesign.h getcomponent.h getproperty.h removecategory.h \
    removedesign.h ping.h getfeatures.h getidsequence.h getboardidslist.h \
    getobjectidslist.h getordertypeslist.h getresourcetypeslist.h \
    getcategoryidslist.h getdesignidslist.h getcomponentidslist.h getpropertyidslist.h \
    idsequence.h objectidslist.h ordertypeslist.h boardidslist.h resourcetypeslist.h \
    categoryidslist.h designidslist.h componentidslist.h propertyidslist.h \
    featuresframe.h player.h resourcedesc.h category.h design.h component.h \
    property.h addcategory.h adddesign.h modifydesign.h getobjectidsbypos.h \
    getobjectidsbycontainer.h framebuilder.h protocollayer.h gamelayer.h

if GNUTLS
GNUTLSSOURCES = tlssocket.cpp
GNUTLSHEADERS = tlssocket.h
else
GNUTLSSOURCES =
GNUTLSHEADERS =
endif

libtpproto_la_SOURCES = $(COMMONSOURCES) $(GNUTLSSOURCES)
libtpproto_HEADERS = $(COMMONHEADERS) $(GNUTLSHEADERS)

