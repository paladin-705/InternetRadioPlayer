#include "MusicStreamsBase.h"

MusicStreamsBase::MusicStreamsBase()
{
    qDebug() << "Opening database...";

    streamBase_ = QSqlDatabase::addDatabase("QSQLITE");
    streamBase_.setDatabaseName(baseFilePath_ + baseName_);

    if (!streamBase_.open())
    {
        qWarning() << streamBase_.lastError().text();
    }
    else
    {
        streamBase_.exec("CREATE TABLE musicStreams (ID INTEGER PRIMARY KEY AUTOINCREMENT, streamName TEXT UNIQUE ON CONFLICT FAIL, streamAddress TEXT);");
        qDebug() << "Database opened";
    }
}

MusicStreamsBase::~MusicStreamsBase()
{

}

MusicStream MusicStreamsBase::getStreamWhereId(int id)
{
	QSqlQuery query;
    query.prepare("SELECT * FROM musicStreams WHERE ID = :id;");
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

MusicStream MusicStreamsBase::getNextStream(int id)
{
	QSqlQuery query;
    query.prepare("SELECT * FROM musicStreams WHERE ID > :id ORDER BY ID ASC LIMIT 1;");
	query.bindValue(":id", id);

	return getStream(query);
}

MusicStream MusicStreamsBase::getPrevStream(int id)
{
	QSqlQuery query;
    query.prepare("SELECT * FROM musicStreams WHERE ID < :id ORDER BY ID DESC LIMIT 1;");
	query.bindValue(":id", id);

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
            qWarning() << "parse error:  " << query.lastError();
        }
	}
    else
    {
        qWarning() << query.lastError();
    }
	return stream;
}

bool MusicStreamsBase::getStreamList(QVector<MusicStream> *streams)
{
	QSqlQuery query;
	if (query.exec("SELECT * FROM musicStreams"))
	{
		MusicStream stream;

		while (query.next())
		{
			QSqlRecord rec = query.record();
			stream.id = query.value(rec.indexOf("ID")).toInt();
			stream.name = query.value(rec.indexOf("streamName")).toString();
			stream.address = query.value(rec.indexOf("streamAddress")).toString();

            streams->append(stream);
		}
		return true;
	}
	return false;
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
        qWarning() << query.lastError();
		return false;
	}

}

bool MusicStreamsBase::updateStream(MusicStream stream)
{
	QSqlQuery query;
	query.prepare("UPDATE  musicStreams SET streamName = :name, streamAddress =  :address WHERE ID = :id;");
	query.bindValue(":name", stream.name);
	query.bindValue(":address", stream.address);
	query.bindValue(":id", stream.id);

	if (query.exec())
	{
		return true;
	}
	else
	{
        qWarning() << query.lastError();
		return false;
	}

}

bool MusicStreamsBase::deleteStream(int id)
{
	QSqlQuery query;
	query.prepare("DELETE FROM musicStreams WHERE ID = :id;");
    query.bindValue(":id", id);

	if (query.exec())
	{
		return true;
	}
	else
	{
        qWarning() << query.lastError();
		return false;
	}
}
