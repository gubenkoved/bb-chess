/****************************************************************************
** Meta object code from reading C++ file 'Chess.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Chess.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Chess.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Chess[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       3,   99, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   27,    6,    6, 0x08,

 // methods: signature, parameters, type, tag, flags
      76,   72,    6,    6, 0x02,
      95,    6,    6,    6, 0x02,
     102,    6,    6,    6, 0x02,
     113,    6,    6,    6, 0x02,
     129,  123,    6,    6, 0x02,
     143,    6,    6,    6, 0x02,
     154,    6,    6,    6, 0x02,
     165,    6,    6,    6, 0x02,
     185,  177,  172,    6, 0x02,
     210,  177,    6,    6, 0x02,
     243,  229,    6,    6, 0x02,
     277,    6,  172,    6, 0x02,
     294,    6,  172,    6, 0x02,
     315,  309,    6,    6, 0x02,
     334,    6,  172,    6, 0x02,

 // properties: name, type, flags
     350,  172, 0x01095001,
     362,  172, 0x01095103,
     372,  172, 0x01495001,

 // properties: notify_signal_id
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_Chess[] = {
    "Chess\0\0aiThinkingChanged()\0bestMoveString\0"
    "HandleFindedBestMove(QString)\0x,y\0"
    "HandleTap(int,int)\0Draw()\0TurnBack()\0"
    "NewGame()\0depth\0StartPVC(int)\0StartPVP()\0"
    "Continue()\0Save()\0bool\0message\0"
    "GetConfirmation(QString)\0ShowToast(QString)\0"
    "title,message\0ShowNotification(QString,QString)\0"
    "getIsGameEnded()\0getIsFlipped()\0value\0"
    "setIsFlipped(bool)\0getAiThinking()\0"
    "isGameEnded\0isFlipped\0aiThinking\0"
};

void Chess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Chess *_t = static_cast<Chess *>(_o);
        switch (_id) {
        case 0: _t->aiThinkingChanged(); break;
        case 1: _t->HandleFindedBestMove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->HandleTap((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->Draw(); break;
        case 4: _t->TurnBack(); break;
        case 5: _t->NewGame(); break;
        case 6: _t->StartPVC((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->StartPVP(); break;
        case 8: _t->Continue(); break;
        case 9: _t->Save(); break;
        case 10: { bool _r = _t->GetConfirmation((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: _t->ShowToast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->ShowNotification((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 13: { bool _r = _t->getIsGameEnded();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->getIsFlipped();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: _t->setIsFlipped((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: { bool _r = _t->getAiThinking();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Chess::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Chess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Chess,
      qt_meta_data_Chess, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Chess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Chess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Chess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Chess))
        return static_cast<void*>(const_cast< Chess*>(this));
    return QObject::qt_metacast(_clname);
}

int Chess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = getIsGameEnded(); break;
        case 1: *reinterpret_cast< bool*>(_v) = getIsFlipped(); break;
        case 2: *reinterpret_cast< bool*>(_v) = getAiThinking(); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: setIsFlipped(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 3;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Chess::aiThinkingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
