#ifndef QvkLog_H 
#define QvkLog_H

#include "ui_formMainWindow.h"

#include <QObject>
#include <QFile>

class QvkLog : public QObject
{
    Q_OBJECT

public:
  QvkLog( Ui_formMainWindow *ui_mainwindow);
  virtual ~QvkLog();
  void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);


public slots:

  
signals:
    void signal_newLogText( QString txt );

  
private slots:
    void slot_addLog( QString value );

  
private:
    QFile logFile;
    void writeToLog( QString string );
    Ui_formMainWindow *ui;


protected:
  
  
};

#endif
