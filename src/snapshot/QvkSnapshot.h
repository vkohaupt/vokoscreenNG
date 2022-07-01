#ifndef QVKSNAPSHOT_H
#define qvksnapshot_h

#include <QWidget>
#include <QScreen>

#include "ui_formMainWindow.h"

class QvkSnapshot : public QWidget
{
    Q_OBJECT

public:
    explicit QvkSnapshot( Ui_formMainWindow *ui_formMainWindow );
    ~QvkSnapshot();


private:
    Ui_formMainWindow *ui;
    QList<QScreen *> screen;
    void supportedImageFormats();
    void screens();

public slots:


private slots:
    void slot_newImage();


protected:


signals:


};

#endif
