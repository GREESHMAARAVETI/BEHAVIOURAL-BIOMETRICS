pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors
import com.DataReadingHandler.h 1.0
import com.logger.h


ApplicationWindow {
    id: root;
    width: 420
    height: 760
    visible: true
    title: qsTr("SensorBasedAuth")
    color: "white";

    readonly property int defaultFontSize: 22
    // readonly property int imageSize: width / 2

    header : ToolBar {
        id: header
        background: Rectangle {
                color: "lightblue"
            }
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            Label {
                id: heading
                text: root.title
                font.pixelSize: root.defaultFontSize
                font.weight: Font.Medium
                verticalAlignment: Qt.AlignVCenter
                Layout.alignment: Qt.AlignCenter
                Layout.preferredHeight: 55
                color: "black"
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        spacing: 10

        component CustomButton: Button {
            // highlighted: true
            font.pixelSize: root.defaultFontSize
            font.letterSpacing: 1.5

            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        CustomButton {
            text: "Start Calibration"
            onClicked: {outfield.text = "State: Calibration Started";
                dataHandler.startCalibration()
                console.log("Calibration started")
                xText: "X: " + accelerometer.x.toFixed(2)
                xValue: 0.5 + (accelerometer.x / 100)
                yText: "Y: " + accelerometer.y.toFixed(2)
                yValue: 0.5 + (accelerometer.y / 100)
                zText: "Z: " + gyroscope.z.toFixed(2)
                zValue: 0.5 + (gyroscope.z / 1000)
            }
        }

        CustomButton {
            text: "Start Pattern"
            onClicked: {outfield.text = "State: Pattern Started";
                accelerometer.accsampleCount = 0
                gyroscope.gyrosampleCount = 0
                dataHandler.startPattern();
                console.log("Start pattern")
            }
        }
        CustomButton {
            text: "Stop Pattern"
            onClicked: {outfield.text = "State: Pattern Stopped";
                dataHandler.stopPattern();
                console.log("Stop pattern")
            }
        }
        CustomButton {
            text: "Start Authentication"
        }
        CustomButton {
            text: "Stop Authentication"
        }

        Text {
            id: accelerometer_data
            text: qsTr("accelerometer data:")
        }

        ProgressXYBar {
            id: accbar
            Layout.fillWidth: true
            property bool caldone: false
            // property double xfiltered: 0
            // property double yfiltered: 0
            // property double zfiltered: 0
            // fontSize: root.fontSize
            xText: "X: " + accelerometer.x.toFixed(2)
            xValue: 0.5 + (accelerometer.x / 100)
            yText: "Y: " + accelerometer.y.toFixed(2)
            yValue: 0.5 + (accelerometer.y / 100)
        }

        Text {
            id: gyroscope_data
            text: qsTr("gyroscope data:")
        }

        ProgressZBar {
            id: gyrobar
            Layout.fillWidth: true
            // fontSize: root.fontSize
            zText: "Z: " + gyroscope.z.toFixed(2)
            zValue: 0.5 + (gyroscope.z / 1000)
        }

        Text {
            id: outfield
            text: qsTr("State: Idle")
        }

        Text {
            id: movementout
            text: qsTr("Movement: 0")
        }
        
        Text {
            id: yvelocityout
            text: qsTr("Velocity Y: 0")
        }
        
        Text {
            id: xvelocityout
            text: qsTr("Velocity X: 0")
        }

        Text {
            id: rotationout
            text: qsTr("Rotation: 0")
        }

        Text {
            id: authenticationout
            text: qsTr("Authentication: 0")
        }
    }
    Accelerometer {
        id: accelerometer
        property real x: 0
        property real y: 0
        property real z: 0
        property real accsampleCount: 0

        active: true
        dataRate: 25

        onReadingChanged: {
            if(active == true){
                accsampleCount += 1
                x = (reading as AccelerometerReading).x
                y = (reading as AccelerometerReading).y
                z = (reading as AccelerometerReading).z
                // xvelocityout.text = "Velocity X: " + x
                // yvelocityout.text = "Velocity Y: " + y
                console.log("X: " + x + " Y: " + y + " " + outfield.text  + " Count:" + accsampleCount
                            + " AccActive:" + active + " GyroActive:" + gyroscope.active)
                dataHandler.accReading(x,y)
            }
        }

    }

    Gyroscope {
        id: gyroscope
        property real x: 0
        property real y: 0
        property real z: 0
        property real gyrosampleCount: 0

        active: true
        dataRate: 25

        onReadingChanged: {
            if(active == true){
                gyrosampleCount += 1
                x = (reading as GyroscopeReading).x
                y = (reading as GyroscopeReading).y
                z = (reading as GyroscopeReading).z
                // rotationout.text = "Rotation: " + z
                console.log("Z: " + z + " " + outfield.text + " Count:" + gyrosampleCount
                            + " AccActive:" + accelerometer.active + " GyroActive:" + active)
                dataHandler.gyroReading(z)
            }
        }
    }

    DataReadingHandler{
        id: dataHandler
        // onMovementChanged: {
        //     console.log("Movement: " + dataHandler.movement)
        //     movementout.text = "Movement: " + dataHandler.movement
        // }

        // onRotationZChanged: {
        //     console.log("Rotation: " + dataHandler.rotationZ)
        //     rotationout.text = "Rotation: " + dataHandler.rotationZ
        // }

        // onVelocityXChanged: {
        //     console.log("VelocityX: " + dataHandler.velocityX)
        //     xvelocityout.text = "Velocity X: " + dataHandler.velocityX
        // }

        onVelocityYChanged: {
            console.log("VelocityY: " + dataHandler.velocityY)
            yvelocityout.text = "Velocity Y: " + dataHandler.velocityY
        }

        onAccActiveChanged: {
            if (dataHandler.accActive) {
                accelerometer.start();
                console.log("Acc activity:",accelerometer.active)
                console.log("Acc Active")
            } else {
                accelerometer.stop();
                console.log("Acc activity:",accelerometer.active)
                console.log("Acc Inactive")
            }
        }
        onGyroActiveChanged: {
            if (dataHandler.gyroActive) {
                gyroscope.start();
                console.log("Gyro activity:",gyroscope.active)
                console.log("Gyro Active")
            } else {
                gyroscope.stop();
                console.log("Gyro activity:",gyroscope.active)
                console.log("Gyro Inactive")
            }
        }
        onCalibrationChanged: {
            console.log("Calibration: " + dataHandler.calibration)
            outfield.text = "Calibration done"
            authenticationout.text = "Calibration: " + dataHandler.calibration
            ProgressXYZBar.caldone = true
        }
        onNewpatternChanged: {
            console.log("move: " + dataHandler.newpattern)
            outfield.text = "New move recorded"
            authenticationout.text = "New move: " + dataHandler.newpattern
        }

        onFilteredXChanged: {
            accbar.xText = "X: " + dataHandler.filteredX.toFixed(2)
            accbar.xValue = 0.5 + (dataHandler.filteredX / 100)
        }
        onFilteredYChanged:{
            accbar.yText = "Y: " + dataHandler.filteredY.toFixed(2)
            accbar.yValue = 0.5 + (dataHandler.filteredY / 100)
        }
        onFilteredZChanged: {

            gyrobar.zText = "Z: " + dataHandler.filteredZ.toFixed(2)
            accbar.zValue = 0.5 + (dataHandler.filteredZ / 100)
        }
    }
}
