#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H

#include <QObject>
#include <QString>
#include <QSettings>

#include <DataStructures.h>
#include <bass.h>

class RadioPlayer: public QObject
{
    Q_OBJECT
public:
    explicit RadioPlayer();
	~RadioPlayer();

    bool play();
    bool pause();
    bool stop();

    bool setStream(MusicStream address);
    inline MusicStream getStream() { return musicStream_; };

    inline float getVolume() { return volume_; };
    bool setVolume(float volume);
	bool decreaseVolume();
	bool increaseVolume();

    inline int getErrorCode(){ return BASS_ErrorGetCode(); };

private:
    bool init();

    bool connect();
    bool disconnect();

	void loadSettings();
	void saveSettings();

    MusicStream musicStream_;
    HSTREAM streamHandle_;
    double volume_;
	int autoSaveInterval_;

protected:
	void timerEvent(QTimerEvent *event);

signals:

public slots:

};

#endif // RADIOPLAYER_H
