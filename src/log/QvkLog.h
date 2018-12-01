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
  

public slots:
  void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
  void slot_addLogVokoscreen( QString value );

  
signals:
    void signal_newLogText( QString txt );

  
private slots:

  
private:
    QFile logFile;
    void writeToLog( QString string );
    Ui_formMainWindow *ui;


protected:
  
  
};

#endif
