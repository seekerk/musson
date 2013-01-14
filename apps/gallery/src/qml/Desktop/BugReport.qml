import QtQuick 1.1

import QtDesktop 0.1

import MussonCore 1.2
import MussonComponents 1.0
import "UIConstants.js" as UI

ApplicationWindow {
    id: bugReport

    property bool isManualFill
    property alias bugTitle: subjectField.text
    property alias bugBody: descriptionArea.text
    property alias bugAuthor: authorField.text

    property string _BUG_TEMPLATE: qsTr("EXACT STEPS LEADING TO PROBLEM:\n\n\nEXPECTED OUTCOME:\n\n\nACTUAL OUTCOME:\n")

    function cleanForm() {
        bugAuthor = ""
        bugTitle = ""
        bugBody = _BUG_TEMPLATE
    }

    minimumWidth: 400
    minimumHeight: 450

    data: Column {
        id: col
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        spacing: 6

        Item {
            id: info
            width: parent.width
            height: infoCol.height + 10

            Column {
                id: infoCol
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                Text {
                    id: sorryText
                    visible: !isManualFill

                    text: qsTr("We're Sorry")
                    font.weight: Font.Bold
                    font.pixelSize: UI.FONT_DEFAULT
                    smooth: true
                    width: parent.width
                }

                Text {
                    id: first
                    visible: !isManualFill
                    text: qsTr("If you receive an error message, think you have found a bug or otherwise require technical support.")
                    font.weight: Font.Light
                    font.pixelSize: UI.FONT_LSMALL
                    wrapMode: Text.Wrap
                    width: parent.width
                }

                Text {
                    id: second
                    text: qsTr("Please provide as much information as possible. It is very hard to fix a bug if the person looking into the problem cannot reproduce it.")
                    font.weight: Font.Light
                    font.pixelSize: UI.FONT_LSMALL
                    wrapMode: Text.Wrap
                    width: parent.width
                }
            }
        }

        Item {
            id: to
            width: parent.width
            height: authorField.height

            Image {
                id: pointer1
                source: "qrc:/icons/pointer.png"
                sourceSize.height: 20
                sourceSize.width: 12
                anchors.leftMargin: -10
                anchors.left: parent.left
                anchors.verticalCenter: authorText.verticalCenter
                visible: authorField.activeFocus && !authorField.readOnly
            }

            Text {
                id: authorText
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10

                text: qsTr("Email:")
                font.weight: Font.Light
                font.pixelSize: UI.FONT_DEFAULT
                smooth: true

                states: State {
                    name: "active"
                    when: (authorField.activeFocus && !authorField.readOnly)
                    PropertyChanges {
                        target: authorText;
                        color: "#006abe"
                    }
                }
            }

            TextField {
                id: authorField
                anchors.left: authorText.right
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.right: parent.right
                placeholderText: qsTr("Enter your email address here")
                validator: RegExpValidator {
                    regExp: /^|(([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+)$/
                }

                Keys.onReturnPressed: {
                    subjectField.forceActiveFocus()
                }
            }
        }

        Item {
            id: subject
            width: parent.width
            height: subjectField.height

            Image {
                id: pointer2
                source: "qrc:/icons/pointer.png"
                sourceSize.height: 20
                sourceSize.width: 12
                anchors.leftMargin: -10
                anchors.left: parent.left
                anchors.verticalCenter: subjectText.verticalCenter
                visible: subjectField.activeFocus && !subjectField.readOnly
            }

            Text {
                id: subjectText
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 10
                anchors.bottomMargin: 10

                text: qsTr("Title:")
                font.weight: Font.Light
                font.pixelSize: UI.FONT_DEFAULT
                smooth: true

                states: State {
                    name: "active"
                    when: (subjectField.activeFocus && !subjectField.readOnly)
                    PropertyChanges {
                        target: subjectText;
                        color: "#006abe"
                    }
                }
            }

            TextField {
                id: subjectField
                anchors.margins: 10
                anchors.left: subjectText.right
                anchors.right: parent.right
                readOnly: !isManualFill

                Keys.onReturnPressed: {
                    descriptionArea.forceActiveFocus()
                }
            }
        }
    }

    Item {
        id: description
        width: parent.width
        anchors.top: col.bottom
        anchors.bottom: buttons.top

        Text {
            id: descriptionText
            anchors.topMargin: 6
            anchors.leftMargin: 8
            anchors.left: parent.left
            anchors.top: parent.top
            text: qsTr("Description")
            font.weight: Font.Light
            font.pixelSize: UI.FONT_LSMALL
        }

        TextArea {
            id: descriptionArea
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: descriptionText.bottom
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            width: parent.width - 2*10
            height: description.height - descriptionText.height - 10
            font.pixelSize: UI.FONT_LSMALL

        }
    }

    ButtonRow {
        id: buttons
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5

        Button {
            anchors.leftMargin: UI.DEFAULT_MARGIN
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Cancel")

            onClicked: close();
        }

        Button {
            id: acceptBtn
            anchors.rightMargin: UI.DEFAULT_MARGIN
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("Send")

            onClicked: bugReport.accept()

            states: State {
                name: "disabled"
                when: !authorField.acceptableInput || (subjectField.text === "" || descriptionArea.text === "")
                PropertyChanges {
                    target: acceptBtn
                    enabled: false
                }
            }
        }
    }

    // This property controls that signal of bug sent is received by correct window
    property bool isSent: false

    function accept() {
        isSent = true;
        if (isManualFill) {
            notificationManager.bugAuthor = authorField.text
            notificationManager.bugTitle = subjectField.text
            notificationManager.bugBody = descriptionArea.text
            notificationManager.sendBug()
        } else {
            notificationManager.bugAuthor = authorField.text
            notificationManager.bugBody = descriptionArea.text
            notificationManager.sendBug()
        }
    }

    Connections {
        target: notificationManager
        onShowResultBugSending: checkClosing()
    }

    // Function checks that signal for closing was received by correct window
    function checkClosing() {
        if (isSent)
            close();
    }
}
