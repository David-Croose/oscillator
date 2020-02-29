#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openfile(QString dragfileName)
{
    QByteArray dat;
    QFile *f = new QFile(dragfileName);
    if (!f->open(QIODevice::ReadOnly)) {
        return;
    }

    dat = f->readAll();
    f->close();

    quint32 dataLen = dat.length();
    quint32 *data = new quint32[dataLen];
    for (quint32 i = 0; i < dataLen; i++) {
        data[i] = (quint8)dat.at(i);
    }

    //////////////////////////////////////////////////////////////////////////
//    data[0] = 10;
//    data[1] = 460;
//    data[2] = 10;
//    data[3] = 56;     // 0x80
//    data[4] = 0;
//    data[5] = 10;
//    data[6] = 200;
//    data[7] = 10;
//    data[8] = 300;
//    data[9] = 10;
//    data[10] = 400;
//    dataLen = 11;
    //////////////////////////////////////////////////////////////////////////

    ui->waveView->readData(data, dataLen);
}

void MainWindow::dragEnterEvent(QDragEnterEvent*event){
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent*event){
    const QMimeData *qm = event->mimeData();
    openfile(qm->urls()[0].toLocalFile());
}

