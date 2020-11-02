#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include<QGraphicsItem>
#include"subwaynet.h"
#include"graphics_view_zoom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //由线路表计算混合颜色
    QColor getLinesColor(QSet<QString>& lineSet);
    //获得线路表的名字集
    QString getLinesName(QList<int>& linesList);
    //将站点的经纬度地理坐标转为视图坐标
    QPointF transferCoord(QPointF coord);
    //绘制网络图的边
    void drawEdges(QVector<QPair<int,int>>& edges);
    void drawEdges(QVector<Edge>& edges);
    //绘制网络图的站点节点
    void drawStations (QVector<Station>& stations);
    void drawTrace();
    void on_actionLineMap_triggered();
private:
    Ui::MainWindow *ui;
    Graphics_view_zoom *myView;     //自定义视图，用于鼠标缩放
    QGraphicsScene *scene;          //场景
    SubwayNet* subwayNet;       //后端管理类
    QColor lineColor;           //当前选定的线路颜色

    QString startStation;
    QVector<QPair<int,int> > trace;//当前的线路导航
public slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_tabWidget_currentChanged(int idx);
};

#endif // MAINWINDOW_H
