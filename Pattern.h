#ifndef PATTERN_H
#define PATTERN_H

#include <QJsonObject>
#include <QJsonArray>


class Pattern
{
private:
    QJsonArray pattern;

    const double moveTolerance = 0.25;
    const double angleTolerance = 0.25;

public:
    Pattern();

    void startNewPattern();
    void addNewSequence(double movement, QString direction, double angle);
    bool isPatternMatch(Pattern source, Pattern tobeAuthed);

};

#endif // PATTERN_H
