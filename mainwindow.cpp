#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QStringList arguments, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    unsigned t0, t1, t2, t3;
    t0=clock();    

    //direccion de la imagen en el primer paramtero
    Mat inputImage = cv::imread(arguments[1].toUtf8().constData());
    Mat auxImage = cv::imread(arguments[1].toUtf8().constData());

    //gamma se pasa como segundo parametro
    float gamma = arguments[2].toUtf8().toFloat();

    Mat dst = auxImage.clone();
    const int channels = dst.channels();
    switch (channels) {
    case 1:
    {
        MatIterator_<uchar> it,end;
        for (it=dst.begin<uchar>(),end = dst.end<uchar>(); it != end; it++){
            *it = saturate_cast<uchar>(pow((float)(*it / 255.0), gamma) * 255.0f);
        }
        break;
    }
    case 3:
    {
        MatIterator_<Vec3b> it,end;
        for(it = dst.begin<Vec3b>(), end = dst.end<Vec3b>(); it != end; it++){

            (*it)[0] = saturate_cast<uchar>(pow((float)((*it)[0] / 255.0), gamma) * 255.0f);

            (*it)[1] = saturate_cast<uchar>(pow((float)((*it)[1] / 255.0), gamma) * 255.0f);

            (*it)[2] = saturate_cast<uchar>(pow((float)((*it)[2] / 255.0), gamma) * 255.0f);
        }
        break;
    }
    }
    imshow("Imagen Original",inputImage);
    imshow("Imagen Modificada",dst);
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion realizando operacion pixel a pixel: " << time << endl;

    //tiempo de metodo por tabla
    t2=clock();

    unsigned char lut[256];
    for (int i = 0; i < 256; i++){
    lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
    }
    Mat dst2 = auxImage.clone();
    const int channels2 = dst2.channels();
    switch (channels2) {
    case 1:
    {
        MatIterator_<uchar> it,end;
        for (it=dst2.begin<uchar>(),end = dst2.end<uchar>(); it != end; it++){
            *it = lut[(*it)];
        }
        break;
    }
    case 3:
    {
        MatIterator_<Vec3b> it,end;
        for(it = dst2.begin<Vec3b>(), end = dst2.end<Vec3b>(); it != end; it++){

            (*it)[0] = lut[((*it)[0])];

            (*it)[1] = lut[((*it)[1])];

            (*it)[2] = lut[((*it)[2])];
        }
        break;
    }
    }

    imshow("Imagen modificada con operacion mediante tabla",dst2);
    t3 = clock();
    double time2 = (double(t3-t2)/CLOCKS_PER_SEC);
    cout << "Tiempo de ejecucion utilizando tabla precalculada: " << time2 << endl;
}


MainWindow::~MainWindow()
{
    delete ui;
}
