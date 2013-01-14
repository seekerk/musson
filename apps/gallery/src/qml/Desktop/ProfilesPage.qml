import QtQuick 1.1
import QtDesktop 0.1
import MussonCore 1.2
import MussonComponents 1.0
import "UIConstants.js" as UI
import "constants.js" as ExtrasConstants

Frame {

    signal profileSelected(string ownerName, string ownerIcon, variant owner)

    onVisibleChanged: {
        if (visible) {
            filterManager.currentTab = _PROFILES_TAB
        }
    }

    function onlineCount() {
        return "(" + profileManager.onlineCount + ")"
    }

    function offlineCount() {
        var offline = profilesView.count - profileManager.onlineCount
        if (offline === 0)
            return ""
        return "(" + offline + ")"
    }


    Item {
        id: profilesList
        anchors.fill: parent
        ProfilesTop {
            id: profilesTop
            //title: qsTr("All networks")
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Connections {
                onRefresh: profileManager.refreshListFriends();
                onServiceChanged: { filterManager.selectService(service, serviceIndex);}
                onShowMe: {
                    profileManager.selected = "";
                    profileSelected(accountManager.myName, accountManager.myIcon, accountManager.myProfile);
                }
            }

            Binding {
                target: profilesTop
                property: "isUpdating"
                value: profileManager.isFriendsUpdate
            }
        }

        ListView {
            id: profilesView
            model: profileManager.model
            anchors.top: profilesTop.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
//            anchors.bottom: profilesFooter.top
            clip: true
            visible: (count != 0)


            delegate: ProfileDelegate {
                id: profileDelegate

                Connections {
                    onDelegateClicked: {
                        profileManager.selected = ownerId;

                        profileSelected(name, profileIcon, profile);
                    }
                }
            }
        }

//        ProfilesFooter {
//            id: profilesFooter
//            y: parent.y + parent.height - 90
//            anchors.left: parent.left
//            anchors.right: parent.right

//            onNameFilterChanged: profileManager.nameFilter = filterName;
//            visible: false
//        }

    }

    EmptyState {
        visible: (profilesView.count == 0)
        text: qsTr("No friends")
        anchors.fill: parent
    }
}
