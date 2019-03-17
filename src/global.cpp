#include "global.h"
#include <QString>

// https://stackoverflow.com/questions/45724741/qt-c-create-a-global-variable-accessible-to-all-classes
namespace global
{
    QString name = "vokosreenNG";
    QString nameOutput = "[" + name + "]";
    QString version = "2.9.5 Beta";
}
