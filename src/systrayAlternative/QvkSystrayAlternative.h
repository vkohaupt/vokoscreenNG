#ifndef SYSTRAYALTERNATIVE_H
#define SYSTRAYALTERNATIVE_H

#include "ui_formMainWindow.h"
#include "QvkSpezialSlider.h"
#include "QvkSystrayAlternativeWindow.h"

#include <QWidget>
#include <QMainWindow>
#include <QSize>


class QvkSystrayAlternative : public QWidget
{
    Q_OBJECT

public:
    explicit QvkSystrayAlternative( QMainWindow *mainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *gui_sliderShowInSystrayAlternative );
    ~QvkSystrayAlternative();
     QvkSystrayAlternativeWindow *vkSystrayAlternativeWindow;
     void setVisible( bool ) override;


private:
    Ui_formMainWindow *ui;
    QMenu *menu;
    QAction *startAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *hideAction;
    QAction *exitAction;

    QAction *cameraAction;
    QAction *magnifierAction;
    QAction *showclickAction;
    QAction *haloAction;


    QSize size;
    QPoint point;
    QvkSpezialSlider *sliderShowInSystrayAlternative;


private slots:
    void slot_setRecordIcon( bool );
    void slot_setSystrayIcon( bool );
    void slot_setPauseIcon( bool );
    void slot_resizeWindow( int );
    void slot_ShowInSystrayAlternativeReset( bool );
    void slot_shortcutSystray( QString device, QString shortcut );


protected:

};

#endif // SYSTRAYALTERNATIVE_H
