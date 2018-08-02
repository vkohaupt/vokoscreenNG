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
  WId getWinID();
  void slot_start();

  
private slots: 
   void selectWindow();
   void mousePosition();

   
signals:
  void windowChanged( bool );
  void signal_showCursor( bool );

  
protected:  
  void paintEvent(QPaintEvent *event);

  
private:
  QTimer *windowTimer;
  QTimer *mouseTimer;
  
  WId lastWinID;
  WId newWinID;
};

#endif
