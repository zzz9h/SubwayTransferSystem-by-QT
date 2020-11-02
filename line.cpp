#include"Line.h"

Line::Line(){


}

Line::Line(int id,QString name, QColor lineColor,QString from,QString to,bool isLoopLine){
    this->id = id;
    this->name = name;
    this->color = lineColor;
    this->from = from;
    this->to = to;
    this->isLoopLine = isLoopLine;
    this->transferTime = 3;
    this->speed = 60;
}
