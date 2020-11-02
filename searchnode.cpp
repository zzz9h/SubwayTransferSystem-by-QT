#include"searchnode.h"

SearchNode::SearchNode(int stationId,int lineId,double sumTime){
    this->stationId = stationId;
    this->lineId = lineId;
    this->sumTime = sumTime;
}

bool operator <(const SearchNode &one,const SearchNode &other){
    return one.sumTime < other.sumTime;
}

bool operator >(const SearchNode &one,const SearchNode &other){
    return one.sumTime > other.sumTime;
}

