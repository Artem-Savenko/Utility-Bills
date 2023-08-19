import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    property var model      // DEFINE THIS

    property alias btnDeleteEnabled: btnDelete.enabled   // FOR USE
    property alias btnEditText: btnEdit.text
    property alias btnEditWidth: btnEdit.width
    property bool btnEditVisible: true
    property int deletingIndexRow: -1
    signal editClicked
    signal deleteClicked

    id: root

    function openXY(x, y, globPos, appWind) {
        if(globPos.x +width >= appWind.width)
            x -= width
        if(globPos.y +height >= appWind.height)
            y -= height

        root.x = x
        root.y = y
        root.open()
    }

    contentItem:     Column {
        spacing: parent.height *0.1

        Button {
            id: btnEdit
            width: 80
            height: 25
            visible: btnEditVisible

            text: "Rename"
            onClicked: {
                root.editClicked()
                close()
            }
        }

        Button {
            id: btnDelete
            width: btnEdit.width
            height: btnEdit.height
            text: "Delete"
            enabled: (model && model.rowCount()) >= 2 ? true : false
            onClicked: {
                root.deleteClicked()
                close()
            }
        }
    }
}
