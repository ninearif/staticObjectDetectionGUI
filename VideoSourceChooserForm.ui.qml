import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

Item {
    property alias cameraRadioButton: cameraRadioButton
    property alias networkRadioButton: networkRadioButton
    property alias fileRadioButton: fileRadioButton
    property alias cameraComboBox: cameraComboBox
    property alias networkField: networkField
    property alias fileText: fileText

    ButtonGroup {
        buttons: [cameraRadioButton, networkRadioButton, fileRadioButton]
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            RadioButton {
                id: cameraRadioButton
                text: "Camera"
                checked: true
                Layout.preferredWidth: networkRadioButton.width
                onCheckedChanged: cameraComboBox.focus = this.checked
            }

            ComboBox {
                id: cameraComboBox
                enabled: cameraRadioButton.checked
                Layout.fillWidth: true
                textRole: "name"
                currentIndex: 0
            }
        }

        RowLayout {
            RadioButton {
                id: networkRadioButton
                text: "Network"
                checked: true
                onCheckedChanged: networkField.focus = this.checked
            }
            TextField {
                id: networkField
                Layout.fillWidth: true
                selectByMouse: true
                placeholderText: "rtsp://user:password@server.host/path"
                anchors.verticalCenter: parent.verticalCenter
                enabled: networkRadioButton.checked
            }
        }

        RowLayout {
            RadioButton {
                id: fileRadioButton
                text: "File"
                Layout.preferredWidth: networkRadioButton.width
                onCheckedChanged: fileButton.focus = this.checked
            }
            Text {
                id: fileText
                Layout.fillWidth: true
                text: ""
                elide: Text.ElideMiddle
                anchors.verticalCenter: parent.verticalCenter
            }
            Button {
                id: fileButton
                enabled: fileRadioButton.checked
                text: "Browse..."
                onClicked: fileDialog.visible = true

                FileDialog {
                    id: fileDialog
                    folder: shortcuts.home
                    nameFilters: ["Videos files (*.mp4 *.avi)"]
                    onAccepted: fileText.text = this.fileUrl
                    onRejected: fileText.text = ""
                }
            }
        }
    }
}
