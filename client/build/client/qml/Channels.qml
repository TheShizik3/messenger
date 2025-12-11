import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates
 
Rectangle {
    width: screenWidth/8
    height: screenHeight/85*84+Window.height-screenHeight
    color: "#2b2b2b"
    radius: screenWidth/300
    anchors.left: parent.left
    anchors.leftMargin: -screenWidth/300
    Grid {
        width: screenWidth/7
        height: screenHeight/85*84+Window.height-screenHeight
        padding: screenWidth/200
        Text {
            text: groups
            color: "#dddddd"
        }
    }
}