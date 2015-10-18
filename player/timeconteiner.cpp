#include "timeconteiner.h"

TimeConteiner::TimeConteiner(QTime start,QTime end)
{
    this->start=start;
    this->end=end;
}
bool TimeConteiner::operator ==(TimeConteiner& rhs){
    return start<=rhs.start&&rhs.end<end;
}
bool operator<(const TimeConteiner& lhs,const TimeConteiner& rhs){
    return lhs.end<rhs.start;
}
TimeConteiner::~TimeConteiner(){

}

