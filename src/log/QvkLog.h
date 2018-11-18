#ifndef QvkLog_H 
#define QvkLog_H

#include <QObject>

class QvkLog : public QObject
{
    Q_OBJECT

public:
  QvkLog();
  virtual ~QvkLog();
  

public slots:
  void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

  
signals:
    void newLogText( QString txt );

  
private slots:

  
private:

  
protected:
  
  
};

#endif
