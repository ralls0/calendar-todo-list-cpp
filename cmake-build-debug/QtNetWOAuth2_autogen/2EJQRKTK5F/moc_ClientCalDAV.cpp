/****************************************************************************
** Meta object code from reading C++ file 'ClientCalDAV.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ClientCalDAV/ClientCalDAV.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClientCalDAV.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClientCalDAV_t {
    const uint offsetsAndSize[12];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ClientCalDAV_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ClientCalDAV_t qt_meta_stringdata_ClientCalDAV = {
    {
QT_MOC_LITERAL(0, 12), // "ClientCalDAV"
QT_MOC_LITERAL(13, 18), // "accessTokenChanged"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 20), // "handleUploadFinished"
QT_MOC_LITERAL(54, 14), // "setAccessToken"
QT_MOC_LITERAL(69, 5) // "token"

    },
    "ClientCalDAV\0accessTokenChanged\0\0"
    "handleUploadFinished\0setAccessToken\0"
    "token"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClientCalDAV[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   33,    2, 0x09,    2 /* Protected */,
       4,    1,   34,    2, 0x0a,    3 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void ClientCalDAV::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClientCalDAV *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->accessTokenChanged(); break;
        case 1: _t->handleUploadFinished(); break;
        case 2: _t->setAccessToken((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ClientCalDAV::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ClientCalDAV::accessTokenChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ClientCalDAV::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ClientCalDAV.offsetsAndSize,
    qt_meta_data_ClientCalDAV,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ClientCalDAV_t
, QtPrivate::TypeAndForceComplete<ClientCalDAV, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>


>,
    nullptr
} };


const QMetaObject *ClientCalDAV::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClientCalDAV::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClientCalDAV.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ClientCalDAV::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ClientCalDAV::accessTokenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
