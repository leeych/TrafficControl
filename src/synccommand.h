#ifndef SYNCCOMMAND_H
#define SYNCCOMMAND_H

#include <QObject>
#include <QTcpSocket>
#include <QMap>

class SyncCommand : public QObject
{
    Q_OBJECT
public:
    static SyncCommand *getInstance();
    void distroyInstance();

    QTcpSocket *getSocket() const;
    void connectToHost(const QString &ip, unsigned int port);
    // unused
    bool syncConnectToHost(const QString &ip, unsigned int port);
    void closeConnection();
    bool isConnectionValid() const;
    void initParseHandler(QObject *obj, const std::string &slot);
    void disconnectFromHost();

    void sendVersionCheck(QObject *obj, const std::string &slot);
    void sendVersionCheck();
    void sendAutoCtrl(QObject *obj, const std::string &slot);
    void sendAutoCtrl();
    void sendManualCtrl(QObject *obj, const std::string &slot);
    void sendManualCtrl();
    void sendSteppingCtrl(QObject *obj, const std::string &slot);
    void sendSteppingCtrl();
    void sendYellowFlashCtrl(QObject *obj, const std::string &slot);
    void sendYellowFlashCtrl();
    void sendAllRedCtrl(QObject *obj, const std::string &slot);
    void sendAllRedCtrl();
    void sendHeartBeat();

signals:
    void connectedSignal();
    void connectErrorSignal();
    void connectErrorStrString(const QString &);
    void disconnectedSignal();

    void readyRead(QByteArray &content);
    void readyRead();

public slots:
    void onConnectEstablished();
    void onDisconnected();
    void onConnectError(QAbstractSocket::SocketError);

private slots:
    void parseReply();
    void socketReadyReadSlot();

private:
    explicit SyncCommand(QObject *parent = 0);
    ~SyncCommand();
    static SyncCommand *instance_;

    void regParseHandler();
    void unRegParseHandler();
    void genConnectErrorDesc();

private:
    QTcpSocket *socket_;
    QString ip_;
    unsigned int port_;
    QByteArray sock_array_;
    QObject *target_obj_;
    std::string slot_;

    QMap<QAbstractSocket::SocketError, QString> sock_err_desc_map_;

};

#endif // SYNCCOMMAND_H
