import bb.cascades 1.2

Page {
    id: puzzleSelector
    actions: [
        ActionItem {
            title: qsTr("Reset progress")
            imageSource: "asset:///images/remove-white.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (app.GetConfirmation(qsTr("All progress will be lost")) == true) {
                    // todo
                }
            }
        },
        ActionItem {
            id: sortBySource
            title: qsTr("Sort by source")
            //imageSource: "asset:///images/remove-white.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                puzzlesView.dataModel.sortingKeys = [ 'source' ];
                puzzlesViewDataModel.grouping = ItemGrouping.ByFullValue;
                sortBySource.enabled = !sortBySource.enabled;
            }
        },
        ActionItem {
            id: sortByAuthors
            enabled: false
            title: qsTr("Sort by authors")
            //imageSource: "asset:///images/remove-white.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            onTriggered: {
                puzzlesView.dataModel.sortingKeys = [ 'authors', 'id' ];                
                puzzlesViewDataModel.grouping = ItemGrouping.ByFirstChar;
                sortByAuthors.enabled = !sortByAuthors.enabled;
            }
        },
        ActionItem {
                    title: qsTr("Change sort order")
                    //imageSource: "asset:///images/remove-white.png"
                    ActionBar.placement: ActionBarPlacement.InOverflow
                    onTriggered: {
                        puzzlesViewDataModel.sortedAscending = !puzzlesViewDataModel.sortedAscending;
                    }
        }
    ]
    // these properties setted by caller page
    property variant app; // Chess class instance here
    property NavigationPane navigationPane; // root naviation pane

    function bindToDataModel() {        
        var model = app.GetPuzzlesDataModel();
        puzzlesViewDataModel.insertList(model)
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        //background: Color.create(0, 0, 0, 0.9)
        //background: backgroundPaint.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/bg.png"
            }
        ]
        layout: DockLayout {
        }

        Container {
            //background: Color.create(1, 0, 0, 0.1)
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            ListView {
                // for ability to find it by QObject::findChild method
                objectName: "puzzlesView"
                id: puzzlesView
                dataModel: GroupDataModel {
                    id: puzzlesViewDataModel
                    sortingKeys: ['authors', 'id']
                }
                onTriggered: {
                    console.log('clicked at ' + indexPath);
                }
                listItemComponents: [
//                    ListItemComponent {
//                        type: 'header'
//                        Container {
//                            id: headerRoot
//                            //preferredHeight: 90
//                            layout: DockLayout {
//                            }
//                            Label {
//                                text: {
//                                    var key = puzzlesView.dataModel.sortingKeys[0];
//                                    
//                                    if (key == 'source')
//                                        return ListItemData.source;
//                                    else 
//                                        return ListItemData.authors;
//                                    
//                                    //return ListItemData[key];
//                                }
//                            }
//                        }
//                    },
                    ListItemComponent {
                        type: 'item'
                        Container {
                            id: itemRoot
                            preferredHeight: 90
                            layout: DockLayout {
                            }
                            Container {
                                verticalAlignment: VerticalAlignment.Fill
                                horizontalAlignment: HorizontalAlignment.Fill
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }

                                Container {
                                    verticalAlignment: VerticalAlignment.Fill
                                    preferredWidth: itemRoot.preferredHeight

                                    layout: DockLayout {
                                    }
                                    Label {
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        text: ListItemData.id
                                        textStyle.fontWeight: FontWeight.W100
                                        textStyle.textAlign: TextAlign.Center
                                    }
                                }

                                Container {
                                    layout: DockLayout {
                                    }
                                    layoutProperties: StackLayoutProperties {
                                        spaceQuota: 1
                                    }
                                    verticalAlignment: VerticalAlignment.Fill
                                    Label {
                                        verticalAlignment: VerticalAlignment.Center
                                        text: ListItemData.authors
                                        //textFit
                                        textStyle.fontSize: FontSize.Small
                                        //bottomMargin: 0
                                        translationY: ListItemData.source != '' ? -10.0 : 0
                                    }
                                    Label {
                                        verticalAlignment: VerticalAlignment.Bottom
                                        text: ListItemData.source
                                        textStyle.fontSize: FontSize.XXSmall
                                        textStyle.color: Color.Gray
                                    }
                                    Label {
                                        visible: false
                                        verticalAlignment: VerticalAlignment.Bottom
                                        horizontalAlignment: HorizontalAlignment.Right
                                        text: ListItemData.date
                                        textStyle.fontSize: FontSize.XXSmall
                                        textStyle.color: Color.Gray
                                    }
                                }

                                Container {
                                    verticalAlignment: VerticalAlignment.Fill
                                    preferredWidth: itemRoot.preferredHeight
                                    visible: ListItemData.solved
                                    layout: DockLayout {
                                    }
                                    ImageView {
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Center
                                        preferredWidth: 64
                                        preferredHeight: preferredWidth
                                        imageSource: ListItemData.solved ? "asset:///images/solved.png" : ""
                                    }
                                }

                            }

                            Container {
                                background: Color.create(0, 0, 0, 0.1)
                                preferredHeight: 2
                                horizontalAlignment: HorizontalAlignment.Fill
                            }
                        }

                    }
                ]
            }
        }

    }
}
