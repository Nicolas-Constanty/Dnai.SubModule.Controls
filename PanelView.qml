import QtQuick 2.0
import QtQuick.Controls 2.2
import Dnai.FontAwesome 1.0

Item {
    id: _control
    property real radius: 3
    property alias background: _background
    property alias content: _content
    property alias header: _header
    property alias branch: _branch
    property real margin: 5
    default property alias _contentChildren: _contentColumn.data
    property bool init: false
    clip: true
    property real initialHeight: init ? initialHeight : height
    property real spacing: 5
    Header {
        id: _header
        radius: _control.radius
        color: "#323232"
        height: 30 + radius
    }
    Item {
        id: _contentZone
        anchors.top: _header.bottom
        anchors.topMargin: - _control.radius
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Rectangle {
            id: _hideRadius
            color: _background.color
            height: _control.radius
            anchors.left: parent.left
            anchors.right: parent.right
        }
        Rectangle {
            id: _background
            anchors.fill: parent
            color: "#424242"
            radius: _control.radius
        }
        Item {
            id: _content
            property Item background: _contentBackground
            anchors.fill: parent
            anchors.leftMargin: _control.margin * 3
            anchors.rightMargin: _control.margin * 3
            anchors.topMargin: _control.margin + _control.radius
            anchors.bottomMargin: _control.margin
            clip: true
            Rectangle {
                id: _contentBackground
                anchors.fill: parent
                color: "transparent"
            }
            ScrollView
            {
                anchors.fill: parent
                Column {
                    spacing: _control.spacing
                    width: _content.width
                    id: _contentColumn
                }
            }
        }
    }
    Item {
        id: _branch
        property alias awesomeIcon: _branchText
        property alias mouseArea: _branchMouseArea
        height: header.height
        width: header.height
        LabelAwesomeSolid {
            id: _branchText
            text: "\uf101"
            anchors.fill: parent
            verticalAlignment: "AlignVCenter"
            horizontalAlignment: "AlignHCenter"
            color: "#ffffff"
        }
        MouseArea {
            id: _branchMouseArea
            anchors.fill: parent
            onClicked: {
                if (_control.state === "Expended")
                {
                    _control.state = "Collapsed"
                }
                else
                {
                    _control.state = "Expended"
                }
            }
        }
    }

    states: [
        State{
            name: "Expended"
            PropertyChanges{ target: _control; height: _control.initialHeight }
            PropertyChanges{ target: _branchText; rotation: 90 }
        },
        State{
            name:"Collapsed"
            PropertyChanges{ target: _control; height: _header.height }
            PropertyChanges{ target: _branchText; rotation: 0 }
        }
    ]
    state: "Expended"
    transitions: [
        Transition {
            from: "Expended"
            to: "Collapsed"

           NumberAnimation {
               target: _control
               property: "height"
               duration: 200
               easing.type: Easing.InOutQuad
           }
           NumberAnimation {
               target: _branchText
               property: "rotation"
               duration: 200
               easing.type: Easing.InOutQuad
           }
           NumberAnimation {
              target: _hideRadius
              property: "opacity"
              duration: 200
              easing.type: Easing.InOutQuad
           }
        },
        Transition {
            from: "Collapsed"
            to: "Expended"
            NumberAnimation {
               target: _control
               property: "height"
               duration: 200
               easing.type: Easing.InOutQuad
            }
            NumberAnimation {
               target: _hideRadius
               property: "opacity"
               duration: 200
               easing.type: Easing.InOutQuad
            }
        }
    ]
    Component.onCompleted: {
        _control.init = true
    }
}
