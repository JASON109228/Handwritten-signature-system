#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include<QImage>
#include<QPen>
#include<QPainter>
#include<QMouseEvent>
#include<QPixmap>
#include<QMoveEvent>
#include<QString>
#include<QDateTime>
#include<QTime>
#include<QDebug>

#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>
#include<QString>
#include<QTimer>
#include<QFile>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
public slots:
    void slot1();
    void sendfile();

private:
    Ui::MainWindow *ui;

    QPixmap imgpix;
    QPen pen;
    QPainter painter;
    QImage *frame;
    QString name;
    QTcpSocket *socket;
    QTcpServer *server;
    QTcpSocket *client;
    QString strStarTime;
    QFile *file;
    QString pname;

};

#endif // MAINWINDOW_H
