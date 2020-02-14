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

    // the action of "关于"
    // connect(ui->menuAbout, SIGNAL(triggered()), this, SLOT(on_openFile()));

    // the spinbox(x,y-axis range)
    ui->spinBox->setRange(0, XAXIS_MAX_SIZE);
    ui->spinBox->setValue(XAXIS_INIT_SIZE);
    ui->spinBox_2->setRange(0, YAXIS_MAX_SIZE);
    ui->spinBox_2->setValue(YAXIS_INIT_SIZE);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
        ///QMessageBox::about(this, "错误", "文件打开失败");
        return;
    }
    this->setWindowTitle(fileName);
    //ui->horizontalScrollBar->setMaximum(this->datlen);
    //ui->horizontalScrollBar->setValue(0);

    // read points from file
    QByteArray dat = f->readAll();
    if ((this->datlen = dat.length()) == 0) {
        QMessageBox::about(this, "错误", "文件为空");
    }
    f->close();
    series->clear();

    // add points to chartview
    qDebug() << "adding... " << this->datlen/8;
    int i, j;
    for (j = 0, i = 0; i < this->datlen; i += 1, j++) {
        series->append(j, dat[i]);
    }
}

void MainWindow::on_horizontalScrollBar_valueChanged(int value)
{
    static int prev;
    double delta = (double)value - prev;

    prev = value;
    delta = delta / 100.0 * this->datlen;
    ui->chartview->chart()->scroll(delta, 0);
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
