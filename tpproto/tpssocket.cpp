#include <sys/types.h>
#include <sys/socket.h>
#include <string>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifndef VERSION
#define VERSION "0.0.0"
#endif

#include "tpssocket.h"

namespace TPProto{
    /*! \brief Default constructor.

    Sets up defaults, that will prevent connecting.
    */
    TpsSocket::TpsSocket() : TlsSocket(){
    }

    /*! \brief Destructor.
    */
    TpsSocket::~TpsSocket(){
    }

    /*! \brief Connects to the host and port in a tps connection.
    \return True if a new connection is made, false if already connected or
    if an error occured.
    */
    bool TpsSocket::connect(){
        return TlsSocket::connect(hostname, portname);
    }

    /*! \brief Sets the server address and port to connect to.

    Host can be DNS name or ip address. Post can be port name or number.
    \param host Host to connect to.
    \param port Port to connect to.
    */
    void TpsSocket::setServerAddr(const char* host, const char* port){
        TlsSocket::setServerAddr(host, port);
    }

}
