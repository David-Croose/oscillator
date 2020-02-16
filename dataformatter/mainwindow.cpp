#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

int currindex;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(290, 230);

    ui->spinBox->setValue(24);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent*event){
    event->acceptProposedAction();
}

int writefile(QString &filename, const char *p, quint64 len)
{
    QFile *f = new QFile(filename);
    if (!f->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return -1;
    }
    f->write(p, len);
    f->close();
    return 0;
}

void MainWindow::dropEvent(QDropEvent*event){
    const QMimeData *qm = event->mimeData();
    QString inputFileName = qm->urls()[0].toLocalFile();

    // read file
    QFile *f1 = new QFile(inputFileName);
    if (!f1->open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray dat = f1->readAll();
    quint64 filelen = dat.length();
    f1->close();
    QByteArray ba;
    for (quint64 i = currindex; i < filelen; i += 8) {
        ba.append(dat.at(i));
    }

    // write file
    inputFileName.remove(QRegExp("\\..*$"));
    int unit  = ui->spinBox->value() * 1024;
    int units = filelen / unit;
    int spare = filelen % unit;
    QString filename;
    char *p = ba.data();
    for (int i = 0; i < units; i++) {
        filename = inputFileName;
        filename.append("__formatted_");
        filename.append(QString::number(i, 10));
        filename.append(".dat");
        writefile(filename, &p[i * unit], unit);
    }
    if (spare > 0) {
        filename = inputFileName;
        filename.append("__formatted_");
        filename.append(QString::number(units, 10));
        filename.append(".dat");
        writefile(filename, &p[units * unit], spare);
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    currindex = index;
}
