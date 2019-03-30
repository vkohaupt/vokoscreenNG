#ifndef QVKCAMERACONTROLLER_H
#define QVKCAMERACONTROLLER_H

#include "ui_QvkCameraController.h"

#include "ui_formMainWindow.h"

#include <QWidget>

namespace Ui {
class QvkCameraController;
}

class QvkCameraController : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraController( Ui_formMainWindow *ui_surface );
    ~QvkCameraController();


private:
    Ui::QvkCameraController *ui;
    Ui_formMainWindow *ui_formMainWindow;

    QSize sliderValue();

    const QPixmap *pixmap;


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent( QMouseEvent *event );
    void resizeEvent( QResizeEvent *event );


};

#endif // QVKCAMERACONTROLLER_H
