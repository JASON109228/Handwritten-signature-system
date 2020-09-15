#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QtNetwork/QTcpServer>
#include<QtNetwork/QTcpSocket>

#include<QString>

#include<QMessageBox>
#include<fcntl.h>
#include<sys/stat.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

public slots:

    void acceptConnection();
    void displayError(QAbstractSocket::SocketError error);
    void readData();
private:
    Ui::MainWindow *ui;

    QTcpServer *server;
    QTcpSocket *client;
    QTcpSocket *socket;
    QString filesize ;
    QString filename;
    int filefd;
    QString temp;

    QMap<QString,QTcpSocket *> *clients;
};

#endif // MAINWINDOW_H
