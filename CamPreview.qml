import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import my.components 1.0

Item {
    property alias status: handler.status
    property real opacityValue: 0.8
    property string videoUrl: "1"
    property string imagePrefix: Math.random().toString()

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    function stop() {
        handler.stop()
    }

    VideoSourceChooser {
        id: videoSourceChooser

        onGotUrl: videoUrl = url
    }

    CamPreviewHandler {
        property var images: [bigImage, smallImage]

        id: handler
        baseImageId: imagePrefix
        threshold: thresholdSlider.value
        onPreview: {
            images.forEach(function(image) {
                var tmp = image.source
                image.source = ""
                image.source = tmp
            })
        }
    }

    RowLayout {
        id: topWrapper
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Image {
                id: bigImage
                cache: false
                source: "image://mat/" + imagePrefix + "raw"
                fillMode: Image.PreserveAspectFit
                smooth: false
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.margins: 5

                Text {
                    id: thresholdLabel
                    text: "Threshold"
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    height: thresholdSlider.height
                }

                Slider {
                    id: thresholdSlider
                    stepSize: 1
                    from: 0
                    to: 255
                    value: 50
                    snapMode: Slider.SnapOnRelease
                    onValueChanged: value = parseInt(value)
                    Layout.fillWidth: true
                }

                Text {
                    id: thresholdValue
                    text: thresholdSlider.value
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    height: thresholdSlider.height
                    Layout.preferredWidth: 20
                }
            }
        }

        Column {
            Layout.preferredWidth: 60
            Layout.fillHeight: true
            Layout.rightMargin: 5
            Layout.topMargin: 5

            Button {
                id: playButton
                text: "\uF04B" // fa-play
                font.family: "FontAwesome"
                width: parent.width
                onClicked: handler.start(videoUrl)
                background: transparent
                visible: handler.status == CamPreviewHandler.Stop
            }

            Button {
                id: stopButton
                text: "\uF04D" // fa-stop
                font.family: "FontAwesome"
                width: parent.width
                onClicked: handler.stop()
                visible: handler.status == CamPreviewHandler.Running
            }

            Button {
                text: "\uF03D" // fa-video-camera
                font.family: "FontAwesome"
                width: parent.width
                background: transparent
                onClicked: videoSourceChooser.open()
            }

            Image {
                id: smallImage
                width: parent.width
                cache: false
                source: "image://mat/" + imagePrefix + "gray"
                fillMode: Image.PreserveAspectFit
                opacity: opacityValue
                smooth: false

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        var tmp = bigImage.source
                        bigImage.source = parent.source
                        parent.source = tmp
                    }
                    hoverEnabled: true
                    onEntered: smallImage.opacity = 1
                    onExited: smallImage.opacity = opacityValue
                }
            }
        }
    }
}
