#include "logger.h"
#include <QDateTime>
#include <QDebug>
#include <fstream>
#include <QStandardPaths>
#include <QDir>

logger::logger(QObject *parent) : QObject(parent), logging(false)
{
}

void logger::startLogging()
{
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(writableLocation);
    if (!dir.exists()) {
        dir.mkpath(writableLocation);
    }
    QString filePath = writableLocation + "/logger.txt";
    file.setFileName(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        stream.setDevice(&file);
        logging = true;
        stream << "Timestamp,AccelerometerX,AccelerometerY,AccelerometerZ,GyroscopeX,GyroscopeY,GyroscopeZ\n";
        qDebug() << "Logging started, writing to" << filePath;
    } else {
        qDebug() << "Unable to open file for logging at" << filePath;
    }
}

void logger::stopLogging()
{
    logging = false;
    file.close();
}

void logger::onAccelerometerData(qreal x, qreal y, qreal z)
{
    if (logging) {
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        stream << timestamp << "," << x << "," << y << "," << z << ",,,\n";
        stream.flush();
    }
}

void logger::onGyroscopeData(qreal x, qreal y, qreal z)
{
    if (logging) {
        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        stream << timestamp << ",,,," << x << "," << y << "," << z << "\n";
        stream.flush();
    }
}
