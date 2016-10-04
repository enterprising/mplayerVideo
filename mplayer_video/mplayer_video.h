#ifndef MPLAYER_VIDEO_H
#define MPLAYER_VIDEO_H

#include <QMainWindow>
#include <QProcess>
#include <QListWidgetItem>
#include <fullscreen.h>
#include <QDir>

namespace Ui {
class mplayer_video;
}

class mplayer_video : public QMainWindow
{
    Q_OBJECT

private:
    Ui::mplayer_video *ui;
    QProcess *process;  //定义一个进程，使用进程进行操作，这样不会冻结界面
    QString arg;         //定义一个arg，用来存放要使用的播放器路径
    QString currentPercent;
    QString currentStr;
    QString totalTime;
public:
    QStringList::Iterator ptr;  // ptr是指向QStringList每个数据项的指针
    QString path_name[100];   //用来存放每个文件的路径
    int number,number2;
    QTimer *timer,*timer2;
    QString fileName;
    QDir *qdir;
    QFileInfoList list;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quit,*max;
    fullscreen f;
    QTimer *time;
public:
    explicit mplayer_video(QWidget *parent = 0);
    ~mplayer_video();



private slots:
    void on_open_clicked();
    void on_zanting_clicked();
    void on_back_clicked();
    void on_prio_clicked();
    void on_next_clicked();
    void on_quick_clicked();
    void dataRecieve();
    void play(QString fileName);
    void on_horizontalSlider_sliderMoved(int value);
    void on_stop_clicked();
    void on_voice_clicked();
    void on_voice_slider_sliderMoved(int position);
    void on_del_clicked();
    void on_list_itemDoubleClicked(QListWidgetItem *item);
    void on_jiansu_clicked();
    void on_jiasu_clicked();
    void on_jietu_clicked();
    void on_lupin_clicked();
    void on_lupinstart_clicked();

    void on_stop_bofang_clicked();

private:
    void photoStart();

public slots:
    void timerUpdate();
    void timerBofang();

};

#endif // MPLAYER_VIDEO_H
