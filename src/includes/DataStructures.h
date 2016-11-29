#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

//Dara structures:
struct MusicStream
{
	int id;					//-1 - stream not found
	int state;
	QString name;
	QString address;
};

//Stream state:
const int STREAM_NOT_FOUND = -1;

const int STREAM_PLAY = 0x10;
const int STREAM_PAUSE = 0x11;
const int STREAM_STOP = 0x12;
const int STREAM_NEXT = 0x13;
const int STREAM_PREV = 0x14;
const int STREAM_INFO = 0x15;

const int STREAM_GET_VOLUME = 0x20;
const int STREAM_SET_VOLUME = 0x21;
const int STREAM_DECREASE_VOLUME = 0x22;
const int STREAM_INCREASE_VOLUME = 0x23;

const int STREAM_SET_WHERE_ID = 0x30;
const int STREAM_SET_WHERE_NAME = 0x31;
const int STREAM_ADD = 0x32;
const int STREAM_UPDATE = 0x33;
const int STREAM_DELETE = 0x34;

//Response:
const int RESPONSE_OK = 0x01;
const int RESPONSE_ERROR = 0x00;

const int RESPONSE_STREAM_STRUCT = 0x10;
const int RESPONSE_VOLUME_STRUCT = 0x11;

#endif // RADIOSERVER_H
