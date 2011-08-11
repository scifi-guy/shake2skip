/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/
#include "shakedetector.h"
#include <QDBusConnection>
#include <QDebug>

ShakeDetector::ShakeDetector(QObject *parent):QObject(parent)
{
    accelFilter = new AccelerometerFilter(this);
    mafwProxy = new MafwRendererProxy(MAFW_RENDERER_DBUS_SERVICE,MAFW_RENDERER_DBUS_PATH,QDBusConnection::sessionBus(),this);
    connect(mafwProxy,SIGNAL(state_changed(int)),SLOT(mediaStateChanged(int)));
    connect(accelFilter,SIGNAL(shakeDetected()),SLOT(skipSong()));
    qDebug() << "Connected to media player signals";
}

ShakeDetector::~ShakeDetector(){
    accelFilter->disableSensor();
}

void ShakeDetector::skipSong(){
    qDebug() << "Shake recognized, Changing song...";
    mafwProxy->next();
}

void ShakeDetector::mediaStateChanged(int mediaState){

    int sensorState = accelFilter->getState();
    bool appendMode = false;

    switch (mediaState)
    {
        case Playing:
            if (sensorState != AccelerometerFilter::Enabled)
                accelFilter->enableSensor(appendMode);
            break;
        case Transitioning:
            //Do Nothing
            break;
        default:
            if (sensorState != AccelerometerFilter::Disabled)
                accelFilter->disableSensor();
    }
}
