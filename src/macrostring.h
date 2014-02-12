#ifndef MACROSTRING_H
#define MACROSTRING_H

#include <QObject>

#define STRING_CONNECT                  QObject::tr("Connect")
#define STRING_DISCONNECT               QObject::tr("Disconnect")
#define STRING_CONNECT_ESTABLISHED      QObject::tr("Connect established")

#define STRING_TIP                      QObject::tr("Tip")
#define STRING_SYSTEM_CONFIG_INIT_ERR   QObject::tr("Init system config file failed")

#define STRING_HOST_UNREACHABLE         QObject::tr("Host unreachable")
#define STRING_CONNECTING               QObject::tr("Connecting...")
#define STRING_BEGIN                    QObject::tr("Begin")
#define STRING_END                      QObject::tr("End")

#define STRING_MANUAL_CTRL              QObject::tr("Manual")
#define STRING_AUTO_CTRL                QObject::tr("Auto")
#define STRING_STEPPING_CTRL            QObject::tr("Stepping ctrl")
#define STRING_YELLOW_FLASH_CTRL        QObject::tr("Yellow-flash ctrl")
#define STRING_ALL_RED_CTRL             QObject::tr("All-red ctrl")

// socket error code
#define STRING_CONNECT_HOST_REFUSED     QObject::tr("Connected host refused")
#define STRING_CONNECT_HOST_CLOSED      QObject::tr("Remote host closed")
#define STRING_CONNECT_HOST_NOTFIND     QObject::tr("Host not found")
#define STRING_CONNECT_SOCK_ACCESS_ERR  QObject::tr("Socket access error")
#define STRING_CONNECT_SOCK_RESOURCE_E  QObject::tr("Socket resource error")
#define STRING_CONNECT_SOCK_TIMEOUT     QObject::tr("Socket timeout error")
#define STRING_CONNECT_DATA_TOO_LARGE   QObject::tr("Datagram is too large")
#define STRING_CONNECT_NETWORK_ERR      QObject::tr("Network error")
#define STRING_CONNECT_ADDRESS_INUSE    QObject::tr("Address is in use")
#define STRING_CONNECT_ADDR_NOT_ABLE    QObject::tr("Socket address not available")
#define STRING_CONNECT_UNSUPPORTED      QObject::tr("Unsupported socket operation")
#define STRING_CONNECT_UNFINISHED       QObject::tr("Unfinished socket operation")
#define STRING_CONNECT_PROXY_AUTH       QObject::tr("Proxy authentication required")
#define STRING_CONNECT_SSL_ERROR        QObject::tr("SSL handshake failed")
#define STRING_CONNECT_PROXY_REFUSED    QObject::tr("Proxy connection refused")
#define STRING_CONNECT_PROXY_CLOSED     QObject::tr("Proxy connection closed")
#define STRING_CONNECT_PROXY_TIMEOUT    QObject::tr("Proxy connection timeout")
#define STRING_CONNECT_PROXY_NOTFIND    QObject::tr("Proxy not found")
#define STRING_CONNECT_PROXY_PROTOCOL   QObject::tr("Proxy protocol error")
#define STRING_CONNECT_UNKNOWN          QObject::tr("Unknown error")

#endif // MACROSTRING_H
