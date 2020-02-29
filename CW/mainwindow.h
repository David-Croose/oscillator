#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openfile(QString dragfileName);

private:
    Ui::MainWindow *ui;

protected:
     void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
     void dropEvent(QDropEvent*event);
};
#endif // MAINWINDOW_H
