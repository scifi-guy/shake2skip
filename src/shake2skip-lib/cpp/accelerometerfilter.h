/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/

#ifndef ACCELEROMETERFILTER_H
#define ACCELEROMETERFILTER_H

#include <QAccelerometer>
#include <QSensor>
#include <QQueue>
#include <QSettings>
#include "logutility.h"
QTM_USE_NAMESPACE

class AccelerometerFilter: public QObject, public QAccelerometerFilter
{
  Q_OBJECT
protected:
    bool filter(QAccelerometerReading *reading);

public:
    explicit AccelerometerFilter(QObject *parent = 0);
    virtual ~AccelerometerFilter();
    int getState();
    enum AccelerometerState {
        Enabled,
        Disabled
    };

signals:
    void shakeDetected();

public slots:
    void enableSensor(bool truncateLogs = false);
    void disableSensor();


private:
    qreal mean(QQueue<qreal> accelDataQueue);
    qreal standardDeviation(QQueue<qreal> accelDataQueue);
    void loadSettings();
    void clearAllQueues();

    int state;

    QSensor* accelerometerSensor;

    qreal xAccelSDThreshold;
    qreal shakeDetectionTimeGapSecs;

    QQueue<qreal> xAccelDataQueue;
    QQueue<qreal> yAccelDataQueue;
    QQueue<qreal> zAccelDataQueue;
    qtimestamp lastDetectionTimestamp;
    bool debugMode;
    QString logFilePath;

    LogUtility *logUtility;
    QSettings *settings;
};

#endif // ACCELEROMETERFILTER_H
