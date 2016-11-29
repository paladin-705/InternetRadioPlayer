#include "RadioClient.h"

RadioClient::RadioClient(QTcpSocket *socket)
{

}

RadioClient::RadioClient(const QString host, int port)
{
    socket_ = new QTcpSocket();

    socket_->connectToHost(host, port);
    connect(socket_, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
	connect(this, SIGNAL(sendCommand(QString command)), this, SLOT(slotSendToServer(QString command)));
}

void RadioClient::slotReadyRead()
{
	quint16 nextBlockSize = 0;
    QDataStream in(socket_);

    while (true)
	{
		if (!nextBlockSize) {
            if (socket_->bytesAvailable() < sizeof(nextBlockSize))
            {
				break;
			}
            in >> nextBlockSize;
		}

        if (socket_->bytesAvailable() < nextBlockSize) {
			break;
		}

		//in >> time >> str;

		nextBlockSize = 0;
	}
}


void RadioClient::slotSendToServer(QString command)
{
	QByteArray  arrBlock;
	QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out << quint16(0) << command;

	out.device()->seek(0);
	out << quint16(arrBlock.size() - sizeof(quint16));

    socket_->write(arrBlock);
}
