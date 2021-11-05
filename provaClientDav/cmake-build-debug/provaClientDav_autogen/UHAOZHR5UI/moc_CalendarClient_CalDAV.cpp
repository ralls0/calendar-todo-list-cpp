/****************************************************************************
** Meta object code from reading C++ file 'CalendarClient_CalDAV.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../CalendarClient/CalendarClient_CalDAV.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CalendarClient_CalDAV.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalendarClient_CalDAV_t {
    const uint offsetsAndSize[104];
    char stringdata0[851];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_CalendarClient_CalDAV_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_CalendarClient_CalDAV_t qt_meta_stringdata_CalendarClient_CalDAV = {
    {
QT_MOC_LITERAL(0, 21), // "CalendarClient_CalDAV"
QT_MOC_LITERAL(22, 11), // "yearChanged"
QT_MOC_LITERAL(34, 0), // ""
QT_MOC_LITERAL(35, 4), // "year"
QT_MOC_LITERAL(40, 12), // "monthChanged"
QT_MOC_LITERAL(53, 5), // "month"
QT_MOC_LITERAL(59, 15), // "usernameChanged"
QT_MOC_LITERAL(75, 8), // "username"
QT_MOC_LITERAL(84, 15), // "passwordChanged"
QT_MOC_LITERAL(100, 8), // "password"
QT_MOC_LITERAL(109, 16), // "syncTokenChanged"
QT_MOC_LITERAL(126, 22), // "syncTokenHasNotChanged"
QT_MOC_LITERAL(149, 21), // "calendarHasNotChanged"
QT_MOC_LITERAL(171, 22), // "calendarUpdateRequired"
QT_MOC_LITERAL(194, 7), // "getYear"
QT_MOC_LITERAL(202, 7), // "setYear"
QT_MOC_LITERAL(210, 8), // "getMonth"
QT_MOC_LITERAL(219, 8), // "setMonth"
QT_MOC_LITERAL(228, 11), // "setUsername"
QT_MOC_LITERAL(240, 11), // "getUsername"
QT_MOC_LITERAL(252, 11), // "setPassword"
QT_MOC_LITERAL(264, 11), // "getPassword"
QT_MOC_LITERAL(276, 20), // "startSynchronization"
QT_MOC_LITERAL(297, 19), // "stopSynchronization"
QT_MOC_LITERAL(317, 7), // "recover"
QT_MOC_LITERAL(325, 9), // "saveEvent"
QT_MOC_LITERAL(335, 3), // "uid"
QT_MOC_LITERAL(339, 8), // "filename"
QT_MOC_LITERAL(348, 7), // "summary"
QT_MOC_LITERAL(356, 8), // "location"
QT_MOC_LITERAL(365, 11), // "description"
QT_MOC_LITERAL(377, 5), // "rrule"
QT_MOC_LITERAL(383, 6), // "exdate"
QT_MOC_LITERAL(390, 13), // "startDateTime"
QT_MOC_LITERAL(404, 11), // "endDateTime"
QT_MOC_LITERAL(416, 11), // "deleteEvent"
QT_MOC_LITERAL(428, 4), // "href"
QT_MOC_LITERAL(433, 15), // "handleHTTPError"
QT_MOC_LITERAL(449, 30), // "handleRequestSyncTokenFinished"
QT_MOC_LITERAL(480, 28), // "handleRequestChangesFinished"
QT_MOC_LITERAL(509, 23), // "handleStateWaitingEntry"
QT_MOC_LITERAL(533, 22), // "handleStateWaitingExit"
QT_MOC_LITERAL(556, 35), // "handleStateRequestingSyncToke..."
QT_MOC_LITERAL(592, 34), // "handleStateRequestingSyncToke..."
QT_MOC_LITERAL(627, 33), // "handleStateRequestingChangesE..."
QT_MOC_LITERAL(661, 32), // "handleStateRequestingChangesExit"
QT_MOC_LITERAL(694, 33), // "handleStateProcessingChangesE..."
QT_MOC_LITERAL(728, 32), // "handleStateProcessingChangesExit"
QT_MOC_LITERAL(761, 21), // "handleStateErrorEntry"
QT_MOC_LITERAL(783, 24), // "debug_handleTimerTimeout"
QT_MOC_LITERAL(808, 21), // "handleUploadHTTPError"
QT_MOC_LITERAL(830, 20) // "handleUploadFinished"

    },
    "CalendarClient_CalDAV\0yearChanged\0\0"
    "year\0monthChanged\0month\0usernameChanged\0"
    "username\0passwordChanged\0password\0"
    "syncTokenChanged\0syncTokenHasNotChanged\0"
    "calendarHasNotChanged\0calendarUpdateRequired\0"
    "getYear\0setYear\0getMonth\0setMonth\0"
    "setUsername\0getUsername\0setPassword\0"
    "getPassword\0startSynchronization\0"
    "stopSynchronization\0recover\0saveEvent\0"
    "uid\0filename\0summary\0location\0description\0"
    "rrule\0exdate\0startDateTime\0endDateTime\0"
    "deleteEvent\0href\0handleHTTPError\0"
    "handleRequestSyncTokenFinished\0"
    "handleRequestChangesFinished\0"
    "handleStateWaitingEntry\0handleStateWaitingExit\0"
    "handleStateRequestingSyncTokenEntry\0"
    "handleStateRequestingSyncTokenExit\0"
    "handleStateRequestingChangesEntry\0"
    "handleStateRequestingChangesExit\0"
    "handleStateProcessingChangesEntry\0"
    "handleStateProcessingChangesExit\0"
    "handleStateErrorEntry\0debug_handleTimerTimeout\0"
    "handleUploadHTTPError\0handleUploadFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalendarClient_CalDAV[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       4,  302, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  230,    2, 0x06,    5 /* Public */,
       4,    1,  233,    2, 0x06,    7 /* Public */,
       6,    1,  236,    2, 0x06,    9 /* Public */,
       8,    1,  239,    2, 0x06,   11 /* Public */,
      10,    0,  242,    2, 0x06,   13 /* Public */,
      11,    0,  243,    2, 0x06,   14 /* Public */,
      12,    0,  244,    2, 0x06,   15 /* Public */,
      13,    0,  245,    2, 0x06,   16 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      14,    0,  246,    2, 0x10a,   17 /* Public | MethodIsConst  */,
      15,    1,  247,    2, 0x0a,   18 /* Public */,
      16,    0,  250,    2, 0x10a,   20 /* Public | MethodIsConst  */,
      17,    1,  251,    2, 0x0a,   21 /* Public */,
      18,    1,  254,    2, 0x0a,   23 /* Public */,
      19,    0,  257,    2, 0x10a,   25 /* Public | MethodIsConst  */,
      20,    1,  258,    2, 0x0a,   26 /* Public */,
      21,    0,  261,    2, 0x10a,   28 /* Public | MethodIsConst  */,
      22,    0,  262,    2, 0x0a,   29 /* Public */,
      23,    0,  263,    2, 0x0a,   30 /* Public */,
      24,    0,  264,    2, 0x0a,   31 /* Public */,
      25,    9,  265,    2, 0x0a,   32 /* Public */,
      35,    1,  284,    2, 0x0a,   42 /* Public */,
      37,    0,  287,    2, 0x09,   44 /* Protected */,
      38,    0,  288,    2, 0x09,   45 /* Protected */,
      39,    0,  289,    2, 0x09,   46 /* Protected */,
      40,    0,  290,    2, 0x09,   47 /* Protected */,
      41,    0,  291,    2, 0x09,   48 /* Protected */,
      42,    0,  292,    2, 0x09,   49 /* Protected */,
      43,    0,  293,    2, 0x09,   50 /* Protected */,
      44,    0,  294,    2, 0x09,   51 /* Protected */,
      45,    0,  295,    2, 0x09,   52 /* Protected */,
      46,    0,  296,    2, 0x09,   53 /* Protected */,
      47,    0,  297,    2, 0x09,   54 /* Protected */,
      48,    0,  298,    2, 0x09,   55 /* Protected */,
      49,    0,  299,    2, 0x09,   56 /* Protected */,
      50,    0,  300,    2, 0x09,   57 /* Protected */,
      51,    0,  301,    2, 0x09,   58 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QDateTime, QMetaType::QDateTime,   26,   27,   28,   29,   30,   31,   32,   33,   34,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       3, QMetaType::Int, 0x00015103, uint(0), 0,
       5, QMetaType::Int, 0x00015103, uint(1), 0,
       7, QMetaType::QString, 0x00015103, uint(2), 0,
       9, QMetaType::QString, 0x00015103, uint(3), 0,

       0        // eod
};

void CalendarClient_CalDAV::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalendarClient_CalDAV *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->yearChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->monthChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 2: _t->usernameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->passwordChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->syncTokenChanged(); break;
        case 5: _t->syncTokenHasNotChanged(); break;
        case 6: _t->calendarHasNotChanged(); break;
        case 7: _t->calendarUpdateRequired(); break;
        case 8: { int _r = _t->getYear();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->setYear((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 10: { int _r = _t->getMonth();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->setMonth((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 12: _t->setUsername((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: { QString _r = _t->getUsername();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->setPassword((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: { QString _r = _t->getPassword();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->startSynchronization(); break;
        case 17: _t->stopSynchronization(); break;
        case 18: _t->recover(); break;
        case 19: _t->saveEvent((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])),(*reinterpret_cast< QString(*)>(_a[6])),(*reinterpret_cast< QString(*)>(_a[7])),(*reinterpret_cast< QDateTime(*)>(_a[8])),(*reinterpret_cast< QDateTime(*)>(_a[9]))); break;
        case 20: _t->deleteEvent((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->handleHTTPError(); break;
        case 22: _t->handleRequestSyncTokenFinished(); break;
        case 23: _t->handleRequestChangesFinished(); break;
        case 24: _t->handleStateWaitingEntry(); break;
        case 25: _t->handleStateWaitingExit(); break;
        case 26: _t->handleStateRequestingSyncTokenEntry(); break;
        case 27: _t->handleStateRequestingSyncTokenExit(); break;
        case 28: _t->handleStateRequestingChangesEntry(); break;
        case 29: _t->handleStateRequestingChangesExit(); break;
        case 30: _t->handleStateProcessingChangesEntry(); break;
        case 31: _t->handleStateProcessingChangesExit(); break;
        case 32: _t->handleStateErrorEntry(); break;
        case 33: _t->debug_handleTimerTimeout(); break;
        case 34: _t->handleUploadHTTPError(); break;
        case 35: _t->handleUploadFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalendarClient_CalDAV::*)(const int & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::yearChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)(const int & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::monthChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::usernameChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::passwordChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::syncTokenChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::syncTokenHasNotChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::calendarHasNotChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CalendarClient_CalDAV::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalendarClient_CalDAV::calendarUpdateRequired)) {
                *result = 7;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<CalendarClient_CalDAV *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getYear(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->getMonth(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getUsername(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getPassword(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<CalendarClient_CalDAV *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setYear(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setMonth(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setUsername(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setPassword(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject CalendarClient_CalDAV::staticMetaObject = { {
    QMetaObject::SuperData::link<CalendarClient::staticMetaObject>(),
    qt_meta_stringdata_CalendarClient_CalDAV.offsetsAndSize,
    qt_meta_data_CalendarClient_CalDAV,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_CalendarClient_CalDAV_t
, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<int, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<QString, std::true_type>, QtPrivate::TypeAndForceComplete<CalendarClient_CalDAV, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>
, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int &, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const int &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QDateTime, std::false_type>, QtPrivate::TypeAndForceComplete<QDateTime, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *CalendarClient_CalDAV::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalendarClient_CalDAV::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalendarClient_CalDAV.stringdata0))
        return static_cast<void*>(this);
    return CalendarClient::qt_metacast(_clname);
}

int CalendarClient_CalDAV::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CalendarClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 36;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CalendarClient_CalDAV::yearChanged(const int & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CalendarClient_CalDAV::monthChanged(const int & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CalendarClient_CalDAV::usernameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CalendarClient_CalDAV::passwordChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CalendarClient_CalDAV::syncTokenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CalendarClient_CalDAV::syncTokenHasNotChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CalendarClient_CalDAV::calendarHasNotChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CalendarClient_CalDAV::calendarUpdateRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
