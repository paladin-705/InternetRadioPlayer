#ifndef MUSICPLAYERCONTROLLER_H
#define MUSICPLAYERCONTROLLER_H

#include <QObject>
#include <QDataStream>
#include <QSettings>
#include <QDebug>
#include<QTimer>

#include "DataStructures.h"
#include "SocketClient.h"

#include "ListViewStreamModel.h"

#include <QQmlContext>

class MusicPlayerController : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlayerController(QObject *parent, QQmlContext *rootContextObj);

protected:
    void timerEvent(QTimerEvent *event);

private:
    QQmlContext *rootContextObj_;

    bool connectState_;

    QString host_;
    int port_;

    void sendToServer(QByteArray command);

    void loadSetting();
    void saveSetting();

    float volume_;
    int streamId_;
    int streamStatus_;
    int autoSaveInterval_;

    //QMap<int, int> streamHash;
    QMap<int, StreamModel> streamHash;
signals:
    void closePlayer();

public slots:
    void buttonSlot(QString id);
    void changeConnectStateSlot(bool state);
    void changeVolumeSlot(qreal volume);
    void changeStreamSlot(int id);

    void addStreamSlot(QString name, QString url);
    void updateStreamSlot(int streanId, QString name, QString url, int state);
    void deleteStreamSlot(int streamId);

    void slotReadyRead(QByteArray dataArray);
    void updateInfoSlot();
};

#endif // MUSICPLAYERCONTROLLER_H
