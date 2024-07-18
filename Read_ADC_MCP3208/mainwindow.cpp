/*

           * Biochemistry
          ------------------

            Light & Diode
           -----------------

        340nm - readwave-0  (6-0)
        405nm - readwave-1  (6-1)
        505nm - readwave-2  (6-2)
        545nm - readwave-3  (6-3)
        570nm - readwave-4  (6-4)
        630nm - readwave-5  (6-5)
        700nm - readwave-6  (6-6)

                  * Turbidity
                 ---------------

      Light                        Diode
   ------------                 ------------

340nm - readwavenm-0            ,readwave-2
405nm - readwavenm-1            ,readwave-1
505nm - readwavenm-2            ,readwave-4
545nm - readwavenm-3            ,readwave-3
570nm - readwavenm-4            ,readwave-6
630nm - readwavenm-5            ,readwave-5
700nm 180-deg - readwavenm-6    ,readwave-7
700nm 135-deg - readwavenm-6    ,readwave-0

*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>
#define LED 26
#define ADC_BASE 100
#define SPI_CHAN 0
#define LED_BASE 200
#define dataPin 21
#define clockPin 22
#define latchPin 23
int filtwave[600],read_intensity=0,read_wave=0,read_wavenm=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    wiringPiSetup () ;
    mcp3004Setup (ADC_BASE, SPI_CHAN) ;
    sr595Setup (LED_BASE, 16, dataPin, clockPin, latchPin) ;
    pinMode (LED, PWM_OUTPUT);
    pwmWrite (LED, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::reading()
{
    int blank[300];
    filtwave[0]=0;
    pwmWrite (LED, read_intensity);
    digitalWrite (LED_BASE + read_wavenm,HIGH) ;
    QThread::msleep(200);
    for(int i=0;i<300;i++)
    {
        blank[i]=readadc(7-read_wave);
        QThread::msleep(1);
        if(i>=250)
            filtwave[0]+=blank[i];
    }
    QThread::msleep(10);
    digitalWrite (LED_BASE + read_wavenm,LOW) ;
    filtwave[0]=filtwave[0]/50;
    if(filtwave[0]==0)
    {
        filtwave[0]=1;
    }
    pwmWrite (LED, 0);
    return filtwave[0];
}

int MainWindow::readadc( int pin)
{
    unsigned char buff[] = {static_cast<char>(0x6 | ((pin & 0x7) >> 2)),
                            static_cast<char>((pin & 0x7) << 6),
                            static_cast<char>(0)};

    wiringPiSPIDataRW(SPI_CHAN, buff, 3);
    return ((buff[1] & 0xf) << 8) | buff[2];
}

void MainWindow::on_pushButton_clicked()
{
    //340 nm
    read_wave=2;        //5
    read_wavenm=0;
    read_intensity=ui->lineEdit->text().toInt();
    reading();
    ui->label_9->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_2_clicked()
{
    //405 nm
    read_wave=1;        //6
    read_wavenm=1;
    read_intensity=ui->lineEdit_2->text().toInt();
    reading();
    ui->label_10->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_3_clicked()
{
    // 505 nm
    read_wave=4;        //3
    read_wavenm=2;
    read_intensity=ui->lineEdit_3->text().toInt();
    reading();
    ui->label_11->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_4_clicked()
{
    //545 nm
    read_wave=3;        //4
    read_wavenm=3;
    read_intensity=ui->lineEdit_4->text().toInt();
    reading();
    ui->label_12->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_5_clicked()
{
    //570 nm
    read_wave=6;        //1
    read_wavenm=4;
    read_intensity=ui->lineEdit_5->text().toInt();
    reading();
    ui->label_13->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_6_clicked()
{
    //630 nm
    read_wave=5;        //2
    read_wavenm=5;
    read_intensity=ui->lineEdit_6->text().toInt();
    reading();
    ui->label_14->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_7_clicked()
{
    //700 nm - 180 dg
    read_wave=7;        //0
    read_wavenm=6;
    read_intensity=ui->lineEdit_7->text().toInt();
    reading();
    ui->label_15->setNum(filtwave[0]);
}

void MainWindow::on_pushButton_8_clicked()
{
    //700 nm - 135 dg
    read_wave=0;
    read_wavenm=6;
    read_intensity=ui->lineEdit_8->text().toInt();
    reading();
    ui->label_16->setNum(filtwave[0]);
}
