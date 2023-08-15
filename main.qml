import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import Common 1.0
import Style 1.0

ApplicationWindow {
    id: mainWnd
    visible: false  // load settings first
    width: 640;     height: 480
    minimumWidth: 450;  minimumHeight: 300 +30 // fixme
    title: "Utility bills v.3.0.0"
    color: "darkSlateGrey"
    Component.onCompleted: {  // restore last window position
        var geometrySettings = Common.loadWindowState()

        width = geometrySettings[2]
        height = geometrySettings[3]
        x = geometrySettings[0]
        y = geometrySettings[1]

        visibility = geometrySettings[4]  // apply loaded window state
        width -= 15  // improveme  :i didn't find another way to update "tvData"
        width += 15
    }
    onClosing: {
        var prevVisibility = visibility

        opacity = 0  // force hiding window!
        visibility = ApplicationWindow.Windowed

        Common.saveWindowState(x, y, width, height, prevVisibility)
    }
    onVisibilityChanged: {
        if(visibility === ApplicationWindow.Windowed) {
            width -= 10  // improveme  :i didn't find another way to update "tvData"
            width += 10
        }
    }
//    onActiveFocusItemChanged: console.log(activeFocusItem)

    Row {
        property bool renaming: false

        id: rowAddresses
        anchors.top: parent.top         // GEOMETRY
        anchors.left: parent.left
        width: parent.width -anchors.margins *2
        height: parent.height *0.07
        anchors.margins: height *0.4

        spacing: anchors.margins

        ComboBox {
            id: cmboxAddresses
            width: parent.width *0.6    // G
            height: parent.height

            textRole: "address"
            model: CmboxModel
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton
                onPressed: {
                    popupAddres.model = CmboxModel
                    var obj = mapToItem(null, mouseX, mouseY) // glob.pos
                    popupAddres.openXY(mouseX, mouseY, obj, mainWnd)
                }
            }
            onCurrentTextChanged: {
                Common.refreshCurrentAddress(currentText)
                if(!rowAddresses.renaming)
                    BillTypesModel.loadBillTypes()
                rowAddresses.renaming = false
            }
        }

        Button {   // btn Add Address
            width: cmboxAddresses.width *0.3
            height: cmboxAddresses.height
            text: "New"
            onClicked: {
                dialogAddress.currentText = "New address"
                dialogAddress.model = CmboxModel
                dialogAddress.open()
            }
        }

          //// notVisible
        DialogNameEdit {
            id: dialogAddress

            onAccepted: {
                Common.refreshCurrentAddress(currentText)
                Common.createAddress(currentText)
                cmboxAddresses.currentIndex = cmboxAddresses.count -1
            }
        }
        DialogNameEdit {
            id: dialogRenameAddress

            onAccepted: {
                rowAddresses.renaming = true
                CmboxModel.rename(oldText, currentText)
            }
        }
        DialogAreYouSure {
            id: dialAreYouSure
            onAccepted: { cmboxAddresses.currentIndex = CmboxModel
            .deleteAt(cmboxAddresses.currentIndex) }
        }
        PopupEditDelete {
            id: popupAddres

            onDeleteClicked: dialAreYouSure.open()
            onEditClicked: {
                dialogRenameAddress.model = CmboxModel
                dialogRenameAddress.currentText = cmboxAddresses.currentText
                dialogRenameAddress.open()
            }
        }
    }
    Image {
        id: btnOpenSummary
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: rowAddresses.anchors.margins

        width: rowAddresses.width *0.05
        height: width

        source: "qrc:/totalCost.png"
        mipmap: true
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            ToolTip.text: "Open \"Summary\" window with added months"
            ToolTip.visible: containsMouse
            onClicked: pageSummary.visible = true
            cursorShape: pressed ? Qt.ClosedHandCursor : Qt.PointingHandCursor
        }
    }

    ///////////////

    Button {
        id: btnAddBillType
        anchors.verticalCenter: tbBillTypes.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: tbBillTypes.anchors.margins
        width:  tbBillTypes.height *0.5
        height: width

        text: "+"
        font.pixelSize: height *0.5
        onClicked: {
            dialogAddBill.currentText = "New Utility Bill"
            dialogAddBill.model = BillTypesModel
            dialogAddBill.open()
        }
    }
    TabBar {
        id: tbBillTypes
        anchors.top: rowAddresses.bottom
        anchors.left: btnAddBillType.right
        anchors.right: parent.right

        anchors.margins: parent.height *0.05
        anchors.leftMargin: 0

        clip: true
        background: Rectangle { color: "transparent" }

        onCurrentIndexChanged: {
            Common.refreshCurrentBillType(currentIndex)
            Model.loadData(cmboxAddresses.currentText,
                               BillTypesModel.billTypeAt(currentIndex))
            tvData.contentY = 0
        }

        Repeater {
            model: BillTypesModel

            TabButton {
                id: btnBillType     // id changes
                width: implicitWidth
                height: mainWnd.height *0.08

                text: model.billType
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onPressed: {
                        dialAreYouSureBT.deletingItemName = btnBillType.text
                        popupBillType.model = BillTypesModel
                        var obj = mapToItem(null, mouseX, mouseY) // glob.pos
                        popupBillType.openXY(mouseX, mouseY, obj, mainWnd)
                    }
                }
                PopupEditDelete {
                    id: popupBillType

                    onDeleteClicked: dialAreYouSureBT.open()
                    onEditClicked: {
                        dialogRenameBill.model = BillTypesModel
                        dialogRenameBill.currentText = btnBillType.text
                        dialogRenameBill.open()
                    }
                }
            }
        }
        DialogNameEdit {
            id: dialogAddBill

            onAccepted: {
                Common.refreshCurrentBillType(currentText)
                BillTypesModel.addNew(currentText)
            }
        }
        DialogNameEdit {
            id: dialogRenameBill

            onAccepted: BillTypesModel.rename(oldText, currentText)
        }
        DialogAreYouSure {
            id: dialAreYouSureBT
            onAccepted: BillTypesModel.deleteBT(deletingItemName)
        }
    }
///////////////
                // on mainWnd change refresh TV
    onWidthChanged: { tvData.forceLayout(); tvHeaders.forceLayout() }
    onHeightChanged: { tvData.forceLayout(); tvHeaders.forceLayout() }
    Row {
        id: tvHeaders
        anchors.top: tbBillTypes.bottom
        x: tvData.x
        width: mainWnd.width
        height: mainWnd.height *0.1

        spacing: tvData.rowSpacing

        Repeater {
            id: headersRepeater
            model: cboxIsPayment.checked ? 2 : 6
            anchors.fill: parent

            delegate: Rectangle {
                width: tvData.columnWidthProvider(index)
                height: parent.height
                color: "teal"

                TextStretchable {
                    id: delegateText
                    anchors.fill: parent
                    anchors.margins: tvData.columnSpacing
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap

                    text: Model.headerData(index, Qt.Horizontal)
                    font.bold: true
                    font.pointSize: Style.headersPointSize
                }
            }
        }
    }
    Button { // btn Add new Month
        id: btnAddMonth
        anchors.left: tvHeaders.left
        anchors.bottom: tvHeaders.bottom
        width: tvHeaders.height *0.3
        height: width
        anchors.margins: width /4

        font.bold: true
        onClicked: Model.addNew()

        TextStretchable {   // calculate size of the text
            anchors.fill: parent
            text: "+"
            horizontalAlignment: Text.AlignHCenter
        }
    }
    TextStretchable {  // for calculating HEADERS font size
        id: tempText
        width: tvData.columnWidthProvider(1)
        height: tvHeaders.height - tvData.rowSpacing
        wrapMode: Text.WordWrap
        text: "Beginning of the month"
        font.bold: true
        visible: false
        onFontInfoChanged: { Style.headersPointSize = fontInfo.pointSize }
    }

    Row {
        id: rowUndoRedoStuff
        anchors.top: tvHeaders.bottom
        anchors.margins: tvHeaders.height *0.2
        anchors.left: tvHeaders.left
        width: mainWnd.width - x
        height: mainWnd.height *0.04

        spacing: btnUndo.height /2

        ImageButton {
            id: btnUndo
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            height: parent.height
            source: enabled ? "qrc:/res/img/undo.png"
                            : "qrc:/res/img/undoDisabled.png"
            toolTipText: "Undo"
        }
        ImageButton {
            id: btnRedo
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height
            height: parent.height
            source: enabled ? "qrc:/res/img/redo.png"
                            : "qrc:/res/img/redoDisabled.png"
            toolTipText: "Redo"
        }

        CheckBox {
            id: cboxIsPayment
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height

            indicator.height: parent.height *0.7
            indicator.width: indicator.height

            text: "<font color=\"white\">Payment mode</font>" // fixme

            MouseArea {
                id: cboxIsPaymentMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor  // todo make your own cbox
                ToolTip.text: { return "Use it when you want to show payments."
                  + "\nIt will be displayed differently in the \"Summary\" window."
                }
                ToolTip.visible: containsMouse

                onClicked: dialAreYouSureIsPayment.visible = true
           }
           DialogAreYouSure {
               id: dialAreYouSureIsPayment
               textWarning: {
                   return "All data in current bill type will be LOST"
                   + "\nProceed?"
               }
               onYes: {
                   cboxIsPayment.checked = !cboxIsPayment.checked
                   console.log("approved")
               }
           }

        }
    }

    TableView {
        id: tvData
        anchors.top: rowUndoRedoStuff.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Math.min (parent.width, parent.height) *0.02

        clip: true
        reuseItems: false
        columnSpacing: 3
        rowSpacing: 3

        ScrollBar.vertical: ScrollBar { //  SCROLLBAR HANDLING
            id: sbar
            anchors.right: parent.right
            width: parent.width *0.02
            policy: parent.height < parent.contentHeight ? ScrollBar.AlwaysOn
                                                 : ScrollBar.AlwaysOff
            stepSize: 0.02
        }

        property real tvWidth: tvData.width -(tvData.columns -1)
                               *tvData.columnSpacing
        columnWidthProvider: function (column) {
            switch (column) {
            case 0:  // month
                return tvWidth *0.325
            case 1:  // beginning
            case 2:  // end
                return tvWidth *0.125
            case 3:  // price
                return tvWidth *0.175
            case 4:  // total units
                return tvWidth *0.1
            case 5:  // total money
                return tvWidth *0.15
            }
        }
        rowHeightProvider: function(row) {
            return tvData.height /10
        }

        model: Model
        delegate: DelegateTableItem {
            id: delegateData
            property color col: model.decorationRole
            color:  {
                if(cboxIsPayment.checked)
                    return "gainsBoro"

                switch(column) {
                   case 0:
                       return col
                   case 1:
                   case 2:
                   case 3:
                       return Qt.hsla(col.hslHue, col.hslSaturation -0.2,
                                      col.hslLightness, 1)
                   case 4:
                   case 5:
                       return Qt.hsla(col.hslHue, col.hslSaturation +0.2,
                                      col.hslLightness, 1)
                   }
            }

            text.text: display
            text.horizontalAlignment: !column ? Text.AlignLeft
                                              : Text.AlignHCenter
            font.pointSize: Style.dataPointSize

            MouseArea {
                id: maDelegate
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: true

                onEntered: {            // additional btns for "price"
                  if(column === 3) {
                    maDelegate.children = btnDetailPrice.mouseArea
                    btnDetailPrice.show(delegateData, row)
                    btnDetailPrice.anchors.bottom = delegateData.bottom
                    dialogDetPrice.row = row

                    ToolTip.delay = 500;
                    if(Model.hasDetailedPrice(row))
                        ToolTip.show(Model.toolTipFor(row, 300))

                    if(parent.text.text === '-'       // 259 = PRICE
                    && row +1 < Model.rowCount()
                    && Model.data(Model.index(row +1, column), 259) !== '-'){
                        maDelegate.children = [btnRepeatPrice.mouseArea,
                                               btnDetailPrice.mouseArea]
                        btnRepeatPrice.show(delegateData, row)
                    }
                  }
                }
                onExited: {
                    if(column !== 3)
                        return

                    ToolTip.hide()
                    btnDetailPrice.visible = false  // hide price btns
                    btnRepeatPrice.visible = false
                }

                onPressed: {
                    if(pressedButtons !== Qt.RightButton)
                        return

                    textField.visible = false
                    popupMonthDel.parent = parent
                    popupMonthDel.deletingIndexRow = row
                    var obj = mapToItem(null, mouseX, mouseY) // glob.pos
                    popupMonthDel.openXY(mouseX, mouseY, obj, mainWnd)

                    dialAreYouSureModel.deletingItemRow = row
                }
                onDoubleClicked: {
                    if(column === 0) {
                        monthPicker.setDate(Common.toMonthYear(parent.text.text))
                        monthPicker.openXY(mouseX, mouseY, row)
                        return
                    }
                    if(column === 1 || column === 2)
                        textField.validator = textField.intValidator
                    if(column === 3)
                        textField.validator = textField.floatValidator
                    if(column > 3)
                        return

                    textField.parent = parent
                    textField.text = parent.text.text
                    textField.clickedRow = row; textField.clickedColumn = column
                    textField.visible = true
                }
            }
        }

        ButtonSmall {
            id: btnRepeatPrice
            onPressed: Model.repeatPrevPrice(row)
            ToolTip.text: "Copy price from month below"
        }
        ButtonSmall {
            id: btnDetailPrice
            text: '*'
            onPressed: dialogDetPrice.open()
            ToolTip.text: "Set advanced pricing"
        }

        MonthPicker { id: monthPicker }
        DialogDetailedPrice { id: dialogDetPrice }
        TextFieldDelegate { id: textField }
        PopupEditDelete {
            id: popupMonthDel
            btnEditWidth: 120
            btnEditText: "Add to summary"
            btnDeleteEnabled: true
            onDeleteClicked: dialAreYouSureModel.open()
            onEditClicked: ModelSummary.add(deletingIndexRow, // it's ADDiNG!
                             cboxIsPayment.checked)
        }
        DialogAreYouSure {
            id: dialAreYouSureModel
            onAccepted: Model.deleteAt(deletingItemRow)
        }
    }

    MouseArea {
        id: maGlobal
        anchors.fill: parent
        visible: (textField.visible && !textField.hovered)
                // || dialogCalendar.visible)
        z: Style.zGlobalMouse
        onPressed: {
            textField.accepted = true
            textField.visible = false
        }
    }

    PageTotal {
        id: pageSummary
        anchors.fill: parent

        Row {
            id: tvSummaryHeader
            anchors.top: pageSummary.textBottomAnchor
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height *0.07
            anchors.margins: pageSummary.width *0.05
            anchors.topMargin: pageSummary.width *0.02

            spacing: tvSummary.rowSpacing

            Repeater {
                model: 5
                anchors.fill: parent

                Rectangle {
                    width: tvSummary.columnWidthProvider(index)
                    height: parent.height
                    color: "gray"
                    border.width: 2
                    border.color: "dimGray"

                    TextStretchable {
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
//                        elide: Text.ElideRight // dint?
                        font.pointSize: Style.summaryPointSize
                        color: "white"
                        text: ModelSummary.headerData(index, Qt.Horizontal)
                    }
                }
            }
        }

        TableView {
            id: tvSummary
            anchors.top: tvSummaryHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: totalCostItem.top

            anchors.leftMargin: pageSummary.width *0.05
            anchors.rightMargin: anchors.leftMargin

            clip: true
            reuseItems: false
            columnSpacing: 3
            rowSpacing: 3

            ScrollBar.vertical: ScrollBar { //  SCROLLBAR HANDLING
                id: sbarSummary
                anchors.right: parent.right
                width: parent.width *0.02
//                policy: parent.height < parent.contentHeight
//                                                     ? ScrollBar.AlwaysOn
//                                                     : ScrollBar.AlwaysOff
//                stepSize: 0.02
            }

            property real tvWidth: tvSummary.width -(tvSummary.columns -1)
                                   *tvSummary.columnSpacing
            columnWidthProvider: function (column) {
                switch (column) {
                case 0:  // billType
                    return tvWidth *0.2
                case 1:  // month
                    return tvWidth *0.3
                case 2:  // price
                    return tvWidth *0.2
                case 3:  // used units
                    return tvWidth *0.1
                case 4:  // total cost
                    return tvWidth *0.2
                }
            }
            rowHeightProvider: function(row) {
                return tvSummary.height /10
            }
            onWidthChanged: tvSummary.forceLayout()
            onHeightChanged: tvSummary.forceLayout()

            model: ModelSummary
            delegate: Rectangle {
                width: TableView.view.columnWidthProvider(column)
                height: TableView.view.rowHeightProvider(row)
                property color col: model.decoration
                color: switch(column) {
                       case 0:
                           return col
                       case 1:
                       case 2:
                           return Qt.hsla(col.hslHue, col.hslSaturation -0.2,
                                          col.hslLightness, 1)
                       case 3:
                       case 4:
                           return Qt.hsla(col.hslHue, col.hslSaturation +0.2,
                                          col.hslLightness, 1)
                       }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onPressed: {
                        var obj = mapToItem(null, mouseX, mouseY) // glob.pos
                        popupSummary.deletingIndexRow = row
                        popupSummary.parent = parent
                        popupSummary.openXY(mouseX, mouseY, obj, pageSummary)
                    }
                }

                TextStretchable {
                    anchors.fill: parent
                    text: model.display
                    horizontalAlignment: column === 1 ? Text.AlignLeft
                                                      : Text.AlignHCenter

                    font.pointSize: Style.summaryPointSize
                    color: "black"
                }
            }
            PopupEditDelete {
                id: popupSummary
                btnEditVisible: false
                btnDeleteEnabled: true
                onDeleteClicked: ModelSummary.deleteAt(deletingIndexRow)
            }
        }

        TotalCostItem {
            id: totalCostItem

            width: parent.width *0.5
            height: parent.height *0.2
            anchors.bottom: parent.bottom
            anchors.right: tvSummary.right

            line.width: tvSummary.columnWidthProvider(4)
            line.y: height *0.1

            textCost.text: ModelSummary.totalCost
        }

        Image {
            id: btnSaveAs
            width: parent.height *0.05
            height: width
            anchors.left: tvSummary.left
            anchors.top: tvSummary.bottom
            anchors.topMargin: tvSummary.anchors.leftMargin

            source: "qrc:/save.png"
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: saveDialog.open()
            }
        }
        Image {
            id: btnClear
            width: parent.height *0.05
            height: width
            anchors.left: btnSaveAs.right
            anchors.leftMargin: width *0.5
            anchors.top: btnSaveAs.top

            source: "qrc:/clearIcon.png"
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: ModelSummary.clearModel()
            }
        }

        FileDialog {
            id: saveDialog
            folder: shortcuts.desktop
            defaultSuffix: "png"

            selectExisting: false
            selectMultiple: false
            nameFilters: [ "Image files (*.jpg *.png)" ]

            onAccepted: {
                var path = saveDialog.fileUrl.toString().substr(8)

                pageSummary.grabToImage(function(result) {
                    result.saveToFile(path) })
            }
        }
    }
}
