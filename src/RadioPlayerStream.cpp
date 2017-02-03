#include "RadioPlayerStream.h"

RadioPlayerStream::RadioPlayerStream()
{
    radioPlayer = new RadioPlayer();
	streamsBase = new MusicStreamsBase();
}

RadioPlayerStream::~RadioPlayerStream()
{
    delete radioPlayer;
	delete streamsBase;
}

void RadioPlayerStream::run()
{

}

void RadioPlayerStream::parseCommandSlot(QTcpSocket *clientSocket)
{
	int commandType;
	int status;
	int errorType = 0;

    QByteArray dataArray = clientSocket->readAll();
    QDataStream inputData(&dataArray, QIODevice::ReadOnly);

    QByteArray  responseArray;
    QDataStream response(&responseArray, QIODevice::WriteOnly);

	inputData >> commandType;
	
	switch (commandType)
	{
	case STREAM_PLAY:
	{
        radioPlayer->play();
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status << errorType;
		break;
	}
	case STREAM_PAUSE:
	{
        radioPlayer->pause();
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status << errorType;
		break;
	}
	case STREAM_STOP:
	{
        radioPlayer->stop();
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status << errorType;
		break;
	}
	case STREAM_NEXT:
	{
        MusicStream nextStream = streamsBase->getNextStream(radioPlayer->getStream().id);

        if(nextStream.id != STREAM_NOT_FOUND)
        {
            if (radioPlayer->setStream(nextStream))
                status = RESPONSE_OK;
            else
                status = RESPONSE_ERROR;

            errorType = radioPlayer->getErrorCode();
        }
        else
        {
            status = RESPONSE_ERROR;
            errorType = ERROR_NEXT_STREAM_NOT_FOUND;
        }

		response << status << errorType;
		break;
	}
	case STREAM_PREV:
	{
        MusicStream prevStream = streamsBase->getPrevStream(radioPlayer->getStream().id);

        if(prevStream.id != STREAM_NOT_FOUND)
        {
            if (radioPlayer->setStream(prevStream))
                status = RESPONSE_OK;
            else
                status = RESPONSE_ERROR;

            errorType = radioPlayer->getErrorCode();
        }
        else
        {
            status = RESPONSE_ERROR;
            errorType = ERROR_PREV_STREAM_NOT_FOUND;
        }

		response << status << errorType;
		break;
	}
	case STREAM_INFO:
	{
		status = RESPONSE_STREAM_STRUCT;
        MusicStream playerStream = radioPlayer->getStream();
        response << status << errorType << playerStream.id << playerStream.state << playerStream.name << playerStream.address;
		break;
	}
	case STREAM_LIST:
	{
        QVector<MusicStream> streamList;

        if (streamsBase->getStreamList(&streamList))
		{
			status = RESPONSE_STREAM_LIST_STRUCT;

			response << status << errorType << streamList.size();
            for (int i = 0;i < streamList.size(); i++)
			{
				response  << streamList.at(i).id << streamList.at(i).state << streamList.at(i).name << streamList.at(i).address;
			}
		}
		else
		{
			status = RESPONSE_ERROR;
			errorType = ERROR_EXEC_QUERY;

			response << status << errorType;
		}
		break;
	}
	case STREAM_GET_VOLUME:
	{
		status = RESPONSE_VOLUME_STRUCT;
        response << status << errorType << radioPlayer->getVolume();
		break;
	}
	//-------

	case STREAM_SET_VOLUME:
	{
        float volume;
		inputData >> volume;

        radioPlayer->setVolume(volume);
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status << errorType;
		break;
	}
	case STREAM_DECREASE_VOLUME:
	{
        radioPlayer->decreaseVolume();
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status << errorType;
		break;
	}
	case STREAM_INCREASE_VOLUME:
	{
        radioPlayer->increaseVolume();
        errorType = radioPlayer->getErrorCode();

        if (!errorType)
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status << errorType;
		break;
	}
	//-------

	case STREAM_SET_WHERE_ID:
	{
		int id;
		inputData >> id;

		MusicStream stream = streamsBase->getStreamWhereId(id);

		if (radioPlayer->setStream(stream))
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		errorType = radioPlayer->getErrorCode();

		response << status << errorType;
		break;
	}
	case STREAM_SET_WHERE_NAME:
	{
		QString name;
		inputData >> name;

		MusicStream stream = streamsBase->getStreamWhereName(name);

		if (radioPlayer->setStream(stream))
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		errorType = radioPlayer->getErrorCode();

		response << status << errorType;
		break;
	}
	case STREAM_ADD:
	{
		MusicStream stream;
		inputData >> stream.id >> stream.state >> stream.name >> stream.address;

		if (streamsBase->addStream(stream))
		{
			status = RESPONSE_OK;
		}
		else
		{
			status = RESPONSE_ERROR;
			errorType = ERROR_ADD_STREAM;
		}

		response << status << errorType;
		break;
	}
	case STREAM_UPDATE:
	{
		MusicStream stream;
		inputData >> stream.id >> stream.state >> stream.name >> stream.address;

		if (streamsBase->updateStream(stream))
		{
			status = RESPONSE_OK;
		}
		else
		{
			status = RESPONSE_ERROR;
			errorType = ERROR_UPDATE_STREAM;
		}

		response << status << errorType;
		break;
	}
	case STREAM_DELETE:
	{
		int id;
		inputData >> id;

		if (streamsBase->deleteStream(id))
		{
			status = RESPONSE_OK;
		}
		else
		{
			status = RESPONSE_ERROR;
			errorType = ERROR_DELETE_STREAM;
		}

		response << status << errorType;
		break;
	}

	default:
		status = RESPONSE_ERROR;
		errorType = ERROR_UNKNOWN_COMMAND;
		
		response << status << errorType;
		break;
	}

    if(status == RESPONSE_ERROR)
    {
        qWarning() << "Response error: " << errorType;
    }

clientSocket->write(responseArray);
clientSocket->close();
}


