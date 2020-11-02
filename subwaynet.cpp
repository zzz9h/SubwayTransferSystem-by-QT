#include"subwaynet.h"

//从文件读取数据
bool SubwayNet::readFileData(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString id_ = in.readLine();

        QString no;
        QString lineName;
        in>>no>>lineName;


        QString colour,color;
        bool ok;
        in>>colour>>color;

        QString fromto,from,to;
        in>>fromto>>from>>to;

        bool isLoopLine = false;
        QString isLoopLine_;
        QString yesOrNo;
        in>>isLoopLine_>>yesOrNo;
        QString str =yesOrNo.toLower();
        if(str != "y"&&str != "n"){
            return false;
        }
        isLoopLine = (str == "y");
        if(isLoopLine){
            int a =1;
            a += 1;
        }

        addLine(lineName,QColor::fromRgba(color.remove(0,1).toUInt(&ok,16)),from,to,isLoopLine);

        QString totolStation;
        int num;
        in>>totolStation>>num;

        int id;
        QString stationName,longilati,preStationName;
        double longitude,latitude;
        for(int i = 0; i < num; i++){
            in>>id>>stationName>>longilati;
            QStringList strList = longilati.split(QChar(','));
            longitude = strList.first().toDouble();
            latitude = strList.last().toDouble();
            addStation(stationName,longitude,latitude);

            if(i>0){
                addEdge(preStationName,stationName,lines.size()-1);
            }

            preStationName = stationName;
        }
        if(isLoopLine){
            addEdge(from,to,lines.size()-1);
        }

        QString empty = in.readLine();
        in.readLine();
    }
    return true;
}

//获取站点最小坐标
QPointF SubwayNet::getMinCoord()
{
    return QPointF(minLongitude,minLatitude);
}

//获取站点最大坐标
QPointF SubwayNet::getMaxCoord()
{
    return QPointF(maxLongitude, maxLatitude);
}

Status SubwayNet::addStation(QString name,double longitude, double latitude){

    if(this->stationsHash.find(name) != stationsHash.end())//已经添加了该站点
        return ERROR;

    maxLatitude = max(maxLatitude,latitude);
    minLatitude = min(minLatitude,latitude);
    maxLongitude = max(maxLongitude,longitude);
    minLongitude = min(minLongitude,longitude);

    this->stationsHash[name] = stations.size();
    stations.push_back(Station(name,longitude,latitude));
    subwayNet.push_back(QVector<Node>());
    return OK;
}

Status SubwayNet::addLine(QString lineName,QColor color,QString from,QString to,bool isLoopLine){
    int newId;
    if(this->linesHash.find(lineName) != linesHash.end()){//已经添加了该线路
        QVector<int> ids = linesHash[lineName];
        for(int i:ids){
            if(lines[i].from == from && lines[i].to == to){
                return ERROR;
            }
        }
        newId = lines.size();
        linesHash[lineName].push_back(newId);
        color = lines[linesHash[lineName][0]].color;
    }
    else{
        newId = lines.size();
        linesHash[lineName] = QVector<int>();
        linesHash[lineName].push_back(newId);
    }
    lines.push_back(Line(newId,lineName,color,from,to,isLoopLine));
    return OK;
}

Status SubwayNet::addEdge(QString sta1,QString sta2,int lineId){
    if(this->stationsHash.find(sta1) == stationsHash.end())//没有站点1
        return ERROR;
    if(this->stationsHash.find(sta2) == stationsHash.end())//没有站点2
        return ERROR;

    int s1 = stationsHash[sta1];
    int s2 = stationsHash[sta2];
    double d = stations[s1].calculateDistance(stations[s2]);

    if(subwayNet[s1].contains(Node(s2,lineId,d))){//在站点1、2间已有线路line
        return ERROR;
    }

    QString lineName = lines[lineId].name;
    subwayNet[s1].push_back(Node(s2,lineId,d));
    subwayNet[s2].push_back(Node(s1,lineId,d));
    if(edges.contains(Edge(s1,s2,lineName))||edges.contains(Edge(s2,s1,lineName))){

    }
    else{
        edges.push_back(Edge(s1,s2,lineName));
    }

    return OK;
}

QSet<QString> SubwayNet::getLinesNameBetweenStation(int s1,int s2){
    QSet<QString> set;
    for(Edge e:edges){
        if((e.from == s1 && e.to == s2)||(e.from == s2 && e.to == s1)){
            set.insert(e.lineName);
        }
    }
    return set;
}

QVector<QPair<int,int> > SubwayNet::getSubwayTransferGuide(QString stationName1,QString stationName2){
    int stationId1 = stationsHash[stationName1];
    int stationId2 = stationsHash[stationName2];

    //time[i][j]记录从起点到station[i]且到达时乘坐列车id为j的最短时间
    double **time = new double*[stations.size()];
    for(int i = 0; i< stations.size();i++){
        time[i] = new double[lines.size()];
        for(int j = 0; j < lines.size(); j++){
            time[i][j] = 99999999;
        }
    }
    //trace[i]记录从起点到station[i/lines.size()]且到达时乘坐列车id为i%lines.size()时依次到达的站点id和到达站点时坐的列车id和
    QVector<QVector<QPair<int,int> > > trace;
    for(int i = 0; i < stations.size()*lines.size(); i++){
        trace.push_back(QVector<QPair<int,int> >());
    }

    priority_queue<SearchNode,vector<SearchNode>,greater<SearchNode> > q;
    q.push(SearchNode(stationId1,-1,0));

    while(!q.empty()){
        SearchNode searchNode = q.top();
        q.pop();
        int stationId = searchNode.stationId;
        int lineId = searchNode.lineId;
        double sumTime = searchNode.sumTime;
        if(lineId >= 0 && sumTime > time[stationId][lineId]){
            continue;
        }
        if(stationId == stationId2){
            for(int i = 0; i< stations.size();i++){
                delete []time[i];
            }
            delete[]time;
            return trace[stationId*lines.size()+lineId];
        }
        //扩展结点
        for(Node node:subwayNet[stationId]){
            int nextStationId = node.stationID;
            int nextLineId = node.lineId;
            double distance = node.distance;
            double nextSumTime = sumTime + distance / lines[nextLineId].speed * 60 + (nextLineId == lineId?0:lines[nextLineId].transferTime);
            if(nextSumTime < time[nextStationId][nextLineId]){
                time[nextStationId][nextLineId] = nextSumTime;
                if(lineId == -1){
                    trace[nextStationId*lines.size()+nextLineId].push_back(QPair<int,int>(nextStationId,nextLineId));
                }
                else{
                    int index = nextStationId*lines.size()+nextLineId;
                    trace[index].clear();
                    trace[index].append(trace[stationId*lines.size()+lineId]);
                    trace[index].push_back(QPair<int,int>(nextStationId,nextLineId));
                }
                q.push(SearchNode(nextStationId,nextLineId,nextSumTime));
            }
        }
    }
    //没有从所输入起点到达终点的地铁线路
    for(int i = 0; i< stations.size();i++){
        delete []time[i];
    }
    delete[]time;
    return QVector<QPair<int,int> >();
}


