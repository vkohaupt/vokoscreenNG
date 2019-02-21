#include "QvkSettings.h" 
#include "ui_formMainWindow.h"

#include <QFile>
#include <QApplication>
#include <QColor>
#include <QRadioButton>

QvkSettings::QvkSettings(){}
QvkSettings::~QvkSettings(){}

void QvkSettings::saveAll( Ui_formMainWindow *ui_mainwindow )
{
    QSettings settings( getProgName(), getProgName() );
    settings.clear();

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        settings.setValue( listRadiobuttons.at(i)->objectName(), listRadiobuttons.at(i)->isChecked() );
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
    }

    QList<QSpinBox *> listSpinBox = ui_mainwindow->centralWidget->findChildren<QSpinBox *>();
    for ( int i = 0; i < listSpinBox.count(); i++ )
    {
        settings.setValue( listSpinBox.at(i)->objectName(), listSpinBox.at(i)->value() );
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        settings.setValue( listComboBox.at(i)->objectName(), listComboBox.at(i)->currentText() );
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        settings.setValue( listSlider.at(i)->objectName(), listSlider.at(i)->value() );
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            settings.setValue( listLineEdit.at(i)->objectName(), listLineEdit.at(i)->text() );
        }
    }
}


void QvkSettings::readAll()
{
    // Read from file VERSION progname and versionsnumber
    QSettings versionSettings(":/VERSION", QSettings::IniFormat );
    versionSettings.beginGroup("Info");
      bool beta = versionSettings.value( "Beta" ).toBool();
      QString Beta;
      if ( beta )
        Beta = "Beta";
      else
        Beta = "";
      
      ProgName = versionSettings.value( "Progname" ).toString();
      Version = versionSettings.value( "Version" ).toString() + " " + Beta;
    versionSettings.endGroup();
    
    // Einstellungen aus .conf einlesen
    QSettings settings( getProgName(), getProgName() );
    
    // Reset all settings at next start from vokoscreen
    settings.beginGroup( "Miscellaneous" );
      int reset = settings.value( "Reset", 0 ).toUInt();
      if ( reset == 2 )
      {
        settings.clear();
      }
    settings.endGroup();
    
    // Dient nur zum anlegen des Profils damit ffmpeglog erstellt werden kann
    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();
  
    settings.beginGroup( "ShowClick" );
        showClickTime     = settings.value( "Showtime", 5 ).toDouble();
        showClickCircle   = settings.value( "Circle", 70 ).toInt();
        showClickRadiant  = settings.value( "Radiant", false ).toBool();
        showClickColor    = settings.value( "Color", QColor( Qt::red ) ).value<QColor>();
        showClickOpacity  = settings.value( "Opacity", 0.5 ).toDouble();
    settings.endGroup();
}

QString QvkSettings::getVersion()
{
  return Version; 
}

QString QvkSettings::getProgName()
{
  return ProgName; 
}

bool QvkSettings::getShowClickRadiant()
{
  return showClickRadiant;
}

QColor QvkSettings::getShowClickColor()
{
  return showClickColor;
}

double QvkSettings::getShowClickOpacity()
{
  return showClickOpacity;
}

int QvkSettings::getShowClickCircle()
{
  return showClickCircle;
}

double QvkSettings::getShowClickTime()
{
  return showClickTime;
}

int QvkSettings::getMagnifierFormValue()
{
  return magnifierFormValue;
}
