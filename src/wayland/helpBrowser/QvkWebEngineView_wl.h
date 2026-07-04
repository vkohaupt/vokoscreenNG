#ifndef QVKWEBENGINEVIEW_WL_H
#define QVKWEBENGINEVIEW_WL_H

#include <QWidget>
#include <QWebEngineView>
#include <QContextMenuEvent>

class QvkWebEngineView_wl : public QWebEngineView {
    Q_OBJECT
public:
    explicit QvkWebEngineView_wl(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif
