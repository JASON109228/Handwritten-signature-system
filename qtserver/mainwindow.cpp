#include "mainwindow.h"
#include "ui_mainwindow.h"


#include<fcntl.h>
#include<sys/stat.h>
#include<QString>
#include<QFile>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    clients = new QMap<QString,QTcpSocket *>();

    ui->setupUi(this);

    server = new QTcpServer (this);

    if(!server-> listen(QHostAddress::Any,7700))
    {
        qDebug()<<"address alread bind";
        QMessageBox::information(this,"error","address alread bind");
        exit(1);
    }
    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::acceptConnection()
{
    QTcpSocket *c = server->nextPendingConnection();


    connect(c,SIGNAL(readyRead()),this,SLOT(readData()));

    connect(c,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    client=c;

}

void MainWindow::displayError(QAbstractSocket::SocketError error)
{
    qDebug()<<client->errorString();
    client->close();
}

int flag=0;
int glength=0;

void MainWindow::readData()
{
    QTcpSocket *client = (QTcpSocket *)sender();

    QByteArray data = client->readAll();

    QString str(data);

    ui->textEdit->append(str);


    QStringList ls = str.split(":");
    glength=0;
    filesize = ls[1];
    filename = ls[2];
    qDebug()<<filesize<<"-"<<filename;

    temp = "/opt/qt/n_"+filename;//jason老师可换为u12

    filefd = open(temp.toStdString().c_str(),O_CREAT | O_RDWR,0777);
    if(filefd>0)
    {
        qDebug()<<"file create success";
    }
    flag = 1;
    ::close(filefd);
    qDebug()<<"close success";


    QFile file(temp.toStdString().c_str());

    file.open(QIODevice::WriteOnly);

    QString chang = filename+filesize;

    int len =chang.length()+4;
    int len1 = filename.length();
    int len2 = filesize.length();

    qDebug()<<"len:"<<len;
    qDebug()<<"len1:"<<len1;
    qDebug()<<"len2:"<<len2;

    glength += data.length();

    data.remove(0,len);

    file.write(data);


    qDebug() << "write length" << data.length() << "get length:" << glength;

    file.close();


}





















