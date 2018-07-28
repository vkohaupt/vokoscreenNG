
#include "QvkRegionController.h"

#include <QDebug>


QvkRegionController::QvkRegionController()
{
    regionChoise = new QvkRegionChoise();
    regionRecord = new QvkRegionRecord();
}


QvkRegionController::~QvkRegionController()
{
}


void QvkRegionController::slot_set_record_start_flag()
{
    regionChoise->hide();
    //regionRecord->show();
}

void QvkRegionController::slot_set_record_stop_flag()
{
    regionChoise->show();
    //regionRecord->hide();
}


void QvkRegionController::show( bool value )
{
    if (  value == true )
        regionChoise->show();
    else
        regionChoise->hide();
}


void QvkRegionController::hide()
{
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
