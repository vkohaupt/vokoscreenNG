#include <QApplication>
#include "screencast.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    screencast cast;
    cast.show();
    
    return app.exec();
}
