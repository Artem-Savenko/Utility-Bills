import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    id: root
    width: 200
    height: 200
    padding: 0


    property int clickedRow: -1 // INTERNAL USAGE

    function apply() {
        Model.setDataAt(root.clickedRow , 0,
                        [cmboxMonth.currentIndex +1, spinYear.value])
        root.visible = false
    }
    function setDate(monthYear) { // MM YYYY
        cmboxMonth.currentIndex = monthYear.x -1
        spinYear.value = monthYear.y
    }
    function openXY(x, y, clickedRow) {
        root.x = x
        root.y = y
        root.clickedRow = clickedRow
        visible = true
        cmboxMonth.forceActiveFocus()
    }


    function onPressed(key) {   // internal
        if(key === Qt.Key_Return)
            root.apply()
    }

    Rectangle {
        anchors.fill: parent
        color: "darkOliveGreen"

        Column {
            anchors.fill: parent
            anchors.leftMargin: parent.width *0.1
            anchors.rightMargin: parent.width *0.1

            Text {
                width: parent.width
                height: parent.height *0.12
                text: qsTr("Year")
                font.pointSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            SpinBox {
                id: spinYear
                width: parent.width
                height: parent.height *0.25
                from: 1
                to: 5000
                value: new Date().getFullYear()

                Keys.onPressed: {//root.onPressed(event.key)
                    if(event.key === Qt.Key_Left) {
//                        down.pressed = true
                        decrease()
                    }

                }

                MouseArea {
                    id: spinYearMouse
                    anchors.fill: parent
                    onWheel: {
                        if(wheel.angleDelta.y > 0) spinYear.increase()
                        else if(wheel.angleDelta.y < 0) spinYear.decrease()
                    }
                    onPressed: mouse.accepted = false
                }
            }
            Item {
                width: 1
                height: parent.height *0.05
            }

            Text {
                width: parent.width
                height: parent.height *0.12
                text: qsTr("Month")
                font.pointSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            ComboBox {
                id: cmboxMonth
                width: parent.width
                height: parent.height *0.25

                popup.height: height *3
                popup.z: root.z +1

                Keys.onPressed: root.onPressed(event.key)

                model: ["01. January", "02. February", "03. March", "04. April",
                   "05. May", "06. June", "07. July", "08. August",
                   "09. September", "10. October", "11. November", "12. December"]
                MouseArea {
                    id: cmboxMonthMouse
                    anchors.fill: parent
                    onWheel: {
                        if(wheel.angleDelta.y > 0) cmboxMonth.incrementCurrentIndex()
                        else if(wheel.angleDelta.y < 0) cmboxMonth.decrementCurrentIndex()
                    }
                    onPressed: mouse.accepted = false
                }
            }

            Item {
                width: 1
                height: parent.height *0.05
            }
            Button {
                id: btnApply
                width: parent.width *0.6
                height: parent.height *0.12
                anchors.horizontalCenter: parent.horizontalCenter

                text: "Ok"
                onClicked: root.apply()
            }
        }
    }
}
