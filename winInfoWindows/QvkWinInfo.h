#ifndef QvkWinInfo_H 
#define QvkWinInfo_H

#include <QDialog>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QMainWindow>

class QvkWinInfo: public QMainWindow
{ 
Q_OBJECT
public:
  QvkWinInfo();
  virtual ~QvkWinInfo();
  

public slots:

  
private slots: 
   void selectWindow();
   void mousePosition();

   
signals:
  void windowChanged( QString );
  
  
protected:  
  void paintEvent(QPaintEvent *event);

  
private:
  QTimer *windowTimer;
  QTimer *mouseTimer;
  
  WId lastWinID;
  WId newWinID;
};

#endif
