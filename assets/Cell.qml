import bb.cascades 1.0

Container {
    property bool isCell: true;
    property bool isHighlightAsPossibleDestination: false;
    property bool isHighlightAsPreviousMove: false;
    property bool isHighlightAsSelected: false;
    property int x: 1
    property int y: 1
    property real cellWidth: 100
    property real cellHeight: 100
    opacity: 0.7
    
    preferredWidth: cellWidth
    preferredHeight: cellHeight
    
    background: getColor()
    layoutProperties: AbsoluteLayoutProperties {
        positionX: (x - 1) * preferredWidth
        positionY: (8 - y) * preferredHeight
    }
    function isBlack()
    {
        return ((x + y) % 2) == 0;
    }
    function getColor()
    {
        if (isHighlightAsPossibleDestination)
            return isBlack() ? Color.DarkGreen : Color.Green;
        else if (isHighlightAsPreviousMove)
            return Color.create("#FBA123"); // orange
        else if (isHighlightAsSelected)
            return Color.Cyan;
        else 
            return isBlack() ? Color.Gray : Color.White;            
    }
}