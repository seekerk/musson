import QtQuick 1.0
import "UIConstants.js" as UI

Item {
    property alias text: noContentText.text

    Text {
        id: noContentText
        anchors.centerIn: parent
        font.pixelSize: UI.FONT_XLARGE
        //color: !theme.inverted ? UI.COLOR_FOREGROUND : UI.COLOR_INVERTED_FOREGROUND
        opacity: 0.5
    }
}
