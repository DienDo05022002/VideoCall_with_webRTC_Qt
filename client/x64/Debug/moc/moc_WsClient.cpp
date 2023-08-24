/****************************************************************************
** Meta object code from reading C++ file 'WsClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../WsClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WsClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WsClient_t {
    QByteArrayData data[20];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WsClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WsClient_t qt_meta_stringdata_WsClient = {
    {
QT_MOC_LITERAL(0, 0, 8), // "WsClient"
QT_MOC_LITERAL(1, 9, 6), // "sendId"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "myid"
QT_MOC_LITERAL(4, 22, 5), // "login"
QT_MOC_LITERAL(5, 28, 27), // "peerConnectedOrDisconnected"
QT_MOC_LITERAL(6, 56, 12), // "loginSuccess"
QT_MOC_LITERAL(7, 69, 15), // "messageFromPeer"
QT_MOC_LITERAL(8, 85, 7), // "jsonObj"
QT_MOC_LITERAL(9, 93, 13), // "sendLoginText"
QT_MOC_LITERAL(10, 107, 21), // "onTextMessageReceived"
QT_MOC_LITERAL(11, 129, 7), // "message"
QT_MOC_LITERAL(12, 137, 11), // "setUsername"
QT_MOC_LITERAL(13, 149, 8), // "username"
QT_MOC_LITERAL(14, 158, 11), // "getUsername"
QT_MOC_LITERAL(15, 170, 10), // "startLogin"
QT_MOC_LITERAL(16, 181, 8), // "serverIp"
QT_MOC_LITERAL(17, 190, 4), // "port"
QT_MOC_LITERAL(18, 195, 7), // "signOut"
QT_MOC_LITERAL(19, 203, 11) // "getPeerList"

    },
    "WsClient\0sendId\0\0myid\0login\0"
    "peerConnectedOrDisconnected\0loginSuccess\0"
    "messageFromPeer\0jsonObj\0sendLoginText\0"
    "onTextMessageReceived\0message\0setUsername\0"
    "username\0getUsername\0startLogin\0"
    "serverIp\0port\0signOut\0getPeerList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WsClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    0,   77,    2, 0x06 /* Public */,
       5,    0,   78,    2, 0x06 /* Public */,
       6,    0,   79,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   83,    2, 0x0a /* Public */,
      10,    1,   84,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
      12,    1,   87,    2, 0x02 /* Public */,
      14,    0,   90,    2, 0x02 /* Public */,
      15,    2,   91,    2, 0x02 /* Public */,
      18,    0,   96,    2, 0x02 /* Public */,
      19,    0,   97,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   16,   17,
    QMetaType::Void,
    QMetaType::QJsonArray,

       0        // eod
};

void WsClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WsClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendId((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->login(); break;
        case 2: _t->peerConnectedOrDisconnected(); break;
        case 3: _t->loginSuccess(); break;
        case 4: _t->messageFromPeer((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 5: _t->sendLoginText(); break;
        case 6: _t->onTextMessageReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->setUsername((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: { QString _r = _t->getUsername();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->startLogin((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->signOut(); break;
        case 11: { QJsonArray _r = _t->getPeerList();
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WsClient::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsClient::sendId)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WsClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsClient::login)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WsClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsClient::peerConnectedOrDisconnected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WsClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsClient::loginSuccess)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WsClient::*)(QJsonObject );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WsClient::messageFromPeer)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WsClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WsClient.data,
    qt_meta_data_WsClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WsClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WsClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WsClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WsClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void WsClient::sendId(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WsClient::login()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WsClient::peerConnectedOrDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WsClient::loginSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void WsClient::messageFromPeer(QJsonObject _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
