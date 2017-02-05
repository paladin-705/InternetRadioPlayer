#include "includes/SocketClient.h"

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    socket_ = new QTcpSocket();
}
SocketClient::SocketClient(QString host, int port)
{
    socket_ = new QTcpSocket();

    socket_->connectToHost(host, port);
    connect(socket_, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

void SocketClient::sendData(QByteArray data)
{
    socket_->write(data);
}

void SocketClient::slotReadyRead()
{
    QByteArray dataArray = socket_->readAll();
    socket_->close();
    dataReceived(dataArray);
}
