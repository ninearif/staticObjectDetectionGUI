import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import my.components 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    property var camPreviews: []

    Timer {
        property int repeatCount: 0
        id: camStopper
        interval: 100
        repeat: true
        onTriggered: {
            repeatCount += 1
            if (repeatCount == 150) {
                console.log('force quit')
                camStopper.stop()
                Qt.quit()
                return
            }

            for (var i = 0; i < camPreviews.length; i += 1) {
                if (camPreviews[i].status === CamPreviewHandler.Running) {
                    return
                }
            }

            camStopper.stop()
            Qt.quit()
        }
    }

    Dialog {
        id: shuttingDownDialog
        x: Math.round((parent.width - width) / 2)
        y: Math.round(parent.height / 6)
        title: qsTr("Shutting down")
        closePolicy: Popup.NoAutoClose
        modal: true

        contentItem: ProgressBar {
            indeterminate: true
        }
    }

    Dialog {
        id: confirmExitDialog
        x: Math.round((parent.width - width) / 2)
        y: Math.round(parent.height / 6)
        title: qsTr("Are you sure you want to exit ?")
        standardButtons: Dialog.Ok | Dialog.Cancel
        closePolicy: Popup.CloseOnEscape
        modal: true

        onAccepted: {
            camPreviews.forEach(function(camPreview) {
                camPreview.stop()
            })
            camStopper.start()
            shuttingDownDialog.open()
        }
    }

    onClosing: {
        close.accepted = false
        confirmExitDialog.open()
    }

    GridLayout {
        rows: 2
        columns: 2
        anchors.fill: parent

        CamPreview {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: camPreviews.push(this)
        }

        CamPreview {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: camPreviews.push(this)
        }

        CamPreview {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: camPreviews.push(this)
        }

        CamPreview {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Component.onCompleted: camPreviews.push(this)
        }
    }
}
