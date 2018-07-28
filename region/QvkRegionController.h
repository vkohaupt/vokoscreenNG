#ifndef QvkRegionController_H
#define QvkRegionController_H

#include <QObject>

#include "QvkRegionChoise.h"
#include "QvkRegionRecord.h"

class QvkRegionController: public QObject
{
    Q_OBJECT
    
public:
   QvkRegionController();
   virtual ~QvkRegionController();

   int getX();
   int getY();
   int getWidth();
   int getHeight();
   
   int getXRecordArea();
   int getYRecordArea();

   void lockFrame( bool status );

   void hide();

   QvkRegionChoise *regionChoise;
   QvkRegionRecord *regionRecord;

signals:

    
private slots:
  
  
public slots:
   void show(bool value);
   void slot_close();

   void slot_set_record_start_flag();
   void slot_set_record_stop_flag();


private:
  
  
protected:  

};

#endif
