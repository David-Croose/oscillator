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

#define CONFIG_MAXPOINTS        (40000)
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

    void openfile(int drag, QString dragfileName);
    QLineSeries series[TOTAL_SERIES];
    myThread *thread;
    int dx;
    int dy;
    QValueAxis *axisX;
    QValueAxis *axisY;

private slots:
    void chartview_move();

    void on_actionOpenfile_triggered();

    void on_actionAbout_triggered();

    void on_checkBoxDispBigPoints_released();

    void on_checkBoxDispCoor_released();

    void on_checkBoxDispGrid_released();

    void on_spinBoxXstart_valueChanged(int arg1);

    void on_spinBoxXdiv_valueChanged(int arg1);

    void on_spinBoxXend_valueChanged(int arg1);

    void on_spinBoxYstart_valueChanged(int arg1);

    void on_spinBoxYdiv_valueChanged(int arg1);

    void on_spinBoxYend_valueChanged(int arg1);

    void on_sliderX_sliderMoved(int position);

    void on_sliderX_sliderReleased();

    void on_sliderY_sliderMoved(int position);

    void on_sliderY_sliderReleased();

private:
    Ui::MainWindow *ui;

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);
};

#endif // MAINWINDOW_H
