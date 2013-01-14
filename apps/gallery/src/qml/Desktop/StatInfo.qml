import QtQuick 1.0
import QtDesktop 0.1
import MussonComponents 1.0
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants
import "stat.js" as Stat

Window {
    id: statInfo

    StatManager {
        id: statManager
        sm: global_sm
    }

    onVisibleChanged: {
        if (visible) {
            statManager.updateStats()
        }
    }

    Component.onCompleted: {
        statManager.updateStats()
    }

    data: Flickable {
        anchors.fill: parent

        flickableDirection: Flickable.VerticalFlick

        Rectangle {
            anchors.fill: parent
            color: "transparent"

            Component {
                id: commonStats

                Item {
                    height: infoColumn.height + UI.DEFAULT_MARGIN
                    width: parent.width

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
                            text: qsTr("Common statistics")
                            font.pixelSize: UI.FONT_DEFAULT
                            font.weight: Font.Bold
                            width: parent.width
                            //color: !theme.inverted ? UI.COLOR_FOREGROUND : UI.COLOR_INVERTED_FOREGROUND
                        }

                        Text {
                            text: qsTr("Requests: ") + statManager.requests + " (" + statManager.totalRequests + ")"
                            //color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
                            font.weight: Font.Light
                            width: parent.width
                            wrapMode: Text.Wrap
                            font.pixelSize: UI.FONT_XSMALL
                        }

                        Text {
                            text: qsTr("Download: ") + Stat.formatSize(statManager.download) + " (" + Stat.formatSize(statManager.totalDownload) + ")"
                            //color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
                            font.weight: Font.Light
                            font.pixelSize: UI.FONT_XSMALL
                        }

                        Text {
                            text: qsTr("Upload: ") + Stat.formatSize(statManager.upload) + " (" + Stat.formatSize(statManager.totalUpload) + ")"
                            //color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
                            font.weight: Font.Light
                            font.pixelSize: UI.FONT_XSMALL
                        }

                        Text {
                            text: qsTr("Errors: ") + statManager.errors + " (" + statManager.totalErrors + ")"
                            //color: (theme.inverted ? ExtrasConstants.LIST_SUBTITLE_COLOR_INVERTED : ExtrasConstants.LIST_SUBTITLE_COLOR)
                            font.weight: Font.Light
                            font.pixelSize: UI.FONT_XSMALL
                        }
                    }

//                    HorizontalDivider {
//                        anchors.top: infoColumn.bottom
//                        anchors.topMargin: 6
//                        margins: 10
//                    }
                }
            }

            ListView {
                id: statsView
                interactive: visibleArea.heightRatio < 1
                anchors.fill: parent
                model: statManager.model
                header: commonStats

                delegate: StatDelegate {
                    id: statDelegate
                }
            }

            EmptyState {
                visible: (statsView.count == 0)
                text: qsTr("No accounts")
                anchors.fill: parent
            }

//            ScrollDecorator {
//                id: scrollDecorator
//                flickableItem: statsView
//            }
        }
    }

}
