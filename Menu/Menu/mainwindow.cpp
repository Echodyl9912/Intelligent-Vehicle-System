#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mypushbutton.h"
#include<QAction>
#include<QPainter>
#include"mypushbutton.h"
#include<QString>
#include<QPushButton>
#include<QProcess>
#include<QDebug>
#include<QPainter>
#include<QTimer>
#include<QTime>
#include<QThread>
#include<QLabel>
#include <QScreen>
#include "QDate"

/*
 * 作者：化作尘
 * csdn：化作尘
 * 哔哩哔哩：化作尘my
 * 邮箱：2809786963@qq.com
 */

QProcess * mypro;
QTimer * time1, *mouse_timer;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    // ui->setupUi(this);
    QList <QScreen *> list_screen =  QGuiApplication::screens();

#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(1024, 600);
#endif

#if 0
    qDebug()<<"width = "<<list_screen.at(0)->geometry().width()<<endl;
    qDebug()<<"height = "<<list_screen.at(0)->geometry().height()<<endl;
#endif

    this->setWindowTitle("车载系统");
    this->setStyleSheet("background-color: rgb(72, 72, 72);");

    // 给进程分配空间
    mypro = new QProcess(this);

    // 安装事件过滤器
    this->installEventFilter(this);

    // 时间显示label
    time_label = new QLabel(this);
    time_label->setGeometry(10,20,300,120);//设置坐标
    time_label->setStyleSheet("font-size:55px;color:white");//设置大小颜色

    // 日期显示label
    date_label = new QLabel(this);
    date_label->setGeometry(10,140,300,120); //设置坐标
    date_label->setStyleSheet("font-size:55px;color:white");//设置大小颜色

    // 定时器刷新显示
    QTimer *timer = new QTimer(this);
    timer->start(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

#if 0
    connect(timer,&QTimer::timeout,[=]()
    {
        QTime tim = QTime::currentTime(); // 获取当前时间
        time_label->setText(tim.toString());

        QDate date = QDate::currentDate();//获取当前日期
        QString date_msg = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
        date_label->setText(date_msg);
        if(mypro->state() == QProcess::NotRunning)
        {
           this->show();//重新显示窗口
        }
        else this->hide();
    });
#endif

    // 定时器用来自动隐藏鼠标光标
    mouse_timer = new QTimer(this);

    // 隐藏鼠标
    connect(mouse_timer,&QTimer::timeout,[=](){this->setCursor(Qt::BlankCursor);});

    // 音乐按钮
    myPushButton * music_button=new myPushButton(":/pic/music.jpg","",300,240);
    time1= new QTimer(this);
    music_button->setParent(this);
    music_button->move(310,20);

    // 音乐按钮按下处理
    connect(music_button, &myPushButton::clicked,[=]()
    {
        music_button->zoom1();//弹跳
        music_button->zoom2();
        time1->start(500);
        connect(time1,&QTimer::timeout,[=]()
        {
            time1->stop();
            mypro->close();
            mypro->start("./musicplayer");});
        });

    // 硬件数据按钮
    myPushButton * hardware_button=new myPushButton(":/pic/setting.jpg","",300,240);
    time1= new QTimer(this);
    hardware_button->setParent(this);
    hardware_button->move(620,20);

    // 硬件数据按下处理
    connect(hardware_button,&myPushButton::clicked,[=]()
    {
        hardware_button->zoom1();//弹跳
        hardware_button->zoom2();
        time1->start(500);
        connect(time1,&QTimer::timeout,[=]()
        {
            time1->stop();
            mypro->close();
            mypro->start("./chart_temp");});
        });

    // 摄像头按钮
    myPushButton * video_button=new myPushButton(":/pic/camera.jpg","",300,240);
    video_button->setParent(this);
    video_button->move(10,340);

    // 摄像按钮按下处理
    connect(video_button,&myPushButton::clicked,[=]()
    {
        video_button->zoom1();//弹跳
        video_button->zoom2();
        time1->start(500);//定时500ms
        connect(time1,&QTimer::timeout,[=](){
            time1->stop(); //关闭定时器
            mypro->close();
            mypro->start("./opencv_camera");});
    });

    // 视频按钮
    myPushButton * map_button=new myPushButton(":/pic/video.jpg","",300,240);
    map_button->setParent(this);
    map_button->move(310,340);

    // 视频按钮按下处理
    connect(map_button,&myPushButton::clicked,[=](){
        map_button->zoom1();
        map_button->zoom2();
        time1->start(300);
        connect(time1,&QTimer::timeout,[=](){
            time1->stop();
            mypro->close();
            mypro->start("./videoplayer");});
    });

    // 天气按钮
    myPushButton * weather_button=new myPushButton(":/pic/weather.jpg","",300,240);
    weather_button->setParent(this);
    weather_button->move(620,340);

    // 天气按钮按下处理
    connect(weather_button,&myPushButton::clicked,[=](){
        weather_button->zoom1();// 弹跳
        weather_button->zoom2();
        time1->start(300);
        connect(time1,&QTimer::timeout,[=](){
            time1->stop();
            mypro->close();
            mypro->start("./HttpGetWeather");});
        });

    // 进程结束处理
    connect(mypro,&QProcess::stateChanged,[=](){});  /*无动作*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

//事件过滤器
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this )
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
                 mouse_timer->start(10000);
                 this->setCursor(Qt::ArrowCursor);  //显示正常鼠标
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(watched,event);//将事件传递给父类
}

void MainWindow::timerTimeOut()
{
    QTime tim = QTime::currentTime(); // 获取当前时间
    time_label->setText(tim.toString());

    QDate date = QDate::currentDate();//获取当前日期
    QString date_msg = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
    date_label->setText(date_msg);

    if(mypro->state() == QProcess::NotRunning)
    {
       this->show();//重新显示窗口
    }
    else this->hide();
}




