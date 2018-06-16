import QtQuick 2.0


import QtGraphicalEffects 1.0 as Qg
import QtQuick.Shapes 1.11
import Dnai.Controls 1.0

ColorPicker {
    id: _control
    radius: 150
    height: radius * 2
    width: radius * 2
    innerPercent: 0.75
    MouseArea {
        anchors.fill: parent
        onMouseXChanged: {
            _control.getColor(_window, mouseX, mouseY)
        }
    }

    Rectangle {
        id: _circle
        anchors.fill: parent
        radius: _control.radius
        clip: true
        color: "red"
        visible: false
    }

    Qg.ConicalGradient {
        id: _gradient
        anchors.fill: parent
        angle: 0.0
        visible: false
        gradient: Gradient {
            GradientStop {
                position: 0.000
                color: Qt.rgba(1, 0, 0, 1)
            }
            GradientStop {
                position: 0.167
                color: Qt.rgba(1, 1, 0, 1)
            }
            GradientStop {
                position: 0.333
                color: Qt.rgba(0, 1, 0, 1)
            }
            GradientStop {
                position: 0.500
                color: Qt.rgba(0, 1, 1, 1)
            }
            GradientStop {
                position: 0.667
                color: Qt.rgba(0, 0, 1, 1)
            }
            GradientStop {
                position: 0.833
                color: Qt.rgba(1, 0, 1, 1)
            }
            GradientStop {
                position: 1.000
                color: Qt.rgba(1, 0, 0, 1)
            }
        }
    }

    Qg.OpacityMask {
        anchors.fill: parent
        source: _gradient
        maskSource: _circle
    }

    Rectangle {
        anchors.centerIn: parent
        width: _control.radius * 2 * _control.innerPercent
        height: width
        color: _window.color
        radius: width / 2
        id: ctr
        property double rad: 0.01745329252

        ControlTriangle {
            anchors.centerIn: parent
            radius: _control.radius * _control.innerPercent
            nbSegments: 4
            borderWidth: 0
            fillColor: _control.color
            antialiasing: true
            rotation: _control.rotateFocus
        }
    }
}
