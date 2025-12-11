import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Window {
    color: "#212121"
    width: screenWidth
    height: screenHeight
    minimumWidth: screenWidth/2
    minimumHeight: screenHeight/2
    visible: true
    title: qsTr("Flow")
    ColumnLayout {
        Grid {
            width: screenWidth
            height: screenHeight/50
            topPadding: (height-screenHeight/55)*2
            leftPadding: screenWidth/200
            Text {
                color: "#dddddd" 
                font.bold: true
                text: "Flow"
                font.pixelSize: screenHeight/55
                height: parent.height-parent.topPadding
            }
        }
        RowLayout {
            spacing: 0
            Rectangle {
                id: contactsViewer
                width: screenWidth/25
                height: screenHeight/45*44+Window.height-screenHeight
                color: "#212121"
                Grid {
                    width: screenWidth/25
                    height: screenHeight/45*44+Window.height-screenHeight
                    padding: screenWidth/170
                    ScrollView {
                        width: parent.width-parent.padding*2
                        height: parent.height-parent.padding*2
                        contentWidth: width
                        contentHeight: contactsCount*width+(contactsCount-1)*parent.padding
                        ColumnLayout {
                            spacing: parent.parent.padding
                            Repeater {
                                model: contactsCount
                                Button {
                                    required property int index
                                    height: parent.parent.width
                                    width: height
                                    Image {
                                        height: parent.height
                                        width: height
                                        source: "file:/home/theshizik3/Desktop/projects/flow/client/main.jpg"
                                    }
                                }
                            }
                        }
                        ScrollBar.vertical: ScrollBar {}
                    }
                }
            }
            Rectangle {
                anchors.left: contactsViewer.right
                anchors.leftMargin: screenWidth/300
                color: "#393939"
                width: screenWidth*167/200+Window.width-screenWidth+screenWidth/8-screenWidth/300
                height: screenHeight/45*44+Window.height-screenHeight
                RowLayout {
                    spacing: 0
                    Rectangle {
                        width: screenWidth/8
                        height: screenHeight/45*44+Window.height-screenHeight
                        color: "#2b2b2b"
                        radius: screenWidth/300
                        anchors.left: parent.left
                        anchors.leftMargin: -screenWidth/300
                        Grid {
                            width: screenWidth/7
                            height: screenHeight/45*44+Window.height-screenHeight
                            padding: screenWidth/200
                            Text {
                                text: "Hello, world!"
                                color: "#dddddd"
                            }
                        }
                    }
                    Rectangle {
                        width: screenWidth*167/200+Window.width-screenWidth
                        height: screenHeight/45*44+Window.height-screenHeight
                        color: "#393939"
                        radius: screenWidth/300
                        Grid {
                            width: screenWidth*167/200+Window.width-screenWidth
                            height: screenHeight/45*44+Window.height-screenHeight
                            padding: screenWidth/100
                            topPadding: screenWidth/200
                            leftPadding: padding
                            Item {
                                width: parent.width - parent.padding*2
                                height: parent.height-parent.topPadding*3
                                Rectangle {
                                    id: messagesViewer
                                    width: screenWidth*163/200+Window.width-screenWidth
                                    height: parent.height-inputMessagesFlickable.height-screenHeight/75-screenHeight/80
                                    color: "#3939dd"
                                    Repeater {
                                            
                                    }
                                }            
                                Rectangle {
                                    id: i
                                    anchors.top: messagesViewer.bottom
                                    anchors.topMargin: screenHeight/80
                                    radius: screenWidth/500
                                    color: "#454545"
                                    width: screenWidth*23/30+Window.width-screenWidth
                                    height: inputMessagesFlickable.height+screenHeight/75
                                    Flickable {
                                        id: inputMessagesFlickable
                                        width: screenWidth*23/30+Window.width-screenWidth
                                        height: Math.min(contentHeight, screenHeight/50*3)
                                        contentWidth: width
                                        contentHeight: messagesInputArea.implicitHeight
                                        anchors {
                                                top: parent.top
                                                topMargin: (parent.height-height)/2-1
                                        }
                                        TextArea.flickable: TextArea {
                                            id: messagesInputArea
                                            wrapMode: Text.WordWrap
                                            color: "#dddddd"
                                            selectedTextColor: "#dddddd"
                                            selectionColor: "#777788"
                                        }
                                        ScrollBar.vertical: ScrollBar {}
                                    }
                                }
                                Button {
                                    height: 20
                                    width: height
                                    id: messageBtn
                                    enabled: true
                                    background: Rectangle {
                                        id: o
                                        height: 20
                                        width: height
                                        radius: height/2
                                        color: "#0000ff"
                                    }
                                    onClicked: {
                                        msgHandler.sendMessage();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
