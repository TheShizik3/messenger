import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Templates

Window {
    color: "#212121"
    width: screenWidth
    height: screenHeight
    visible: true
    title: qsTr("Flow")
    ColumnLayout {
        Text {
            color: "#ffffff" 
            text: "Flow"
            width: Window.width
            height: Window.height/45
        }
        RowLayout {
            spacing: 0
            ColumnLayout { 
                Rectangle {
                    width: screenWidth/25
                    height: screenHeight/45*44
                    color: "#212121"
                    Grid {
                        width: screenWidth/25
                        height: screenHeight/45*44
                        padding: screenWidth/200
                        Text {
                            text: "HW!"
                            color: "#ffffff"
                        }
                    }
                }
            }
            Grid {
                height: screenHeight/45*44
                width: Window.width*24/25
                leftPadding: screenWidth/300
                Rectangle {
                    color: "#393939"
                    height: screenHeight/45*44
                    width: screenWidth*24/25-screenWidth/300
                    Grid {
                        height: screenHeight/45*44
                        width: screenWidth*24/25-screenWidth/300
                        leftPadding: -screenWidth/300
                        RowLayout {
                            spacing: 0
                            ColumnLayout {
                                Rectangle {
                                    width: screenWidth/8
                                    height: screenHeight/45*44
                                    color: "#2b2b2b"
                                    radius: screenWidth/300
                                    Grid {
                                        width: screenWidth/7
                                        height: screenHeight/45*44
                                        padding: screenWidth/200
                                        Text {
                                            text: "Hello, world!"
                                            color: "#ffffff"
                                        }
                                }
                                }
                            }
                            ColumnLayout {
                                Rectangle {
                                    width: screenWidth*167/200
                                    height: screenHeight/45*44
                                    color: "#393939"
                                    radius: screenWidth/300
                                    Grid {
                                        width: screenWidth*167/200
                                        height: screenHeight/45*44
                                        padding: screenWidth/100
                                        topPadding: screenWidth/200
                                        ColumnLayout {
                                            spacing: 0
                                            ScrollView {
                                                width: screenWidth*165/200
                                                height: screenHeight*93/100
                                                Text {
                                                    text: l[0]+""
                                                }
                                            }
                                            TextField {
                                                width: screenWidth*3/4
                                                height: screenHeight/25
                                                text: "Hello World"
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
    }
}
