#include"Edge.h"


Edge::Edge(int fr,int t, QString l){
    from = fr;
    to = t;
    lineName = l;
}

Edge::Edge(){

}

bool operator ==(const Edge& one,const Edge& other){
    return one.from == other.from && one.lineName == other.lineName && one.to == other.to;
}
