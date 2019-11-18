#ifndef SYSTRAYALTERNATIVE_H
#define SYSTRAYALTERNATIVE_H

#include "ui_formMainWindow.h"
#include "QvkSpezialSlider.h"

#include <QLabel>
#include <QMainWindow>
#include <QSize>


class QvkSystrayAlternative : public QLabel
{
    Q_OBJECT

public:
    explicit QvkSystrayAlternative( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *gui_sliderShowInSystrayAlternative );
    ~QvkSystrayAlternative();

private:
    Ui_formMainWindow *ui;
    QMenu *menu;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *hideAction;
    QAction *exitAction;
    QSize size;
    QPoint point;


private slots:
    void slot_setRecordIcon( bool );
    void slot_setSystrayIcon( bool );
    void slot_setPauseIcon( bool );
    void slot_resizeWindow( int );


protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

};

#endif // SYSTRAYALTERNATIVE_H
