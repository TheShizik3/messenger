import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Window {
    property string stateLogRegWindow: "login"
    minimumHeight: screenHeight/16*7
    minimumWidth: screenWidth/4
    maximumHeight: screenHeight/2
    maximumWidth: screenWidth/4
    color: "#212121"
    title: "Login"
    onClosing: (close) => {
        user.closeWindow();
    }
    Text {
        id: welcomeHeader
        text: "Welcome back!"
        color: "#dddddd"
        font.pixelSize: parent.height/25/7*8
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height/9.5
    }
    Text {
        id: welcomeTxt
        text: "We`re so excited to see you again!"
        color: "#aaaaaa"
        font.pixelSize: parent.height/35/7*8
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        anchors.top: welcomeHeader.bottom
        anchors.topMargin: parent.height/100/7*8
    }
    Text {
        id: nameExplanation
        text: "NAME"
        color: "#aaaaaa"
        font.pixelSize: parent.height/35/7*8
        anchors {
            left: parent.left
            leftMargin: parent.width/15
            top: welcomeTxt.bottom
            topMargin: parent.height/49*6
        }
    }
    Rectangle {
        id: nameFieldBackground
        width: parent.width*13/15
        height: parent.height/16/7*8
        color: "#2b2b2b"
        radius: screenWidth/500
        anchors {
            left: parent.left
            leftMargin: parent.width/15
            top: nameExplanation.bottom
            topMargin: parent.height/100/7*8
        }
        TextField {
            id: nameField
            color: "#dddddd"
            width: parent.width*29/30
            height: parent.height*0.45
            font.pixelSize: height
            anchors {
                top: parent.top
                left: parent.left
                topMargin: (parent.height/7*8-height)/2-1
                leftMargin: (parent.width-width)/2
            }
        }
    }
    Text {
        id: passwordExplanation
        text: "PASSWORD"
        color: "#aaaaaa"
        font.pixelSize: parent.height/35/7*8
        anchors {
            left: parent.left
            leftMargin: parent.width/15
            top: nameFieldBackground.bottom
            topMargin: parent.height/16/7*8-height
        }
    }
    Rectangle {
        id: passwordFieldBackground
        width: parent.width*13/15
        height: parent.height/16/7*8
        color: "#2b2b2b"
        radius: screenWidth/500
        anchors {
            left: parent.left
            leftMargin: parent.width/15
            top: passwordExplanation.bottom
            topMargin: parent.height/100/7*8
        }
        TextField {
            id: passwordField
            color: "#dddddd"
            width: parent.width*29/30
            height: parent.height*0.45/7*8
            font.pixelSize: height
            anchors {
                top: parent.top
                left: parent.left
                topMargin: (parent.height/7*8-height)/2-1
                leftMargin: (parent.width-width)/2
            }
        }
    }
    Rectangle {
        id: logRegBtnBackground
        width: parent.width*13/15
        height: parent.height/16/7*8
        radius: screenWidth/500
        color: "#393939"
        anchors {
                top: passwordFieldBackground.bottom
                topMargin: screenHeight/14-1
                left: parent.left
                leftMargin: parent.width/15
        }
        Button {
            id: logRegBtn
            text: "Login"
            width: parent.width
            height: parent.height
            onClicked: {
                if (stateLogRegWindow == "login") {
                    if (!user.loginUser(passwordField.text, nameField.text)) {
                        parent.visible = false;
                        mainWindow.visible = true;
                    }
                }
                else if (stateLogRegWindow == "register") {
                    if (!user.regUser(passwordField.text, nameField.text)) {
                        parent.visible = false;
                        mainWindow.visible = true;
                    }
                }
            }
            Text {
                text: parent.text
                anchors.centerIn: parent
                color: "#dddddd"
            }
        }
    }
    Text {
        text: "Need an account? Register"
        anchors.top: logRegBtnBackground.bottom
        anchors.topMargin: screenHeight/50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#aaaaaa"
        Button {
            width: parent.width
            height: parent.height
            onClicked: {
                if (stateLogRegWindow == "login") {
                    stateLogRegWindow = "register";
                    logRegBtn.text = "Register";
                    welcomeHeader.text = "Create an account";
                    welcomeTxt.text = "";
                    parent.text = "Already have an account?";
                }
                else if (stateLogRegWindow == "register") {
                    stateLogRegWindow = "login";
                    logRegBtn.text = "Login";
                    welcomeHeader.text = "Welcome back!";
                    welcomeTxt.text = "We`re so excited to see you again!";
                    parent.text = "Need an account? Register";
                }
                passwordField.text = "";
                nameField.text = "";
            }
        }
    }
}
