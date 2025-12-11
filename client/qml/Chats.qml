import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Rectangle {
    width: screenWidth/8
    height: screenHeight/85*84+Window.height-screenHeight
    color: "#212121"
    Grid {
        id: chatsWrap
        width: screenWidth/8  
        height: Math.min(screenHeight/85*84+Window.height-screenHeight-screenWidth/25+screenWidth/85, chatsNames.length*width+(chatsNames.length-1)*parent.padding)
        padding: screenWidth/170
        ScrollView {
            width: parent.width-parent.padding*2
            height: parent.height-parent.padding*2
            contentWidth: width
            contentHeight: chatsNames.length*width+(chatsNames.length-1)*parent.padding
            ColumnLayout {
                spacing: parent.parent.padding
                Repeater {
                    model: chatsNames.length
                    Button {
                        required property int i
                        height: parent.parent.width
                        width: height
                        Flickable {
                            anchors.fill: parent
                            width: parent.width
                            height: parent.height
                            contentWidth: chatName.width
                            contentHeight: chatName.height
                            flickableDirection: Flickable.HorizontalFlick
                            Text {
                                id: chatName
                                wrapMode: Text.NoWrap
                                text: chatsNames[i]
                            }
                            ScrollBar.horizontal: ScrollBar {}
                        }
                        onClicked: {
                            selectedChat = chatsNames[i]
                        }
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {}
        }
    }
    Rectangle {
        width: screenWidth/25-screenWidth/85
        height: width
        radius: screenWidth/500
        color: "#2b2b2b"
        anchors {
            top: chatsWrap.bottom
            topMargin: width
            left: parent.left
            leftMargin: (parent.width-width)/2
        }
        Rectangle {
            width: 2
            height: parent.height*0.7
            color: "#393939"
            anchors.centerIn: parent
        }
        Rectangle {
            height: 2
            width: parent.width*0.7
            color: "#393939"
            anchors.centerIn: parent
        }
        Button {
            width: parent.width
            height: parent.height
            onClicked: {
                console.log("nyan");
            }
        }
    }
}