import bb.cascades 1.0

Container {
    property bool isPiece: true
    property string pieceFENChar: 'p'
    property int x: 1
    property int y: 1
    property real cellWidth: 200    
    property real cellHeight: 200
    
    onPieceFENCharChanged: {
        var source = getImageSource();
        pieceImage.imageSource = source;
    }
    preferredWidth: cellWidth
    preferredHeight: cellHeight
    layout: DockLayout {
    }
    layoutProperties: AbsoluteLayoutProperties {
        positionX: (x - 1) * preferredWidth
        positionY: (8 - y) * preferredHeight
    }
    
    ImageView {
        id: pieceImage
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        imageSource: getImageSource();
    }
    
    function getImageSource()
    {
        switch(pieceFENChar)
        {
            case 'p': return "asset:///images/pieces/black-pawn.png";
            case 'r': return "asset:///images/pieces/black-rock.png";
            case 'n': return "asset:///images/pieces/black-knight.png";
            case 'b': return "asset:///images/pieces/black-bishop.png";
            case 'k': return "asset:///images/pieces/black-king.png";
            case 'q': return "asset:///images/pieces/black-queen.png";            
                        
            case 'P': return "asset:///images/pieces/white-pawn.png";
            case 'R': return "asset:///images/pieces/white-rock.png";
            case 'N': return "asset:///images/pieces/white-knight.png";
            case 'B': return "asset:///images/pieces/white-bishop.png";
            case 'K': return "asset:///images/pieces/white-king.png";
            case 'Q': return "asset:///images/pieces/white-queen.png";
            
            default: return "";
        }
    }        
}
