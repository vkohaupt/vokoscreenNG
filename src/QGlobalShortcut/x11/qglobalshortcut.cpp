#include "qglobalshortcut.h"
#include <QApplication>
#include <QStringList>
#include <QKeySequence>
#include <QVector>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xcb/xcb.h>


namespace
{
    QString strShortcuts[49] = {"Esc","Tab","BackTab","Backspace","Return","Enter","Ins","Del",
                             "Pause", "Print","SysReq","Clear","Home","End","Left","Up","Right",
                             "Down","PgUp","PgDown","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",
                             "F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24",
                             "Space","*",",","-","/"};

    unsigned int codeShortcuts[49] = {XK_Escape,XK_Tab,XK_Tab,XK_BackSpace,XK_Return,XK_Return,XK_Insert,XK_Delete,
                                     XK_Pause,XK_Print,XK_Sys_Req,XK_Clear,XK_Home,XK_End,XK_Left,XK_Up,XK_Right,
                                     XK_Down,XK_Page_Up,XK_Page_Down,XK_F1,XK_F2,XK_F3,XK_F4,XK_F5,XK_F6,XK_F7,XK_F8,XK_F9,XK_F10,XK_F11,XK_F12,
                                     XK_F13,XK_F14,XK_F15,XK_F16,XK_F17,XK_F18,XK_F19,XK_F20,XK_F21,XK_F22,XK_F23,XK_F24,
                                     XK_space,XK_asterisk,XK_comma,XK_minus,XK_slash};
}

class QGlobalData
{
    Q_PROPERTY(unsigned int key READ key WRITE setKey)
    Q_PROPERTY(unsigned int modifier READ modifier WRITE setModifier)

public:
    QGlobalData() {}

    QGlobalData(const QGlobalData &other) :
        m_key(other.m_key),
        m_modifier(other.m_modifier)
    {

    }

    unsigned int key(){return m_key;}
    unsigned int modifier(){return m_modifier;}
    void setkey(unsigned int key){m_key = key;}
    void setModifier(unsigned int modifier){m_modifier = modifier;}

private:
    unsigned int m_key;
    unsigned int m_modifier;
};

class QGlobalShortcutPrivate
{
public:
    QKeySequence keys;
    QList<QGlobalData*>listKeys;
    QHash <QString, unsigned int>hash;
    bool enabled;
    Display * m_display;
    Window m_win;

    QGlobalShortcutPrivate() {

    }

    void initHash()
    {
        for(int i = 0; i < 49; i++){
            hash.insert(strShortcuts[i],codeShortcuts[i]);
        }
    }

    QVector<quint32> maskModifiers(){
        return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
    }

    unsigned int X11KeyModificator(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        unsigned int keyModificator = 0;

        foreach (QString str, list) {
            if(str == "Ctrl"){
                keyModificator |= XCB_MOD_MASK_CONTROL;
                continue;
            } else if(str == "Alt"){
                keyModificator |= XCB_MOD_MASK_1;
                continue;
            } else if(str == "Shift"){
                keyModificator |= XCB_MOD_MASK_SHIFT;
                continue;
            } else if(str == "Meta"){
                keyModificator |= XCB_MOD_MASK_4;
                continue;
            }
        }
        return keyModificator;
    }

    unsigned int X11HotKey(Display *display, const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        QString str = list.last();
        if(str.length() == 0){
            return XKeysymToKeycode(display, XK_plus);
        } else if(str.length() == 1){
            return XKeysymToKeycode(display, str.at(0).unicode()); // return Key Letters and Numbers
        } else {
            return XKeysymToKeycode(display, this->hash.value(str));
        }
        return 0;
    }
};

QGlobalShortcut::QGlobalShortcut(QObject *parent) :
    QObject(parent),
    sPrivate(new QGlobalShortcutPrivate)
{
    sPrivate->m_display = QX11Info::display();
    sPrivate->m_win = DefaultRootWindow(sPrivate->m_display);
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
        xcb_key_press_event_t *keyEvent = 0;
        if (eventType == "xcb_generic_event_t") {
            xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);
            if ((event->response_type & 127) == XCB_KEY_PRESS){
                keyEvent = static_cast<xcb_key_press_event_t *>(message);
                foreach (QGlobalData *data, sPrivate->listKeys) {
                    foreach (quint32 maskMods, sPrivate->maskModifiers()) {
                        if((keyEvent->state == (data->modifier() | maskMods ))
                                &&  keyEvent->detail == data->key()){
                            emit activated();
                            return true;
                        }
                    }
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
        data->setkey(sPrivate->X11HotKey(sPrivate->m_display, QKeySequence(str)));
        data->setModifier(sPrivate->X11KeyModificator(QKeySequence(str)));
        sPrivate->listKeys.append(data);
        foreach (quint32 maskMods, sPrivate->maskModifiers()) {
            XGrabKey(sPrivate->m_display, data->key() , data->modifier() | maskMods, sPrivate->m_win,True, GrabModeAsync, GrabModeAsync);
        }
    }
    return true;
}

bool QGlobalShortcut::unsetShortcut()
{
    if(!sPrivate->keys.isEmpty()){
        foreach (QGlobalData *data, sPrivate->listKeys) {
            foreach (quint32 maskMods, sPrivate->maskModifiers()) {
                XUngrabKey(sPrivate->m_display, data->key(),data->modifier() | maskMods, sPrivate->m_win);
            }
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
