#include "MusicStreamsBase.h"

MusicStreamsBase::MusicStreamsBase()
{
    streamBase_ = QSqlDatabase::addDatabase("QSQLITE");
    streamBase_.setDatabaseName(baseFilePath_ + baseName_);

    if (!streamBase_.open())
    {
        qDebug() << streamBase_.lastError().text();
    }
    else
    {
        qDebug() << "------- Database opened -------";
    }
}

MusicStreamsBase::~MusicStreamsBase()
{

}

bool MusicStreamsBase::addStream(MusicStream stream)
{
	QSqlQuery query;
    query.prepare("INSERT INTO musicStreams(streamName, streamAddress) VALUES (:name , :address);");
	query.bindValue(":name", stream.name);
    query.bindValue(":address", stream.address);

    if (query.exec())
	{
		return true;
	}
	else
	{
        qDebug() << "addStream error:  " << query.lastError();
		return false;
	}

}

MusicStream MusicStreamsBase::getStreamWhereId(int id)
{
	QSqlQuery query;
    query.prepare("SELECT * FROM musicStreams WHERE ID = :id;");
    query.bindValue(":id", id);

    return getStream(query);
}

MusicStream MusicStreamsBase::getNextStream(int id)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM musicStreams WHERE ID > :id LIMIT 1;");
	query.bindValue(":id", id);

	return getStream(query);
}

MusicStream MusicStreamsBase::getPrevStream(int id)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM musicStreams WHERE ID = :id LIMIT 1;");
	query.bindValue(":id", id);

	return getStream(query);
}

MusicStream MusicStreamsBase::getStreamWhereName(QString name)
{
	QSqlQuery query;
    query.prepare("SELECT * FROM musicStreams WHERE streamName = :name;");
    query.bindValue(":name", name);

    return getStream(query);
}


MusicStream MusicStreamsBase::getStream(QSqlQuery query)
{
    MusicStream stream;
	stream.id = STREAM_NOT_FOUND;

    if (query.exec())
	{
        if(query.next())
        {
            QSqlRecord rec = query.record();
            stream.id = query.value(rec.indexOf("ID")).toInt();
            stream.name = query.value(rec.indexOf("streamName")).toString();
            stream.address = query.value(rec.indexOf("streamAddress")).toString();
        }
        else
        {
            qDebug() << "getStream parse error:  " << query.lastError();
        }
	}
    else
    {
        qDebug() << "getStream error:  " << query.lastError();
    }
	return stream;
}
