#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QValueAxis>

#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>
#include <QThread>

#define CONFIG_MAXPOINTS        (30000)
#define CONFIG_EACHSERIESCARRY  (500)
#define TOTAL_SERIES            (CONFIG_MAXPOINTS / CONFIG_EACHSERIESCARRY)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class myThread : public QThread
{
    Q_OBJECT

public:
    void kill();

signals:
    void call();

private:
    void run() override;
    int runflag;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLineSeries series[TOTAL_SERIES];
    void openfile(int drag, QString dragfileName);
    myThread *thread;
    int dx;

private slots:
    void chartview_move();

    void on_checkBox_released();

    void on_checkBox_2_released();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_actionOpenfile_triggered();

    void on_actionAbout_triggered();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);
};

#endif // MAINWINDOW_H
