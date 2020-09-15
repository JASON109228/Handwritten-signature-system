#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QImage>
#include<QPen>
#include<QPainter>
#include<QMouseEvent>
#include<QDebug>
#include<QMoveEvent>
#include<QMouseEvent>
#include<QWidget>
#include<QFileDialog>
#include<QDateTime>
#include<QTime>
#include<QTimer>

#include<fcntl.h>
#include<sys/stat.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    unsigned char *pp =(unsigned char *) malloc(320*240*3);
    QImage *frame = new QImage(pp,320,240,QImage::Format_RGB888);
    frame->loadFromData(pp,320*240*3);

    imgpix = QPixmap::fromImage(*frame,Qt::AutoColor);

    ui->label->setPixmap(imgpix);

    QPainter painter(&imgpix);
    painter.setPen(pen);

    ui->label->setPixmap(imgpix);

    socket = new QTcpSocket(this);

    socket->connectToHost("127.0.0.1", 7700);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sendfile()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slot1()
{

    QPushButton *button = (QPushButton *)sender();
    QString s = button->text();

    if(s.compare("Red")==0)
    {
        pen.setColor(Qt::red);
        pen.setWidth(5);
    }
    if(s.compare("Green")==0)
    {
        pen.setColor(Qt::green);
        pen.setWidth(5);
    }
    if(s.compare("Blue")==0)
    {
        pen.setColor(Qt::blue);
        pen.setWidth(5);
    }
    if(s.compare("Save")==0)
    {
        QDateTime time=QDateTime::currentDateTime();
        strStarTime = time.toString("yyyyMMddhhmmss");
        qDebug()<<"strStarTime"<<strStarTime;
        QString filepath = "/opt/qt/";//jason老师可换为u12
        name = filepath+strStarTime;
        imgpix.save(name+".jpg");
       // imgpix.save("/home/jason/QT/qtpaint/1.jpg");
    }
    if(s.compare("Clear")==0)
    {

        imgpix.fill(Qt::black);

        ui->label->setPixmap(imgpix);
    }
    if(s.compare("Send")==0)
    {
        sendfile();
    }
    if(s.compare("Eraser")==0)
    {
        pen.setColor(Qt::black);
        pen.setWidth(25);
    }


}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    qDebug() <<event->x()<<" "<<event->y();
    int x=event->x();
    int y=event->y();


    QPainter painter(&imgpix);
    painter.setPen(pen);
    painter.drawPoint(x,y);

    ui->label->setPixmap(imgpix);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() <<event->pos().x()<<" "<<event->pos().y();

    int x=event->x();
    int y=event->y();

    QPainter painter(&imgpix);
    painter.setPen(pen);

    painter.drawPoint(x,y);

    ui->label->setPixmap(imgpix);
}




void MainWindow::sendfile()
{
    pname = name+".jpg";

    int fd = open(pname.toStdString().c_str(),O_RDONLY);

    struct stat filesata;

    stat(pname.toStdString().c_str(),&filesata);
    int filesize = filesata.st_size;

    QString sfilesize;
    sfilesize.setNum(filesize);

    QString fileinfo = "f:"+sfilesize;
    fileinfo =fileinfo+":";
    fileinfo = fileinfo+strStarTime+".jpg";
    fileinfo = fileinfo+":";

    //send file info
    socket->write(fileinfo.toLatin1());
    ::close(fd);


    file = new QFile(pname.toStdString().c_str());
    if (!file->open(QIODevice::ReadOnly))
        return;

     while (!file->atEnd()) {
         QByteArray line = file->read(1024);
         QString s(line);
         usleep(100000);
         qDebug() <<"send length:"<< line.length();
         socket->write(line);
     }

    file->close();

}






