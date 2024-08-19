#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class logger : public QObject
{
    Q_OBJECT
public:
    explicit logger(QObject *parent = nullptr);
    Q_INVOKABLE void startLogging();
    Q_INVOKABLE void stopLogging();
    Q_INVOKABLE void onAccelerometerData(qreal x, qreal y, qreal z);
    Q_INVOKABLE void onGyroscopeData(qreal x, qreal y, qreal z);

private:
    QFile file;
    QTextStream stream;
    bool logging;
};


#endif // LOGGER_H

