#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QTcpSocket>
#include <QObject>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = 0);
    explicit SocketClient(QString host, int port);
    void sendData(QByteArray data);

private:
    QTcpSocket *socket_;

signals:
    void dataReceived(QByteArray data);

private slots:
    void slotReadyRead();
};

#endif // SOCKETCLIENT_H
