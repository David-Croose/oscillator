#include "cw.h"
#include "ui_cw.h"
#include <math.h>

#include <QPainter>
#include <QDebug>

quint32 *data1;
quint32 data1Len;
long long xpos;
long long ypos;
long long ydelta;

double pixelsPerPointsX;
double pixelsPerPointsY;

int dispDataPoints;

int dispLineAA;

CW::CW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CW)
{
    ui->setupUi(this);

    // setup wave-view
    waveView = new Waveview();

    // the axis configuration
    ui->spinBox_XaxisLen->setMinimum(1);
    ui->spinBox_XaxisLen->setMaximum(100000);
    ui->spinBox_XaxisLen->setValue(100);
    ui->spinBox_YaxisLen->setMinimum(1);
    ui->spinBox_YaxisLen->setMaximum(100000);
    ui->spinBox_YaxisLen->setValue(480);
    ui->spinBox_Xdiv->setValue(10);
    ui->spinBox_Ydiv->setValue(10);

    // v-slider
    ui->verticalSlider->setMaximum(101);
    ui->verticalSlider->setValue(50);

    // others
    pixelsPerPointsX = CONFIG_WAVEVIEWWIDTH / ui->spinBox_XaxisLen->value();
    pixelsPerPointsY = CONFIG_WAVEVIEWHEIGHT / ui->spinBox_YaxisLen->value();
}

CW::~CW()
{
    delete ui;
}


void CW::readData(const quint32 *p, quint32 plen)
{
    data1 = (quint32 *)p;
    data1Len = plen;

    // h-scrollbar
    ui->horizontalScrollBar->setMaximum(data1Len - ui->spinBox_XaxisLen->value());
    ui->horizontalScrollBar->setMinimum(0);
    ui->horizontalScrollBar->setPageStep(ui->spinBox_XaxisLen->value());
}



void CW::paintEvent(QPaintEvent *)
{
    // draw the scale in axis



    // draw the grid



    // display wave

}



void CW::on_horizontalScrollBar_valueChanged(int value)
{
    if (value < 0) {
        return;
    }
    xpos = value;

    qDebug() << xpos;
}



void CW::on_verticalSlider_valueChanged(int value)
{
    if (value < 0) {
        return;
    }
    ydelta = value - 50;
}

void CW::on_verticalSlider_sliderReleased()
{
    ui->verticalSlider->setValue(50);
}

void CW::on_spinBox_XaxisLen_valueChanged(int arg1)
{
    pixelsPerPointsX = CONFIG_WAVEVIEWWIDTH / (double)arg1;

    // h-scrollbar
    ui->horizontalScrollBar->setMaximum(data1Len - ui->spinBox_XaxisLen->value());
    ui->horizontalScrollBar->setMinimum(0);
    ui->horizontalScrollBar->setPageStep(ui->spinBox_XaxisLen->value());
}

void CW::on_spinBox_YaxisLen_valueChanged(int arg1)
{
    pixelsPerPointsY = CONFIG_WAVEVIEWHEIGHT / (double)arg1;
}

void CW::on_pushButton_Reset_released()
{
    xpos = 0;
    ypos = 0;
}

void CW::on_checkBox_DispDataPoint_clicked()
{
    dispDataPoints = ui->checkBox_DispDataPoint->isChecked();
}

void CW::on_checkBox_AA_clicked()
{
    dispLineAA = ui->checkBox_AA->isChecked();
}
