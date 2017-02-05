#include "includes/MusicPlayerController.h"

MusicPlayerController::MusicPlayerController(QObject *parent, QQmlContext *rootContextObj) : QObject(parent)
{
    rootContextObj_ = rootContextObj;

    loadSetting();
    startTimer(autoSaveInterval_);

    connectState_ = false;
}

void MusicPlayerController::sendToServer(QByteArray command)
{
    SocketClient *socketClient = new SocketClient(host_,port_);
    connect(socketClient, SIGNAL(dataReceived(QByteArray)), this, SLOT(slotReadyRead(QByteArray)));
    socketClient->sendData(command);
}

void MusicPlayerController::saveSetting()
{
    QSettings *settings = new QSettings("settings.conf", QSettings::IniFormat);
    settings->setValue("radioPlayer/volume", volume_);
    settings->setValue("radioPlayer/address", host_);
    settings->setValue("radioPlayer/port", port_);

    settings->sync();
    delete settings;
}

void MusicPlayerController::loadSetting()
{
    QSettings *settings = new QSettings("settings.conf", QSettings::IniFormat);

    volume_ = settings->value("radioPlayer/volume", 0.5).toFloat();
    host_ = settings->value("radioPlayer/address", "127.0.0.1").toString();
    port_ = settings->value("radioPlayer/port", 1234).toInt();

    autoSaveInterval_ = settings->value("radioPlayer/autoSaveInterval", 5000).toInt();

    delete settings;

    QObject* VolumeSlider = parent()->findChild<QObject*>("sliderVolume");
    QObject* AddressField = parent()->findChild<QObject*>("textFieldAddress");
    QObject* PortField = parent()->findChild<QObject*>("textFieldPort");

    VolumeSlider->setProperty("value", QVariant(volume_));
    AddressField->setProperty("text", QVariant(host_));
    PortField->setProperty("text", QVariant(port_));
}

//Slots:
void MusicPlayerController::buttonSlot(QString id)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    if(id == "prev")
    {
        if(connectState_)
        {
            out << STREAM_PREV;
        }
    }
    else if(id == "play")
    {
        if(connectState_)
        {
            QObject* buttonPlay = parent()->findChild<QObject*>("buttonPlay");
            QString buttonPlayText = (buttonPlay->property("text")).toString();

            if(buttonPlayText == "Play")
            {
                out << STREAM_PLAY;
                //buttonPlay->setProperty("text", QVariant("Stop"));
            }
            else
            {
                out << STREAM_STOP;
                //buttonPlay->setProperty("text", "Play");
            }
        }
    }
    else if(id == "next")
    {
        if(connectState_)
        {
            out << STREAM_NEXT;
        }
    }
    else if(id == "closeProgram")
    {
        saveSetting();
        exit(0);        //Временное решение проблемы краша приложеня при вызове quit()
        closePlayer();
    }
    else if(id == "saveSetting")
    {
        QObject* AddressField = parent()->findChild<QObject*>("textFieldAddress");
        QObject* PortField = parent()->findChild<QObject*>("textFieldPort");

        host_ = (AddressField->property("text")).toString();
        port_ = (PortField->property("text")).toInt();

        saveSetting();
    }

    if(connectState_ && id != "saveSetting" && id != "closeProgram")
    {
        sendToServer(arrBlock);
    }
}

void MusicPlayerController::changeConnectStateSlot(bool state)
{
    connectState_ = state;

    QObject* switchMode = parent()->findChild<QObject*>("switchMode");
    QString switchModeText = (switchMode->property("text")).toString();

    if(switchModeText == "On")
    {
        switchMode->setProperty("text", QVariant("Off"));
    }
    else
    {
        //Ping запрос к серверу
        //QString errorStr = QString(socket_->errorString());
        QString errorStr = NULL;

        if(errorStr == NULL)
        {
            switchMode->setProperty("text", QVariant("On"));
        }
        else
        {
            switchMode->setProperty("checked", QVariant("false"));
        }
    }
}

void MusicPlayerController::changeVolumeSlot(qreal volume)
{
    volume_ = volume;

    if(connectState_)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out << STREAM_SET_VOLUME << volume_;
        sendToServer(arrBlock);
    }
}


void MusicPlayerController::slotReadyRead(QByteArray dataArray)
{
    QDataStream inputData(&dataArray, QIODevice::ReadOnly);

    int status;
    int errorType;
    inputData >> status >> errorType;

    switch (status)
    {
    case RESPONSE_OK:
    {
        //qDebug() << "RESPONSE_OK";
        break;
    }
    case RESPONSE_ERROR:
    {
        qWarning() << "RESPONSE_ERROR: " << errorType;
        break;
    }
    case RESPONSE_STREAM_STRUCT:
    {
        MusicStream stream;
        inputData >> stream.id >> stream.state >> stream.name >> stream.address;

        QObject* labelInfo = parent()->findChild<QObject*>("labelInfo");
        labelInfo->setProperty("text", QVariant(QString::number(stream.id) + " " + stream.name));

        QObject* buttonPlay = parent()->findChild<QObject*>("buttonPlay");
        QString buttonPlayText = (buttonPlay->property("text")).toString();

        if(stream.state == STREAM_PLAY)
        {
            if(buttonPlayText == "Play")
            {
                buttonPlay->setProperty("text", QVariant("Stop"));
            }
        }
        else
        {
            if(buttonPlayText != "Play")
            {
                buttonPlay->setProperty("text", QVariant("Play"));
            }
        }
        break;
    }
    case RESPONSE_VOLUME_STRUCT:
    {
        float volume;
        inputData >> volume;
        break;
    }
    case RESPONSE_STREAM_LIST_STRUCT:
    {
        MusicStream stream;
        int len;
        inputData >> len;

        ListViewStreamModel *dataModel = new ListViewStreamModel();

        if(streamHash.size() != len)
            streamHash.clear();

        bool flg = false;
        for(int i=0;i<len;i++)
        {
            inputData >> stream.id >> stream.state >> stream.name >> stream.address;
            int hash = qHash(stream.name + stream.address + QString::number(stream.id) );

            QMap<int, StreamModel>::const_iterator iter = streamHash.find(stream.id);

            if((iter == streamHash.end() || iter.value().getHash() != hash) && stream.id != 0 && stream.id != -1)
            {
                flg = true;
                //qDebug()<<stream.id << " : " << stream.address <<" : " << iter.value().getHash() << " : " <<hash;
                streamHash.insert(stream.id, StreamModel(stream.id, stream.name, stream.address, stream.state, hash));
            }
        }

        if(flg)
        {
            foreach (StreamModel value, streamHash)
            {
                dataModel->addStream(value);
            }
            rootContextObj_->setContextProperty("streamModel", dataModel);

        }
        delete dataModel;
        break;
    }
    }
}

void MusicPlayerController::timerEvent(QTimerEvent *event)
{
    saveSetting();
}

void MusicPlayerController::updateInfoSlot()
{
    if(connectState_)
    {
        QByteArray  streamCommand;
        QDataStream out(&streamCommand, QIODevice::WriteOnly);

        out << STREAM_INFO;
        sendToServer(streamCommand);

        QByteArray  streamListCommand;
        QDataStream listOut(&streamListCommand, QIODevice::WriteOnly);

        listOut << STREAM_LIST;
        sendToServer(streamListCommand);

        //Test:
        QByteArray  out1;
        QDataStream arr1(&out1, QIODevice::WriteOnly);

        arr1 << STREAM_GET_VOLUME;
        sendToServer(out1);

    }
}

void MusicPlayerController::addStreamSlot(QString name, QString url)
{
    if(connectState_)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out << STREAM_ADD << NULL << NULL << name << url;
        sendToServer(arrBlock);
    }
}

void MusicPlayerController::updateStreamSlot(int streamId, QString name, QString url, int state)
{
    if(connectState_)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out << STREAM_UPDATE << streamId << state << name << url;
        sendToServer(arrBlock);
    }
}

void MusicPlayerController::deleteStreamSlot(int streamId)
{
    if(connectState_)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out << STREAM_DELETE << streamId;
        sendToServer(arrBlock);
    }

}

void MusicPlayerController::changeStreamSlot(int id)
{
    if(connectState_)
    {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out << STREAM_SET_WHERE_ID << id;
        sendToServer(arrBlock);
    }
}
