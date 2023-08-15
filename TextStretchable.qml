import QtQuick 2.12

Text {
    id: root

//    wrapMode: Text.Wrap      // choosing one of those is necessary
//    elide: Text.ElideRight

    verticalAlignment: Text.AlignVCenter

    clip: true

    minimumPointSize: 3
    font.pointSize: 333
    fontSizeMode: Text.Fit
}

