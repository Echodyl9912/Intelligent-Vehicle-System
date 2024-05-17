#include "led.h"
#include <QDebug>


Led::Led(QWidget *parent) : QWidget (parent)
{
    system("echo none > /sys/class/leds/sys-led/trigger");

    /* 开发板的LED控制接口 */
    file.setFileName("/sys/devices/platform/leds/leds/sys-led/brightness");

    if (!file.exists())
        /* 设置按钮的初始化文本 */
        ui->ledBt->setText("未获取到LED设备！");

    /* 获取LED的状态 */
    getLedState();

    /* 信号槽连接 */
    connect(ui->ledBt, SIGNAL(clicked()),
            this, SLOT(on_ledBt_clicked()));
}

Led::~Led()
{

}

void Led::setLedState()
{ /* 在设置LED状态时先读取 */
    bool state = getLedState();

    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"0", "1"};

    /* 写0或1 */
    if (state)
        file.write(buf[0]);
    else
        file.write(buf[1]);

    /* 关闭文件 */
    file.close();

    /*重新获取LED的状态 */
    getLedState();

}

bool Led::getLedState()
{
    /* 如果文件不存在，则返回 */
       if (!file.exists())
           return false;

       if(!file.open(QIODevice::ReadWrite))
           qDebug()<<file.errorString();

       QTextStream in(&file);

       /* 读取文件所有数据 */
       QString buf = in.readLine();

       /* 打印出读出的值 */
       qDebug()<<"buf: "<<buf<<endl;
       file.close();
       if (buf == "1") {
           ui->ledBt->setText("LED点亮");
           return true;
       } else {
           ui->ledBt->setText("LED熄灭");
           return false;
       }
}

void Led::on_ledBt_clicked()
{
    /* 设置LED的状态 */
       setLedState();
}
