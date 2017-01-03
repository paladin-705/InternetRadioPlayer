#ifndef RADIOSERVER_H
#define RADIOSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class RadioServer: public QTcpServer
{
    Q_OBJECT
public:
    explicit RadioServer();
	~RadioServer();

private:
	QTcpServer *server;
    int _serverPort = 1234;

signals:
    void parseCommand(QTcpSocket *clientSocket);

private slots :
	void slotNewConnection();
	void slotReadyRead();
};

#endif // RADIOSERVER_H
