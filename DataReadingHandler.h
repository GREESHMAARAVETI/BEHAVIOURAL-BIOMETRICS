#ifndef DATAREADINGHANDLER_H
#define DATAREADINGHANDLER_H

#include <QObject>
#include <math.h>
#include "Pattern.h"

class DataReadingHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double movement READ movement WRITE setMovement
                   NOTIFY movementChanged FINAL)

    Q_PROPERTY(double velocityX READ velocityX WRITE setvelocityX
                   NOTIFY velocityXChanged FINAL)

    Q_PROPERTY(double velocityY READ velocityY WRITE setvelocityY
                   NOTIFY velocityYChanged FINAL)

    Q_PROPERTY(double rotationZ READ rotationZ WRITE setRotationZ
                   NOTIFY rotationZChanged FINAL)

    Q_PROPERTY(bool gyroActive READ gyroActive WRITE setgyroActive
                   NOTIFY gyroActiveChanged FINAL)

    Q_PROPERTY(bool accActive READ accActive WRITE setaccActive
                   NOTIFY accActiveChanged FINAL)

    Q_PROPERTY(QString calibration READ calibration WRITE setCalibration
                   NOTIFY calibrationChanged FINAL)

    Q_PROPERTY(double filteredX READ filteredX WRITE setfilteredX
                   NOTIFY filteredXChanged FINAL)

    Q_PROPERTY(double filteredY READ filteredY WRITE setfilteredY
                   NOTIFY filteredYChanged FINAL)

    Q_PROPERTY(double filteredZ READ filteredZ WRITE setfilteredZ
                   NOTIFY filteredZChanged FINAL)

    Q_PROPERTY(QString newpattern READ newpattern WRITE setNewpattern NOTIFY newpatternChanged FINAL)


public:
    DataReadingHandler();
    Q_INVOKABLE void accReading(double accX, double accY);
    Q_INVOKABLE void gyroReading(double gyroV);
    Q_INVOKABLE void startPattern();
    Q_INVOKABLE void stopPattern();
    Q_INVOKABLE void startCalibration();

    enum ReadingState { Idle, Calibration, Initial, MoveX, MoveY, Rotation };
    Q_ENUM(ReadingState)
    ReadingState state = Idle;

    enum MoveDirection { Left, Right, Up, Down };
    Q_ENUM(MoveDirection)

    std::map<MoveDirection, QString> DirectionMap = {
        {MoveDirection::Down, "Down"},
        {MoveDirection::Up, "Up"},
        {MoveDirection::Left, "Left"},
        {MoveDirection::Right, "Right"}
    };

    double movement() const;
    void setMovement(double newMovement);

    double velocityX() const;
    void setvelocityX(double newVelocityX);

    double velocityY() const;
    void setvelocityY(double newVelocityY);

    double rotationZ() const;
    void setRotationZ(double newRotationZ);

    bool gyroActive() const;
    void setgyroActive(bool newGyroActive);

    bool accActive() const;
    void setaccActive(bool newAccActive);

    QString calibration() const;
    void setCalibration(const QString &newCalibration);

    double filteredX() const;
    void setfilteredX(double newFilteredX);

    double filteredY() const;
    void setfilteredY(double newFilteredY);

    double filteredZ() const;
    void setfilteredZ(double newFilteredZ);

    QString newpattern() const;
    void setNewpattern(const QString &newNewpattern);

signals:
    void movementChanged();

    void velocityXChanged();

    void velocityYChanged();

    void rotationZChanged();

    void gyroActiveChanged();

    void accActiveChanged();

    void calibrationChanged();

    void filteredXChanged();

    void filteredYChanged();

    void filteredZChanged();

    void newpatternChanged();

private:
    MoveDirection currentDirection = Up;
    double prevAccX = 0;
    double prevAccY = 0;
    double prevRotation = 0;

    // Constants
    const double accThresh = 2;
    const double rotationThresh = 40;
    const double datarate = 25;

    // Thresholds
    const int calibrationLimit = 100;
    const double stationaryAccXThresh = 0.15;
    const double stationaryAccYThresh = 0.0;
    const double stationaryRotationThresh = 0.1;

    // Q_PROPERTY values
    double m_movement = 0;
    double m_velocityX = 0;
    double m_velocityY = 0;
    double m_rotationZ = 0;
    bool m_gyroActive = true;
    bool m_accActive = true;
    double m_filteredX;
    double m_filteredY;
    double m_filteredZ;

    // Calibration variables
    double accXSum = 0;
    double accXCount = 0;
    double accYSum = 0;
    double accYCount = 0;
    double rotationSum = 0;
    double rotationCount = 0;
    double rotationNoise = 0;
    double accXnoise = 0;
    double accYnoise = 0;

    // Calibration ranges
    double accXmax = 0;
    double accXmin = 0;
    double accYmax = 0;
    double accYmin = 0;
    double rotationMax = 0;
    double rotationMin = 0;
    double rangecoef = 1;

    // Pattern
    Pattern authSource;
    Pattern toBeAuthed;

    // Functions
    void handleMovementX(double a);
    void handleMovementY(double a);
    void handleRotation(double gyroV);
    void updateCalibrationInfo(double newData, double &sum, double &count, double &max, double &min);
    void stopCalibration();

    QString m_calibration;

    // Tests
    int county = 0;
    int countx = 0;
    int countz = 0;

    //total
    double totalrotation = 0;

    // Check done
    int countzeroX = 0;
    int countzeroY = 0;
    int countzeroZ = 0;
    QString m_newpattern;
};

#endif // DATAREADINGHANDLER_H
