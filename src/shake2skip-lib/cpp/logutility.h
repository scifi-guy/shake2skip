/*
@version: 0.1
@author: Sudheer K. <scifi1947 at gmail.com>
@license: GNU General Public License
*/

#ifndef LOGUTILITY_H
#define LOGUTILITY_H
#include <QFile>
#include <QIODevice>
#include <QTextStream>

class LogUtility : public QObject
{
    Q_OBJECT

public:
    LogUtility(QString logPath,QObject *parent = 0) :
        QObject(parent){
        logFilePath = logPath;
    }

    ~LogUtility(){
    }

public slots:
    void logMessage(QString strMessage, bool appendMode = true) {

        QFile logFile(logFilePath);

        bool success = false;

        if (appendMode){
            success = logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
        }
        else{
            success = logFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
        }

        if (!success) { return; }
        QTextStream logStream(&logFile);
        logStream <<  strMessage << endl;
    }

private:
    QString logFilePath;
};

#endif // LOGUTILITY_H
