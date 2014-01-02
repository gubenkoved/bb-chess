import bb.cascades 1.2

Page {
    id: puzzleSelector    
    
    // these properties setted by caller page
    property variant app; // Chess class instance here
    property NavigationPane navigationPane; // root naviation pane
    
    onCreationCompleted: {
        puzzlesView.setDataModel(app.GetPuzzlesDataModel())
    }
    
    Container {        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        //background: Color.create(0, 0, 0, 0.9)
        background: backgroundPaint.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/bg.png"
            }
        ]
        layout: DockLayout {
        }

        ActivityIndicator {
            id: loadingIndicator
            enabled: true
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }
                
        ListView {
            id: puzzlesView
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill            
            listItemComponents: [                
                ListItemComponent {
                    type: "listItem"
                    StandardListItem {
                        title: ListItemData.Id + ListItemData.Authors
                        description: ListItemData.PositionFEN
                        status: ListItemData.status
                    }
                }
            ]
        }

    }
}
