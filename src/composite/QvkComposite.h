#ifndef COMPOSITE_H
#define COMPOSITE_H

#include <QWidget>
#include <QMainWindow>
#include <QPaintEvent>

class QvkComposite: public QWidget
{
    Q_OBJECT
public:
    QvkComposite(QMainWindow *parent);
    void start_detect_compositeEnabled();


public slots:
    bool get_cmpositeEnabled();


private:
    bool compositeEnabled = false;


private slots:
    void slot_shot();


protected:
    void paintEvent(QPaintEvent *event);

    
signals:
    void signal_compositeEnabled(bool);

};

#endif // COMPOSITE_H
