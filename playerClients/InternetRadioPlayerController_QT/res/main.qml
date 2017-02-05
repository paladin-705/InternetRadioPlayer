import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0

ApplicationWindow {
    id: applicationWindow

    //Убирает рамку но оставляет таскбар:
    flags: Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.Window

    visible: true
    width: 640
    height: 480
    title: qsTr("Internet radio player")

    //Signals:
    signal buttonSignal(string id);

    signal changeConnectStateSignal(bool state);
    signal changeVolumeSignal(real volume);
    signal changeStreamSignal(int id);

    signal addStreamSignal(string name, string url);
    signal updateStreamSignal(int streamId, string name, string url, int state);
    signal deleteStreamSignal(int streamId);

    Menu {
        id: menu
        height: applicationWindow.height - toolBar.height

        title: "Edit"

        MenuItem {
            text: "Add stream"
            onClicked: {
                popup_addStream.open();
            }
        }
        MenuSeparator { }

        MenuItem {
            text: "Setting"
            onClicked: {
                swipeView.setCurrentIndex(1)
            }
        }
    }

    header: ToolBar {
            id: toolBar

            RowLayout {
                anchors.fill: parent

                ToolButton {
                    font.pointSize: 16
                    text: "\u2261"
                    onClicked: menu.open()
                }

                Label {
                    text: "Internet radio player"

                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    Layout.fillWidth: true

                    //Позволяет перемещать окно без рамки за тулбар:
                    MouseArea {
                        anchors.fill: parent

                        property variant previousPosition
                        onPressed: {
                            previousPosition = Qt.point(mouseX, mouseY)
                        }
                        onPositionChanged: {
                            if (pressedButtons == Qt.LeftButton) {
                                var dx = mouseX - previousPosition.x
                                var dy = mouseY - previousPosition.y
                                applicationWindow.x = applicationWindow.x + dx
                                applicationWindow.y = applicationWindow.y + dy
                            }
                        }
                    }
                }

                ToolButton {
                    id: buttonHideProgram
                    objectName: "buttonHideProgram"

                    text: qsTr("-")
                    onClicked: applicationWindow.showMinimized()
                }
                ToolButton {
                    id: buttonCloseProgram
                    objectName: "buttonCloseProgram"

                    text: qsTr("x")
                    onClicked:
                    {
                        buttonSignal("closeProgram");
                    }
                }
            }
        }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {

        }

        Page {
            Button {
                id: buttonSave
                objectName: "buttonSave"

                x: 8
                y: 100
                text: qsTr("Save")

                onClicked:
                {
                    if(textFieldAddress.text != "" && textFieldPort.text != "")
                    {
                        buttonSignal("saveSetting");
                    }
                    else
                    {
                        if(textFieldAddress.text == "" )
                            textFieldAddress.forceActiveFocus();
                        else if(textFieldPort.text == "" )
                            textFieldPort.forceActiveFocus();
                    }
                }
            }

            TextField {
                id: textFieldAddress
                objectName: "textFieldAddress"

                x: 8
                y: 19
                width: 624
                height: 43
                placeholderText: qsTr("Enter IP address")

            }

            TextField {
                id: textFieldPort
                objectName: "textFieldPort"

                x: 8
                y: 63
                width: 624
                height: 43
                placeholderText: qsTr("Enter port")
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Управление")
        }
        TabButton {
            text: qsTr("Настройки")
        }
    }

    //Popups:
    Popup {
        id: popup_stream

        property int index;
        property int streamId;
        property string name;
        property string url;
        property int state;

        height: applicationWindow.height
        modal: true;

        //ColumnLayout {
        Column {
                anchors.fill: parent
                spacing: 8

                //Заголовок
                Label {
                    font.pixelSize: 22
                    text: "Редактирование потока"
                }

                //Название
                Label {
                    text: "Название потока"
                }

                TextField {
                    id: popup_stream_name;
                    text: popup_stream.name
                    placeholderText: qsTr("Stream name                          ")
                }

                //Адрес
                Label {
                    text: "Адрес потока"
                }
                TextField {
                    id: popup_stream_url;
                    text: popup_stream.url
                    placeholderText: qsTr("Stream url address or file patch")
                }


                Row{
                    anchors.right: parent.right
                    spacing: 10

                    Button {
                        text: qsTr("Save")

                        onClicked:
                        {
                            if(popup_stream_name.text != "" && popup_stream_url.text != "")
                            {
                                updateStreamSignal(popup.streamId, popup_stream_name.text, popup_stream_url.text, popup.state);
                                popup_stream.close();
                            }
                            else
                            {
                                if(popup_stream_name.text == "")
                                    popup_stream_name.forceActiveFocus();
                                else if(popup_stream_url.text == "")
                                    popup_stream_url.forceActiveFocus();
                            }
                        }
                    }
                    Button {
                        text: qsTr("Delete")

                        onClicked:
                        {
                            deleteStreamSignal(popup_stream.streamId);
                            popup_stream.close();
                        }
                    }
                    Button {
                        text: qsTr("Close")

                        onClicked:
                        {
                            popup_stream.close()
                        }
                    }
                }
            }
    }

    Popup {
        id: popup_addStream
        modal: true;

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        Column
        {
            Row {
                spacing: 10

                TextField{
                    id: popup_addStream_name;
                    placeholderText: qsTr("Stream name")
                }
                TextField{
                    id: popup_addStream_url;
                    placeholderText: qsTr("Stream url address or file patch")
                }
            }
            Button {
                anchors.right: parent.right
                text: "Add"

                onClicked: {
                    if(popup_addStream_name.text != "" && popup_addStream_url.text != "")
                    {
                        addStreamSignal(popup_addStream_name.text, popup_addStream_url.text);
                        popup_addStream_name.text = "";
                        popup_addStream_url.text = "";
                        popup_addStream.close();
                    }
                    else
                    {
                        if(popup_addStream_name.text == "")
                            popup_addStream_name.forceActiveFocus();
                        else if(popup_addStream_url.text == "")
                            popup_addStream_url.forceActiveFocus();
                    }
                }
            }
        }
    }
}
