#include <QCoreApplication>
#include <QDebug>

#include <RadioPlayerStream.h>
#include <RadioServer.h>

#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	RadioPlayerStream *playerStream = new RadioPlayerStream();
	RadioServer *radioServer = new RadioServer();

    QObject::connect(radioServer, SIGNAL(parseCommand(QTcpSocket*)), playerStream, SLOT(parseCommandSlot(QTcpSocket*)));

    return a.exec();
	delete playerStream;
    delete radioServer;
}
