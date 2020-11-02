#ifndef SEARCHNODE_H
#define SEARCHNODE_H

class SearchNode{
protected:
    int stationId;//当前所到车站
    int lineId;//到达车站所乘线路
    double sumTime;//到达该车站所需时间
public:
    SearchNode(int stationId,int lineId,double sumTime);
    friend bool operator <(const SearchNode &one,const SearchNode &other);
    friend bool operator >(const SearchNode &one,const SearchNode &other);
    friend class SubwayNet;
};

#endif // SEARCHNODE_H
