import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Rectangle {
    width: screenWidth/25
    height: screenHeight/85*84+Window.height-screenHeight
    color: "#212121"
    Grid {
        width: screenWidth/25
        height: screenHeight/85*84+Window.height-screenHeight
        padding: screenWidth/170
        ScrollView {
            width: parent.width-parent.padding*2
            height: parent.height-parent.padding*2
            contentWidth: width
            contentHeight: contactsCount*width+(contactsCount-1)*parent.padding
            ColumnLayout {
                spacing: parent.parent.padding
                Repeater {
                    model: groups.length
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