#ifndef FULLSCREEN_H
#define FULLSCREEN_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
class fullscreen;
}

class fullscreen : public QWidget
{
    Q_OBJECT

public:
    explicit fullscreen(QWidget *parent = 0);
    ~fullscreen();

private:
    Ui::fullscreen *ui;

public:
    QPixmap background;
    QImage screen,screen_dark;

private:
    QPoint point;
    QPainter *painter;
    QPoint start,end;
    QRect currentRect;
protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);
    void currentRectInit();
    void keyPressEvent(QKeyEvent *k);
    void screencutSave();
    QImage clipImage(const QImage& srcImage, QRect rect);

public slots:
    void esc_click();
    void screen_save();

signals:
    void on_show();
    void esc_click2();

public:
    kitTool *kt;
    void WindowsInit();
};

#endif // FULLSCREEN_H
