/****************************************************************************
** Meta object code from reading C++ file 'CalendarClient_ICS.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../CalendarClient/CalendarClient_ICS.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalendarClient_ICS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalendarClient_ICS_t {
    const uint offsetsAndSize[32];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_CalendarClient_ICS_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_CalendarClient_ICS_t qt_meta_stringdata_CalendarClient_ICS = {
    {
QT_MOC_LITERAL(0, 18), // "CalendarClient_ICS"
QT_MOC_LITERAL(19, 11), // "readingDone"
QT_MOC_LITERAL(31, 0), // ""
QT_MOC_LITERAL(32, 20), // "startSynchronization"
QT_MOC_LITERAL(53, 19), // "stopSynchronization"
QT_MOC_LITERAL(73, 7), // "recover"
QT_MOC_LITERAL(81, 10), // "setHostURL"
QT_MOC_LITERAL(92, 7), // "hostURL"
QT_MOC_LITERAL(100, 23), // "handleStateWaitingEntry"
QT_MOC_LITERAL(124, 22), // "handleStateWaitingExit"
QT_MOC_LITERAL(147, 23), // "handleStateReadingEntry"
QT_MOC_LITERAL(171, 22), // "handleStateReadingExit"
QT_MOC_LITERAL(194, 21), // "handleStateErrorEntry"
QT_MOC_LITERAL(216, 24), // "debug_handleTimerTimeout"
QT_MOC_LITERAL(241, 21), // "handleRequestFinished"
QT_MOC_LITERAL(263, 15) // "handleHTTPError"

    },
    "CalendarClient_ICS\0readingDone\0\0"
    "startSynchronization\0stopSynchronization\0"
    "recover\0setHostURL\0hostURL\0"
    "handleStateWaitingEntry\0handleStateWaitingExit\0"
    "handleStateReadingEntry\0handleStateReadingExit\0"
    "handleStateErrorEntry\0debug_handleTimerTimeout\0"
    "handleRequestFinished\0handleHTTPError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalendarClient_ICS[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   92,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   93,    2, 0x0a,    2 /* Public */,
       4,    0,   94,    2, 0x0a,    3 /* Public */,
       5,    0,   95,    2, 0x0a,    4 /* Public */,
       6,    1,   96,    2, 0x0a,    5 /* Public */,
       8,    0,   99,    2, 0x09,    7 /* Protected */,
       9,    0,  100,    2, 0x09,    8 /* Protected */,
      10,    0,  101,    2, 0x09,    9 /* Protected */,
      11,    0,  102,    2, 0x09,   10 /* Protected */,
      12,    0,  103,    2, 0x09,   11 /* Protected */,
      13,    0,  104,    2, 0x09,   12 /* Protected */,
      14,    0,  105,    2, 0x09,   13 /* Protected */,
      15,    0,  106,    2, 0x09,   14 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QUrl,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CalendarClient_ICS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalendarClient_ICS *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->readingDone(); break;
        case 1: _t->startSynchronization(); break;
        case 2: _t->stopSynchronization(); break;
        case 3: _t->recover(); break;
        case 4: { bool _r = _t->setHostURL((*reinterpret_cast< const QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->handleStateWaitingEntry(); break;
        case 6: _t->handleStateWaitingExit(); break;
        case 7: _t->handleStateReadingEntry(); break;
        case 8: _t->handleStateReadingExit(); break;
        case 9: _t->handleStateErrorEntry(); break;
        case 10: _t->debug_handleTimerTimeout(); break;
        case 11: _t->handleRequestFinished(); break;
        case 12: _t->handleHTTPError(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalendarClient_ICS::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_ICS::readingDone)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CalendarClient_ICS::staticMetaObject = { {
    QMetaObject::SuperData::link<CalendarClient::staticMetaObject>(),
    qt_meta_stringdata_CalendarClient_ICS.offsetsAndSize,
    qt_meta_data_CalendarClient_ICS,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_CalendarClient_ICS_t
, QtPrivate::TypeAndForceComplete<CalendarClient_ICS, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<const QUrl, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *CalendarClient_ICS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalendarClient_ICS::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalendarClient_ICS.stringdata0))
        return static_cast<void*>(this);
    return CalendarClient::qt_metacast(_clname);
}

int CalendarClient_ICS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CalendarClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void CalendarClient_ICS::readingDone()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
