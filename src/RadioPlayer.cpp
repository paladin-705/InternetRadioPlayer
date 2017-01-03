#include "RadioPlayer.h"

RadioPlayer::RadioPlayer()
{
	loadSettings();

    if(musicStream_.state == STREAM_PLAY)
        play();
    else
        init();

	startTimer(autoSaveInterval_);
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

bool RadioPlayer::setVolume(float volume)
{
	if (volume < 0)
		volume = 0;

	if (volume > 1)
		volume = 1;

    volume_ = volume;
	if (BASS_SetVolume(volume_))
	{
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
bool RadioPlayer::init()
{
    int device = -1;
    int freq = 44100;

    BASS_Free();
    bool state =  BASS_Init(device, freq, BASS_DEVICE_STEREO, 0, NULL);

    if(state)
    {
        BASS_INFO info;
        BASS_GetInfo(&info);

        BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, buffLen_/10);
        BASS_SetConfig(BASS_CONFIG_BUFFER, buffLen_);
        BASS_SetConfig(BASS_CONFIG_DEV_BUFFER, buffLen_);
        BASS_SetConfig(BASS_CONFIG_NET_BUFFER, buffLen_);
    }
    return state;
}
//private:
bool RadioPlayer::connect()
{
    bool state = init();

    streamHandle_ = BASS_StreamCreateURL(musicStream_.address.toStdString().c_str(), 0, BASS_STREAM_BLOCK, 0, 0);

    if(streamHandle_ == 0)
        streamHandle_ = BASS_StreamCreateFile(FALSE,musicStream_.address.toStdString().c_str(),0,0,BASS_STREAM_DECODE);

    state &= streamHandle_;

    return state;
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
	
	autoSaveInterval_ = settings->value("radioPlayer/autoSaveInterval", 5000).toInt();
    buffLen_ = settings->value("radioPlayer/bufferLeghtn_ms", 15000).toInt();

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

	settings->setValue("radioPlayer/autoSaveInterval", autoSaveInterval_);
    settings->setValue("radioPlayer/bufferLeghtn_ms", buffLen_);

	settings->sync();
	delete settings;
}

//protected:
void RadioPlayer::timerEvent(QTimerEvent *event)
{
	saveSettings();
}

//callbacks:
void CALLBACK RadioPlayer::downloadProc(const void *buffer, DWORD length, void *user)
{

}
