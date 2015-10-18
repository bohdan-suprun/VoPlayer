#ifndef TIMECONTEINER_H
#define TIMECONTEINER_H

#include <QObject>
#include <QTime>

class TimeConteiner
{
protected:
    QTime end,start;
public:
    TimeConteiner(QTime start, QTime end);
    ~TimeConteiner();
     bool operator==(TimeConteiner& rhs);
     friend bool operator<(const TimeConteiner& lhs,const TimeConteiner& rhs);
};


#endif // TIMECONTEINER_H
