/****************************************************************************
** Meta object code from reading C++ file 'CalendarManager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../CalendarClient/CalendarManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalendarManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalendarManager_t {
    const uint offsetsAndSize[58];
    char stringdata0[370];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_CalendarManager_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_CalendarManager_t qt_meta_stringdata_CalendarManager = {
    {
QT_MOC_LITERAL(0, 15), // "CalendarManager"
QT_MOC_LITERAL(16, 11), // "dateChanged"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 22), // "listOfCalendarsChanged"
QT_MOC_LITERAL(52, 19), // "listOfEventsChanged"
QT_MOC_LITERAL(72, 13), // "eventsUpdated"
QT_MOC_LITERAL(86, 7), // "getDate"
QT_MOC_LITERAL(94, 7), // "setDate"
QT_MOC_LITERAL(102, 7), // "newDate"
QT_MOC_LITERAL(110, 18), // "getListOfCalendars"
QT_MOC_LITERAL(129, 15), // "QList<QObject*>"
QT_MOC_LITERAL(145, 15), // "getListOfEvents"
QT_MOC_LITERAL(161, 18), // "addCalDAV_Calendar"
QT_MOC_LITERAL(180, 5), // "color"
QT_MOC_LITERAL(186, 12), // "calendarName"
QT_MOC_LITERAL(199, 3), // "url"
QT_MOC_LITERAL(203, 8), // "username"
QT_MOC_LITERAL(212, 8), // "password"
QT_MOC_LITERAL(221, 15), // "addICS_Calendar"
QT_MOC_LITERAL(237, 13), // "getListItemAt"
QT_MOC_LITERAL(251, 15), // "CalendarClient*"
QT_MOC_LITERAL(267, 5), // "index"
QT_MOC_LITERAL(273, 16), // "removeListItemAt"
QT_MOC_LITERAL(290, 16), // "getCalendarCount"
QT_MOC_LITERAL(307, 17), // "handleEventUpdate"
QT_MOC_LITERAL(325, 13), // "eventsForDate"
QT_MOC_LITERAL(339, 4), // "date"
QT_MOC_LITERAL(344, 15), // "listOfCalendars"
QT_MOC_LITERAL(360, 9) // "eventList"

    },
    "CalendarManager\0dateChanged\0\0"
    "listOfCalendarsChanged\0listOfEventsChanged\0"
    "eventsUpdated\0getDate\0setDate\0newDate\0"
    "getListOfCalendars\0QList<QObject*>\0"
    "getListOfEvents\0addCalDAV_Calendar\0"
    "color\0calendarName\0url\0username\0"
    "password\0addICS_Calendar\0getListItemAt\0"
    "CalendarClient*\0index\0removeListItemAt\0"
    "getCalendarCount\0handleEventUpdate\0"
    "eventsForDate\0date\0listOfCalendars\0"
    "eventList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalendarManager[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       3,  150, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  110,    2, 0x06,    4 /* Public */,
       3,    0,  111,    2, 0x06,    5 /* Public */,
       4,    0,  112,    2, 0x06,    6 /* Public */,
       5,    0,  113,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  114,    2, 0x10a,    8 /* Public | MethodIsConst  */,
       7,    1,  115,    2, 0x0a,    9 /* Public */,
       9,    0,  118,    2, 0x0a,   11 /* Public */,
      11,    0,  119,    2, 0x0a,   12 /* Public */,
      12,    5,  120,    2, 0x0a,   13 /* Public */,
      18,    3,  131,    2, 0x0a,   19 /* Public */,
      19,    1,  138,    2, 0x0a,   23 /* Public */,
      22,    1,  141,    2, 0x0a,   25 /* Public */,
      22,    0,  144,    2, 0x2a,   27 /* Public | MethodCloned */,
      23,    0,  145,    2, 0x10a,   28 /* Public | MethodIsConst  */,
      24,    0,  146,    2, 0x09,   29 /* Protected */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      25,    1,  147,    2, 0x02,   30 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::QDate,
    QMetaType::Void, QMetaType::QDate,    8,
    0x80000000 | 10,
    0x80000000 | 10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QUrl, QMetaType::QString, QMetaType::QString,   13,   14,   15,   16,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QUrl,   13,   14,   15,
    0x80000000 | 20, QMetaType::Int,   21,
    QMetaType::Bool, QMetaType::Int,   21,
    QMetaType::Bool,
    QMetaType::Int,
    QMetaType::Void,

 // methods: parameters
    0x80000000 | 10, QMetaType::QDate,   26,

 // properties: name, type, flags
      26, QMetaType::QDate, 0x00015103, uint(0), 0,
      27, 0x80000000 | 10, 0x00015009, uint(1), 0,
      28, 0x80000000 | 10, 0x00015009, uint(2), 0,

       0        // eod
};

void CalendarManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalendarManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dateChanged(); break;
        case 1: _t->listOfCalendarsChanged(); break;
        case 2: _t->listOfEventsChanged(); break;
        case 3: _t->eventsUpdated(); break;
        case 4: { QDate _r = _t->getDate();
            if (_a[0]) *reinterpret_cast< QDate*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->setDate((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 6: { QList<QObject*> _r = _t->getListOfCalendars();
            if (_a[0]) *reinterpret_cast< QList<QObject*>*>(_a[0]) = std::move(_r); }  break;
        case 7: { QList<QObject*> _r = _t->getListOfEvents();
            if (_a[0]) *reinterpret_cast< QList<QObject*>*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->addCalDAV_Calendar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QUrl(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5]))); break;
        case 9: _t->addICS_Calendar((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QUrl(*)>(_a[3]))); break;
        case 10: { CalendarClient* _r = _t->getListItemAt((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< CalendarClient**>(_a[0]) = std::move(_r); }  break;
        case 11: { bool _r = _t->removeListItemAt((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->removeListItemAt();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->getCalendarCount();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->handleEventUpdate(); break;
        case 15: { QList<QObject*> _r = _t->eventsForDate((*reinterpret_cast< const QDate(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QList<QObject*>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalendarManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarManager::dateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalendarManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarManager::listOfCalendarsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalendarManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarManager::listOfEventsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CalendarManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarManager::eventsUpdated)) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QObject*> >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CalendarManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QDate*>(_v) = _t->getDate(); break;
        case 1: *reinterpret_cast< QList<QObject*>*>(_v) = _t->getListOfCalendars(); break;
        case 2: *reinterpret_cast< QList<QObject*>*>(_v) = _t->getListOfEvents(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<CalendarManager *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDate(*reinterpret_cast< QDate*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject CalendarManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CalendarManager.offsetsAndSize,
    qt_meta_data_CalendarManager,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_CalendarManager_t
, QtPrivate::TypeAndForceComplete<QDate, std::true_type>, QtPrivate::TypeAndForceComplete<QList<QObject*>, std::true_type>, QtPrivate::TypeAndForceComplete<QList<QObject*>, std::true_type>, QtPrivate::TypeAndForceComplete<CalendarManager, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<QDate, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>, QtPrivate::TypeAndForceComplete<QList<QObject*>, std::false_type>, QtPrivate::TypeAndForceComplete<QList<QObject*>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QUrl, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QUrl, std::false_type>, QtPrivate::TypeAndForceComplete<CalendarClient *, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<QList<QObject*>, std::false_type>, QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>

>,
    nullptr
} };


const QMetaObject *CalendarManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalendarManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalendarManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CalendarManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CalendarManager::dateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CalendarManager::listOfCalendarsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CalendarManager::listOfEventsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CalendarManager::eventsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
