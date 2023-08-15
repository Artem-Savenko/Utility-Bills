import QtQuick 2.12
import QtQuick.Controls 2.5

Image {
    id: root
    property string toolTipText
    signal clicked


    ToolTip.visible: mouseArea.containsMouse
    ToolTip.text: toolTipText
    ToolTip.delay: 700
    mipmap: true

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        hoverEnabled: true

        onClicked: root.clicked()
    }
}
