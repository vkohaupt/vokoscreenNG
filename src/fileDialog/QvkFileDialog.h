
#ifndef QVKFILEDIALOG_H
#define QVKFILEDIALOG_H

#include <QFileDialog>
#include <QWidget>
#include <QObject>
#include <QEvent>

class QvkFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    QvkFileDialog(QWidget *parent = nullptr);
    void setVideoPath(QString videoPath );

    
public slots:


protected:
    bool eventFilter(QObject *o, QEvent *event);


private:


signals:

    
};
#endif
