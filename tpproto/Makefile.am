

lib_LTLIBRARIES = libtpproto.la

AM_CXXFLAGS = @libgnutls_CFLAGS@ @BOOST_CPPFLAGS@

libtpproto_la_LIBADD = @libgnutls_LIBS@ @BOOST_LDFLAGS@ @BOOST_SIGNALS_LIB@
libtpproto_la_LDFLAGS = -version-info 2:0:0
COMMONSOURCES = framecodec.cpp tpsocket.cpp frame.cpp buffer.cpp \
    connect.cpp okframe.cpp failframe.cpp login.cpp tcpsocket.cpp sequence.cpp\
    getobjectbyid.cpp vector3d.cpp getobjectbypos.cpp object.cpp \
    universe.cpp galaxy.cpp starsystem.cpp planet.cpp fleet.cpp \
    board.cpp getboard.cpp getmessage.cpp message.cpp gettime.cpp timeremaining.cpp \
    removemessage.cpp order.cpp getorder.cpp removeorder.cpp \
    orderdesc.cpp getorderdesc.cpp orderparameter.cpp spacecoord.cpp timeparameter.cpp \
    objectorderparameter.cpp listparameter.cpp playerparameter.cpp \
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
    getobjectidsbycontainer.cpp framebuilder.cpp protocollayer.cpp gamelayer.cpp getbyid.cpp \
    cache.cpp cachemethod.cpp cachenonemethod.cpp objectcache.cpp playercache.cpp \
    boardcache.cpp resourcecache.cpp categorycache.cpp designcache.cpp \
    componentcache.cpp propertycache.cpp createaccount.cpp finished.cpp getgameinfo.cpp \
    setfilters.cpp gameinfo.cpp getobjecttypeslist.cpp getobjectdesc.cpp \
    objecttypeslist.cpp objectdesc.cpp objectparamgroup.cpp objectparameter.cpp \
    connection.cpp simpleeventloop.cpp orderdesccache.cpp
libtpprotodir = $(includedir)/tpproto
COMMONHEADERS = framecodec.h frame.h tpsocket.h frame.h buffer.h \
    connect.h okframe.h failframe.h login.h tcpsocket.h sequence.h \
    getobjectbyid.h vector3d.h getobjectbypos.h object.h \
    universe.h galaxy.h starsystem.h planet.h fleet.h \
    board.h getboard.h getmessage.h message.h gettime.h timeremaining.h \
    removemessage.h asyncframelistener.h order.h getorder.h removeorder.h \
    orderdesc.h getorderdesc.h orderparameter.h spacecoord.h timeparameter.h \
    objectorderparameter.h listparameter.h orderparametervisitor.h playerparameter.h \
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
    getobjectidsbycontainer.h framebuilder.h protocollayer.h gamelayer.h \
    gamestatuslistener.h getbyid.h cache.h cachemethod.h cachenonemethod.h \
    objectcache.h playercache.h boardcache.h resourcecache.h categorycache.h \
    designcache.h componentcache.h propertycache.h createaccount.h \
    finished.h getgameinfo.h setfilters.h gameinfo.h getobjecttypeslist.h \
    getobjectdesc.h objecttypeslist.h objectdesc.h objectparamgroup.h \
    objectparameter.h connection.h eventloop.h simpleeventloop.h orderdesccache.h

if GNUTLS
GNUTLSSOURCES = tlssocket.cpp httpssocket.cpp tpssocket.cpp
GNUTLSHEADERS = tlssocket.h httpssocket.h tpssocket.h
else
GNUTLSSOURCES =
GNUTLSHEADERS =
endif

libtpproto_la_SOURCES = $(COMMONSOURCES) $(GNUTLSSOURCES)
libtpproto_HEADERS = $(COMMONHEADERS) $(GNUTLSHEADERS)

