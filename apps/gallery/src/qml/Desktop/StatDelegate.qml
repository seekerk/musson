import QtQuick 1.1
import QtDesktop 0.1
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants
import "stat.js" as Stat

Rectangle {
    id: statDelegate
    height: infoColumn.height + UI.DEFAULT_MARGIN
    width: statDelegate.ListView.view.width
    color: "transparent"

    signal delegateClicked()

    ListHighlight {
        id: highlight
        visible: false
    }

    Column {
        id: infoColumn
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        anchors.topMargin: 8
        anchors.bottomMargin: 12
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 1

        Text {
            id: titleText
            text: accountId
            font.pixelSize: UI.FONT_DEFAULT
            font.weight: Font.Bold
            wrapMode: Text.Wrap
            width: parent.width
            color: !theme.inverted ? UI.COLOR_FOREGROUND : UI.COLOR_INVERTED_FOREGROUND
        }

        Text {
            text: qsTr("Requests: ") + requests + " (" + totalRequests + ")"
            color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
            font.weight: Font.Light
            width: parent.width
            wrapMode: Text.Wrap
            font.pixelSize: UI.FONT_XSMALL
        }

        Text {
            text: qsTr("Download: ") + Stat.formatSize(download) + " (" + Stat.formatSize(totalDownload) + ")"
            color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
        }

        Text {
            text: qsTr("Upload: ") + Stat.formatSize(upload) + " (" + Stat.formatSize(totalUpload) + ")"
            color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
        }

        Text {
            text: qsTr("Errors: ") + errors + " (" + totalErrors + ")"
            color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
            font.weight: Font.Light
            font.pixelSize: UI.FONT_XSMALL
        }
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        onClicked: delegateClicked()
    }
}
