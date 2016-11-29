#ifndef RADIOCLIENT_H
#define RADIOCLIENT_H

#include <QTcpSocket>

class RadioClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit RadioClient(const QString host, int port);
    explicit RadioClient(QTcpSocket *socket);

private:
	QTcpSocket *socket_;
	void slotReadyRead();
	void slotError(QAbstractSocket::SocketError);
	void slotSendToServer(QString command);
	void slotConnected();

signals:
	void sendCommand(QString command);

public slots:

};

#endif // RADIOCLIENT_H
