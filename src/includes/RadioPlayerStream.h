#ifndef RADIOPLAYERSTREAM_H
#define RADIOPLAYERSTREAM_H

#include <QThread>
#include <QTcpSocket>

#include <RadioPlayer.h>
#include <MusicStreamsBase.h>
#include <DataStructures.h>

class RadioPlayerStream : public QThread
{
    Q_OBJECT
public:
    explicit RadioPlayerStream();
    ~RadioPlayerStream();

    void run();
	
private:
    RadioPlayer *radioPlayer;
	MusicStreamsBase *streamsBase;

public slots:
    void parseCommandSlot(QTcpSocket *clientSocket);
};

#endif // RADIOPLAYERSTREAM_H
