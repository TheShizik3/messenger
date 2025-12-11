import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Item {
    Component.onCompleted: {
        var res = !user.checkData();
        console.log(res);
        if (res) {
            mainWindow.visible = true;
        }
        else {
            visible = false;
            mainLogRegWindow.visible = true;
            console.log(mainLogRegWindow.visible);
            console.trace();
        }
    }
    LogRegWindow { id: mainLogRegWindow }
    Window {
        id: mainWindow
        property string selectedChat: ""
        color: "#212121"
        width: screenWidth
        height: screenHeight
        minimumWidth: screenWidth/2
        minimumHeight: screenHeight/2
        visible: false
        title: "Flow"
        onClosing: (close) => {
            user.closeWindow();
        }
        ColumnLayout {
            Header { id: header }
            RowLayout {
                spacing: 0
                Chats { id: chats }
                Rectangle {
                    anchors.left: chats.right
                    anchors.leftMargin: 0
                    anchors.top: header.bottom
                    anchors.topMargin: screenWidth/300
                    color: "#2b2b2b"
                    width: screenWidth*7/8+Window.width-screenWidth
                    height: screenHeight/45*44+Window.height-screenHeight-screenWidth/300
                    RowLayout {
                        spacing: 0
                        Chat { id: chat }
                    }
                }
            }
        }
    }
}
