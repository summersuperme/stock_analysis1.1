#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("color:rgb(255,255,0)");
    ui->label_2->setStyleSheet("color:rgb(255,0,0)");
    ui->label_3->setStyleSheet("color:rgb(0,0,255)");
    span = 1;
    min = 0;
    max = 1;
    QColor color(0,0,30);
    QPalette pale;
    pale.setColor(QPalette::Background,color);
    setAutoFillBackground(true);
    setPalette(pale);

}

MyWidget::~MyWidget()
{
    delete ui;
}

/*
 * 绘制坐标轴
*/
void MyWidget::paintEvent(QPaintEvent *)
{
    QPen pen(Qt::white);
    QString price;
    double tmp;
    QString data;
    QPainter painter(this);
    painter.setPen(pen);
    //x轴长度900
    painter.drawLine(100,520,1000,520);
    //y轴高度500
    painter.drawLine(100,20,100,520);
    //x轴坐标绘制
    for (int i=125;i<1000;i+=MyWidget::_X) {
        painter.drawLine(i,516,i,520);
        painter.drawText(i-2,515,data);
    }
    //y轴坐标绘制
    for (int i=452,j=1;i>0;i-=MyWidget::_Y,j++) {
        painter.drawLine(100,i,104,i);
        tmp = min + 0.48/span*j;
        price = QString("%1").arg(tmp);
        painter.drawText(25,i+5,price);
    }
    charts(painter);
}

/*
 * 绘制折线
 * 参数说明:
 * painter:QPainter类用于绘制折线的笔。
*/
void MyWidget::charts(QPainter &painter)
{
    QPen pen(Qt::yellow);
    pen.setWidth(3);
    painter.setPen(pen);
    //支持位
    for (int i=0;i<pointZ.size()-1;i++) {
        painter.drawLine(100+pointZ.at(i).x*MyWidget::_X,520-pointZ.at(i).y,100+pointZ.at(i+1).x*MyWidget::_X,520-pointZ.at(i+1).y);
    }
    pen.setColor(Qt::red);
    painter.setPen(pen);
    //压力位
    for (int i=0;i<pointY.size()-1;i++) {
        painter.drawLine(100+pointY.at(i).x*MyWidget::_X,520-pointY.at(i).y,100+pointY.at(i+1).x*MyWidget::_X,520-pointY.at(i+1).y);
    }
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    //收盘价
    for (int i=0;i<pointS.size()-1;i++) {
        painter.drawLine(100+pointS.at(i).x*MyWidget::_X,520-pointS.at(i).y,100+pointS.at(i+1).x*MyWidget::_X,520-pointS.at(i+1).y);
    }
}

/*
 * 点击事件,作用打开并处理文件内容
*/
void MyWidget::on_pushButton_clicked()
{
    Point temp;
    QStringList Qlist;
    QStringList QlistZ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("ALL files(*.*);;Images (*.png *.xpm *.jpg)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"文件","打开失败");
        return;
    }
    exsit = true;
    QTextStream txtInput(&file);
    QString linestr;
    QString line;
    //获取日期
    line = file.readLine();
    if(!line.contains("日期:"))
    {
        QMessageBox::warning(this,"警告","文件格式有误:请输入日期!!");
        return;
    }
    else {
        line.remove("日期:");
        line.remove("\n");
        QDate = line.split(",");
        len = QDate.length();
    }
    //将股票分组
    while (!file.atEnd()) {
        line = file.readLine();
        while(!line.contains("股票:"))
        {
            linestr += line;
            line = file.readLine();
        }
        if(line.contains("股票:"))
        {
            line.remove("股票:");
            line.remove("\n");
            stocks_name.append(line);
        }
        stocks.append(linestr);
        linestr.clear();
    }
    curnum = 0;
    switchto();
    file.close();
}
/*
 * 获取当前最小价格
 * 参数:Qlist为分割之后的字符串数据链表
*/
double MyWidget::getminprice(QStringList &Qlist)
{
    double min = Qlist[0].toDouble();
    double temp;

    for(int i=0;i<Qlist.size();i++)
    {
        temp = Qlist[i].toDouble();
        if(temp < min)
        {
            min = temp;
        }
    }
    return min;
}
/*
 * 获取当前最大价格
 * 参数:Qlist为分割之后的字符串数据链表
*/
double MyWidget::getmaxprice(QStringList &Qlist)
{
    double max = 0;
    double temp;

    for(int i=0;i<Qlist.size();i++)
    {
        temp = Qlist[i].toDouble();
        if(temp > max)
        {
            max = temp;
        }
    }
    return max;
}

void MyWidget::setspan()
{
    double tmp;
    tmp = max - min;
    for (double i=0.03125;i < 50;i*=2) {
        if(tmp*100*i>500)
        {
            span = i/2;
            break;
        }
    }
}

void MyWidget::mousePressEvent(QMouseEvent *event)
{
    if(stocks.length() == 0)
    {
         QMessageBox::warning(this,"警告","请先导入文件!!");
         return;
    }

    int j;
    QString info;
    QPointF point;
    QString state;
    point = event->localPos();
    if(point.x()>100 && point.x()<1000 && point.y()>20 && point.y()<520)
    {
        j = static_cast<int>((point.x()-125)/MyWidget::_X);
        if(j>len)
            return;
        if((pointY[j].getprice() - pointS[j].getprice()) < (pointS[j].getprice() - pointZ[j].getprice()))
        {
            state = "高位";
        }else{
            state = "低位";
        }
        info = "支持位:" + QString("%1").arg(pointZ[j].getprice()) + "\n" + "压力位:" + QString("%1").arg(pointY[j].getprice()) + "\n" + "收盘价:" + QString("%1").arg(pointS[j].getprice()) + "\n" + state + "\n" + "日期:" +pointS[j].getDate();
        QMessageBox::information(this,"股票信息",info);
    }
}

void MyWidget::switchto()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->name->setPalette(pa);
    ui->name->setText(stocks_name[curnum]);
    pointS.clear();
    pointY.clear();
    pointZ.clear();
    Point temp;
    double tmpprice;
    QStringList Qlist;
    QStringList QlistZ;
    QStringList stockbox;
    QString line;
    stockbox = stocks[curnum].split("\n");
    for (int i=0;i<stockbox.size();i++) {
        line = stockbox[i];
        if(line.contains("支持位:"))
        {
            line.remove("支持位:");
            line.remove("\n");
            QlistZ = line.split(",");
            if(QlistZ.length() != len)
            {
                QMessageBox::warning(this,"警告","支持位数据有误!!");
                return;
            }
            min = getminprice(QlistZ);
        }
        if(line.contains("压力位:"))
        {
            line.remove("压力位:");
            line.remove("\n");
            Qlist = line.split(",");
            if(Qlist.length() != len)
            {
                QMessageBox::warning(this,"警告","压力位数据有误!!");
                return;
            }
            max = getmaxprice(Qlist);
            for (int i=0;i<Qlist.size();i++) {
                tmpprice = Qlist[i].toDouble();
                temp.setprice(tmpprice);
                setspan();
                temp.initpoint(min,span);
                temp.x = i+1;
                pointY.append(temp);
            }
        }
        if(line.contains("收盘价:"))
        {
            line.remove("收盘价:");
            line.remove("\n");
            Qlist = line.split(",");
            if(Qlist.length() != len)
            {
                QMessageBox::warning(this,"警告","收盘价数据有误!!");
                return;
            }
            for (int i=0;i<Qlist.size();i++) {
                tmpprice = Qlist[i].toDouble();
                temp.setprice(tmpprice);
                temp.initpoint(min,span);
                temp.x = i+1;
                temp.setDate(QDate[i]);
                pointS.append(temp);
            }
        }
    }
    for (int i=0;i<QlistZ.size();i++) {
        tmpprice = QlistZ[i].toDouble();
        temp.setprice(tmpprice);
        temp.initpoint(min,span);
        temp.x = i+1;
        pointZ.append(temp);
    }
}

void MyWidget::on_last_clicked()
{
    if(!exsit)
        return;
    update();
    if(--curnum < 0)
    {
        curnum = stocks.size()-1;
        switchto();
        update();
    }else{
        switchto();
    }
}

void MyWidget::on_next_clicked()
{
    if(!exsit)
        return;
    update();
    if(++curnum > stocks.size()-1)
    {
        curnum = 0;
        switchto();
        update();
    }else{
        switchto();
    }
}
