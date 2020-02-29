#ifndef CW_H
#define CW_H

#include <QWidget>
#include "waveview.h"

namespace Ui {
class CW;
}

class CW : public QWidget
{
    Q_OBJECT

public:
    explicit CW(QWidget *parent = nullptr);
    ~CW();

    //====================================================================
    void drawChannel(QPainter &painter, const quint32 *p, quint32 plen);
    void readData(const quint32 *p, quint32 plen);
    //--------------------------------------------------------------------

private:
    Ui::CW *ui;
    class Waveview *waveView;

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_horizontalScrollBar_valueChanged(int value);
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_sliderReleased();
    void on_spinBox_XaxisLen_valueChanged(int arg1);
    void on_spinBox_YaxisLen_valueChanged(int arg1);
    void on_pushButton_Reset_released();
    void on_checkBox_DispDataPoint_clicked();
    void on_checkBox_AA_clicked();
};

#endif // CW_H
