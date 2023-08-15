import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.4

Dialog {
    id: root

    property int row: -1    // DEFINE    


    onVisibleChanged: {     // onOpen
        if(visible) {
            tfieldFirstUnits.forceActiveFocus()
            setDetailedPrice(Model.getDetailedPriceInfo(row))
            checkFields()
        }
    }
    function setDetailedPrice(list) {// units, percentage, priceFirst, priceRest
        if(list.length < 4)
            return

        if(list[0]) {
            typeFirst.text = "units"
            tfieldFirstUnits.text = list[0]
        }
        else {
            typeFirst.text = "%"
            tfieldFirstUnits.text = list[1]
        }

        tfieldFirstPrice.text = list[2]
        tfieldRest.text = list[3]
    }

    width: 260
    title: "Setting price details"
    modality: Qt.ApplicationModal
    standardButtons: textWarning.visible ? StandardButton.Cancel
                      : StandardButton.Cancel | StandardButton.Ok
    onActionChosen: {
        if(textWarning.visible && action.button === StandardButton.Ok)
            action.accepted = false
    }

    RegExpValidator { id: intValidator; regExp : /\d{0,6}/ }
    RegExpValidator { id: procValidator; regExp : /\d{0,2}/ }
    RegExpValidator { id: floatValidator; regExp : /\d{1,4}[\.,]\d{0,2}/ }
    function checkFields() {
        if(!root.visible)
            return

        if(tfieldFirstUnits.text <= 0) {
            textWarning.text = "\"First n units(percentages)\"  field must be > 0"
            textWarning.visible = true
        }
        else if(!tfieldFirstPrice.text || !tfieldFirstUnits.text
             || !tfieldRest.text ) {
            textWarning.text = "All fields must be set!"
            textWarning.visible = true
        }
        else if(tfieldFirstPrice.text == 0 && tfieldRest.text == 0) {
            textWarning.text = "Both prices cannot be 0"
            textWarning.visible = true
        }
        else
            textWarning.visible = false
    }

    onAccepted: Model.setDataAt(row, 3, [tfieldFirstUnits.text, typeFirst.text,
                              tfieldFirstPrice.text, tfieldRest.text])

    Grid {
        id: grid
        columns: 2
        spacing: 5
        verticalItemAlignment: Grid.AlignVCenter

/*row1*/Row {
            id: cell00
            width: 130

            Text {
                id: labelFirst
                width: cell00.width *0.2
                height: tfieldFirstPrice.height
                text: "First"
                verticalAlignment: Text.AlignVCenter
            }
            TextField {
                id: tfieldFirstUnits
                width: cell00.width *0.3
                text: "50"
                onTextChanged: checkFields()

                validator: typeFirst.text === 'units' ? intValidator
                                                      : procValidator
            }
            Button {
                id: typeFirst
                width: cell00.width *0.5
                text: "%"
                onClicked: { if(text === "%") text = "units"
                        else text = "%" }

                onTextChanged: {
                    if(text === "%" && tfieldFirstUnits.text > 100)
                        tfieldFirstUnits.text = '50'
                }

            }
        }
        Text {
            width: cell00.width
            text: "Rest"
        }

/*row2*/TextField {
            id: tfieldFirstPrice
            width: cell00.width
            text: "0.00"
            onTextChanged: checkFields()
            validator: floatValidator
        }
        TextField {
            id: tfieldRest
            width: cell00.width
            text: "0.00"
            onTextChanged: checkFields()
            validator: floatValidator
        }
    }


    Text {
        id: textWarning
        anchors.top: grid.bottom
        anchors.topMargin: 10
        text: "Something bad!"
        color: "crimson"

        visible: false
        verticalAlignment: Text.AlignVCenter
    }
}
