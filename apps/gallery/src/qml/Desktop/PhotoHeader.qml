import QtQuick 1.1
import QtDesktop 0.1
import "UIConstants.js" as UI

Rectangle {
    id: photoHeader
    height: 64
    width: parent.width
    color: "#137A94"

    property string title: ""
    property string icon: ""
    property bool hideRefresh: false
    property bool isUpdating: false
    signal refresh()

    ToolButton {
        id: refreshBtn
        width: 48
        height: 48
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.leftMargin: UI.DEFAULT_MARGIN

        iconSource: "qrc:/icons/refresh.png"
        text: qsTr("Refresh list")

        enabled: isUpdating ? false : true;
        visible: !hideRefresh
        onClicked: refresh()
    }

    Item {
        id: albumInfo
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: UI.DEFAULT_MARGIN

        height: 60
        width: 60

            Image {
                id: albumImage
                fillMode: Image.PreserveAspectCrop
                clip: true
                smooth: true
                anchors.fill: parent
                source: !icon ? "qrc:/icons/photoalbum.png" : "file:/" + icon
            }
    }

    Text {
        id: titleText
        anchors.left: albumInfo.right
        anchors.verticalCenter: albumInfo.verticalCenter
        anchors.leftMargin: UI.DEFAULT_MARGIN
        anchors.right: refreshBtn.left
        elide: Text.ElideRight
        text: title
        font.pixelSize: UI.FONT_XLARGE
        font.weight: Font.Light
        color: "white"
        smooth: true
    }

}
