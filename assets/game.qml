import bb.cascades 1.0

Page {
    actions: [
        ActionItem {
            title: qsTr("Flip board")
            imageSource: "asset:///images/flip.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                app.isFlipped = ! app.isFlipped
                if (app.isFlipped == true) flipAnimation.play(); else unflipAnimation.play();
                app.Draw();
            }
        },
        ActionItem {
            title: qsTr("Turn back")
            imageSource: "asset:///images/back.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                app.TurnBack();
                app.Draw();
            }
        },
        ActionItem {
            title: qsTr("Restart")
            imageSource: "asset:///images/new.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (app.GetConfirmation(qsTr("Unsaved progress will be lost")) == true) {
                    app.NewGame();
                    app.Draw();
                }
            }
        },
        ActionItem {
            title: qsTr("Save")
            imageSource: "asset:///images/save.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                app.Save();
            }
        }
    ]
    attachedObjects: [
        Dialog {
            id: endGameNotificationDialog
            onOpened: {
                endGameFadeIn.play();
                extLabelA1.play();
            }
            Container {
                opacity: 0
                animations: [
                    FadeTransition {
                        id: endGameFadeIn
                        fromOpacity: 0.0
                        toOpacity: 1.0
                        duration: 400
                        delay: 500
                    },
                    FadeTransition {
                        id: endGameFadeOut
                        fromOpacity: 1.0
                        toOpacity: 0.0
                        duration: 300
                        onEnded: endGameNotificationDialog.close()
                    }
                ]
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                background: Color.create(0, 0, 0, 0.9)
                layout: DockLayout {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 600
                    layout: DockLayout {
                    }
                    Label {
                        id: endGameNotificationTextLabel
                        text: "end-game-notification"                        
                        textStyle.fontSize: FontSize.XLarge
                        textStyle.color: Color.White
                        textStyle.textAlign: TextAlign.Center
                        multiline: true
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                Label {
                    textStyle.color: Color.White
                    textStyle.fontSize: FontSize.Small
                    text: qsTr("Press and hold to close...")
                    verticalAlignment: VerticalAlignment.Bottom
                    horizontalAlignment: HorizontalAlignment.Center
                    bottomMargin: 30.0                    
                    animations: [
                        ScaleTransition {
                            id: extLabelA1
                            fromX: 1
                            fromY: 1
                            toX: 1.03
                            toY: 1.03
                            duration: 400
                            onEnded: extLabelA2.play()
                        },
                        ScaleTransition {
                            id: extLabelA2
                            fromX: 1.03
                            fromY: 1.03
                            toX: 1
                            toY: 1
                            duration: 400
                            onEnded: extLabelA1.play()
                        }
                    ]
                    multiline: true
                    textStyle.textAlign: TextAlign.Center
                }
                gestureHandlers: [
                    //                    TapHandler {
                    //                        onTapped: {
                    //                        }
                    //                    },
                    LongPressHandler {
                        onLongPressed: endGameFadeOut.play()
                    }
                ]
            }
        }
    ]
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr("Menu")
            onTriggered: {
                console.log(app.isGameEnded);
                if (app.aiThinking) {
                    app.ShowToast(qsTr("Please, wait while AI is thinking"));
                } else {
                    if (! app.isGameEnded) {
                        if (app.GetConfirmation(qsTr("Unsaved progress will be lost")) == true) {
                            navigationPane.pop();
                        }
                    } else {
                        navigationPane.pop();
                    }
                }
            }
        }
    }
    function showEndGameNotification(text) {
        endGameNotificationTextLabel.text = text;
        endGameNotificationDialog.open();
    }
    Container {
        layout: DockLayout {
        }
        background: backgroundPaint.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/bg_crop.png"
            }
        ]
        Container {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Fill
            //background: Color.create(0, 0.5, 1, 0.2)
            //translationY: 16.0                
            bottomPadding: 16
            topPadding: 20            
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                //background: Color.create(1,1,1, 0.4)
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                ImageView {
                    imageSource: "asset:///images/info.png"
                    preferredWidth: 40.0
                    preferredHeight: 40.0
                }
                Label {
                    objectName: "turnInfoLabel"
                    text: "turn-notification"
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.fontSize: FontSize.Small
                }
                ActivityIndicator {
                    visible: app.aiThinking
                    running: app.aiThinking
                    verticalAlignment: VerticalAlignment.Center
                }
            }        
        }
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 740
            Container {
                background: Color.create(1, 0, 0, 0.2)
                horizontalAlignment: HorizontalAlignment.Fill
                bottomMargin: 10.0
                property real indention: 5
                topPadding: indention
                bottomPadding: indention
                leftPadding: indention
                rightPadding: indention
                Container {
                    background: Color.create(1, 1, 1, 0.3)
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    ScrollView {
                        id: topKilledPiecesDockScroll
                        scrollViewProperties.scrollMode: ScrollMode.Horizontal
                        scrollViewProperties.pinchToZoomEnabled: false
                        //scrollViewProperties.maxContentScale: 0.8
                        horizontalAlignment: HorizontalAlignment.Fill
                        minHeight: 90.0
                        Container {
                            id: topKilledPiecesDock
                            objectName: "topKilledPiecesDock"
                            //background: Color.Cyan
                            //preferredHeight: 100
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            attachedObjects: [
                                LayoutUpdateHandler {
                                    onLayoutFrameChanged: {
                                        console.log("Layout Frame: [" + layoutFrame.x + ", " + layoutFrame.y + "," + layoutFrame.width + ", " + layoutFrame.height + "]");
                                        topKilledPiecesDockScroll.scrollToPoint(layoutFrame.width, 0);
                                    }
                                }
                            ]
                            verticalAlignment: VerticalAlignment.Center
                        }
                    }
                }
            }
            Container {
                property real indention: 20
                background: Color.create(0, 0, 0, 0.2)
                preferredHeight: parent.preferredWidth
                preferredWidth: preferredHeight
                layout: DockLayout {
                }
                clipContentToBounds: false
                Container {
                    //id: boardFrame
                    layout: DockLayout {
                    }
                    background: Color.create(0, 0, 0, 0.7)
                    preferredHeight: parent.preferredWidth - 2 * parent.indention
                    preferredWidth: preferredHeight
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    property real indention: 5
                    animations: [
                        RotateTransition {
                            id: flipAnimation
                            toAngleZ: 180
                            duration: 500
                            onEnded: board.setPiecesScaleY(-1)
                        },
                        RotateTransition {
                            id: unflipAnimation
                            toAngleZ: 0
                            duration: 500
                            onEnded: board.setPiecesScaleY(1)
                        }
                    ]
                    Container {
                        id: board
                        objectName: "board"
                        preferredHeight: parent.preferredWidth - 2 * parent.indention
                        preferredWidth: preferredHeight
                        background: Color.White
                        layout: AbsoluteLayout {
                        }
                        property real cellWidth: preferredWidth / 8
                        property real cellHeight: preferredHeight / 8
                        attachedObjects: [
                            ComponentDefinition {
                                id: cellDef
                                source: "Cell.qml"
                            },
                            ComponentDefinition {
                                id: pieceDef
                                source: "Piece.qml"
                            }
                        ]
                        gestureHandlers: [
                            TapHandler {
                                onTapped: {
                                    var x = Math.floor((event.x / board.cellWidth) + 1);
                                    var y = Math.floor(8 - (event.y / board.cellHeight) + 1);
                                    if (x >= 1 && x <= 8 && y >= 1 && y <= 8) board.handleTap(x, y);
                                }
                            }
                        ]
                        function setPiecesRotation(value) {
                            console.log("setPiecesRotation");
                            for (var i = 0; i < board.controls.length; ++ i) {
                                var child = board.controls[i];
                                if (child.isPiece === true) {
                                    child.rotationZ = value;
                                }
                            }
                        }
                        function setPiecesScaleY(value) {
                            console.log("setPiecesRotation");
                            for (var i = 0; i < board.controls.length; ++ i) {
                                var child = board.controls[i];
                                if (child.isPiece === true) {
                                    child.scaleY = value;
                                }
                            }
                        }
                        function handleTap(x, y) {
                            console.log("tap on cell: " + x + ", " + y);
                            app.HandleTap(x, y);
                            app.Draw();
                        }
                        function createBoard() {
                            console.log("board creation started");
                            for (var x = 1; x <= 8; ++ x) {
                                for (var y = 1; y <= 8; ++ y) {
                                    var cell = cellDef.createObject(board);
                                    cell.cellWidth = cellWidth;
                                    cell.cellHeight = cellHeight;
                                    cell.x = x;
                                    cell.y = y;
                                    board.add(cell);
                                }
                            }
                            console.log("board creation ended");
                        }
                        function findCell(x, y) {
                            for (var i = 0; i < board.controls.length; ++ i) {
                                var child = board.controls[i];
                                if (child.isCell === true && child.x === x && child.y === y) return child;
                            }
                        }
                        function createPiece() {
                            console.log("create piece");
                            var piece = pieceDef.createObject(board);
                            piece.cellWidth = cellWidth;
                            piece.cellHeight = cellHeight;
                            board.add(piece)
                            console.log(piece);
                            return piece;
                        }
                        onCreationCompleted: {
                            console.log(" onCreationCompleted");
                            createBoard();
                        }
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                    }
                }
            }
            Container {
                background: Color.create(1, 0, 0, 0.2)
                horizontalAlignment: HorizontalAlignment.Fill
                topMargin: 10.0
                property real indention: 5
                topPadding: indention
                bottomPadding: indention
                leftPadding: indention
                rightPadding: indention
                Container {
                    background: Color.create(1, 1, 1, 0.3)
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    ScrollView {
                        id: bottomKilledPiecesDockScroll
                        scrollViewProperties.scrollMode: ScrollMode.Horizontal
                        scrollViewProperties.pinchToZoomEnabled: false
                        //scrollViewProperties.maxContentScale: 0.8
                        minHeight: 90.0
                        horizontalAlignment: HorizontalAlignment.Fill
                        Container {
                            id: bottomKilledPiecesDock
                            objectName: "bottomKilledPiecesDock"
                            //preferredHeight: 100
                            layout: StackLayout {
                                orientation: LayoutOrientation.LeftToRight
                            }
                            attachedObjects: [
                                LayoutUpdateHandler {
                                    onLayoutFrameChanged: {
                                        console.log("Layout Frame: [" + layoutFrame.x + ", " + layoutFrame.y + "," + layoutFrame.width + ", " + layoutFrame.height + "]");
                                        bottomKilledPiecesDockScroll.scrollToPoint(layoutFrame.width, 0);
                                    }
                                }
                            ]
                            verticalAlignment: VerticalAlignment.Center
                        }
                    }
                }
            }
        }
    }
}
