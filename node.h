#ifndef NODE_H
#define NODE_H

class Node{
public:
    int stationID;      //邻接点ID
    int lineId;         //两邻接点之间的线路ID
    double distance;    //两点距离
    Node(int stationID,int lineId,double distance);
    Node();
    friend bool operator ==(const Node one, const Node other);
};

#endif // NODE_H
