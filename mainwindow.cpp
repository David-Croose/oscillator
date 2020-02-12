#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(848, 532);

    series = new QLineSeries();
    series->append(0, 6);  series->append(1, 9);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    series->append(11, 9);


    series->append(12, 19);
    series->append(13, 2);
    series->append(14, 6);
    series->append(15, 9);
    series->append(16, 12);
    series->append(17, 17);
    series->append(18, 5);


    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    /// chart->createDefaultAxes();
    ui->chartview->setChart(chart);


    QValueAxis *x = new QValueAxis();
    x->setTickCount(20);
    chart->addAxis(x, Qt::AlignBottom);


    chart->axisX()->setRange(0, 10);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int prev;

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    // qDebug("%d\n", position);

    int delta = position - prev;

    prev = position;

    this->chart->scroll(delta * 8, 0);
}

