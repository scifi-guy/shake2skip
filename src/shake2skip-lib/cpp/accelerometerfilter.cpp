/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/

#include "accelerometerfilter.h"
#include <QDebug>
#include <QDateTime>
#include <qmath.h>

const qreal GravityPull = 9.8f;  //But N950 shows more than 10.8. Is it just California effect?

#define QREAL_TOSTRING(x) (QString::number(x,'f',6))

AccelerometerFilter::AccelerometerFilter(QObject *parent):QObject(parent)
{
    loadSettings();

    // Initialize and start acceleration sensor.
    accelerometerSensor = new QSensor("QAccelerometer");
    accelerometerSensor->addFilter(this);
    logUtility = new LogUtility(logFilePath,this);
}

AccelerometerFilter::~AccelerometerFilter(){
    //Disable sensor when closing even if the music player is working
    disableSensor();
    accelerometerSensor->deleteLater();
}

void AccelerometerFilter::loadSettings(){
    //Load settings from ini file (if available)
    settings = new QSettings("/opt/shake2skip/config/settings.ini",QSettings::IniFormat,this);
    debugMode = settings->value("Debug",false).toBool();
    logFilePath = settings->value("LogFilePath","/home/user/.shake2skip/shake2skip-data.log").toString();
    xAccelSDThreshold = settings->value("ShakeThreshold",7.0f).toReal();
    shakeDetectionTimeGapSecs = settings->value("TimeGap",2.0f).toReal();

    //Sanitize values from ini file. If the ini file is not available or the values are unusable, then load default values
    if (!debugMode)  {
        debugMode = false;
        settings->setValue("Debug",debugMode);
    }

    if (logFilePath.trimmed().isEmpty()) {
        logFilePath = "/home/user/.shake2skip/shake2skip-data.log";
        settings->setValue("LogFilePath",logFilePath);
    }

    if (xAccelSDThreshold <= 7.0f) {
        xAccelSDThreshold = 7.0f;
        settings->setValue("ShakeThreshold",QREAL_TOSTRING(xAccelSDThreshold));
    }
    if (shakeDetectionTimeGapSecs <= 2.0f) {
        shakeDetectionTimeGapSecs = 2.0f;
        settings->setValue("TimeGap",QREAL_TOSTRING(shakeDetectionTimeGapSecs));
    }

    qDebug() << "Debug Mode = " << debugMode;
    qDebug() << "Log File Path = " << logFilePath;
    qDebug() << "ShakeThreshold = " << xAccelSDThreshold;
    qDebug() << "TimeGap = " << shakeDetectionTimeGapSecs;
}

int AccelerometerFilter::getState(){
    //Return state of the filter Enabled/Disabled
    return state;
}

void AccelerometerFilter::clearAllQueues(){
    //Clear the last 20 accelermoter readings stored for each direction.
    while(!xAccelDataQueue.isEmpty()){
        xAccelDataQueue.dequeue();
    }

    while(!yAccelDataQueue.isEmpty()){
        yAccelDataQueue.dequeue();
    }

    while(!zAccelDataQueue.isEmpty()){
        zAccelDataQueue.dequeue();
    }
}

qreal AccelerometerFilter::mean(QQueue<qreal> accelDataQueue){
 // Calculate average acceleration from the range of (20) values
    qreal sum = 0;
    qreal mean = 0;

    for (int i=0; i < accelDataQueue.length(); i++) {
        sum = sum+ accelDataQueue.at(i);
    }
    if (accelDataQueue.length() != 0) mean = sum/((qreal) accelDataQueue.length());
    return mean;
}

qreal AccelerometerFilter::standardDeviation(QQueue<qreal> accelDataQueue){
    // Calculate Standard Deviation from the range of (20) accelerometer readings

    qreal ydiffSquare = 0;
    qreal standardDeviation = 0;
    qreal meanValue = mean(accelDataQueue);

    for (int i=0; i < accelDataQueue.length(); i++) {
        ydiffSquare = ydiffSquare+ qPow(accelDataQueue.at(i) - meanValue,2);
    }

    if (accelDataQueue.length() != 0) standardDeviation = qSqrt(ydiffSquare/((qreal) accelDataQueue.length()));
    return standardDeviation;
}

bool AccelerometerFilter::filter(QAccelerometerReading *reading){
    //Capture and process each accelerometer reading

    qreal x = reading->x();
    qreal y = reading->y();
    qreal z = reading->z();

    quint64 timestamp = (quint64) reading->timestamp();
    QString strMessage;

    qreal xAccelStandardDeviation =  0;
    qreal yAccelStandardDeviation =  0;
    qreal zAccelStandardDeviation =  0;

    //We store past 20 accel. readings for each direction.

    //If the queue reaches 20, dequeue/remove the oldest reading
    if (xAccelDataQueue.length() >= 20){
        xAccelDataQueue.dequeue();
        yAccelDataQueue.dequeue();
        zAccelDataQueue.dequeue();

        //Now add the current reading to the list (at last position - which is taken care by the awesome QQueue class)
        xAccelDataQueue.enqueue(x);
        yAccelDataQueue.enqueue(y);
        zAccelDataQueue.enqueue(z);

        //Calculate standard deviation in each direction for the last 20 readings
        xAccelStandardDeviation = standardDeviation(xAccelDataQueue);
        yAccelStandardDeviation = standardDeviation(yAccelDataQueue);
        zAccelStandardDeviation = standardDeviation(zAccelDataQueue);

    }
    else{
        /* We don't have sufficient (20) readings to determine which direction the phone is moving.
           So, keep adding the readings to the queue.
         */
        xAccelDataQueue.enqueue(x);
        yAccelDataQueue.enqueue(y);
        zAccelDataQueue.enqueue(z);
    }


    if (debugMode){
        strMessage = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7").arg(QString::number(timestamp),
                                                   QREAL_TOSTRING(x),
                                                   QREAL_TOSTRING(y),
                                                   QREAL_TOSTRING(z),
                                                   QREAL_TOSTRING(xAccelStandardDeviation),
                                                   QREAL_TOSTRING(yAccelStandardDeviation),
                                                   QREAL_TOSTRING(zAccelStandardDeviation));
        logUtility->logMessage(strMessage);
    }


    /*Now for the interesting part.

      If the standard deviation in x axis is more than the set threshold and also more than double the SD in y and z axes, we identify the phone is being shaken horizontally.
      This avoids accidental triggers due to walking etc. In harmattan sensors readings are not available when the display is off,
      which means this function will not be called when display is off (screen locked). This also reduces the possibility of accidental triggers.
     */

    if (xAccelStandardDeviation > xAccelSDThreshold
            && xAccelStandardDeviation > yAccelStandardDeviation *2.0f
            && xAccelStandardDeviation > zAccelStandardDeviation * 2.0f){

        qDebug() << "Time since last detection (secs): "<<(reading->timestamp() - lastDetectionTimestamp)/qPow(10,6);
        if ((reading->timestamp() - lastDetectionTimestamp)/qPow(10,6) > shakeDetectionTimeGapSecs) {

            emit shakeDetected();

            lastDetectionTimestamp = reading->timestamp();
            strMessage = QString("Trigger:  %1\t%2\t%3\t%4").arg(QString::number(timestamp),
                                                                 QREAL_TOSTRING(xAccelStandardDeviation),
                                                                 QREAL_TOSTRING(yAccelStandardDeviation),
                                                                 QREAL_TOSTRING(zAccelStandardDeviation));
            qDebug() << strMessage;
        }
        else{
            qDebug() << "Shake recognized but "
                     <<  (shakeDetectionTimeGapSecs - ((reading->timestamp() - lastDetectionTimestamp)/qPow(10,6)))
                     << " more seconds remain until next trigger";
        }

        qDebug()<< "Clearing last " << xAccelDataQueue.length() << " readings";
        clearAllQueues();
    }

    return true;
}

void AccelerometerFilter::enableSensor(bool appendMode){
    qDebug() << "Enabling sensor";

    if (accelerometerSensor->connectToBackend()){
        if (debugMode){
            QString strTimeNow = QDateTime::currentDateTime().toString("dd-MMM-yyyy HH:mm:ss");
            QString message = QString("========Started at %1=============").arg(strTimeNow);
            logUtility->logMessage(message,appendMode);
        }
        accelerometerSensor->start();
        state = Enabled;
    }
    else
      qFatal("Failed to initialize acceleration sensor.");
}


void AccelerometerFilter::disableSensor(){
    qDebug() << "Disabling sensor";   
    accelerometerSensor->stop();
    state = Disabled;

    if (debugMode){
        QString strTimeNow = QDateTime::currentDateTime().toString("dd-MMM-yyyy HH:mm:ss");
        QString message = QString("========Stopped at %1=============").arg(strTimeNow);
        logUtility->logMessage(message);
    }
}
