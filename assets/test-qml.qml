import bb.cascades 1.2

Page {
    Container {

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
                        text: '999'
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
                    //background: Color.Red
                    verticalAlignment: VerticalAlignment.Fill
                    Label {
                        verticalAlignment: VerticalAlignment.Center
                        text: 'Authors goes here'
                        //textFit
                        textStyle.fontSize: FontSize.Small
                        //bottomMargin: 0
                        translationY: -10.0
                    }
                    Label {
                        //visible: false 
                        verticalAlignment: VerticalAlignment.Bottom
                        text: 'source goes here (Jul, 2013)'
                        textStyle.fontSize: FontSize.XXSmall
                        textStyle.color: Color.Gray
                    }
                    Container {
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Right
                        //background: Color.Red
                        rightPadding: 40
                        Label {
                            //visible: false
                            
                            text: 'Jul, 1992'
                            textStyle.fontSize: FontSize.XXSmall
                            textStyle.color: Color.Gray
                            rotationZ: 90
                            translationY: 40
                            //rightMargin: 
                        }    
                    }
                    
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Fill
                        preferredWidth: itemRoot.preferredHeight
                        
                        layout: DockLayout {
                        }
                        ImageView {
                            
                            verticalAlignment: VerticalAlignment.Center
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: 48
                            preferredHeight: preferredWidth
                            imageSource: "asset:///images/solved.png"
                        }
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
}
