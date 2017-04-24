import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.1

Dialog {
    property string url: ""
    property ListModel cameraModel: ListModel {
        ListElement { value: "0"; name: "Camera 0" }
        ListElement { value: "1"; name: "Camera 1" }
        ListElement { value: "2"; name: "Camera 2" }
        ListElement { value: "3"; name: "Camera 3" }
    }

    title: "Choose video source"
    focus: true
    modal: true
    x: Math.round((parent.width - width) / 2)
    y: Math.round(parent.height / 6)
    width: 550
    standardButtons: Dialog.Ok | Dialog.Cancel
    closePolicy: Popup.CloseOnEscape
    parent: ApplicationWindow.overlay

    function extractUrl() {
        if (container.cameraRadioButton.checked) {
            var index = container.cameraComboBox.currentIndex
            return cameraModel.get(index).value
        } else if (container.networkRadioButton.checked) {
            return container.networkField.text
        } else {
            return container.fileText.text.replace("file://", "")
        }
    }

    MessageDialog {
        id: warningDialog
        icon: StandardIcon.Warning
        text: "You have no select the video source"
    }

    signal gotUrl(string url)

    onAccepted: {
        var url = extractUrl()
        if (url.length === 0) {
            warningDialog.open()
            return
        }
        gotUrl(url)
    }

    contentItem: VideoSourceChooserForm {
        id: container
        cameraComboBox.model: cameraModel
    }
}
