import QtQuick 2.12
import QtQuick.Dialogs 1.3

Dialog {
    property string deletingItemName: ""    // FOR USE
    property int deletingItemRow: -1

    property alias textWarning: textWarning.text // for "isPayment" cbox only

    id: root

    modality: Qt.ApplicationModal
    standardButtons: StandardButton.Yes | StandardButton.No

    Text {
        id: textWarning
        text: qsTr("Delete? This cannot be undone.")
        color: "crimson"
    }

    onYes: root.accepted()
}
