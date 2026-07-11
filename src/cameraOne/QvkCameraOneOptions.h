#ifndef QVKCAMERAONEOPTIONS_H
#define QVKCAMERAONEOPTIONS_H

#include <QWidget>

namespace Ui {
class QvkCameraOneOptions;
}

class QvkCameraOneOptions : public QWidget
{
    Q_OBJECT

public:
    explicit QvkCameraOneOptions(QWidget *parent = nullptr);
    ~QvkCameraOneOptions();
    Ui::QvkCameraOneOptions *ui;

private:


};

#endif // QVKCAMERAONEOPTIONS_H
