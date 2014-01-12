import bb.cascades 1.2

Page {
    id: levelDialog

    actionBarVisibility: ChromeVisibility.Hidden

    // these properties setted by caller page
    property variant app; // Chess class instance here
    property NavigationPane navigationPane; // root naviation pane
            
    Container {        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        //background: Color.create(0, 0, 0, 0.9)
        background: backgroundPaint2.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint2
                imageSource: "asset:///images/bg.png"
            }
        ]
        layout: DockLayout {
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: 600
            layout: StackLayout {
            }
            Button {
                text: qsTr("Easy")
                imageSource: "asset:///images/easy.png"
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    levelDialog.navigationPane.pop();
                    app.StartPVC(4);
                }
            }
            Button {
                text: qsTr("Medium")
                imageSource: "asset:///images/medium.png"
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    //levelDialog.close();
                    levelDialog.navigationPane.pop();
                    app.StartPVC(5);
                }
            }
            Button {
                text: qsTr("Hard")
                imageSource: "asset:///images/hard.png"
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    //levelDialog.close()
                    levelDialog.navigationPane.pop();
                    app.StartPVC(6);
                }
            }
        }
    }
}
