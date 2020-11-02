#ifndef LINE_H
#define LINE_H

#include"Station.h"
#include<QList>
class Line{
protected:
    int id;//线路编号
    QString name;//线路名称
    QList<Station> stations;//途经站点
    QColor color;//线路颜色
    QString from;//起点站
    QString to;//终点站
    bool isLoopLine;//是否为环线
    double transferTime;//换乘等待时间 min
    double speed;//列车车速 km/h
public:
    //构造函数
    Line();
    Line(int id,QString name, QColor lineColor,QString from,QString to,bool isLoopLine);
    friend class SubwayNet;
    friend class MainWindow;
};

#endif // LINE_H
