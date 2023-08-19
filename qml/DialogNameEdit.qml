import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.4
import Common 1.0

Dialog {
    property alias currentText: textField.text        // DEFINE THESE
    property var model

    property string oldText   // for 'rejected'      // EXTERNAL USAGE

    property bool nameIsBad: false               // INTERNAL USAGE
    property var warnings: ["Name can not be empty!", "This name already exists!"]

    id: root

    modality: Qt.ApplicationModal
    standardButtons: nameIsBad ? StandardButton.Cancel
                               : (StandardButton.Cancel | StandardButton.Ok)

    onVisibleChanged: {  // "onOpened()"
        if(visible) {
            oldText = root.currentText
            textField.forceActiveFocus()
            textField.checkEnteredName()
        }
    }

    onActionChosen: {
        if(nameIsBad && action.button === StandardButton.Ok)
            action.accepted = false
    }

    Column {
        anchors.left: parent.left
        anchors.right: parent.right

        TextField {
            id: textField
            anchors.left: parent.left
            anchors.right: parent.right

            onTextChanged: checkEnteredName()

            function checkEnteredName () {
                if(Common.delNonPathChars(text) === "") {
                    textWarning.text = warnings[0]
                    nameIsBad = true
                }
                else if(model && model.contains(text)) {
                    textWarning.text = warnings[1]
                    nameIsBad = true
                }
                else
                    nameIsBad = false
            }
        }

        Text {      // text for warning
            id: textWarning
            visible: nameIsBad
            color: "crimson"
        }
    }
}
