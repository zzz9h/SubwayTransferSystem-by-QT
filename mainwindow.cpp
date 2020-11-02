#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDialogButtonBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myView = new Graphics_view_zoom(ui->graphicsView);
    myView->set_modifiers(Qt::NoModifier);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene=new QGraphicsScene;
    scene->setSceneRect(-LINE_INFO_WIDTH,0,SCENE_WIDTH,SCENE_HEIGHT);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    subwayNet=new SubwayNet();


    QDialog  dialog;
    QDialogButtonBox *button = new QDialogButtonBox(&dialog);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(button, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QLabel* label= new QLabel("是否加载配置文件");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(button);
    dialog.setLayout(layout);


    if ( dialog.exec() == QDialog::Accepted)
    {
        bool flag = subwayNet->readFileData("C:/Users/zzz9h/Desktop/11/subway-guide/SubwayGuide/outLine.txt");
        if (!flag)
        {
            QMessageBox box;
            box.setWindowTitle(tr("error information"));
            box.setIcon(QMessageBox::Warning);
            box.setText("读取数据错误!\n将无法展示内置线路！");
            box.addButton(tr("确定"), QMessageBox::AcceptRole);
            if (box.exec() == QMessageBox::Accepted)
            {
                box.close();
            }
        }
    }

    on_actionLineMap_triggered();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//由线路表计算混合颜色
QColor MainWindow::getLinesColor(QSet<QString>& lineSet)
{
    QColor color1=QColor(255,255,255);
    QColor color2;
    for(QString lineName:lineSet){
        int lineHash = subwayNet->linesHash[lineName][0];
        color2 = subwayNet->lines[lineHash].color;
        color1.setRed(color1.red()*color2.red()/255);
        color1.setGreen(color1.green()*color2.green()/255);
        color1.setBlue(color1.blue()*color2.blue()/255);
    }
    return color1;
}


//将站点的经纬度地理坐标转为视图坐标
QPointF MainWindow::transferCoord(QPointF coord)
{
    QPointF minCoord=subwayNet->getMinCoord();
    QPointF maxCoord=subwayNet->getMaxCoord();
    double x = (coord.x()-minCoord.x())/(maxCoord.x()-minCoord.x())*NET_WIDTH+MARGIN;
    double y = (maxCoord.y()-coord.y())/(maxCoord.y()-minCoord.y())*NET_HEIGHT+MARGIN;
    return QPointF(x,y);
}

//绘制网络图的边
void MainWindow::drawEdges(QVector<Edge>& edges){
    for(Edge e:edges){
        int s1 = e.from;
        int s2 = e.to;
        QString stationName1 = subwayNet->stations[s1].name;
        QString stationName2 = subwayNet->stations[s2].name;
        QString lineName = e.lineName;
        int lineId = subwayNet->linesHash[lineName][0];
        QSet<QString>lineSet;
        lineSet.insert(lineName);
        QColor color = getLinesColor(lineSet);
        QColor(subwayNet->lines[lineId].color);
        QString tip="途经： "+stationName1+"--"+stationName2+"\n线路："+lineName;
        QPointF s1Pos=transferCoord(subwayNet->stations[s1].getCoord());
        QPointF s2Pos=transferCoord(subwayNet->stations[s2].getCoord());
        QGraphicsLineItem* edgeItem=new QGraphicsLineItem();
        edgeItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        edgeItem->setCursor(Qt::PointingHandCursor);
        edgeItem->setToolTip(tip);
        edgeItem->setPos(s1Pos);
        edgeItem->setLine(0, 0, s2Pos.x()-s1Pos.x(), s2Pos.y()-s1Pos.y());
        scene->addItem(edgeItem);
    }
}

void MainWindow::drawEdges(QVector<QPair<int,int>>& edges){
    for(QPair<int,int> e:edges){
        int s1 = e.first;
        int s2 = e.second;
        Station sta1 = subwayNet->stations[s1];
        Station sta2 = subwayNet->stations[s2];
        QSet<QString> lineSet = subwayNet->getLinesNameBetweenStation(s1,s2);

        QColor color = getLinesColor(lineSet);
        QString tip="途经： "+sta1.name+"--"+sta2.name+"\n线路：";
        for(QString lineName:lineSet){
            tip += lineName;
        }
        QPointF s1Pos=transferCoord(subwayNet->stations[s1].getCoord());
        QPointF s2Pos=transferCoord(subwayNet->stations[s2].getCoord());
        QGraphicsLineItem* edgeItem=new QGraphicsLineItem();
        edgeItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        edgeItem->setCursor(Qt::PointingHandCursor);
        edgeItem->setToolTip(tip);
        edgeItem->setPos(s1Pos);
        edgeItem->setLine(0, 0, s2Pos.x()-s1Pos.x(), s2Pos.y()-s1Pos.y());
        scene->addItem(edgeItem);

    }
}

//绘制网络图的站点节点
void MainWindow::drawStations (QVector<Station>& stations){
    QVector<Station>::iterator iter;
    for(iter = stations.begin();iter!=stations.end();iter++){
        QString name = iter->name;
        QColor color= QColor();
        QPointF coord=transferCoord(iter->getCoord());
        QString tip="站名：  "+name+"\n"+
                "经度：  "+QString::number(iter->getCoord().x(),'f',7)+"\n"+
                "纬度：  "+QString::number(iter->getCoord().y(),'f',7)+"\n";

        QGraphicsEllipseItem* stationItem=new QGraphicsEllipseItem();
        stationItem->setRect(-NODE_HALF_WIDTH, -NODE_HALF_WIDTH, NODE_HALF_WIDTH<<1, NODE_HALF_WIDTH<<1);
        stationItem->setPos(coord);
        //qDebug()<<coord.x()<<coord.y();
        stationItem->setPen(color);
        stationItem->setCursor(Qt::PointingHandCursor);
        stationItem->setToolTip(tip);

        //        if(iter->linePass.size()<=1)
        //        {
        //            stationItem->setBrush(QColor(QRgb(0xffffff)));
        //        }

        scene->addItem(stationItem);

        QGraphicsTextItem* textItem=new QGraphicsTextItem();
        textItem->setPlainText(name);
        textItem->setFont(QFont("consolas",4,1));
        textItem->setPos(coord.x(),coord.y()-NODE_HALF_WIDTH*2);
        scene->addItem(textItem);
    }
}

void MainWindow::on_actionLineMap_triggered()
{
    //    statusLabel3->setText(tr("图示：上海地铁网络线路图"));
    scene->clear();
    QVector<QPair<int,int>>edges;
    for(Edge edge:subwayNet->edges){
        if(edges.contains(QPair<int,int>(edge.from,edge.to))||edges.contains(QPair<int,int>(edge.to,edge.from))){
            continue;
        }
        edges.push_back(QPair<int,int>(edge.from,edge.to));
    }
    drawEdges(edges);
    drawStations(subwayNet->stations);
}

void MainWindow::on_pushButton_clicked(){
    QString stationName = ui->lineEdit->text();
    QString longiString = ui->lineEdit_2->text();
    QString latiString = ui->lineEdit_3->text();

    if(stationName.isEmpty()||longiString.isEmpty()||latiString.isEmpty()){
        QMessageBox::information(this,"提示","请将信息填写完整");
        return;
    }

    bool ok;
    double longitude = longiString.toDouble(&ok);
    if(!ok){
        QMessageBox::information(this,"提示","经度包含非法字符");
        return;
    }
    if(longitude <121 || longitude > 122){
        QMessageBox::information(this,"提示","经度超出范围");
        return;
    }

    double latitude = latiString.toDouble(&ok);
    if(!ok){
        QMessageBox::information(this,"提示","纬度包含非法字符");
        return;
    }
    if(latitude<30.9 || latitude>31.5){
        QMessageBox::information(this,"提示","经度超出范围");
        return;
    }

    if(subwayNet->addStation(stationName,longitude,latitude)==ERROR){
        QMessageBox::information(this,"提示","站点已存在");
        return;
    }

    on_actionLineMap_triggered();
    return;
}

void MainWindow::on_pushButton_5_clicked(){
    QString lineName = ui->lineEdit_6->text();
    QString from = ui->lineEdit_10->text();
    QString to = ui->lineEdit_11->text();
    bool isLoopLine = ui->radioButton->isChecked();
    if(lineName.isEmpty()){
        QMessageBox::information(this,"提示","请输入线路名称");
        return;
    }
    if(from.isEmpty()){
        QMessageBox::information(this,"提示","请输入起点站");
        return;
    }
    if(to.isEmpty()){
        QMessageBox::information(this,"提示","请输入终点站");
        return;
    }
    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()){
        QMessageBox::information(this,"提示","请选择是否为环线");
        return;
    }

    QVector<QString> stationNames;
    QTextDocument* doc=ui->plainTextEdit->document () ; //文本对象
    int cnt = doc->blockCount();
    for(int i = 0; i < cnt; i++){
        QTextBlock textLine=doc->findBlockByNumber (i) ; // 文本中的一段
        QString stationName=textLine.text();
        if(subwayNet->stationsHash.find(stationName) == subwayNet->stationsHash.end()){
            QMessageBox::information(this,"提示","途经站点中包含不存在的站点");
            return;
        }
        stationNames.push_back(stationName);
    }

    if(subwayNet->addLine(lineName,lineColor,from,to,isLoopLine)==ERROR){
        QMessageBox::information(this,"提示","该线路已被添加");
        return;
    }

    for(int i = 0; i < stationNames.size() - 1; i++){
        subwayNet->addEdge(stationNames[i],stationNames[i+1],subwayNet->lines.size()-1);
    }
    if(isLoopLine){
        subwayNet->addEdge(stationNames.last(),stationNames.first(),subwayNet->lines.size()-1);
    }
    on_actionLineMap_triggered();
}

void MainWindow::on_pushButton_3_clicked(){
    QColorDialog colorDialog;
    colorDialog.setOptions(QColorDialog::ShowAlphaChannel);
    colorDialog.exec();
    lineColor=colorDialog.currentColor();

}

void MainWindow::on_pushButton_2_clicked(){
    QString stationName1 = ui->lineEdit_4->text();
    QString stationName2 = ui->lineEdit_5->text();
    if(stationName1.isEmpty()){
        QMessageBox::information(this,"提示","请输入起点站");
        return;
    }
    if(stationName2.isEmpty()){
        QMessageBox::information(this,"提示","请输入终点站");
        return;
    }
    if(subwayNet->stationsHash.find(stationName1) == subwayNet->stationsHash.end()){
        QMessageBox::information(this,"提示","起点站不存在");
        return;
    }
    if(subwayNet->stationsHash.find(stationName2) == subwayNet->stationsHash.end()){
        QMessageBox::information(this,"提示","目的站不存在");
        return;
    }

    startStation = stationName1;
    trace = subwayNet->getSubwayTransferGuide(stationName1,stationName2);
    drawTrace();

}

void MainWindow::on_tabWidget_currentChanged(int idx){
    if(idx == 0||idx ==1){
        on_actionLineMap_triggered();
    }
}

void MainWindow::drawTrace(){
    QString content = "";
    int preLineId = -1;
    int preStationId = subwayNet->stationsHash[startStation];
    QVector<QPair<int,int> >::iterator iter;
    QVector<Station> stations;
    stations.push_back(subwayNet->stations[subwayNet->stationsHash[startStation]]);
    QVector<Edge>edges;
    for(iter = trace.begin(); iter !=trace.end(); iter++){
        stations.push_back(subwayNet->stations[iter->first]);
        edges.push_back(Edge(preStationId,iter->first,subwayNet->lines[iter->second].name));

        if(preLineId != iter->second){
            content += subwayNet->stations[preStationId].name;
            content += "      "+        subwayNet->lines[iter->second].name+"\n";
            preLineId = iter->second;
        }
        preStationId = iter->first;
    }
    content += subwayNet->stations[(trace.end()-1)->first].name;
    ui->textBrowser->setPlainText(content);
    scene->clear();
    drawEdges(edges);
    drawStations(stations);
}
