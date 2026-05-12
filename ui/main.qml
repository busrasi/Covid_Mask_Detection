import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtMultimedia

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Mask Defence")

    property int cameraIndex: 0

    MediaDevices {
        id: mediaDevices
    }

    CaptureSession {
        id: captureSession
        camera: camera
        videoOutput: videoOutput
    }

    Camera {
        id: camera
        active: true
        cameraDevice: mediaDevices.videoInputs[cameraIndex]
    }

    VideoOutput {
        id: videoOutput
        objectName: "videoOutput"
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
    }

    Button {
        id: switchCameraButton
        text: "Switch Camera"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        z: 10

        onClicked: {
            if (mediaDevices.videoInputs.length > 1) {
                camera.stop()
                cameraIndex = (cameraIndex + 1) % mediaDevices.videoInputs.length
                camera.cameraDevice = mediaDevices.videoInputs[cameraIndex]
                camera.start()

                statusBox.color = "blue"
                predictText.text = "Waiting for detection..."

                console.log("Selected camera:", camera.cameraDevice.description)
            }
        }
    }

    Rectangle {
        id: statusBox
        width: 280
        height: 72
        radius: 8
        color: "blue"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 18
        z: 10

        Text {
            id: predictText
            anchors.centerIn: parent
            text: qsTr("Waiting for detection...")
            font.pointSize: 18
            color: "white"
        }
    }

    Connections {
        target: maskDetectionFilter

        function onObjectDetected(result) {
            if (result === true) {
                statusBox.color = "green"
                predictText.text = "Mask Detected"
            } else {
                statusBox.color = "red"
                predictText.text = "Mask Not Detected"
            }
        }
    }

    Component.onCompleted: {
        console.log("Camera count:", mediaDevices.videoInputs.length)

        for (let i = 0; i < mediaDevices.videoInputs.length; ++i) {
            console.log(i, mediaDevices.videoInputs[i].description)
        }

        camera.start()
    }
}