#include "DataReadingHandler.h"
#include <QDebug>

DataReadingHandler::DataReadingHandler() {}

void DataReadingHandler::accReading(double accX, double accY)
{
    // double inputx = accX - accXnoise;
    // if(inputx <= accXmax && inputx >= accXmin)
    // {
    //     inputx = 0;
    // }
    // double inputy = accY - accYnoise;
    // if(inputy <= accYmax && inputy >= accYmin)
    // {
    //     inputy = 0;
    // }
    // if(accX <= accX && accX >= accXmin)
    // {
    //     accX = 0;
    // }
    double inputx;
    double inputy;


    if(accX <= accXmax && accX >= accXmin)
    {
        inputx = 0;
    }
    else
    {
        inputx = accX - accXnoise;
    }
    if(accY <= accYmax && accY >= accYmin)
    {
        inputy = 0;
    }
    else
    {
        inputy = accY - accYnoise;
    }

    if(state != Calibration && state != Idle)
    {
        setfilteredX(inputx);
        setfilteredY(inputy);
    }

    if(state == Idle)
    {
        setaccActive(false);
        setgyroActive(false);
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(accX, accXSum, accXCount, accXmax, accXmin);
        updateCalibrationInfo(accY, accYSum, accYCount, accYmax, accYmin);

    }
    else if(state == Initial)
    {
        if((fabs(prevAccX - inputx) > accThresh) || (fabs(prevAccY - inputy) > accThresh))
        {
            if(fabs(prevAccX - inputx) > fabs(prevAccY - inputy))
            {
                state = MoveX;
                setgyroActive(false);
                currentDirection = (inputx > 0) ? Right : Left;
                DataReadingHandler::handleMovementX(inputx);
            }
            else
            {
                state = MoveY;
                setgyroActive(false);
                currentDirection = (inputy > 0) ? Up : Down;
                DataReadingHandler::handleMovementY(inputy);
            }
        }
    }
    else if(state == MoveX)
    {
        handleMovementX(inputx);
    }
    else if(state == MoveY)
    {
        handleMovementY(inputy);
    }
    else
    {
        return;
    }
}

void DataReadingHandler::gyroReading(double gyroV)
{
    double inputz = gyroV - rotationNoise;
    if(inputz <= rotationMax && inputz >= rotationMin)
    {
        inputz = 0;
    }

    if(state != Calibration)
    {
        setfilteredZ(inputz);
    }

    if(state == Idle)
    {
        setaccActive(false);
        setgyroActive(false);
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(gyroV, rotationSum, rotationCount, rotationMax, rotationMin);
    }
    else if(state == Initial)
    {
        if(fabs(prevRotation - inputz) > rotationThresh)
        {
            state = Rotation;
            setaccActive(false);
            DataReadingHandler::handleRotation(inputz);
        }
    }
    else if(state == Rotation)
    {
        handleRotation(inputz);
    }
    else
    {
        return;
    }
}

void DataReadingHandler::startPattern()
{
    if(state == Idle)
   {
        setaccActive(true);
        setgyroActive(true);
        state = Initial;
        authSource.startNewPattern();
    }
}

void DataReadingHandler::stopPattern()
{
    setaccActive(false);
    setgyroActive(false);
    state = Idle;

    setMovement(0);
    setvelocityX(0);
    setvelocityY(0);
    setRotationZ(0);
}

void DataReadingHandler::startCalibration()
{
    setaccActive(true);
    setgyroActive(true);
    state = Calibration;
    accXSum = 0;
    accXCount = 0;
    accYSum = 0;
    accYCount = 0;
    rotationSum = 0;
    rotationCount = 0;
    rotationNoise = 0;
    accXnoise = 0;
    accYnoise = 0;
    accXmax = -100;
    accXmin = 100;
    accYmin = 100;
    accYmax = -100;
    rotationMax = -100;
    rotationMin = 100;
}

double DataReadingHandler::movement() const
{
    return m_movement;
}

void DataReadingHandler::setMovement(double newMovement)
{
    if (m_movement == newMovement)
        return;
    m_movement = newMovement;
    emit movementChanged();
}

double DataReadingHandler::velocityX() const
{
    return m_velocityX;
}

void DataReadingHandler::setvelocityX(double newVelocityX)
{
    if (m_velocityX == newVelocityX)
        return;
    m_velocityX = newVelocityX;
    emit velocityXChanged();
}

double DataReadingHandler::velocityY() const
{
    return m_velocityY;
}

void DataReadingHandler::setvelocityY(double newVelocityY)
{
    if (m_velocityY == newVelocityY)
        return;
    m_velocityY = newVelocityY;
    emit velocityYChanged();
}

double DataReadingHandler::rotationZ() const
{
    return m_rotationZ;
}

void DataReadingHandler::setRotationZ(double newRotationZ)
{
    if (m_rotationZ == newRotationZ)
        return;
    m_rotationZ = newRotationZ;
    emit rotationZChanged();
}

bool DataReadingHandler::gyroActive() const
{
    return m_gyroActive;
}

void DataReadingHandler::setgyroActive(bool newGyroActive)
{
    if (m_gyroActive == newGyroActive)
        return;
    m_gyroActive = newGyroActive;

    emit gyroActiveChanged();
}

bool DataReadingHandler::accActive() const
{
    return m_accActive;
}

void DataReadingHandler::setaccActive(bool newAccActive)
{
    if (m_accActive == newAccActive)
        return;
    m_accActive = newAccActive;
    emit accActiveChanged();
}

void DataReadingHandler::handleMovementX(double a)
{
    double v = m_velocityX + ((a + prevAccX)/2)/datarate;
    double x = ((a + prevAccX)/4)/(datarate * datarate) + m_velocityX/datarate + m_movement;
    prevAccX = a;
    countx += 1;
    if(fabs(a) < 0.5)
        countzeroX += 1;
    else
        countzeroX = 0;
    // if(fabs(v) <= stationaryAccXThresh)
    if(countzeroX >= 5)
    {
        auto it = DirectionMap.find(currentDirection);
        authSource.addNewSequence(x, it->second , m_rotationZ);
        QString newmove = "Movement: " + QString::number(x) + "Direction: " + it->second;
        setNewpattern(newmove);

        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
        prevAccX = 0;
        countx = 0;
        countzeroX = 0;
    }
    setvelocityX(v);
    setMovement(x);
}

void DataReadingHandler::handleMovementY(double a)
{
    QList<double> accYList;
    accYList.append(a);
    QList<double> velocityYList;
    velocityYList.append(m_velocityY);
    QList<double> movementList;
    movementList.append(m_movement);
    double v = m_velocityY + ((a + prevAccY)/2)/datarate;
    double x = ((a + prevAccY)/4)/(datarate * datarate) + m_velocityY/datarate + m_movement;
    prevAccY = a;
    county += 1;
    if(fabs(a) < 0.05)
        countzeroY += 1;
    else
        countzeroY = 0;
    // if(fabs(v) <= stationaryAccYThresh)
    if(countzeroY >= 5)
    {
        auto it = DirectionMap.find(currentDirection);
        authSource.addNewSequence(x, it->second , m_rotationZ);
        QString newmove = "Movement: " + QString::number(x) + "Direction: " + it->second; ;
        setNewpattern(newmove);

        velocityYList.append(v);
        movementList.append(x);
        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
        prevAccY = 0;
        county = 0;
        countzeroY = 0;
    }
    setvelocityY(v);
    setMovement(x);
}

void DataReadingHandler::handleRotation(double gyroV)
{
    double teta = m_rotationZ + ((gyroV + prevRotation)/2)/datarate;
    prevRotation = gyroV;
    countz += 1;
    if(fabs(gyroV) < 0.5)
        countzeroZ += 1;
    else
        countzeroZ = 0;
    // if(fabs(gyroV) <= rotationThresh)
    if(countzeroZ >= 5)
    {
        auto it = DirectionMap.find(currentDirection);
        authSource.addNewSequence(m_movement, it->second  , teta);
        //curentrotation
        QString newmove = "Rotation: " + QString::number(teta);
        setNewpattern(newmove);
        teta = 0;
        setaccActive(true);
        state = Initial;
        prevRotation = 0;
        countz = 0;
        countzeroZ = 0;
    }
    setRotationZ(teta);}

void DataReadingHandler::updateCalibrationInfo(double newData, double &sum, double &count, double &max, double &min)
{
    sum += newData;
    count++;
    if(newData > max)
    {
        max = newData;
    }
    if(newData < min)
    {
        min = newData;
    }
    if(count > 100)
    {
        stopCalibration();
    }
}

void DataReadingHandler::stopCalibration()
{
    rotationNoise = rotationSum / rotationCount;
    accXnoise = accXSum / accXCount;
    accYnoise = accYSum / accYCount;

    accXmax = accXmax - accXnoise;
    accXmin = accXmin - accXnoise;
    accYmax = accYmax - accYnoise;
    accYmin = accYmin - accYnoise;
    rotationMax = rotationMax - rotationNoise;
    rotationMin = rotationMin - rotationNoise;

    accXmax = accXmax * rangecoef;
    accXmin = accXmin * rangecoef;
    accYmax = accYmax * rangecoef;
    accYmin = accYmin * rangecoef;
    rotationMax = rotationMax * rangecoef;
    rotationMin = rotationMin * rangecoef;

    // accXmax = accXnoise + fabs(accXnoise/2);
    // accXmin = accXnoise - fabs(accXnoise/2);
    // accYmax = accYnoise + fabs(accYnoise/2);
    // accYmin = accYnoise - fabs(accYnoise/2);

    QString cal = "Rotation noise: " + QString::number(rotationNoise) + "\nAccX noise: " +
                  QString::number(accXnoise) + "\nAccY noise: " + QString::number(accYnoise);
    setCalibration(cal);
    state = Idle;
}


QString DataReadingHandler::calibration() const
{
    return m_calibration;
}

void DataReadingHandler::setCalibration(const QString &newCalibration)
{
    if (m_calibration == newCalibration)
        return;
    m_calibration = newCalibration;
    emit calibrationChanged();
}


double DataReadingHandler::filteredX() const
{
    return m_filteredX;
}

void DataReadingHandler::setfilteredX(double newFilteredX)
{
    if (qFuzzyCompare(m_filteredX, newFilteredX))
        return;
    m_filteredX = newFilteredX;
    emit filteredXChanged();
}

double DataReadingHandler::filteredY() const
{
    return m_filteredY;
}

void DataReadingHandler::setfilteredY(double newFilteredY)
{
    if (qFuzzyCompare(m_filteredY, newFilteredY))
        return;
    m_filteredY = newFilteredY;
    emit filteredYChanged();
}

double DataReadingHandler::filteredZ() const
{
    return m_filteredZ;
}

void DataReadingHandler::setfilteredZ(double newFilteredZ)
{
    if (qFuzzyCompare(m_filteredZ, newFilteredZ))
        return;
    m_filteredZ = newFilteredZ;
    emit filteredZChanged();
}

QString DataReadingHandler::newpattern() const
{
    return m_newpattern;
}

void DataReadingHandler::setNewpattern(const QString &newNewpattern)
{
    // if (m_newpattern == newNewpattern)
    //     return;
    m_newpattern = newNewpattern;
    emit newpatternChanged();
}
