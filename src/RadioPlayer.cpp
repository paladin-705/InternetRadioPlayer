#include "RadioPlayer.h"

RadioPlayer::RadioPlayer()
{
	loadSettings();
}

RadioPlayer::~RadioPlayer()
{
	saveSettings();
}

bool RadioPlayer::setStream(MusicStream musicStream)
{
	int streamState = musicStream_.state;
    musicStream_ = musicStream;
	musicStream_.state = streamState;
	
	if (musicStream_.id == STREAM_NOT_FOUND)
		return false;

	if (musicStream_.state == STREAM_PLAY)
		play();

	return true;
}

bool RadioPlayer::play()
{
    bool state = connect();
	state &= BASS_ChannelPlay(streamHandle_, TRUE);

	if (state)
		musicStream_.state = STREAM_PLAY;

    return state;
}

bool RadioPlayer::pause()
{
	bool state = BASS_ChannelStop(streamHandle_);
	state &= disconnect();

	if (state)
		musicStream_.state = STREAM_PAUSE;

	return state;
}

bool RadioPlayer::stop()
{
	bool state = BASS_ChannelStop(streamHandle_);
	state &= disconnect();

	if (state)
		musicStream_.state = STREAM_STOP;

	return state;
}

bool RadioPlayer::setVolume(double volume)
{
	if (volume < 0)
		volume = 0;

	if (volume > 1)
		volume = 1;

	if (BASS_SetVolume(volume_))
	{
		volume_ = volume;
		return true;
	}
	else
	{
		return false;
	}
}

bool RadioPlayer::decreaseVolume()
{
	return setVolume(volume_ - 0.01);
}

bool RadioPlayer::increaseVolume()
{
	return setVolume(volume_ + 0.01);
}

//private func:
bool RadioPlayer::connect()
{
    int device = -1;
    int freq = 44100;
    BASS_Init(device, freq, 0, 0, NULL);

    streamHandle_ = BASS_StreamCreateURL(musicStream_.address.toStdString().c_str(), 0, 0, NULL, 0);

    return streamHandle_;
}

bool RadioPlayer::disconnect()
{
	bool state = BASS_StreamFree(streamHandle_);
	state &= BASS_Free();
	return state;
}

void RadioPlayer::loadSettings()
{
	QSettings *settings = new QSettings("settings.conf", QSettings::NativeFormat);

    volume_ = settings->value("radioPlayer/volume", 0.5).toDouble();

	musicStream_.state = settings->value("radioPlayer/streamState", -1).toInt();
    musicStream_.id = settings->value("radioPlayer/streamId", -1).toInt();
    musicStream_.name = settings->value("radioPlayer/streamName", "").toString();
    musicStream_.address = settings->value("radioPlayer/streamAddress", "").toString();
	
	delete settings;
}

void RadioPlayer::saveSettings()
{
	QSettings *settings = new QSettings("settings.conf", QSettings::NativeFormat);

	settings->setValue("radioPlayer/volume", volume_);

	settings->setValue("radioPlayer/streamState", musicStream_.state);
	settings->setValue("radioPlayer/streamId", musicStream_.id);
	settings->setValue("radioPlayer/streamName", musicStream_.name);
	settings->setValue("radioPlayer/streamAddress", musicStream_.address);

	settings->sync();
	delete settings;
}
