#include "RadioServer.h"

RadioServer::RadioServer()
{
	server = new QTcpServer();
    qDebug() << "server listen = " << server->listen(QHostAddress::Any, 1234);
    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection())); //
}

RadioServer::~RadioServer()
{
    delete server;
}

void RadioServer::slotNewConnection()
{
	while (server->hasPendingConnections())
	{
		QTcpSocket *clientSocket = server->nextPendingConnection();

		connect(clientSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
		connect(clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
	}
}

void RadioServer::slotReadyRead()
{
	QTcpSocket *clientSocket = (QTcpSocket*)sender();

    emit parseCommand(clientSocket);
}
