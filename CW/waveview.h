#ifndef WAVEVIEW_H
#define WAVEVIEW_H

#include <QWidget>
#include <QTimer>

#define CONFIG_TOTALPOINTS      (1000)

#define CONFIG_WAVEVIEWWIDTH    (1000)
#define CONFIG_WAVEVIEWHEIGHT   (480)

namespace Ui {
class Waveview;
}

class Waveview : public QWidget
{
    Q_OBJECT

public:
    explicit Waveview(QWidget *parent = nullptr);
    ~Waveview();
    //========================================================================
    void drawChannel(const quint32 *p, quint32 plen);
    QTimer *timerUpdateWaveView;
    //------------------------------------------------------------------------

private slots:
    void TimerTimeOut();

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::Waveview *ui;
};

#endif // WAVEVIEW_H
