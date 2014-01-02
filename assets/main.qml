// Navigation pane project template
import bb.cascades 1.0

NavigationPane {
    id: navigationPane
    peekEnabled: false
    Page {
        attachedObjects: [
            Dialog {
                id: levelDialog
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
                                levelDialog.close();
                                app.StartPVC(3);
                            }
                        }
                        Button {
                            text: qsTr("Medium")
                            imageSource: "asset:///images/medium.png"
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                levelDialog.close();
                                app.StartPVC(4);
                            }
                        }
                        Button {
                            text: qsTr("Hard")
                            imageSource: "asset:///images/hard.png"
                            horizontalAlignment: HorizontalAlignment.Center
                            onClicked: {
                                levelDialog.close()
                                app.StartPVC(5);
                            }
                        }
                    }
                }
            }
        ]
        Container {
            background: backgroundPaint.imagePaint
            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/bg.png"
                }
            ]
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout {
            }
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                //preferredWidth: 400
                Button {
                    text:  qsTr("Player vs. Blackberry")
                    horizontalAlignment: HorizontalAlignment.Center
                    minWidth: 450
                    onClicked: {
                        levelDialog.open()
                        //app.StartPVC()
                    }
                    imageSource: "asset:///images/pvc.png"
                }
                Button {
                    text: qsTr("Player vs. player")
                    horizontalAlignment: HorizontalAlignment.Center
                    minWidth: 450
                    onClicked: app.StartPVP()
                    imageSource: "asset:///images/user_group.png"
                }
                Button {
                    text: qsTr("Puzzles")
                    horizontalAlignment: HorizontalAlignment.Center
                    minWidth: 450
                    //onClicked: ...
                    imageSource: "asset:///images/puzzle.png"
                }
                Button {
                    text: qsTr("Continue")
                    horizontalAlignment: HorizontalAlignment.Center
                    minWidth: 450
                    onClicked: app.Continue()
                    imageSource: "asset:///images/load.png"
                }
                Button {
                    text: qsTr("Quit")
                    horizontalAlignment: HorizontalAlignment.Center
                    minWidth: 450
                    onClicked: Application.quit()
                    topMargin: 80.0
                }
            }
        }
    }
}
