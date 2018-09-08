import QtQuick 2.0
import QtQuick.Controls 2.3
//import QtQuick.Window 2.11

import Dnai.Controls 1.0
import Dnai.FontAwesome 1.0

BackendSearchableMenu {
    id: _backend
    property Item background: null
    property Component matchDelegate: _itemDelComponent
    property int matchViewWidth: 300
    property var triggeredAction: null

    height: itemHeight + model.rowCount() * itemHeight
    width: itemWidth


    function open()
    {
        _menu.popup()
    }

    visible: _menu.opened

    menu: Item {
        property Menu menu: _menu
        property int itemHeight: 30
        property int itemWidth: 200
        height: _backend.height
        width: _backend.width

        function open() {
            _menu.open()
        }

        function addMenu(basemenu) {
            _menu.addMenu(basemenu)
        }
        function addAction(baseaction) {
            _menu.addAction(baseaction)
        }

        function removeMenu(baseaction) {
            _menu.removeMenu(baseaction)
        }

        Menu {
            id: _menu
            TextField {
                id: _editableText
                y: -itemHeight
                height: itemHeight
                onTextChanged: {
                    if (text.length > 0 &&  _matchPopup.opened === false)
                        _matchPopup.open()
                    _backend.searchFuzzy(text)
                }
            }

            onClosed: {
                _editableText.text = "";
            }

            closePolicy: Popup.CloseOnPressOutsideParent

            delegate: MenuItem {
                id: menuItem
                implicitWidth: itemWidth
                implicitHeight: itemHeight

                arrow: Item {
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


    background: Rectangle {
        id: _background
        anchors.fill: parent
    }

   Component {
        id: _itemDelComponent
        ItemDelegate {
            id: _itemDel
            text: modelData
            width: parent.width
            height: itemHeight
            onClicked: {
                if (_backend.triggeredAction)
                {
                    _backend.triggeredAction(_backend.getActionObj(text))
                    _menu.close()
                }
            }

            highlighted: ListView.isCurrentItem
            background: Rectangle {
                color: _itemDel.hovered ? "#639DBD" : "#ffffff"
            }
        }
    }

   function getAbsolutePosition(node) {
         var returnPos = {};
         returnPos.x = 0;
         returnPos.y = 0;
         if(node !== undefined && node !== null) {
             var parentValue = getAbsolutePosition(node.parent);
             returnPos.x = parentValue.x + node.x;
             returnPos.y = parentValue.y + node.y;
         }
         return returnPos;
     }

    Popup {
        id: _matchPopup
        contentWidth: _backend.matchViewWidth
        x: getAbsolutePosition(_menu).x + _menu.width + contentWidth > ApplicationWindow.window.width ? _menu.x - contentWidth : _menu.x + _menu.width
        y: _menu.y
        padding: 0
        ScrollView {
            id: _scrollView
            height: parent.height
            width: parent.width
            contentWidth: _backend.matchViewWidth
            clip: true
            background: Rectangle {
            }

            ListView {
                id: _matchView
                width: _backend.matchViewWidth
                onModelChanged: {
                    var h = _backend.matchedList.length * itemHeight
                    _matchPopup.height = h < 200 ? h : 200
                }
                model: _backend.matchedList
                delegate: _backend.matchDelegate
            }
        }
    }

    onBackgroundChanged: {
        _backend.background.parent = _backend
        _backend.background.z = -1
    }
}
