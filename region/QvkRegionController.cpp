
#include "QvkRegionController.h"

#include <QDebug>


QvkRegionController::QvkRegionController()
{
  regionChoise = new QvkRegionChoise();
  //regionRecord = new QvkRegionRecord();
}

QvkRegionController::~QvkRegionController()
{
}

void QvkRegionController::myClicked()
{
  regionChoise->myClicked();
}

void QvkRegionController::Abbruch()
{
  regionChoise->Abbruch();
}


void QvkRegionController::hide()
{
  regionChoise->hide();
}


void QvkRegionController::show( bool value )
{
    if (  value == true )
      regionChoise->show();
    else
      regionChoise->hide();
}

void QvkRegionController::slot_close()
{
  regionChoise->close();
}

int QvkRegionController::getX()
{
  return regionChoise->geometry().x(); 
}

int QvkRegionController::getY()
{
  return regionChoise->geometry().y();
}

int QvkRegionController::getWidth()
{
  return regionChoise->getWidth();
}

int QvkRegionController::getHeight()
{
  return regionChoise->getHeight();
}

int QvkRegionController::getXRecordArea()
{
  return regionChoise->getXRecordArea();
}

int QvkRegionController::getYRecordArea()
{
  return regionChoise->getYRecordArea();
}

/*
void QvkRegionController::lockFrame( bool status )
{
  regionRecord->setRecordGeometry( getX(), getY(), getWidth(), getHeight() );
  
  regionChoise->lockFrame( status );
  
  if ( status == true )
  {
    regionChoise->hide();
    regionRecord->show();
  }
  else
  {
    regionRecord->hide();
    regionChoise->show();
  }
}
*/
