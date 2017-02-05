import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.0

Item {
    id: page1

    property alias buttonPrev: buttonPrev
    property alias buttonPlay: buttonPlay
    property alias buttonNext: buttonNext
    property alias switchMode: switchMode
    property alias sliderVolume: sliderVolume

    property alias listView: listView

    RowLayout {
        anchors.horizontalCenterOffset: 155
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 396
        anchors.top: parent.top
    }

    Slider {
        id: sliderVolume
        objectName: "sliderVolume"

        x: 8
        y: 225
        width: 220
        height: 48
        z: 1
        value: 0.5
    }

    Button {
        id: buttonPlay
        objectName: "buttonPlay"

        x: 86
        y: 181
        text: qsTr("Play")
        z: 1
    }

    Button {
        id: buttonNext
        objectName: "buttonNext"

        x: 160
        y: 181
        text: qsTr("Next")
        z: 1
    }

    Button {
        id: buttonPrev
        objectName: "buttonPrev"

        x: 8
        y: 181
        text: qsTr("Prev")
        z: 1
    }

    ProgressBar {
        id: progressBar
        objectName: "progressBar"

        x: 234
        y: 224
        width: 398
        height: 4
        value: 0.5
    }

    Switch {
        id: switchMode
        objectName: "switchMode"

        x: 234
        y: 229
        text: qsTr("Off")
    }

    Text {
        id: labelInfo
        objectName: "labelInfo"

        x: 234
        y: 187
        width: 398
        height: 36
        text: qsTr("Stream info")
        font.pixelSize: 12
    }

    Rectangle {
        id: rectangle
        x: 8
        y: 8
        width: 220
        height: 167
        color: "#ffffff"
    }

    Rectangle {
        id: rectangle1
        x: 234
        y: 8
        width: 398
        height: 169

        ListView {
            id: listView
            clip: true
            anchors.fill: parent
            model: streamModel
        }
    }
}
