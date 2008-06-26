/*  AdminLayer class
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <iostream>
#include <boost/bind.hpp>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "tpsocket.h"
#include "framefactory.h"
#include "protocollayer.h"
#include "eventloop.h"
#include "logger.h"
#include "adminstatuslistener.h"
#include "asyncframelistener.h"
#include "silentlogger.h"
#include "framecodec.h"
#include "framebuilder.h"
#include "tcpsocket.h"
#ifdef HAVE_LIBGNUTLS
#include "tpssocket.h"
#endif

// caches
#include "cachemethod.h"
#include "commanddesccache.h"

// Frame Types

#include "okframe.h"
#include "failframe.h"
#include "sequence.h"
#include "connect.h"
#include "login.h"
#include "command.h"
#include "commanddesc.h"
#include "getcommanddesc.h"
#include "logmessage.h"
#include "redirect.h"

#include "adminlayer.h"

namespace TPProto {

    class TimeRemaining;

    class AdminLayerAsyncFrameListener : public AsyncFrameListener{
    public:
        
        virtual ~AdminLayerAsyncFrameListener()
        {
        }
        
        void setAdminLayer(AdminLayer* al)
        {
            layer = al;
        }

        // TimeRemaining frames are not used by admin clients
        void recvTimeRemaining(TimeRemaining * trf){}
        
        void recvLogMessage(LogMessage * lmf)
        {
            switch(lmf->getLevel()){
            case LML_Debug:
                layer->logger->debug(lmf->getMessage().c_str());
                break;
            case LML_Info:
                layer->logger->info(lmf->getMessage().c_str());
                break;
            case LML_Warning:
                layer->logger->warning(lmf->getMessage().c_str());
                break;
            case LML_Error:
                layer->logger->error(lmf->getMessage().c_str());
                break;
            }
        }
        
    private:
        AdminLayer* layer;
    };

    /*! \brief Constructs object and sets up defaults.
    
    Defaults are
        - The default ProtocolLayer
        - Disconnected state.
        - "Unknown client" for the client string
    */
    AdminLayer::AdminLayer() : protocol(NULL), eventloop(NULL), logger(NULL), statuslistener(NULL), status(asDisconnected),
        clientid("Unknown admin client"), asyncframes(new AdminLayerAsyncFrameListener()),
        commanddesccache(new CommandDescCache())
    {
        protocol = new ProtocolLayer();
        logger = new SilentLogger();
        sock = NULL;
        asyncframes->setAdminLayer(this);
        protocol->getFrameCodec()->setAsyncFrameListener(asyncframes);
        commanddesccache->setProtocolLayer(protocol);
        protocol->getFrameBuilder()->setCommandDescCache(commanddesccache);
    }

    /*! \brief Destructor.
     */
    AdminLayer::~AdminLayer()
    {
        if(status != asDisconnected){
            sock->disconnect();
        }
        delete protocol;

        delete asyncframes;
        delete commanddesccache;
    }

    /*! \brief Sets the client string.

    The client string can be set to anything.  The perferred format is
    "name/version".  The library name and version is added the the client
    string later.
    \param name The client name string.
    */
    void AdminLayer::setClientString(const std::string & name)
    {
        clientid = name;
    }

    /*! \brief Sets the Logger to use.
        \param nlog THe new Logger to use.
    */
    void AdminLayer::setLogger(Logger * nlog)
    {
        logger = nlog;
        protocol->getFrameCodec()->setLogger(nlog);
    }

    /*! \brief Sets the AdminStatusListener to use.
        \param asl The new AdminStatusListener to use.
    */
    void AdminLayer::setAdminStatusListener(AdminStatusListener * asl)
    {
        statuslistener = asl;
    }

    /*! \brief Sets the CacheMethod for the caches to use.
    Changes the default CacheMethod used, defaults to CacheNoneMethod.
    \param prototype A CacheMethod that will be cloned for each Cache to use.
    */
    void AdminLayer::setCacheMethod(CacheMethod * prototype)
    {
        commanddesccache->setCacheMethod(prototype->clone());
    }
    
    /*! \brief Sets the EventLoop abstraction to use.
    Sets which implementation of EventLoop to use.
    
    Must be set before connecting to a server.
    \param el The implementation of the EventLoop to use.
    */
    void AdminLayer::setEventLoop(EventLoop* el)
    {
        eventloop = el;
        protocol->getFrameCodec()->setEventLoop(eventloop);
    }

    /*! \brief Gets the state of the admin connection.
    \return The AdminStatus enum value for the current state.
    */
    AdminStatus AdminLayer::getStatus()
    {
        if((sock == NULL || !sock->isConnected()) && status != asDisconnected)
        {
            status = asDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
        return status;
    }

    /*! \brief Gets the ProtocolLayer being used.
    This could be used to do low level calls to the protocol itself, or 
    more importantly, set the FrameFactory, FrameBuilder and/or FrameCodec
    classes in the ProtocolLayer.
    \return The pointer to the ProtocolLayer.
    */
    ProtocolLayer * AdminLayer::getProtocolLayer() const
    {
        return protocol;
    }

    /*! \brief Connects to the given address url
    This method connects to the server given as the address. The types of url
    supported are tp and tps. Tps depends on TLS being enabled.
    \param address The URL to connect to.
    \return True if connection in progress, false otherwise.
    */
    bool AdminLayer::connect(const std::string & address)
    {
        if(status != asDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        
        std::string type, host, port;
        // parse address to type, host, and port
        size_t tpos = address.find("://");
        if(tpos != address.npos){
            type = address.substr(0, tpos);
            tpos += 3;
        }else{
            tpos = 0;
        }
        size_t ppos = address.rfind(':');
        size_t bpos = address.rfind(']');
        if(ppos != std::string::npos && ppos > tpos){
            if(bpos == std::string::npos || ppos > bpos){
                port = address.substr(ppos + 1);
            }else{
                ppos = std::string::npos;
            }
        }else{
            ppos = std::string::npos;
        }
        host = address.substr(tpos, ppos - tpos);

        TPSocket *sock = NULL;
        if(type.empty() || type == "tp"){
            if(port.empty()){
                port = "6925";
            }
            sock = new TcpSocket();
            static_cast<TcpSocket*>(sock)->setServerAddr(host.c_str(), port.c_str());
        }else if(type == "tps"){
            if(port.empty()){
                port = "6926";
            }
#ifdef HAVE_LIBGNUTLS
            sock = new TpsSocket();
            static_cast<TpsSocket*>(sock)->setServerAddr(host.c_str(), port.c_str());
#endif
        }else{
            logger->error("Type of connection to create was not known");
            return false;
        }

        if(sock != NULL)
            return connect(sock);
        else
            return false;
    }

    /*! \brief Connects using a given TPSocket.
    Connects to a server using a given TPSocket.
    \param nsock The TPSocket to connect using.
    \return True if connection in progress, false otherwise.
    */
    bool AdminLayer::connect(TPSocket* nsock){
        if(status != asDisconnected){
            logger->warning("Already connected, ignoring connection attempt");
            return false;
        }
        
        sock = nsock;
        sock->setConnection(protocol->getFrameCodec());
        eventloop->listenForSocketRead(sock);
        if(sock->connect()){
            logger->debug("Connection opened");
            status = asConnecting;
            Connect * cf = protocol->getFrameFactory()->createConnect();
            cf->setClientString(std::string("libtpproto-cpp/") + VERSION + " " + clientid + " (admin)");
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Connect>(cf), boost::bind(&AdminLayer::connectCallback, this, _1));
            return true;
        }else{
            logger->error("Could not open socket to server");
        }
        return false;
    }
    
    /*! \brief Logs in to the server.
    
    Sends a Login Frame to the server and waits for a reply.
    \param username The username to connect as.
    \param password The password of the account of the username.
    \return True if successfully started, false otherwise.
    */
    bool AdminLayer::login(const std::string &username, const std::string &password){
        if(status == asConnected && sock->isConnected()){
            Login * login = protocol->getFrameFactory()->createLogin();
            login->setUser(username);
            login->setPass(password);
            protocol->getFrameCodec()->sendFrame(boost::shared_ptr<Login>(login), boost::bind(&AdminLayer::loginCallback, this, _1));
            return true;
        }
        if(!sock->isConnected())
        {
            status = asDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
        return false;
    }

    /*! \brief Disconnects from server.
    
    Closes the underlying TPSocket.
    */
    void AdminLayer::disconnect(){
        if(status != asDisconnected && sock != NULL){
            sock->disconnect();
            logger->info("Disconnected");
            status = asDisconnected;
            if(statuslistener != NULL)
                statuslistener->disconnected();
        }
    }

    /*! \brief Tells all the caches to update.
    Called automatically after logged in, and after EOT has finished.
    Call if you want the caches to be updated.
    */
    void AdminLayer::updateCaches(){
        commanddesccache->update();
    }

    
    /*! \brief Gets the CommandDescCache.
    \return The CommandDescCache.
    */
    CommandDescCache* AdminLayer::getCommandDescCache() const{
        return commanddesccache;
    }
    
    void AdminLayer::connectCallback(Frame* frame){
        if(frame->getType() == ft02_OK){
            delete frame;
            status = asConnected;
            logger->info("Connected");
            if(statuslistener != NULL)
                statuslistener->connected();
        }else if(frame->getType() == ft03_Redirect){
            status = asDisconnected;
            sock->disconnect();
            if(statuslistener == NULL || (statuslistener != NULL && statuslistener->redirected(static_cast<Redirect*>(frame)->getUrl())))
                connect(static_cast<Redirect*>(frame)->getUrl());
            delete frame;
        }else{
            status = asDisconnected;
            logger->error("Could not connect");
            //TODO: check why, maybe wrong protocol version?
            sock->disconnect();
            if(statuslistener != NULL)
                statuslistener->disconnected();
            delete frame;
        }
    }
    
    void AdminLayer::loginCallback(Frame* frame){
        if(frame->getType() == ft02_OK){
            delete frame;
            status = asLoggedIn;
            logger->info("Logged In");
            if(statuslistener != NULL)
                statuslistener->loggedIn(true);
        }else if(frame->getType() == ft03_Redirect){
            status = asDisconnected;
            sock->disconnect();
            if(statuslistener == NULL || (statuslistener != NULL && statuslistener->redirected(static_cast<Redirect*>(frame)->getUrl())))
                connect(static_cast<Redirect*>(frame)->getUrl());
            delete frame;
        }else{
            logger->error("Login failed: %s", ((FailFrame*)frame)->getErrorString().c_str());
            if(statuslistener != NULL)
                statuslistener->loggedIn(false);
        }
    }

}
