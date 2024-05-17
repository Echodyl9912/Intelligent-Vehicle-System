#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *watched, QEvent *event);
    QLabel *date_label;
    QLabel *time_label;

private slots:
    /* 定时器时间到处理函数，获取时间、日期 */
    void timerTimeOut();
};
#endif // MAINWINDOW_H
