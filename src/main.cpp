#include <QCoreApplication>
#include <QDebug>

#include <RadioPlayerStream.h>
#include <RadioServer.h>

#include <QTcpSocket>

QScopedPointer<QFile> logFile;

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Log:
    QDir logDir("log");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    logFile.reset(new QFile(logDir.path() + "/" + QString("%1/log-%2.log").arg(".").arg(QDate::currentDate().toString("dd.MM.yy"))));
    logFile.data()->open(QFile::Append | QFile::Text);

    qInstallMessageHandler(logMessageHandler);

	RadioPlayerStream *playerStream = new RadioPlayerStream();
	RadioServer *radioServer = new RadioServer();

    QObject::connect(radioServer, SIGNAL(parseCommand(QTcpSocket*)), playerStream, SLOT(parseCommandSlot(QTcpSocket*)));

    return a.exec();
	delete playerStream;
    delete radioServer;
}

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ") << "[ ";
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtDebugMsg:
        out << "DBG ";
        break;
    case QtWarningMsg:
        out << "WRN ";
        break;
    case QtCriticalMsg:
        out << "CRT "; break;
    case QtFatalMsg:
        out << "FTL "; break;
    }

    QRegExp rx("([\\w-]+::[\\w-]+)");
    if (rx.indexIn(context.function) ==-1) return;

    QString function = rx.cap(1);
    QString msgSep = (msg.length()>0)? " >> " : "";

    out << "] "<< function << msgSep << msg << endl;
    out.flush();
}
