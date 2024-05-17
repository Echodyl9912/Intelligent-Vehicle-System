#ifndef LED_H
#define LED_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>


class Led : public QWidget
{
    Q_OBJECT
#include "ui_widget.h"
public:
    explicit Led(QWidget *parent = 0);
    ~Led();

     Ui::Widget *ui;

private:

    /* 文件 */
    QFile file;

    /* 设置lED的状态 */
    void setLedState();

    /* 获取lED的状态 */
    bool getLedState();

private slots:
    //


    void on_ledBt_clicked();
};

#endif // LED_H
