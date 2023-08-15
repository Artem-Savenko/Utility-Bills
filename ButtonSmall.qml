import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    id: root

    function show(parentObj, rowIndex)        // DEFINE
    {
        root.row = rowIndex

        root.parent = parentObj
        root.anchors.right = parentObj.right
        root.height = parentObj.height *0.45
        root.width = height
        mouseArea.x = root.x
        mouseArea.y = root.y

        root.visible = true
    }
    signal pressed              // FOR USE
    property int row: -1
    property alias text: btnText.text
    property alias mouseArea: mouseArea

    visible: false
    color: "lavender"

    TextStretchable {
        id: btnText
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter

        text: "⬍"
        color: "black"
    }
    ToolTip.visible: mouseArea.containsMouse
    ToolTip.delay: 600

    MouseArea {
        id: mouseArea
        anchors.fill: root
        hoverEnabled: true
        enabled: root.visible

        onPressed: root.pressed()
    }
}
