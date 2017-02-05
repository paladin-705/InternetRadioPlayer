import QtQuick 2.7

Page1Form {
    id: formController

    sliderVolume.onValueChanged: {
        changeVolumeSignal(sliderVolume.value);
    }

    switchMode.onCheckedChanged: {
        changeConnectStateSignal(switchMode.checked);
    }

    buttonPrev.onClicked: {
        buttonSignal("prev");
    }

    buttonPlay.onClicked: {
        buttonSignal("play");
    }

    buttonNext.onClicked: {
        buttonSignal("next");
    }

    listView {
        delegate: Rectangle {
            id: delegateItem
            width: parent.width
            height: 40
            border.color: "#EEEEEE"
            border.width: 1
            radius: 2

            property string name_: name
            property string url_: url
            property int streamId_: streamId
            property int state_: state

            Row {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4

                Text {
                    text: name_
                }
            }

            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {

                    if (mouse.button == Qt.RightButton)
                    {
                        console.log(index)
                        popup_stream.open();

                        popup_stream.streamId = streamId_;
                        popup_stream.index = index;
                        popup_stream.name = name_;
                        popup_stream.url = url_;
                        popup_stream.state = state_;
                    }
                    else
                    {
                        listView.currentIndex = index
                    }

                }
            }
        }
        highlight: Rectangle {
            width: listView.width
            height: 40
            border.color: "#000000"
            border.width: 1
            radius: 2
        }
        focus: true
        onCurrentItemChanged:
        {
            changeStreamSignal(listView.currentItem.streamId_);
        }
    }
}
