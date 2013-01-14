import QtQuick 1.1
import QtDesktop 0.1
import "UIConstants.js" as UI

Item {
    id: accountItem
    height: accountName.height + nameText.height + UI.DEFAULT_MARGIN * 2
    width: parent.width

    property alias name: accountName.text
    signal nameChanged(string name)

    onVisibleChanged: {
        if (visible) {
            accountName.text = ""
        }
    }

    Text {
        id: nameText
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: UI.DEFAULT_MARGIN
        anchors.leftMargin: UI.DEFAULT_MARGIN
        text: qsTr("Account name (optional)")
        font.weight: Font.Light
        font.pixelSize: UI.FONT_LSMALL
        smooth: true
    }

    TextField {
        id: accountName;
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: nameText.bottom
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.rightMargin: UI.DEFAULT_MARGIN
        anchors.bottomMargin: UI.DEFAULT_MARGIN
        smooth: true
        onTextChanged: nameChanged(text)
    }
}
