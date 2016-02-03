#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QLayout>
#include <QtNetwork>

#include <cv.h>
#include <highgui.h>

#include "simplehanddetector.h"
#include "svmhanddetector.h"

#ifndef IMG_B
#define IMG_B(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x)]
#endif
#ifndef IMG_G
#define IMG_G(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x) + 1]
#endif
#ifndef IMG_R
#define IMG_R(img,x,y) ((uchar*)(img->imageData + img->widthStep * (y)))[3 * (x) + 2]
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent *e);
private slots:
    void showCamer();
private:
    bool initCamer();
    void reduceLight(IplImage* img);
private:
    Ui::MainWindow *ui;
    QImage* qImg;
    CvCapture *capture;
    IplImage* frame;
    IplImage* frameCopy;
    IplImage* cvImg;
    QTimer* timer;
    QLabel* labels[10];

    HandDetector* hd;
    CvRect hand_area;
    int vx;
    int vy;

    QString res[10];

    int pre_ret;

    QUdpSocket *udp_sender;
};

#endif // MAINWINDOW_H
