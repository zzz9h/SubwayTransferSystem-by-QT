#include"Station.h"
#include<math.h>
Station::Station(){

}

Station::Station(QString n,double longi, double lati){
    this->name = n;
    this->longitude = longi;
    this->latitude = lati;
}

QPointF Station::getCoord(){
    return QPointF(longitude,latitude);
}

double Station::calculateDistance(Station other){
    double delta_y = 111*(this->latitude - other.latitude);
    double delta_x = 111*(this->longitude - other.longitude);
    return sqrt(delta_x*delta_x+delta_y*delta_y);
}
