#include "httpdownload.h"
#include "ui_httpdownload.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDate>
#include <QTextStream>
#include <QProcess>
#include "QDate"
#include<QTimer>
#include<QTime>
QProcess * mypro;

HttpDownLoad::HttpDownLoad(QWidget *parent) :QMainWindow(parent), ui(new Ui::HttpDownLoad)
{
    ui->setupUi(this);

    //关联请求完毕信号finished(QNetworkReply* )
    connect(&manager, &QNetworkAccessManager::finished,this, &HttpDownLoad::read_data);
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));
}

HttpDownLoad::~HttpDownLoad()
{
    delete ui;
}

void HttpDownLoad::on_getBt_clicked()
{
    QString place = ui->placeBox->currentText();
    QString num;

#if 1
    if(place=="襄阳")
    {
        num = "101200201";
    }
    if(place == "武汉")
    {
        num="101200101";
    }
    if(place=="北京")
    {
        num = "101010100";
    }
    if(place == "广州")
    {
        num="101280101";
    }

    if(place=="宣城")
    {
        num = "101221401";
    }
    if(place == "南京")
    {
        num="101190101";
    }
#endif

    ui->numEdit->setText(num);
    QUrl url("https://devapi.qweather.com/v7/weather/7d?location=" + num +"&key=5d4700c9796647f1835490add25cffab");

    //创建一个请求对象
    QNetworkRequest request(url);

    //发送请求
    manager.get(request);

}

void HttpDownLoad::read_data(QNetworkReply *reply)
{
    qDebug()<<"读取天气预报数据"<<endl;
    QByteArray array = reply->readAll();//读取所有数据

    //json解析
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(array, &error);
    if(error.error != QJsonParseError::NoError)// 校验格式是否有错
    {
        qDebug()<<"json error";
        return ;
    }

    QJsonObject obj = doc.object();
    // QJsonObject weatherObj = obj.value("daily").toObject();
    QJsonArray forecast = obj.value("daily").toArray();
    // qDebug()<<forecast.at(0).toObject().value("fxDate").toString()<<endl;

    // 未来四天
    QLabel *week[4];  /* 星期几 */
    week[0] = ui->label_week1;
    week[1] = ui->label_week1_2;
    week[2] = ui->label_week1_3;
    week[3] = ui->label_week1_4;

    QLabel *date[4]; /* 日期 */
    date[0] = ui->label_date1;
    date[1] = ui->label_date2;
    date[2] = ui->label_date3;
    date[3] = ui->label_date4;

    QLabel *hw[5]; /*温度*/
    hw[0] = ui->label_hw;  /*当前温度*/
    hw[1] = ui->label_hw1;
    hw[2] = ui->label_hw2;
    hw[3] = ui->label_hw3;
    hw[4] = ui->label_hw4;

    QLabel *type[5]; /*天气预报*/
    type[0] = ui->label_type; /*当前天气预报*/
    type[1] = ui->label_type1;
    type[2] = ui->label_type2;
    type[3] = ui->label_type3;
    type[4] = ui->label_type4;

    QLabel *fxfl[5]; /* 风力 */
    fxfl[0] = ui->label_fxfl;  /*当前风力*/
    fxfl[1] = ui->label_fxfl1;
    fxfl[2] = ui->label_fxfl2;
    fxfl[3] = ui->label_fxfl3;
    fxfl[4] = ui->label_fxfl4;

    QLabel *aqi[5];
    aqi[0] = ui->label_aqi;
    aqi[1] = ui->label_aqi1;
    aqi[2] = ui->label_aqi2;
    aqi[3] = ui->label_aqi3;
    aqi[4] = ui->label_aqi4;

    QLabel *tyim[5];  /* 图标 */
    tyim[0] = ui->label_im;
    tyim[1] = ui->label_im1;
    tyim[2] = ui->label_im2;
    tyim[3] = ui->label_im3;
    tyim[4] = ui->label_im4;

    QTime tim = QTime::currentTime(); // 获取当前时间
    QDate date0 = QDate::currentDate();//获取当前日期
    QString date_msg = QString("%1-%2-%3").arg(date0.year()).arg(date0.month()).arg(date0.day());
    QLabel *time=ui->label_time;
    time->setText(date_msg+tim.toString());

    for(int i=1; i<5; i++)
    {
        QJsonObject o = forecast.at(i).toObject();
        qDebug()<<"当前城市:"<<ui->placeBox->currentText();
        qDebug()<<"时间:"<<o.value("fxDate").toString();
        qDebug()<<"最高温度:"<<o.value("tempMax").toString();
        qDebug()<<"最低温度:"<<o.value("tempMin").toString();
        qDebug()<<"白天:"<<o.value("textDay").toString();
        qDebug()<<"晚间:"<<o.value("textNight").toString();
        qDebug()<<"能见度"<<o.value("vis").toString();
        qDebug()<<"风向:"<<o.value("windDirDay").toString();
        qDebug()<<"风速"<<o.value("windScaleDay").toString();
        qDebug()<<"-----------------------";
        week[i-1]->setText(o.value("week").toString());
        date[i-1]->setText(o.value("fxDate").toString());
    }

    for(int i=0; i<5; i++)
    {
        QJsonObject o = forecast.at(i).toObject();
        hw[i]->setText(o.value("tempMin").toString() + "~" + o.value("tempMax").toString() + "℃");
        QString ty = o.value("textDay").toString();  /*白天气候*/
        type[i]->setText(ty);

        if(ty=="小雨")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/rain3.png);");
        }
        else if(ty=="中雨")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/rain2.png);");
        }
        else if(ty=="大雨")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/rain1.png);");
        }
        else if(ty=="阵雨")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/rain5.png);");
        }
        else if(ty=="晴")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/6.png);");
        }
        else if(ty=="阴")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/5.png);");
        }
        else if(ty=="多云")
        {
            tyim[i]->setStyleSheet("border-image: url(:/new/prefix1/images/cloudy1.png);");
        }

        fxfl[i]->setText(o.value("windDirDay").toString() +" "+ o.value("windScaleDay").toString()+"级");

        int aqi1 = o.value("vis").toString().toInt();
        if(aqi1 < 50)
        {
            aqi[i]->setText(QString::number(aqi1) + " 优");
        }
        else if(aqi1>=50 && aqi1< 100)
        {
            aqi[i]->setText(QString::number(aqi1) + " 中");
        }
        else
        {
            aqi[i]->setText(QString::number(aqi1) + " 差");
        }
    }
}

void HttpDownLoad::on_EXIT_clicked()
{
    mypro->close();
    exit(1);//退出程序
}

/*定时器根据城市ID实时更新*/
void HttpDownLoad::timerTimeOut()
{
    QTime tim = QTime::currentTime(); // 获取当前时间
    QDate date0 = QDate::currentDate();//获取当前日期
    QString date_msg = QString("%1-%2-%3").arg(date0.year()).arg(date0.month()).arg(date0.day());
    QLabel *time=ui->label_time;
    time->setText(date_msg+tim.toString());

    QString num;
    num=ui->numEdit->text().toUtf8().data();
    QUrl url("https://devapi.qweather.com/v7/weather/7d?location=" + num +"&key=5d4700c9796647f1835490add25cffab");

    //创建一个请求对象
    QNetworkRequest request(url);

    //发送请求
    manager.get(request);
}


