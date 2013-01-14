import QtQuick 1.0
import QtDesktop 0.1
import MussonComponents 1.0

Frame {

    id: albumsPage

    signal albumSelected(string albumTitle, string albumIcon, variant album)
    signal back()

    AlbumsManager { id: albumsManager; sm: global_sm; }

    function setOwner(ownerName, ownerIcon, owner) {
        albumsManager.owner = owner;
        albumHeader.title = ownerName;
        albumHeader.icon = ownerIcon;
    }

    function showBackButton(isShow) {
        if (isShow) {
            albumHeader.backButton.visible = true;
        } else {
            albumHeader.backButton.visible = false;
        }
    }

    Item {
        id: albumsList
        anchors.fill: parent

        AlbumHeader {
            id: albumHeader
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Connections {
                onRefresh: albumsManager.refreshAlbums()
                onBack: back()
            }

            Binding {
                target: albumHeader
                property: "isUpdating"
                value: albumsManager.isAlbumsUpdate
            }
        }

        ListView {
            id: albumsView
            anchors.top: albumHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            model: albumsManager.model
            clip: true

            delegate: AlbumDelegate {
                id: albumDelegate

                Connections {
                    onDelegateClicked: {
                        albumsManager.selected = albumId;

                        albumSelected(title, albumIcon, album);
                    }
                }
            }
        }

    }

    EmptyState {
        visible: (albumsView.count === 0)
        text: qsTr("No albums")
        anchors.fill: parent
    }

    Component.onCompleted: {
        albumsManager.initAlbums();
        setOwner(accountManager.myName, accountManager.myIcon, accountManager.myProfile);
    }
}
