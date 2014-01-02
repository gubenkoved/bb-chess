/****************************************************************************
** Meta object code from reading C++ file 'chessapp.h'
**
** Created: Mon 22. Oct 12:05:10 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/chessapp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chessapp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ChessApp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   10,    9,    9, 0x08,
      46,    9,    9,    9, 0x0a,
      62,   58,   53,    9, 0x0a,
     104,   84,   53,    9, 0x0a,
     136,   84,    9,    9, 0x0a,
     171,   58,  158,    9, 0x0a,
     192,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ChessApp[] = {
    "ChessApp\0\0bestMove\0HandleFindedBestMove(Move)\0"
    "Draw()\0bool\0x,y\0IsFigureCell(int,int)\0"
    "fromX,fromY,toX,toY\0IsPossibleStep(int,int,int,int)\0"
    "Step(int,int,int,int)\0QVariantList\0"
    "GetPossible(int,int)\0TurnBack()\0"
};

void ChessApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ChessApp *_t = static_cast<ChessApp *>(_o);
        switch (_id) {
        case 0: _t->HandleFindedBestMove((*reinterpret_cast< Move(*)>(_a[1]))); break;
        case 1: _t->Draw(); break;
        case 2: { bool _r = _t->IsFigureCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->IsPossibleStep((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->Step((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: { QVariantList _r = _t->GetPossible((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = _r; }  break;
        case 6: _t->TurnBack(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ChessApp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ChessApp::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ChessApp,
      qt_meta_data_ChessApp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ChessApp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ChessApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ChessApp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ChessApp))
        return static_cast<void*>(const_cast< ChessApp*>(this));
    return QObject::qt_metacast(_clname);
}

int ChessApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
