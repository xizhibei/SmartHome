#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frameCopy = NULL;
    qImg = NULL;

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(showCamer()));
    bool flag = initCamer();
    //hd = new SimpleHandDetector("f.txt",cvGetSize(frame));
    hd = new SVMHandDetector("svmResult.txt",cvGetSize(frame));


    hand_area = cvRect(frame->width / 2 - 100,frame->height / 2 - 100,200,200);
    vx = 5;
    vy = 5;

    if(!flag)
    {
        //ui->imgLabel->setText("Camer not found...");
        //ui->fromFile->setChecked(true);
        //ui->startGrab->setEnabled(false);
       // ui->picNum->setEnabled(false);
    }
    else
    {
        timer->setInterval(100);
        timer->start();
    }

    for(int i = 0;i < 10;i++)
    {
        labels[i] = new QLabel();
        res[i] = QString::fromUtf8(":/hands/hands/") + QString(i + '0') + QString::fromUtf8(".png");
        labels[i]->setPixmap(QPixmap(res[i]).scaled(100,100, Qt::KeepAspectRatio));
        ui->horizontalLayout->addWidget(labels[i]);
    }

    udp_sender=new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hd;
}

bool MainWindow::initCamer()
{
    capture = cvCaptureFromCAM(1);
    if(!capture)
    {
        QMessageBox::information(this,"Error","Can not open camer!!!");
        return false;
    }
    if(frame = cvQueryFrame(capture))
    {
        qImg = new QImage(QSize(frame->width,frame->height),QImage::Format_RGB888);
        cvImg = cvCreateImageHeader(cvGetSize(frame),IPL_DEPTH_8U,3);
        cvImg->imageData = (char*)qImg->bits();
    }
    return true;
}

void MainWindow::reduceLight(IplImage *img)
{
#define RATE 0.4
    for(int i = 0;i < img->height;i++)
        for(int j = 0;j < img->width;j++)
        {
            IMG_B(img,j,i) *= RATE;
            IMG_G(img,j,i) *= RATE;
            IMG_R(img,j,i) *= RATE;
        }
}

void MainWindow::showCamer()
{
    if(frame = cvQueryFrame(capture))
    {
        if(frameCopy)
            cvReleaseImage(&frameCopy);
        frameCopy = cvCloneImage(frame);
        //cvRectangle(frame,center1,center2,cvScalar(255,0,255));
        //cvLine(frame,crossX1,crossX2,cvScalar(255,0,255));
        //cvLine(frame,crossY1,crossY2,cvScalar(255,0,255));
        int ret = hd->detect_hand(frame,&hand_area);
        if(ret == -1)
        {
            if(hand_area.x < 0 || hand_area.x + hand_area.width >= frame->width)
                vx = -vx;
            if(hand_area.y < 0 || hand_area.y + hand_area.height >= frame->height)
                vy = -vy;
            hand_area.x += vx;
            hand_area.y += vy;
        }else if(ret >= 0 && ret < 10)
        {
            ui->label->setPixmap(QPixmap(res[ret]).scaled(150,150, Qt::KeepAspectRatio));
            if(ret == 0 && pre_ret != 0)
            {
                QByteArray datagram = QString(pre_ret + '0').toAscii();
                udp_sender->writeDatagram(datagram.data(),datagram.size(),QHostAddress("192.168.102.21"),45454);
                qDebug() << "Sended!";
            }
            pre_ret = ret;
        }
        CvRect rect = hand_area;
        reduceLight(frame);
        //cvSmooth(frame,frame,CV_GAUSSIAN,7,7,3,3);
        cvSetImageROI(frameCopy,rect);
        cvSetImageROI(frame,rect);
        cvCopy(frameCopy,frame);
        cvResetImageROI(frameCopy);
        cvResetImageROI(frame);
        cvRectangle(frame, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height), cvScalar(255, 255, 255), 2);
        cvLine(frame,cvPoint(rect.x + rect.width / 2,0),                   cvPoint(rect.x + rect.width / 2,rect.y),            cvScalar(255,255,255),1);
        cvLine(frame,cvPoint(rect.x + rect.width / 2,rect.y + rect.height),cvPoint(rect.x + rect.width / 2,frame->height - 1),cvScalar(255,255,255),1);
        cvLine(frame,cvPoint(0,rect.y + rect.height / 2),                  cvPoint(rect.x,rect.y + rect.height / 2),           cvScalar(255,255,255),1);
        cvLine(frame,cvPoint(rect.x + rect.width,rect.y + rect.height / 2),cvPoint(frame->width - 1,rect.y + rect.height / 2),cvScalar(255,255,255),1);

        if (frame->origin == IPL_ORIGIN_TL)
        {
            cvCopy(frame,cvImg,0);
        }
        else
        {
            cvFlip(frame,cvImg,0);
        }
        cvCvtColor(cvImg,cvImg,CV_BGR2RGB);
        this->update();
    }
}
void MainWindow::paintEvent(QPaintEvent *e)
{
    if(qImg != NULL)
    {
        QPainter painter(this);
        painter.drawImage(QRect(10,30,640,480),*qImg);
    }
}
