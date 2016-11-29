#ifndef MUSICSTREAMSBASE_H
#define MUSICSTREAMSBASE_H

#include <QObject>
#include <QString>
#include <QtSql>
#include <QDebug>

#include <DataStructures.h>

class MusicStreamsBase : public QObject
{
    Q_OBJECT
public:
    explicit MusicStreamsBase();
	~MusicStreamsBase();

    MusicStream getStreamWhereId(int id);
    MusicStream getStreamWhereName(QString name);

	MusicStream getNextStream(int id);
	MusicStream getPrevStream(int id);

    bool addStream(MusicStream stream);

private:
    QSqlDatabase streamBase_;
    QString baseFilePath_ = "/media/sf_repository/InternetRadioPlayer/base/";
    QString baseName_ = "MusicStreamsBase.db";

    MusicStream getStream(QSqlQuery query);

signals:

public slots:

};

#endif // MUSICSTREAMSBASE_H
