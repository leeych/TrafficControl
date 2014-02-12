#include "synccommand.h"
#include "macrostring.h"
#include "command.h"

#include <QDebug>

#define WAIT_FOR_CONNECT_MS     (30000)

SyncCommand *SyncCommand::instance_ = NULL;

SyncCommand *SyncCommand::getInstance()
{
    if (instance_ == NULL)
    {
        instance_ = new SyncCommand;
    }
    return instance_;
}

void SyncCommand::distroyInstance()
{
    this->~SyncCommand();
}

QTcpSocket *SyncCommand::getSocket() const
{
    return socket_;
}

void SyncCommand::connectToHost(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
}

bool SyncCommand::syncConnectToHost(const QString &ip, unsigned int port)
{
    ip_ = ip;
    port_ = port;
    socket_->connectToHost(ip, port);
    if (!socket_->waitForConnected(WAIT_FOR_CONNECT_MS))
    {
        onConnectError(socket_->error());
        return false;
    }
    return true;
}

void SyncCommand::closeConnection()
{
    socket_->abort();
}

bool SyncCommand::isConnectionValid() const
{
    return socket_->isValid();
}

void SyncCommand::initParseHandler(QObject *obj, const std::string &slot)
{
    unRegParseHandler();
    target_obj_ = obj;
    slot_ = slot;
    regParseHandler();
    socket_->readAll();
}

void SyncCommand::disconnectFromHost()
{
    socket_->disconnectFromHost();
}

void SyncCommand::sendVersionCheck(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendVersionCheck();
}

void SyncCommand::sendVersionCheck()
{
    qint64 sz = socket_->write(Command::WTLVersion.c_str());
    qDebug() << Command::WTLVersion.c_str() << " bytes:" << sz;
}

void SyncCommand::sendAutoCtrl(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendAutoCtrl();
}

void SyncCommand::sendAutoCtrl()
{
    qint64 sz = socket_->write(Command::AutoCtrl.c_str());
    qDebug() << Command::AutoCtrl.c_str() << " bytes:" << sz;
}

void SyncCommand::sendManualCtrl(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendManualCtrl();
}

void SyncCommand::sendManualCtrl()
{
    qint64 sz = socket_->write(Command::ManualCtrl.c_str());
    qDebug() << Command::ManualCtrl.c_str() << " bytes:" << sz;
}

void SyncCommand::sendSteppingCtrl(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendSteppingCtrl();
}

void SyncCommand::sendSteppingCtrl()
{
    qint64 sz = socket_->write(Command::StepCtrl.c_str());
    qDebug() << Command::StepCtrl.c_str() << " bytes:" << sz;
}

void SyncCommand::sendYellowFlashCtrl(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendYellowFlashCtrl();
}

void SyncCommand::sendYellowFlashCtrl()
{
    qint64 sz = socket_->write(Command::YellowFlashCtrl.c_str());
    qDebug() << Command::YellowFlashCtrl.c_str() << " bytes:" << sz;
}

void SyncCommand::sendAllRedCtrl(QObject *obj, const std::string &slot)
{
    initParseHandler(obj, slot);
    sendAllRedCtrl();
}

void SyncCommand::sendAllRedCtrl()
{
    qint64 sz = socket_->write(Command::AllRedCtrl.c_str());
    qDebug() << Command::AllRedCtrl.c_str() << " bytes:" << sz;
}

void SyncCommand::sendHeartBeat()
{
    qint64 sz = socket_->write(Command::HearBeat.c_str());
    qDebug() << Command::HearBeat.c_str() << " bytes:" << sz;
}

void SyncCommand::onConnectEstablished()
{
    emit connectedSignal();
}

void SyncCommand::onDisconnected()
{
    emit disconnectedSignal();
}

void SyncCommand::onConnectError(QAbstractSocket::SocketError err)
{
    emit connectErrorSignal();
    emit connectErrorStrString(sock_err_desc_map_.value(err));
}

void SyncCommand::parseReply()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        connect(this, SIGNAL(readyRead()), target_obj_, slot_.c_str());
    }
    if ((target_obj_ != NULL) && (!slot_.c_str()))
    {
        disconnect(this, SIGNAL(readyRead()), target_obj_, slot_.c_str());
    }
}

void SyncCommand::socketReadyReadSlot()
{
    sock_array_ = socket_->readAll();
    emit readyRead(sock_array_);
}

SyncCommand::SyncCommand(QObject *parent) :
    QObject(parent)
{
    socket_ = new QTcpSocket(this);
    target_obj_ = NULL;

    connect(socket_, SIGNAL(connected()), this, SLOT(onConnectEstablished()));
//    connect(socket_, SIGNAL(error(QAbstractSocket)), this, SLOT(onConnectError(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(socketReadyReadSlot()));
    genConnectErrorDesc();
}

SyncCommand::~SyncCommand()
{
    delete socket_;
    socket_ = NULL;
}

void SyncCommand::regParseHandler()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        connect(this, SIGNAL(readyRead(QByteArray&)), target_obj_, slot_.c_str());
    }
}

void SyncCommand::unRegParseHandler()
{
    if ((target_obj_ != NULL) && (!slot_.empty()))
    {
        disconnect(this, SIGNAL(readyRead(QByteArray&)), target_obj_, slot_.c_str());
    }
}

void SyncCommand::genConnectErrorDesc()
{
    sock_err_desc_map_.insert(QAbstractSocket::ConnectionRefusedError, STRING_CONNECT_HOST_REFUSED);
    sock_err_desc_map_.insert(QAbstractSocket::RemoteHostClosedError, STRING_CONNECT_HOST_CLOSED);
    sock_err_desc_map_.insert(QAbstractSocket::HostNotFoundError, STRING_CONNECT_HOST_NOTFIND);
    sock_err_desc_map_.insert(QAbstractSocket::SocketAccessError, STRING_CONNECT_SOCK_ACCESS_ERR);
    sock_err_desc_map_.insert(QAbstractSocket::SocketResourceError, STRING_CONNECT_SOCK_RESOURCE_E);
    sock_err_desc_map_.insert(QAbstractSocket::SocketTimeoutError, STRING_CONNECT_SOCK_TIMEOUT);
    sock_err_desc_map_.insert(QAbstractSocket::DatagramTooLargeError, STRING_CONNECT_DATA_TOO_LARGE);
    sock_err_desc_map_.insert(QAbstractSocket::NetworkError, STRING_CONNECT_NETWORK_ERR);
    sock_err_desc_map_.insert(QAbstractSocket::AddressInUseError, STRING_CONNECT_ADDRESS_INUSE);
    sock_err_desc_map_.insert(QAbstractSocket::SocketAddressNotAvailableError, STRING_CONNECT_ADDR_NOT_ABLE);
    sock_err_desc_map_.insert(QAbstractSocket::UnsupportedSocketOperationError, STRING_CONNECT_UNSUPPORTED);
    sock_err_desc_map_.insert(QAbstractSocket::UnfinishedSocketOperationError, STRING_CONNECT_UNFINISHED);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyAuthenticationRequiredError, STRING_CONNECT_PROXY_AUTH);
    sock_err_desc_map_.insert(QAbstractSocket::SslHandshakeFailedError, STRING_CONNECT_SSL_ERROR);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyConnectionRefusedError, STRING_CONNECT_PROXY_REFUSED);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyConnectionClosedError, STRING_CONNECT_PROXY_CLOSED);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyConnectionTimeoutError, STRING_CONNECT_PROXY_TIMEOUT);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyNotFoundError, STRING_CONNECT_PROXY_NOTFIND);
    sock_err_desc_map_.insert(QAbstractSocket::ProxyProtocolError, STRING_CONNECT_PROXY_PROTOCOL);
    sock_err_desc_map_.insert(QAbstractSocket::UnknownSocketError, STRING_CONNECT_UNKNOWN);
}
