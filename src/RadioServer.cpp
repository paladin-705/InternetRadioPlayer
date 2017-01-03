#include "RadioServer.h"

RadioServer::RadioServer()
{
    qDebug() << "Starting server...";

	server = new QTcpServer();

    if(server->listen(QHostAddress::Any, _serverPort))
        qDebug() << "Server listen port: " << _serverPort;
    else
        qCritical() << "Server starting error: " << server->serverError();

    connect(server, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
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
