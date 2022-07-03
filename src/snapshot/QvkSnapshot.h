#ifndef QVKSNAPSHOT_H
#define qvksnapshot_h

#include <QWidget>
#include <QScreen>

#include "ui_formMainWindow.h"
#include "mainwindow.h"

class QvkSnapshot : public QWidget
{
    Q_OBJECT

public:
    explicit QvkSnapshot( QvkMainWindow *vkMainWindow,  Ui_formMainWindow *ui_formMainWindow );
    ~QvkSnapshot();
    QvkMainWindow *vkMainWindow;

private:
    Ui_formMainWindow *ui;
    QList<QScreen *> screen;
    void supportedImageFormats();
    bool is_imageFolderExists_and_haveWritePermission();


public slots:


private slots:
    void slot_newImage();


protected:


signals:


};

#endif
