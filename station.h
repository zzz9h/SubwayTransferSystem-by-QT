#ifndef STATION_H
#define STATION_H

typedef int Status;
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFESIBLE -1
#define OVERFLOW -2
#include<algorithm>
#include<QPointF>
#include<QLineF>
#include<QString>
#include<QSet>
#include<QVector>
#include<QQueue>
#include<queue>
#include<vector>
#include<functional>
#include<QPair>
#include<QDebug>
#include<QColor>
#include<QMessageBox>
#include<QColorDialog>
#include<QTextBlock>
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
#define LINE_INFO_WIDTH 0   //预留边框用于信息展示
#define MARGIN 30           //视图左边距
#define NET_WIDTH 2000      //网络图最大宽度
#define NET_HEIGHT 2000     //网络图最大高度
#define SCENE_WIDTH (LINE_INFO_WIDTH+MARGIN*2+NET_WIDTH)    //视图宽度
#define SCENE_HEIGHT (MARGIN*2+NET_HEIGHT)                  //视图高度

#define EDGE_PEN_WIDTH 2    //线路边宽
#define NODE_HALF_WIDTH 3   //节点大小

using namespace  std;

class Station{
protected:
    QString name;//站点名称

    double longitude;//站点经度
    double latitude;//站点纬度

public:
    Station();
    Station(QString n,double longi, double lati);
    double calculateDistance(Station other);
    QPointF getCoord();
    friend class SubwayNet;
    friend class MainWindow;
};

#endif // STATION_H
