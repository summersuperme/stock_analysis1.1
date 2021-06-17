#include "point.h"

Point::Point()
{

}

void Point::initpoint(double &min,double &span)
{
    double a;
    a = (this->price-min)*span*100;
    this->y = static_cast<int>(a);
}
