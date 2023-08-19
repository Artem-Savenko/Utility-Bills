import QtQuick 2.12
import QtQuick.Controls 2.5
import Style 1.0

Page {
    id: root

    property alias textBottomAnchor: labelSummary.bottom


    visible: false

    Rectangle{ anchors.fill: parent; color:  Qt.darker(Style.mainBGcolor, 1.3) }

    TextStretchable {
        id: labelSummary
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: parent.width *0.05
        width: parent.width *0.5
        height: parent.height *0.1

        text: qsTr("Summary:")
        font.family: "Century Regular"
        color: "White"
    }

    Image {
        id: btnClose
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: labelSummary.anchors.margins
        width: parent.width *0.05
        height: width

        source: "qrc:/img/closeIcon.png"
        mipmap: true

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: root.visible = false
        }
    }
}
