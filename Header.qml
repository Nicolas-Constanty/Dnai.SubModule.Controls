import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    property alias title: _title
    property alias icon: _icon
    id: _control
    height: 40
    anchors.left: parent.left
    anchors.right: parent.right
    Label {
        id: _title
        anchors.left: parent.left
        anchors.leftMargin: _control.height
        verticalAlignment: "AlignVCenter"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#FFFFFF"
        font.bold: true
        font.pointSize: 10
        font.capitalization: Font.Capitalize
    }
    Image {
        id: _icon
        property int padding: 2
        anchors.left: _title.right
        height: _control.height - padding
        width: _control.height - padding
    }
}
