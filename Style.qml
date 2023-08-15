pragma Singleton
import QtQuick 2.0

QtObject {
    property color mainBGcolor: "darkSlateGrey"
    property real headersPointSize: 10  // size of text in HEADERS
    property real dataPointSize: headersPointSize +3 // size of text in DATA
    property real summaryPointSize: 12     // size of text in DATA

    property int zDefault: 0        // properties for z layers
    property int zGlobalMouse: 1
    property var monthColors: [ "#08979c", "#006d75", // winter
                "#a0d911", "#7cb305","#5b8c00",  // spring
                "#fadb14", "#d4b106", "#ad8b00",   // summer
                "#faad14", "#d48806", "#ad6800",   // fall
                "#13c2c2" ]          // winter
}
