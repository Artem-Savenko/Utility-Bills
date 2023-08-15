import QtQuick 2.12
import QtQuick.Controls 2.5
import Style 1.0

FocusScope {
    property int clickedRow: -1     // MUST DEFINE
    property int clickedColumn: -1

    property alias text: textField.text
    property alias validator: textField.validator
    property RegExpValidator intValidator: intValidator
    property RegExpValidator floatValidator: floatValidator
    property alias hovered: textField.hovered

    property bool accepted: false        // INTERNAL USAGE
    property string oldText: ""

    id: root
    visible: false
    anchors.fill: parent

    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Return:
            accepted = true
            visible = false
        break;

        case Qt.Key_Escape:
            accepted = false
            visible = false
        break;
        }
    }

    onVisibleChanged: {
        if(visible) {       // "onOpen"
            textField.forceActiveFocus()
            textField.selectAll()
            oldText = textField.text
            if(textField.text === '-')
                textField.text = ''
        }
        else if(accepted && oldText !== textField.text ) {   // "onAccepted"
            Model.setDataAt(clickedRow, clickedColumn, textField.text)
        }
    }

    TextField {
        id: textField

        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        topPadding: 0
        bottomPadding: 0

        selectByMouse: true

        font.pointSize: Style.dataPointSize -1
        hoverEnabled: true
    }

    RegExpValidator { id: intValidator; regExp : /\d{0,8}/ }
    RegExpValidator { id: floatValidator; regExp : /\d{1,4}[\.,]\d{0,2}/ }
}
