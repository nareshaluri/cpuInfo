import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    width: 800
    height: 600
    visible: true
    objectName: "rootWindow"

    ListView {
        width: parent.width
        height: parent.height
        objectName: "cpuInfoList"
        boundsBehavior : Flickable.StopAtBounds
        delegate: Rectangle {
            height: 30
            width: parent.width
            color: "#350814"
            Text {
                height: 25
                width: parent.width
                color : "white"
                text: modelData
                wrapMode: Text.Wrap
            }
        }
    }
}
