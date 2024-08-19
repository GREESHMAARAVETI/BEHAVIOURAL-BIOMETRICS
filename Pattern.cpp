#include "Pattern.h"

Pattern::Pattern() {}

void Pattern::startNewPattern()
{
    while(pattern.count() > 0)
    {
        pattern.pop_back();
    }
}

void Pattern::addNewSequence(double movement, QString direction, double angle)
{
    QJsonObject qjsobj = {
        {"movement", movement},
        {"direction", direction},
        {"angle", angle}
    };
    pattern.append(qjsobj);
}

bool Pattern::isPatternMatch(Pattern source, Pattern tobeAuthed)
{
    if(source.pattern.size() != tobeAuthed.pattern.size())
    {
        return false;
    }
    for(int i = 0; i < source.pattern.size(); i++)
    {
        double moveLowerBound = source.pattern.at(i).toObject().value("movement").toDouble() * (1-moveTolerance);
        double moveHigherBound = source.pattern.at(i).toObject().value("movement").toDouble() * (1+moveTolerance);
        double angleLowerBound = source.pattern.at(i).toObject().value("angle").toDouble() * (1-angleTolerance);
        double angleHigherBound = source.pattern.at(i).toObject().value("angle").toDouble() * (1+angleTolerance);

        if(moveLowerBound > tobeAuthed.pattern.at(i).toObject().value("movement").toDouble() ||
            moveHigherBound < tobeAuthed.pattern.at(i).toObject().value("movement").toDouble())
        {
            return false;
        }
        if(angleLowerBound > tobeAuthed.pattern.at(i).toObject().value("angle").toDouble() ||
            angleHigherBound < tobeAuthed.pattern.at(i).toObject().value("angle").toDouble())
        {
            return false;
        }
    }
    return true;
}

