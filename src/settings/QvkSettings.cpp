#include "QvkSettings.h"
#include "ui_formMainWindow.h"

#include <QFile>
#include <QApplication>
#include <QColor>

QvkSettings::QvkSettings()
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

    // Dient nur zum anlegen des Profils damit das log erstellt werden kann
    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();

}

QvkSettings::~QvkSettings(){}

void QvkSettings::readAll( Ui_formMainWindow *ui_mainwindow, QMainWindow *parent )
{
    // Einstellungen aus .conf einlesen
    QSettings settings( getProgName(), getProgName() );

    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();

    parent->move( settings.value( "MainWindow_X" ).toInt(), settings.value( "MainWindow_Y" ).toInt() );

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        bool value = settings.value( listRadiobuttons.at(i)->objectName(), false ).toBool();
        if ( value == true )
        {
           listRadiobuttons.at(i)->click();
        }
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        // We have no settings-file(first start after install) but this object we want set as Standard.
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxShowInSystray" ) and ( settings.value( listCheckBox.at(i)->objectName(), true ).toBool() == true ) )
        {
            listCheckBox.at(i)->click();
            continue;
        }

        // We found a setting, then we want set or not.
        if ( settings.value( listCheckBox.at(i)->objectName(), false ).toBool() == true )
        {
            listCheckBox.at(i)->click();
        }
    }

    QList<QSpinBox *> listSpinBox = ui_mainwindow->centralWidget->findChildren<QSpinBox *>();
    for ( int i = 0; i < listSpinBox.count(); i++ )
    {
        int value = settings.value( listSpinBox.at(i)->objectName(), 0 ).toInt();
        if ( value > 0  )
        {
           listSpinBox.at(i)->setValue( value );
        }
    }

    QList<QComboBox *> listComboBox = ui_mainwindow->centralWidget->findChildren<QComboBox *>();
    for ( int i = 0; i < listComboBox.count(); i++ )
    {
        QString valueText = settings.value( listComboBox.at(i)->objectName(), "" ).toString();
        int valueInt = listComboBox.at(i)->findText( valueText );
        if ( valueInt > -1 )
        {
            listComboBox.at(i)->setCurrentIndex( valueInt );
        }
    }

    QList<QSlider *> listSlider = ui_mainwindow->centralWidget->findChildren<QSlider *>();
    for ( int i = 0; i < listSlider.count(); i++ )
    {
        QVariant variant = settings.value( listSlider.at(i)->objectName() );
        if ( variant.isValid() )
        {
            listSlider.at(i)->setValue( variant.toInt() );
        }
    }

    QList<QLineEdit *> listLineEdit = ui_mainwindow->centralWidget->findChildren<QLineEdit *>();
    for ( int i = 0; i < listLineEdit.count(); i++ )
    {
        if ( listLineEdit.at(i)->objectName().contains( "lineEdit" ) )
        {
            QString value = settings.value( listLineEdit.at(i)->objectName() ).toString();
            if ( !value.isEmpty() )
            {
               listLineEdit.at(i)->setText( value );
            }
        }
    }

    // All TabWidget start with value=0, also if in Qt-Creator is the value an other
    QList<QTabWidget *> listTabWidget = ui_mainwindow->centralWidget->findChildren<QTabWidget *>();
    for ( int i = 0; i < listTabWidget.count(); i++ )
    {
        int value = settings.value( listTabWidget.at(i)->objectName(), 0 ).toInt();
        listTabWidget.at(i)->setCurrentIndex( 0 );
    }

}


void QvkSettings::saveAll(Ui_formMainWindow *ui_mainwindow , QMainWindow *parent)
{
    QSettings settings( getProgName(), getProgName() );
    settings.clear();

    settings.beginGroup("vokoscreen");
      settings.setValue("Version", getVersion());
    settings.endGroup();

    settings.setValue( "MainWindow_X", parent->pos().x() );
    settings.setValue( "MainWindow_Y", parent->pos().y() );

    QList<QRadioButton *> listRadiobuttons = ui_mainwindow->centralWidget->findChildren<QRadioButton *>();
    for ( int i = 0; i < listRadiobuttons.count(); i++ )
    {
        settings.setValue( listRadiobuttons.at(i)->objectName(), listRadiobuttons.at(i)->isChecked() );
    }

    QList<QCheckBox *> listCheckBox = ui_mainwindow->centralWidget->findChildren<QCheckBox *>();
    for ( int i = 0; i < listCheckBox.count(); i++ )
    {
        if ( ( listCheckBox.at(i)->objectName() == "checkBoxStopRecordingAfter" ) or
             ( listCheckBox.at(i)->objectName() == "checkBoxStartTime" ) )
        {
            // We do not save
        }
        else
        {
            settings.setValue( listCheckBox.at(i)->objectName(), listCheckBox.at(i)->isChecked() );
        }
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
/*
    QList<QTabWidget *> listTabWidget = ui_mainwindow->centralWidget->findChildren<QTabWidget *>();
    for ( int i = 0; i < listTabWidget.count(); i++ )
    {
        settings.setValue( listTabWidget.at(i)->objectName(), listTabWidget.at(i)->currentIndex() );
    }
*/
}


QString QvkSettings::getVersion()
{
  return Version; 
}

QString QvkSettings::getProgName()
{
  return ProgName; 
}

QString QvkSettings::getFileName()
{
    QSettings settings( getProgName(), getProgName() );
    return settings.fileName();
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
