import QtQuick 2.0

Rectangle {
    id: root
       // DEFINE WIDTH + HEIGHT

    property alias line: line
    property alias textCost: textCost

    color: "transparent"

    Rectangle {
        id: line
        anchors.right: parent.right
        width: parent.width *0.4
        height: parent.height *0.05
        radius: 3

        color: "white"
    }

    TextStretchable {
        id: text
        width: parent.width *0.6
        height: parent.height *0.9

        color: "white"
        text: "Total cost: "
    }

    TextStretchable {
        id: textCost
        anchors.right: parent.right
        width: parent.width *0.4
        height: text.height
        color: "white"

        font.pointSize: text.fontInfo.pointSize
        horizontalAlignment: Text.AlignHCenter
        text: "0.00"
    }
}
