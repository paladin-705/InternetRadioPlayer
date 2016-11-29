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

    QByteArray dataArray = clientSocket->readAll();
    QDataStream inputData(&dataArray, QIODevice::ReadOnly);

    QByteArray  responseArray;
    QDataStream response(&responseArray, QIODevice::WriteOnly);

	inputData >> commandType;
	
	switch (commandType)
	{
	case STREAM_PLAY:
	{
		if (radioPlayer->play())
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	case STREAM_PAUSE:
	{
		if (radioPlayer->pause())
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	case STREAM_STOP:
	{
		if (radioPlayer->stop())
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	case STREAM_NEXT:
	{
        MusicStream nextStream = streamsBase->getNextStream(radioPlayer->getStream().id);

		if (radioPlayer->setStream(nextStream))
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status;
		break;
	}
	case STREAM_PREV:
	{
        MusicStream prevStream = streamsBase->getNextStream(radioPlayer->getStream().id);

		if (radioPlayer->setStream(prevStream))
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

		response << status;
		break;
	}
	case STREAM_INFO:
	{
		status = RESPONSE_STREAM_STRUCT;
        MusicStream playerStream = radioPlayer->getStream();
        response << status << playerStream.id << playerStream.state << playerStream.name << playerStream.address;
		break;
	}
	case STREAM_GET_VOLUME:
	{
		status = RESPONSE_VOLUME_STRUCT;
        response << status << radioPlayer->getVolume();
		break;
	}
	//-------

	case STREAM_SET_VOLUME:
	{
		double volume;
		inputData >> volume;

		if (radioPlayer->setVolume(volume))
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	case STREAM_DECREASE_VOLUME:
	{
		if (radioPlayer->decreaseVolume())
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	case STREAM_INCREASE_VOLUME:
	{
		if (radioPlayer->increaseVolume())
			status = RESPONSE_OK;
		else
			status = RESPONSE_ERROR;

        response << status;
		break;
	}
	//-------

	case STREAM_SET_WHERE_ID:
	{
		break;
	}
	case STREAM_SET_WHERE_NAME:
	{
		break;
	}
	case STREAM_ADD:
	{
		break;
	}
	case STREAM_UPDATE:
	{
		break;
	}
	case STREAM_DELETE:
	{
		break;
	}

	default:
		status = RESPONSE_ERROR;
		break;
	}
clientSocket->write(responseArray);
}
