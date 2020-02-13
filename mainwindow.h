#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QValueAxis>

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


private slots:
    void on_openFile();

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_checkBox_released();

    void on_checkBox_2_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
