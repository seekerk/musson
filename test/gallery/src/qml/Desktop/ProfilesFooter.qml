import QtQuick 1.1
import QtDesktop 0.1
import MussonComponents 1.0
import "UIConstants.js" as UI

Item {
    id: profilesFooter
    height: searchInput.height + UI.DEFAULT_MARGIN
    width: parent.width

    signal nameFilterChanged(string filterName)

    Row {
        id: searchArea
        anchors { top: parent.top; left: parent.left; right: parent.right }
        anchors.margins: UI.DEFAULT_MARGIN
        spacing: 10

        TextField {
            id: searchInput
            placeholderText: qsTr("Search")
            width: parent.width - searchButton.width - 10

            Image {
                id: clearText
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                source: searchInput.text ?
                            'qrc:/icons/clear.png' :
                            'qrc:/icons/search.png'
            }

            MouseArea {
                id: searchInputMouseArea
                anchors.fill: clearText
                onClicked: {
                    searchInput.text = ''
                    nameFilterChanged('')
                }
            }
        }

        Button {
            id: searchButton
            text: qsTr('Search')
            width: 100
            enabled: searchInput.text !== ''
            onClicked: {
                nameFilterChanged(searchInput.text)
            }
        }
    }
}
