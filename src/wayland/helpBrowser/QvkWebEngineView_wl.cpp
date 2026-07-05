#include "QvkWebEngineView_wl.h"

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QWebEngineView>
#include <QContextMenuEvent>
#include <QDesktopServices>

QvkWebEngineView_wl::QvkWebEngineView_wl(QWidget *parent) : QWebEngineView(parent) {}

void QvkWebEngineView_wl::contextMenuEvent(QContextMenuEvent *event) {
    // 1. Standard-Menü von Chromium / Qt WebEngine abrufen
    //QMenu *menu = this->createStandardContextMenu();

    // ODER: Erstellen Sie ein komplett leeres Menü:
    QMenu *menu = new QMenu(this);

    // 2. Eigene Menüpunkte hinzufügen (optional)
    QAction *customAction = menu->addAction("Donate");
    connect(customAction, &QAction::triggered, this, []() {
        QString donate = "https://linuxecke.volkoh.de/vokoscreen/vokoscreen-donate.html";
        qDebug() << QDesktopServices::openUrl(QUrl(donate));
    });
/*
    QAction *customAction1 = menu->addAction("And there's nothing here either.");
    connect(customAction1, &QAction::triggered, this, []() {
        qDebug() << "Zweiter Menüpunkt geklickt!";
    });
*/
    // 3. Menü anzeigen (an der Mausposition)
    menu->popup(event->globalPos());

    // 4. WICHTIG: Ereignis akzeptieren, damit das Standardmenü des Browsers nicht zusätzlich erscheint
    event->accept();
}
