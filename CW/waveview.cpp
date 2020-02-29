#include "waveview.h"
#include "ui_waveview.h"

#include <QPainter>
#include <QDebug>

extern quint32 *data1;
extern quint32 data1Len;
extern long long xpos;
extern long long ypos;
extern long long ydelta;

extern double pixelsPerPointsX;
extern double pixelsPerPointsY;

extern int dispDataPoints;

extern int dispLineAA;

Waveview::Waveview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Waveview)
{
    ui->setupUi(this);

    // window display style
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: rgb(255, 255, 255)");


    // the timer for updating wave-view
    timerUpdateWaveView = new QTimer;
    timerUpdateWaveView->setSingleShot(false);
    timerUpdateWaveView->start(100);
    connect(timerUpdateWaveView, SIGNAL(timeout()), this, SLOT(TimerTimeOut()));
}

Waveview::~Waveview()
{
    delete ui;
}

void Waveview::TimerTimeOut()
{
    QWidget::update();
}

void Waveview::drawChannel(const quint32 *p, quint32 plen)
{
    static int lastX;
    static int lastY;
    int currX;
    int currY;
    int totalPointsInScreen = CONFIG_WAVEVIEWWIDTH / pixelsPerPointsX;

    if (!p || !plen) {
        return;
    }

    ypos += ydelta;

    QPainter painter(this);
    if (dispLineAA) {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }
    painter.setBrush(Qt::SolidPattern);
    int radius = 3;
    QPoint center;

    // draw data points
    if (dispDataPoints) {
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::black);
        painter.setPen(mypen);
        for (int i = 0; i < totalPointsInScreen && i + xpos <= plen - 1; i++) {
            currX = i * pixelsPerPointsX;
            currY = CONFIG_WAVEVIEWHEIGHT - p[i + xpos] * pixelsPerPointsY + ypos;

            // painter.drawPoint(currX, currY);
            center.setX(currX);
            center.setY(currY);
            painter.drawEllipse(center, radius, radius);
        }
    }

    // draw lines
    QPen mypen2;
    mypen2.setWidth(1);
    mypen2.setColor(Qt::black);
    painter.setPen(mypen2);
    for (int i = 0; i < totalPointsInScreen && i + xpos <= plen - 1; i++) {
        currX = i * pixelsPerPointsX;
        currY = CONFIG_WAVEVIEWHEIGHT - p[i + xpos] * pixelsPerPointsY + ypos;

        if (i > 0) {
            painter.drawLine(lastX, lastY, currX, currY);
        }
        lastX = currX;
        lastY = currY;
    }
}

void Waveview::paintEvent(QPaintEvent *)
{

    // draw the scale in axis



    // draw the grid



    // display wave
    drawChannel(data1, data1Len);
}
