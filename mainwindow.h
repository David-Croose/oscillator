#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QValueAxis>

#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLineSeries *series;
    int datlen;
    void openfile(int drag, QString dragfileName);

private slots:
    void on_checkBox_released();

    void on_checkBox_2_released();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_horizontalScrollBar_valueChanged(int value);

    void on_actionOpenfile_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);
};
#endif // MAINWINDOW_H
