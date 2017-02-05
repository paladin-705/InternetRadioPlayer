#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQuickWindow>
#include <QQuickView>
#include <QTimer>

#include <QDir>
#include <QDateTime>

#include "includes/MusicPlayerController.h"
#include "includes/ListViewStreamModel.h"

QScopedPointer<QFile> logFile;
void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QObject* root = engine.rootObjects()[0];

    MusicPlayerController *playerController = new MusicPlayerController(root, engine.rootContext());

    QTimer *timer = new QTimer();
    timer->start(1000);

    QObject::connect(root, SIGNAL(buttonSignal(QString)),playerController, SLOT(buttonSlot(QString)));
    QObject::connect(root, SIGNAL(changeConnectStateSignal(bool)),playerController, SLOT(changeConnectStateSlot(bool)));
    QObject::connect(root, SIGNAL(changeVolumeSignal(qreal)),playerController, SLOT(changeVolumeSlot(qreal)));
    QObject::connect(root, SIGNAL(changeStreamSignal(int)),playerController, SLOT(changeStreamSlot(int)));

     QObject::connect(root, SIGNAL(addStreamSignal(QString, QString)), playerController, SLOT(addStreamSlot(QString, QString)));
    QObject::connect(root, SIGNAL(updateStreamSignal(int, QString, QString, int)), playerController, SLOT(updateStreamSlot(int, QString, QString, int)));
    QObject::connect(root, SIGNAL(deleteStreamSignal(int)),playerController, SLOT(deleteStreamSlot(int)));

    QObject::connect(playerController, SIGNAL(closePlayer()), QCoreApplication::instance(), SLOT(quit()));
    QObject::connect(timer,SIGNAL(timeout()), playerController, SLOT(updateInfoSlot()));

    //Log:
    QDir logDir("log");
    if (!logDir.exists()) {
        logDir.mkpath(".");
    }

    logFile.reset(new QFile(logDir.path() + "/" + QString("%1/log-%2.log").arg(".").arg(QDate::currentDate().toString("dd.MM.yy"))));
    logFile.data()->open(QFile::Append | QFile::Text);

    qInstallMessageHandler(logMessageHandler);

    return app.exec();
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

    QString function = rx.cap(1);
    QString msgSep = (msg.length()>0)? " >> " : "";

    out << "] "<< function << msgSep << msg << endl;
    out.flush();
}
