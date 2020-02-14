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

    // the spinbox(x,y-axis range)
    ui->spinBox->setRange(0, XAXIS_MAX_SIZE);
    ui->spinBox->setValue(XAXIS_INIT_SIZE);
    ui->spinBox_2->setRange(0, YAXIS_MAX_SIZE);
    ui->spinBox_2->setValue(YAXIS_INIT_SIZE);

    // thread
    thread = new myThread();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray dat;

void MainWindow::openfile(int drag, QString dragfileName)
{
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
    if ((this->datlen = dat.length()) == 0) {
        QMessageBox::about(this, "错误", "文件为空");
    }
    f->close();
    series->clear();

    // add points to chartview
    int i, j;
    for (j = 0, i = 0; i < this->datlen; i += 1, j++) {
        series->append(j, dat[i]);
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

void MainWindow::chartview_moveright()
{
    ui->chartview->chart()->scroll(10, 0);
}

void MainWindow::chartview_moveleft()
{
    ui->chartview->chart()->scroll(-10, 0);
}

void myThread::run()
{
    int delay = 100 * 1000;
    int i = 0;

    runflag = 1;
    while (runflag) {
        emit call();

        if (i < 15 && ++i >= 15) {     // when the loop runs up to some lap, speed it up
            delay = 5 * 1000;
        }
        QThread::usleep(delay);
    }
}

void myThread::kill()
{
    runflag = 0;
}

void MainWindow::on_pushButtonLeft_pressed()
{
    this->thread->disconnect(SIGNAL(call()));
    connect(this->thread, SIGNAL(call()), this, SLOT(chartview_moveleft()));
    thread->start();
}

void MainWindow::on_pushButtonRight_pressed()
{
    this->thread->disconnect(SIGNAL(call()));
    connect(this->thread, SIGNAL(call()), this, SLOT(chartview_moveright()));
    thread->start();
}

void MainWindow::on_pushButtonLeft_released()
{
    thread->kill();
}

void MainWindow::on_pushButtonRight_released()
{
    thread->kill();
}
