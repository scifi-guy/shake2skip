/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/

#include <QtCore/QCoreApplication>
#include "accelerometerfilter.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    qputenv("N900_PORTRAIT_SENSORS", "1");
    QCoreApplication a(argc, argv);

    AccelerometerFilter accelFilter(&a);
    accelFilter.enableSensor();
    return a.exec();
}
