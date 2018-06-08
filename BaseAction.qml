import QtQuick 2.0
import QtQuick.Controls 2.3


Item
{
    property Action action: _action
    property var triggeredAction: null
    Action {
        property var model: null
        id: _action
        onTriggered: {
            if (triggeredAction !== null)
                triggeredAction(model)
        }
    }
}
