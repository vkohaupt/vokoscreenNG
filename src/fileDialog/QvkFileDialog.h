
#ifndef QVKFILEDIALOG_H
#define QVKFILEDIALOG_H

#include <QFileDialog>
#include <QString>

class QWidget;
class QObject;
class QEvent;

class QvkFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    QvkFileDialog(QWidget *parent = nullptr);
    void setVideoPath(QString videoPath );

    
public slots:


protected:
    bool eventFilter(QObject *o, QEvent *event) override;


private:


signals:

    
};
#endif
