#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtDebug>

#define XAXIS_INIT_SIZE     (10)
#define XAXIS_MAX_SIZE      (10000)
#define YAXIS_INIT_SIZE     (10)
#define YAXIS_MAX_SIZE      (10000)

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

    // the series
    for (quint64 i = 0; i < sizeof(this->series) / sizeof(this->series[0]); i++) {
        ui->chartview->chart()->addSeries(&series[i]);
        series[i].attachAxis(axisX);
        series[i].attachAxis(axisY);
        series[i].setPointLabelsVisible(false);
        series[i].setPointsVisible(false);
    }

    // the chartview
    ui->chartview->chart()->legend()->hide();

    // the checkbox for chartview
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);

    // the spinbox(x,y-axis range)
    ui->spinBox->setRange(0, XAXIS_MAX_SIZE);
    ui->spinBox->setValue(XAXIS_INIT_SIZE);
    ui->spinBox_2->setRange(0, YAXIS_MAX_SIZE);
    ui->spinBox_2->setValue(YAXIS_INIT_SIZE);

    // thread for chartview slider
    thread = new myThread();

    // chartview slider
    ui->horizontalSlider->setMaximum(100);      // [0,100]
    ui->horizontalSlider->setValue(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openfile(int drag, QString dragfileName)
{
    QByteArray dat;
    QString fileName;

    if (drag == 0) {
        // select file
        fileName = QFileDialog::getOpenFileName(this, tr("选择文件"),
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
            tr("binary file(*.bin *.hex *.dat);;all files(*.*)"));
    } else {
        fileName = dragfileName;
    }

    // open file
    QFile *f = new QFile(fileName);
    if (!f->open(QIODevice::ReadOnly)) {
        return;
    }
    this->setWindowTitle(fileName);

    // read points from file
    dat = f->readAll();
    f->close();
    quint64 datlen = dat.length();
    if (datlen == 0) {
        QMessageBox::about(this, "错误", "文件为空!");
        return;
    }
    if (datlen > CONFIG_MAXPOINTS) {
        QString tmp;
        tmp.sprintf("数据点超过%d个!", CONFIG_MAXPOINTS);
        QMessageBox::about(this, "错误", tmp);
        return;
    }
    ui->horizontalSlider->setValue(50);

    // clear all series before adding points to chartview
    for (quint64 i = 0; i < sizeof(this->series) / sizeof(this->series[0]); i++) {
        this->series[i].clear();
    }

    // add points to chartview
    int i, j, group, spare;
    for (i = 0, group = datlen / CONFIG_EACHSERIESCARRY; i < group; i++) {
        for (j = 0; j < CONFIG_EACHSERIESCARRY; j++) {
            series[i].append(i * CONFIG_EACHSERIESCARRY + j, dat[i * CONFIG_EACHSERIESCARRY + j]);
        }
    }
    for (spare = datlen % CONFIG_EACHSERIESCARRY, j = 0; j < spare; j++) {
        series[i].append(i * CONFIG_EACHSERIESCARRY + j, dat[i * CONFIG_EACHSERIESCARRY + j]);
    }
}

void MainWindow::on_checkBox_released()
{
    series->setPointsVisible(ui->checkBox->isChecked());
}

void MainWindow::on_checkBox_2_released()
{
    series->setPointLabelsVisible(ui->checkBox_2->isChecked());
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Horizontal, NULL)[0]->setRange(0, arg1);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Vertical, NULL)[0]->setRange(0, arg1);
}

void MainWindow::dragEnterEvent(QDragEnterEvent*event){
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent*event){
    const QMimeData *qm = event->mimeData();
    openfile(1, qm->urls()[0].toLocalFile());
}

void MainWindow::on_actionOpenfile_triggered()
{
    openfile(0, 0);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "关于", "这是一个波形查看软件");
}

void MainWindow::chartview_move()
{
    ui->chartview->chart()->scroll(this->dx, 0);
}

void myThread::run()
{
    runflag = 1;
    while (runflag) {
        emit call();
        QThread::usleep(10 * 1000);
    }
}

void myThread::kill()
{
    runflag = 0;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    ui->horizontalSlider->setValue(50);
    thread->kill();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    this->dx = position - 50;
    this->thread->disconnect(SIGNAL(call()));
    connect(this->thread, SIGNAL(call()), this, SLOT(chartview_move()));
    thread->start();
}
