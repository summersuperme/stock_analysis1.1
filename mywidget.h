#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QApplication>
#include "point.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QMouseEvent>
#include <QPointF>
QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
public:
    void paintEvent(QPaintEvent *event);
    void charts(QPainter &painter);
    double getminprice(QStringList &Qlist);
    double getmaxprice(QStringList &Qlist);
    void setspan();
    void mousePressEvent(QMouseEvent *event);
    void switchto();
private slots:
    void on_pushButton_clicked();

    void on_last_clicked();

    void on_next_clicked();

private:
    Ui::MyWidget *ui;
    QList<QString> stocks;//股票集合
    QList<QString> stocks_name;//股票名字集合
    QList<Point> pointZ;
    QList<Point> pointY;
    QList<Point> pointS;
    QStringList QDate;
public:
    static const int _X = 30;
    static const int _Y = 48;
    double min;
    double max;
    double span;//跨度
    int  curnum;
    bool exsit = false;
    int len;
};
#endif // MYWIDGET_H
