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
    bool setVolume(double volume);
	bool decreaseVolume();
	bool increaseVolume();

private:
    bool connect();
    bool disconnect();
	void loadSettings();
	void saveSettings();

    MusicStream musicStream_;
    HSTREAM streamHandle_;
    double volume_;

signals:

public slots:

};

#endif // RADIOPLAYER_H
