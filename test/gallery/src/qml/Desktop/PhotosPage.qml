import QtQuick 1.1
import QtDesktop 0.1
import MussonCore 1.2
import MussonComponents 1.0
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants

Frame {
    id: photosPage

    signal itemSelected(variant data)

    function setAlbum(albumTtl, albumIcn, album) {
        photosManager.album = album;
        photosPage.albumTitle = albumTtl;
        photosPage.albumIcon = albumIcn
    }

    property string albumTitle: ""
    property string albumIcon: ""
    property bool isMyAlbums: false

    PhotosManager {
        id: photosManager
        sm: global_sm

        onPhotoUploaded: {
            uploadPhotos.removeFile(filePath)
        }
    }

    Item {
        id: photosList
        anchors.fill: parent

        PhotoHeader {
            id: photoHeader
            icon: albumIcon
            title: albumTitle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right

            Connections {
                onRefresh: photosManager.refreshPhotos()
            }

            Binding {
                target: photoHeader
                property: "isUpdating"
                value: photosManager.isPhotosUpdate
            }
        }

        GridView {
            id: photosView
            interactive: visibleArea.heightRatio < 1
            cellWidth: 100
            cellHeight: cellWidth

            contentItem.anchors.margins: 2
            anchors.left: parent.left
            anchors.top: photoHeader.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true
            model: photosManager.model
            delegate: PhotoDelegate {
                id: photoDelegate

                Connections {
                    onDelegateClicked: {
                        photosManager.selected = photoId;
                        itemSelected(photoData);
                    }
                }
            }
        }

        EmptyState {
            visible: (photosView.count == 0)
            text: qsTr("No photos")
            anchors.fill: parent
        }
    }

    Component.onCompleted: {
        photosManager.initPhotos()
    }
}
