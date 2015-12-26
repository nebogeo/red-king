/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      29,   27,   11,   11, 0x08,
      48,   46,   11,   11, 0x08,
      63,   46,   11,   11, 0x08,
      78,   46,   11,   11, 0x08,
      93,   46,   11,   11, 0x08,
     108,   46,   11,   11, 0x08,
     121,   46,   11,   11, 0x08,
     138,   46,   11,   11, 0x08,
     158,   46,   11,   11, 0x08,
     170,   46,   11,   11, 0x08,
     182,   46,   11,   11, 0x08,
     197,   46,   11,   11, 0x08,
     212,   46,   11,   11, 0x08,
     228,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0slot_restart()\0s\0"
    "slot_model(bool)\0v\0slot_amin(int)\0"
    "slot_amax(int)\0slot_umin(int)\0"
    "slot_umax(int)\0slot_ap(int)\0"
    "slot_betmin(int)\0slot_bemaxtime(int)\0"
    "slot_g(int)\0slot_h(int)\0slot_vmin(int)\0"
    "slot_vmax(int)\0slot_betap(int)\0"
    "update_model()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->slot_restart(); break;
        case 1: _t->slot_model((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slot_amin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slot_amax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slot_umin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->slot_umax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slot_ap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slot_betmin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->slot_bemaxtime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slot_g((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slot_h((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->slot_vmin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->slot_vmax((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->slot_betap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->update_model(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
