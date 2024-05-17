#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class HttpDownLoad;
}

class HttpDownLoad : public QMainWindow
{
    Q_OBJECT

public:
    explicit HttpDownLoad(QWidget *parent = nullptr);
    ~HttpDownLoad();

private slots:
    void on_getBt_clicked();
    void read_data(QNetworkReply *reply);
    // void on_HttpDownLoad_iconSizeChanged(const QSize &iconSize);
    void on_EXIT_clicked();
    void timerTimeOut();

private:
    Ui::HttpDownLoad *ui;
    QNetworkAccessManager manager;
};

#endif // HTTPDOWNLOAD_H
