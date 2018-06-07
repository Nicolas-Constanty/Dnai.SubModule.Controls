import QtQuick 2.0
import QtQuick.Controls 2.4
import Dnai.FontAwesome 1.0

Item {
    property Menu menu: _menu
    property int itemHeight: 30
    property int itemWidth: 200

    function open() {
        _menu.open()
    }

    function addMenu(basemenu) {
        _menu.addMenu(basemenu)
    }
    function addAction(baseaction) {
        _menu.addAction(baseaction)
    }
    Menu {
        id: _menu

        delegate: MenuItem {
            id: menuItem
            implicitWidth: itemWidth
            implicitHeight: itemHeight

            arrow: Item {
                visible: menuItem.subMenu
                implicitWidth: itemHeight
                implicitHeight: itemHeight
                anchors.right: parent.right
                LabelAwesomeSolid {
                    font.pointSize: itemHeight / 2
                    text: "\uf105"
                    color: menuItem.highlighted ? "#ffffff" : "#639DBD"
                    verticalAlignment: "AlignVCenter"
                    anchors.centerIn: parent
                }
            }

            contentItem: Text {
                text: menuItem.text
                font: menuItem.font
                opacity: enabled ? 1.0 : 0.3
                color: menuItem.highlighted ? "#ffffff" : "#639DBD"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                opacity: enabled ? 1 : 0.3
                color: menuItem.highlighted ? "#639DBD" : "transparent"
            }
        }

        background: Rectangle {
            implicitWidth: itemWidth
            implicitHeight: itemHeight
            color: "#ffffff"
            border.color: "#639DBD"
            radius: 2
        }
    }
}
