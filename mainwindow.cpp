#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtDebug>

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

    // the action of "文件：打开"
    QAction *openFileAction = ui->menuFile->addAction("打开");
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(on_openFile()));

    // the action of "关于"
    // connect(ui->menuAbout, SIGNAL(triggered()), this, SLOT(on_openFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFile()
{
    // select file
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"),
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        tr("binary file(*.bin *.hex *.dat);;all files(*.*)"));

    // open file
    QFile *f = new QFile(fileName);
    if (!f->open(QIODevice::ReadOnly)) {
        QMessageBox::about(this, "错误", "文件打开失败");
        return;
    }

    // read points from file
    QByteArray dat = f->readAll();
    int totlen = dat.length();
    if (totlen == 0) {
        QMessageBox::about(this, "错误", "文件为空");
    }
    f->close();
    series->clear();

    // add points to chartview
    /// for (int i = 0; i < totlen; i++) {
    ///     series->append(i, dat[i]);
    /// }

    for (int i = 0; i < totlen; i += 8) {
        series->append(i, dat[i]);
    }
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
