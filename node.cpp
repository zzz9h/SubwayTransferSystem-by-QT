#include"node.h"


Node::Node(int stationID,int lineId,double distance){
    this->stationID = stationID;
    this->lineId = lineId;
    this->distance = distance;
}

Node::Node(){

}

bool operator ==(const Node one, const Node other){
    return one.distance == other.distance && one.lineId == other.lineId && one.stationID == other.stationID;
}
