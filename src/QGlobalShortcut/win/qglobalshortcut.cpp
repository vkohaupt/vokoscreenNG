#include "qglobalshortcut.h"
#include <QApplication>
#include <QStringList>
#include <QKeySequence>
#include "windows.h"

namespace
{
    QString strShortcuts[56] = {"Esc","Tab","BackTab","Backspace","Return","Enter","Ins","Del",
                             "Pause", "Print","SysReq","Clear","Home","End","Left","Up","Right",
                             "Down","PgUp","PgDown","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",
                             "F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24",
                             "Space","*",",","-","/","Media Next","Media Previous","Media Play","Media Stop",
                             "Volume Down","Volume Up","Volume Mute"};

    unsigned int codeShortcuts[56] = {VK_ESCAPE,VK_TAB,VK_TAB,VK_BACK,VK_RETURN,VK_RETURN,VK_INSERT,VK_DELETE,
                                     VK_PAUSE,VK_PRINT,VK_SNAPSHOT,VK_CLEAR,VK_HOME,VK_END,VK_LEFT,VK_UP,VK_RIGHT,
                                     VK_DOWN,VK_PRIOR,VK_NEXT,VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12,
                                     VK_F13,VK_F14,VK_F15,VK_F16,VK_F17,VK_F18,VK_F19,VK_F20,VK_F21,VK_F22,VK_F23,VK_F24,
                                     VK_SPACE,VK_MULTIPLY,VK_SEPARATOR,VK_SUBTRACT,VK_DIVIDE,VK_MEDIA_NEXT_TRACK,VK_MEDIA_PREV_TRACK,VK_MEDIA_PLAY_PAUSE,VK_MEDIA_STOP,
                                     VK_VOLUME_DOWN,VK_VOLUME_UP,VK_VOLUME_MUTE};
}

class QGlobalData
{
    Q_PROPERTY(unsigned int id READ id WRITE setId)

public:
    QGlobalData() {}

    QGlobalData(const QGlobalData &other) :
        m_id(other.m_id)
    {

    }

    unsigned int id(){return m_id;}
    void setId(unsigned int id){m_id = id;}

private:
    unsigned int m_id;
};

class QGlobalShortcutPrivate
{
public:
    QKeySequence keys;
    QList<QGlobalData*>listKeys;
    QHash <QString, unsigned int>hash;
    bool enabled;

    QGlobalShortcutPrivate() {

    }

    void initHash()
    {
        for(int i = 0; i < 56; i++){
            hash.insert(strShortcuts[i],codeShortcuts[i]);
        }
    }

    unsigned int winHotKey(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        QString str = list.last();
        if(str.length() == 0){
            return VK_ADD;
        } else if(str.length() == 1){
            return str.at(0).unicode(); // return Key Letters and Numbers
        } else {
            return this->hash.value(str);
        }
        return 0;
    }

    unsigned int winKeyModificator(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        unsigned int keyModificator = 0;

        foreach (QString str, list) {
            if(str == "Ctrl"){
                keyModificator |= MOD_CONTROL;
                continue;
            } else if(str == "Alt"){
                keyModificator |= MOD_ALT;
                continue;
            } else if(str == "Shift"){
                keyModificator |= MOD_SHIFT;
                continue;
            } else if(str == "Meta"){
                keyModificator |= MOD_WIN;
                continue;
            }
        }
        return keyModificator;
    }

    unsigned int winId(const QKeySequence &keySequence)
    {
        return this->winHotKey(keySequence) ^ this->winKeyModificator(keySequence);
    }
};

QGlobalShortcut::QGlobalShortcut(QObject *parent) :
    QObject(parent),
    sPrivate(new QGlobalShortcutPrivate)
{
    sPrivate->enabled = true;
    sPrivate->initHash();
    qApp->installNativeEventFilter(this);
}

QGlobalShortcut::~QGlobalShortcut()
{
    unsetShortcut();
    qApp->removeNativeEventFilter(this);
    delete sPrivate;
}

bool QGlobalShortcut::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    if(!sPrivate->keys.isEmpty() && sPrivate->enabled){
        MSG* msg = reinterpret_cast<MSG*>(message);
        if(msg->message == WM_HOTKEY){
            foreach (QGlobalData *data, sPrivate->listKeys) {
                if(msg->wParam == data->id()){
                    emit activated();
                    return true;
                }
            }
        }
    }
    return false;
}

bool QGlobalShortcut::setShortcut(const QKeySequence &keySequence)
{
    unsetShortcut();
    sPrivate->keys = keySequence;
    QStringList list = sPrivate->keys.toString().split(", ");
    foreach (QString str, list) {
        QGlobalData * data = new QGlobalData();
        data->setId(sPrivate->winId(QKeySequence(str)));
        sPrivate->listKeys.append(data);
        RegisterHotKey(0, data->id(), sPrivate->winKeyModificator(QKeySequence(str)), sPrivate->winHotKey(QKeySequence(str)));
    }
    return true;
}

bool QGlobalShortcut::unsetShortcut()
{
    if(!sPrivate->keys.isEmpty()){
        foreach (QGlobalData *data, sPrivate->listKeys) {
            UnregisterHotKey(0, data->id());
        }
        sPrivate->listKeys.clear();
    }
    return true;
}

QKeySequence QGlobalShortcut::shortcut()
{
    if(!sPrivate->keys.isEmpty()){
        return sPrivate->keys;
    } else {
        return QKeySequence("");
    }
}

bool QGlobalShortcut::isEmpty()
{
    return sPrivate->keys.isEmpty();
}

void QGlobalShortcut::setEnabled(bool enable)
{
    sPrivate->enabled = enable;
}

bool QGlobalShortcut::isEnabled()
{
    return sPrivate->enabled;
}
