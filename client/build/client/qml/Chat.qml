import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Templates
import QtQuick.Dialogs

Rectangle {
    width: screenWidth*7/8+Window.width-screenWidth
    height: screenHeight/85*84+Window.height-screenHeight
    color: "#2b2b2b"
    radius: screenWidth/300
    anchors.top: parent.top
    anchors.topMargin: -screenWidth/300
    Grid {
        width: screenWidth*7/8+Window.width-screenWidth
        height: screenHeight/85*84+Window.height-screenHeight
        padding: screenWidth/70
        topPadding: screenWidth/200
        Item {
            width: parent.width-parent.padding*2
            height: parent.height-parent.topPadding-parent.padding
            Rectangle {
                id: messagesArea
                width: parent.width
                height: parent.height-messagesInputWrapper.height-screenHeight/80
                color: "#2b2bbb"
                Repeater {
                    model: messages[selectedChat]
                    Rectangle {
                        required property int j
                        width: messageTxt.width
                        height: messageTxt.height 
                        color: "#393939"
                        radius: width/8
                        Text {
                            id: messageTxt
                            text: messages[selectedChat][j]
                        }
                    }
                }
            }            
            Rectangle {
                id: messagesInputWrapper
                anchors {
                    top: messagesArea.bottom
                    topMargin: screenHeight/80
                }
                radius: screenWidth/500
                color: "#393939"
                width: screenWidth*23/30+Window.width-screenWidth
                height: scrollingMessagesInput.height+screenHeight/75
                Flickable {
                    id: scrollingMessagesInput
                    width: parent.width-messageFileBtn.width*1.7
                    height: Math.min(contentHeight, screenHeight/50*3)
                    contentWidth: width
                    contentHeight: messagesInput.implicitHeight
                    anchors {
                            top: parent.top
                            topMargin: screenHeight/150
                            left: parent.left
                            leftMargin: messageFileBtn.width*1.7
                    }
                    TextArea.flickable: TextArea {
                        id: messagesInput
                        font.pixelSize: screenHeight/65
                        wrapMode: Text.WordWrap
                        color: "#dddddd"
                        selectedTextColor: "#dddddd"
                        selectionColor: "#777788"
                    }
                    ScrollBar.vertical: ScrollBar {}
                }
            }
            Button {
                id: messageFileBtn
                height: screenHeight/45
                width: height
                enabled: true
                background: Rectangle {
                    id: messageFileBtnBackground
                    height: parent.height
                    width: height
                    radius: height/2
                    color: "#454545"
                    Rectangle {
                        width: 2
                        height: parent.height*0.7
                        radius: width/2
                        color: "#393939"
                        anchors {
                            top: parent.top
                            topMargin: (parent.height-height)/2
                            left: parent.left
                            leftMargin: (parent.width-width)/2
                        }
                    }
                    Rectangle {
                        width: parent.width*0.7
                        height: 2
                        radius: height/2
                        color: "#393939"
                        anchors {
                            top: parent.top
                            topMargin: (parent.height-height)/2
                            left: parent.left
                            leftMargin: (parent.width-width)/2
                        }
                    }
                }
                anchors {
                    top: messagesArea.bottom
                    topMargin: (messagesInputWrapper.height-messageFileBtnBackground.height)/2+screenHeight/80
                    left: messagesInputWrapper.right
                    leftMargin: -scrollingMessagesInput.width-width*1.5
                }
                onClicked: {
                    fileDialog.open()
                }
            }
            Rectangle {
                width: 2
                height: scrollingMessagesInput.height
                color: "#666666"
                anchors {
                    top: messagesInputWrapper.top
                    topMargin: screenHeight/150
                    left: messageFileBtn.right
                    leftMargin: messageFileBtn.width*0.2
                }
            }
            Button {
                id: messageBtn
                height: screenHeight/40
                width: height
                enabled: true
                background: Rectangle {
                    id: messageBtnBackground
                    height: parent.height
                    width: height
                    radius: height/2
                    color: "#0000ff"
                }
                anchors {
                    top: messagesArea.bottom
                    topMargin: (messagesInputWrapper.height-messageBtnBackground.height)/2+screenHeight/80
                    left: messagesInputWrapper.right
                    leftMargin: width/4
                }
                onClicked: {
                    user.handleSendMessage(selectedChat, messagesInput.text);
                    messagesInput.text = "";
                }
            }
        }
    }
    FileDialog { id: fileDialog }
}