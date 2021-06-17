#ifndef POINT_H
#define POINT_H

#include <QDebug>
#include <QString>
class Point
{
public:
    Point();
public:
    int x;
    int y;
    double getprice() const
    {
        return this->price;
    }
    void setprice(double &price)
    {
        this->price = price;
    }
    void setDate(QString &Date)
    {
        this->date = Date;
    }
    QString getDate() const
    {
        return date;
    }
    void initpoint(double &min,double &span);
private:
    double price;
    QString date;
};

#endif // POINT_H
