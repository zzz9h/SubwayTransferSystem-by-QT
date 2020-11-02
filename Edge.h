#ifndef EDGE_H
#define EDGE_H
#include<QString>
class Edge{
public :
    int from;
    int to;
    QString lineName;

    Edge(int fr,int t, QString l);
    Edge();
    friend bool operator ==(const Edge& one,const Edge& other);
};
#endif // EDGE_H
