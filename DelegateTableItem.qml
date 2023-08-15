import QtQuick 2.12
import Style 1.0

Rectangle {
    id: root
    property alias text: delegateText      // DEFINE THIS

    property alias font: delegateText.font           // FOR EXTERNAL USE


    implicitWidth: TableView.view.columnWidthProvider(column)
    implicitHeight: TableView.view.rowHeightProvider(row)
    color: "grey"

    Text {
        id: delegateText
        anchors.fill: parent
        anchors.margins: parent.TableView.view.columnSpacing
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "black"

        wrapMode: Text.WordWrap
    }
}

