import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtMultimedia 5.15

ApplicationWindow {
    id:root
    width: 640
    height: 480
    visible: true
    title: qsTr("Mask Defence")

    // CAMERA
    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        focus {
            focusMode: CameraFocus.FocusContinuous
            focusPointMode: CameraFocus.FocusPointAuto
        }
        position: Camera.FrontFace
    }

    // CAMERA OUTPUT
    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        source: camera
        focus: visible
        //orientation: 0

        autoOrientation: true
        visible: true
        //fillMode: VideoOutput.PreserveAspectCrop;
        filters: [maskDetectionFilter]
        Rectangle{
            id:rect
            width: 225
            height: 70
            radius: 5
            color: "blue"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            Text {
                id: predictText
                text: qsTr("Bekleniyor...")
                font.pointSize: 25
                color: "white"
                topPadding: 20

            }
        }


    }

    Connections {
        target: maskDetectionFilter
        function onObjectDetected(result)
        {
            if(result === true) {

                rect.color = "green"
                predictText.text ="Geçebilirsiniz..."
            }
            else {
                rect.color = "red"
                predictText.text ="Maske Algılanmadı!"

            }
        }
    }
}
