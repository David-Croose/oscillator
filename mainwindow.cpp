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
    this->setFixedSize(878, 559);

    // the x-axis
    axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setTickCount(11);
    axisX->setLabelFormat("%.2f");
    axisX->setGridLineVisible(false);
    ui->chartview->chart()->addAxis(axisX, Qt::AlignBottom);

    // the y-axis
    axisY = new QValueAxis;
    axisY->setRange(0, 10);
    axisY->setTickCount(11);
    axisY->setLabelFormat("%.2f");
    axisY->setGridLineVisible(false);
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
    ui->checkBoxDispBigPoints->setChecked(false);
    ui->checkBoxDispCoor->setChecked(false);
    ui->checkBoxDispGrid->setChecked(false);

    // the spinbox(x,y-axis range)
    ui->spinBoxXstart->setRange(0, 20000);
    ui->spinBoxXstart->setValue(0);
    ui->spinBoxXdiv->setRange(0, 20000);
    ui->spinBoxXdiv->setValue(11);
    ui->spinBoxXend->setRange(0, 20000);
    ui->spinBoxXend->setValue(100);
    ui->spinBoxYstart->setRange(-20000, 20000);
    ui->spinBoxYstart->setValue(-10);
    ui->spinBoxYdiv->setRange(-20000, 20000);
    ui->spinBoxYdiv->setValue(11);
    ui->spinBoxYend->setRange(-20000, 20000);
    ui->spinBoxYend->setValue(10);

    // thread for chartview slider
    thread = new myThread();

    // chartview slider
    ui->sliderX->setMaximum(100);      // [0,100]
    ui->sliderX->setValue(50);
    ui->sliderY->setMaximum(100);      // [0,100]
    ui->sliderY->setValue(50);
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
    ui->sliderX->setValue(50);
    ui->sliderY->setValue(50);

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
    ui->chartview->chart()->scroll(this->dx, this->dy);
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

void MainWindow::on_checkBoxDispBigPoints_released()
{
    series->setPointsVisible(ui->checkBoxDispBigPoints->isChecked());
}

void MainWindow::on_checkBoxDispCoor_released()
{
    series->setPointLabelsVisible(ui->checkBoxDispCoor->isChecked());
}

void MainWindow::on_checkBoxDispGrid_released()
{
    ui->chartview->chart()->axes(Qt::Horizontal, NULL)[0]->setGridLineVisible(ui->checkBoxDispGrid->isChecked());
    ui->chartview->chart()->axes(Qt::Vertical, NULL)[0]->setGridLineVisible(ui->checkBoxDispGrid->isChecked());
}

void MainWindow::on_spinBoxXstart_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Horizontal, NULL)[0]->setMin(arg1);
}

void MainWindow::on_spinBoxXdiv_valueChanged(int arg1)
{
    axisX->setTickCount(arg1);
}

void MainWindow::on_spinBoxXend_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Horizontal, NULL)[0]->setMax(arg1);
}

void MainWindow::on_spinBoxYstart_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Vertical, NULL)[0]->setMin(arg1);
}

void MainWindow::on_spinBoxYdiv_valueChanged(int arg1)
{
    axisY->setTickCount(arg1);
}

void MainWindow::on_spinBoxYend_valueChanged(int arg1)
{
    ui->chartview->chart()->axes(Qt::Vertical, NULL)[0]->setMax(arg1);
}

void MainWindow::on_sliderX_sliderMoved(int position)
{
    this->dx = position - 50;
    this->dy = 0;
    this->thread->disconnect(SIGNAL(call()));
    connect(this->thread, SIGNAL(call()), this, SLOT(chartview_move()));
    thread->start();
}

void MainWindow::on_sliderX_sliderReleased()
{
    ui->sliderX->setValue(50);
    thread->kill();
}

void MainWindow::on_sliderY_sliderMoved(int position)
{
    this->dx = 0;
    this->dy = position - 50;
    this->thread->disconnect(SIGNAL(call()));
    connect(this->thread, SIGNAL(call()), this, SLOT(chartview_move()));
    thread->start();
}

void MainWindow::on_sliderY_sliderReleased()
{
    ui->sliderY->setValue(50);
    thread->kill();
}
