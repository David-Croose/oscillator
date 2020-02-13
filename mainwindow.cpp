#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(848, 532);

    // the x-axis
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setTickCount(11);
    axisX->setLabelFormat("%.2f");
    ui->chartview->chart()->addAxis(axisX, Qt::AlignBottom);

    // the y-axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 10);
    axisY->setTickCount(11);
    axisY->setLabelFormat("%.2f");
    ui->chartview->chart()->addAxis(axisY, Qt::AlignLeft);

    // append points
    series = new QLineSeries();
    series->append(0, 6);
    series->append(1, 9);
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
    ui->chartview->chart()->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // the points and chartview
    series->setPointLabelsVisible(true);
    series->setPointsVisible(true);
    ui->chartview->chart()->legend()->hide();

    // the checkbox
    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    static int prev;
    int delta = position - prev;
    prev = position;
    delta *= 8;
    this->ui->chartview->chart()->scroll(delta, 0);
}


void MainWindow::on_checkBox_released()
{
    series->setPointsVisible(ui->checkBox->isChecked());
}

void MainWindow::on_checkBox_2_released()
{
    series->setPointLabelsVisible(ui->checkBox_2->isChecked());
}
