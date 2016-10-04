#include "mplayer_video.h"
#include "ui_mplayer_video.h"
#include "QFileDialog"
#include <QMessageBox>
#include <QInputDialog>
#include <QApplication>
#include <QDesktopWidget>
#include "QPalette"
#include "QColor"
#include "iostream"
#include <QTimer>
static double speed;
using namespace std;

mplayer_video::mplayer_video(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mplayer_video)
{

    process = new QProcess();
    process->setProcessChannelMode(QProcess::MergedChannels);
    arg="C:/mplayer/MPlayer for Windows/mplayer.exe";
    // ptr=NULL;  // ptr是指向QStringList每个数据项的指针，
    speed=1;
    ui->setupUi(this);
    setWindowTitle("视频播放器");
    QPixmap pixmap;
    pixmap.load("F:/mplayer_video/voice.jpg");
    ui->voice->setFixedSize(pixmap.width(),pixmap.height());
    ui->voice->setIcon(pixmap);
    ui->voice->setIconSize(QSize(pixmap.width(),pixmap.height()));
    ui->voice->setToolTip("音量");
    ui->voice->show();
    ui->voice_slider->setSliderPosition(20);
    ui->voice_slider->setValue(20);

    ui->centralWidget -> setAutoFillBackground(true);

    QPalette p = ui->centralWidget ->palette();

    p.setColor(QPalette::Window,QColor(136, 136, 136));

    //p.setBrush(QPalette::Window,brush);
    ui->centralWidget -> setPalette(p);

    number=0;
    timer = new QTimer();
    timer2 = new QTimer();
    connect(timer2,SIGNAL(timeout()),this,SLOT(timerBofang()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
}

mplayer_video::~mplayer_video()
{
    delete ui;
}

void mplayer_video::photoStart()
{
    QDir *d = new QDir();
    QString dirName("D:\\video");
    if(!d->exists(dirName))
    {
        d->mkdir(dirName);
    }
}

void mplayer_video::timerUpdate()
{
    QPixmap p;
    QString dirNamee("D:\\video\\");
    dirNamee=dirNamee+QString::number(number)+".jpg";
    p = QPixmap::grabWindow(QApplication::desktop()->winId());
    p.save(dirNamee);
    number+=1;
}

void mplayer_video::timerBofang()
{
    ui->lupin_lable->setVisible(true);
    process->write("pause\n");
    //ui->widget->setVisible(false);
    QFileInfo info = list.at(number2);
    QPixmap p(info.filePath());
    QPixmap q = p.scaled(ui->lupin_lable->width(),ui->lupin_lable->height());
    ui->lupin_lable->setPixmap(q);
    //ui->label->setText(info.filePath());
    ui->lupin_lable->update();
    if(number2>list.length()-4)timer2->stop();
    number2++;
}

void mplayer_video::on_open_clicked()
{
      QStringList filenames = QFileDialog::getOpenFileNames(this,tr("选择文件"),"/",tr("视频文件(*mp3 *mp4 *wma *3gp *wav *avi *rmvb *mkv *ts *wmv)"));    //  *号和前面的要隔开至少一个空格，不能连起来。
      if(filenames.count()!=0)
      ui->list->addItems(filenames);
}

void mplayer_video::on_zanting_clicked()
{
    ui->lupin_lable->setVisible(false);
    process->write("pause\n");
    //ui->list->currentRow()!=-1   判断是否有选中某条数据
    //play(ui->list->item(ui->list->currentRow())->text()); 播放选中的某条数据
    if(ui->zanting->text()=="播放")
    {
         if(ui->list->count()==0)
          QMessageBox::warning(this,"提示","播放列表为空",QMessageBox::Yes);
         else{
         connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
         process->write("get_time_length\n");
         process->write("get_time_pos\n");
         process->write("get_percent_pos\n");
         ui->zanting->setText("暂停");
         }
    }
    else
     {
       disconnect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
       ui->zanting->setText("播放");
     }

}

void mplayer_video::on_prio_clicked()
{
    if(ui->list->currentRow()==0)
      ui->list->setCurrentRow(ui->list->count()-1);
    else{
        speed=1;
      ui->list->setCurrentRow(ui->list->currentRow()-1);
      play(ui->list->currentItem()->text());
      ui->zanting->setText("暂停");}
}

void mplayer_video::on_next_clicked()
{
    if(ui->list->currentRow()==ui->list->count()-1)
      {

        ui->list->setCurrentRow(0);
      }
    else{
        speed=1;
      ui->list->setCurrentRow(ui->list->currentRow()+1);
      play(ui->list->currentItem()->text());
      ui->zanting->setText("暂停");}
}

void mplayer_video::on_back_clicked()
{
     process->write("seek -10\n");
}

void mplayer_video::on_quick_clicked()
{
     process->write("seek +10\n");
}

void mplayer_video::on_horizontalSlider_sliderMoved(int value)
{
     process->write("pause\n");
     process->write(QString("seek "+QString::number(value)+" 2\n").toUtf8());
     //process->start();
}

void mplayer_video::dataRecieve()
{
     process->write("get_time_length\n");
     process->write("get_time_pos\n");
     process->write("get_percent_pos\n");
    while(process->canReadLine())
    {
      QByteArray b=process->readLine();
      if(b.startsWith("ANS_TIME_POSITION"))
      {
       b.replace(QByteArray("\n"),QByteArray(""));
       QString s(b);
       currentStr=s.mid(18);
       ui->horizontalSlider->setValue(s.mid(18).toFloat());    //更新进度条
      }
      else if((b.startsWith("ANS_LENGTH")))
      {
         b.replace(QByteArray("\n"),QByteArray(""));
         QString s(b);
         totalTime=s.mid(11);
         ui->label_time->setText(currentStr+"秒/"+totalTime+"秒  "); //显示时间进度
         ui->horizontalSlider->setRange(0,s.mid(11).toFloat());
      }
      else if((b.startsWith("ANS_PERCENT_POSITION")))
      {
         b.replace(QByteArray("\n"),QByteArray(""));
         QString s(b);
         currentPercent=s.mid(21);
         ui->percentLabel->setText(currentPercent+"%");
       }
    }
}

void mplayer_video::on_stop_clicked()
{
    process->write("quit\n");
    ui->horizontalSlider->setSliderPosition(0);
    ui->label_time->clear();
    ui->percentLabel->clear();
    ui->zanting->setText("播放");
}

void mplayer_video::on_voice_clicked()
{
     //process->write("mute 0\n"); //开启静音
    process->write("volume -2\n");
}

void mplayer_video::on_voice_slider_sliderMoved(int v)
{
    if(ui->zanting->text()=="播放")
    {
        process->write(QString("volume "+QString::number(v)+" 2\n").toUtf8());
        process->write("pause\n");
    }
    else{
        process->write(QString("volume "+QString::number(v)+" 2\n").toUtf8());
    }
}

void mplayer_video::on_del_clicked()
{
    if(ui->list->currentRow()==-1)
       QMessageBox::warning(this,"提示","未选中项目或列表为空",QMessageBox::Yes);
    else
    {
     ui->list->takeItem(ui->list->currentRow());
     process->close();
     ui->horizontalSlider->setSliderPosition(0);    //更新进度条
     ui->label_time->clear();
     ui->percentLabel->clear();
    }
}

void mplayer_video::play(QString fileName)
{
    QStringList arg1;
    process->kill();
    process=new QProcess(this);
    //connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
    arg1 << fileName;
    arg1 << "-slave";//默认情况下，mplayer接受键盘的命令，而"-slave"使其不再接受键盘事件，而是作为后台程序运行，
                     //接受以“\n”结束的命令控制，这样我们可以在进程中给他发送命令，而不需要操作键盘了.
    arg1 << "-quiet"; //尽可能的不打印播放信息
    arg1 << "-zoom"; //视频居中，四周黑条，全屏播放
    arg1 << "-wid" << QString::number((unsigned int)(ui->widget->winId()));
             // "-wid <窗口标识>" 是指让MPlayer依附于那个窗口，
             //ui->widget->winId() 这个指令就是用来获取widget的标识码 ,
             //这样视频播放的时候，就在这个部件里播放，相当于给他固定起来。

    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
    process->start(arg,arg1);   //开始播放进程
    ui->zanting->setText("暂停");

}

void mplayer_video::on_list_itemDoubleClicked(QListWidgetItem *item)
{
    speed=1;
    ui->lupin_lable->setVisible(false);
    play(item->text());
}


void mplayer_video::on_jiansu_clicked()
{
    speed=speed/2;
    process->write(QString("speed_set "+QString::number(speed)+" 2\n").toUtf8());
}

void mplayer_video::on_jiasu_clicked()
{

    speed=speed*2;
    process->write(QString("speed_set "+QString::number(speed)+" 2\n").toUtf8());
}

void mplayer_video::on_jietu_clicked()
{
    cout<<"ok"<<endl;
    //QString slcStr=".jpg";
    //QString filename = QFileDialog::getSaveFileName(this,tr("保存图片"),QDir::currentPath(),tr("PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)"),&slcStr);
    // snapImage.save(filename);//snapName是文件名字
    process->write("pause\n");
    if(ui->zanting->text()=="播放")
    {
        cout<<"1"<<endl;
        QPixmap snapImage = QPixmap::grabWindow(QApplication::desktop()->winId());
        //QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./未命名.jpg", "Image Files(*.jpg *.png)");
        //snapImage.save(filename);
        QString filename;
        QString slcStr;
        filename = QFileDialog::getSaveFileName(this,tr("保存图片"),"./未命名",tr("PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)"),&slcStr);//弹出保存图片的文件窗口
        if(slcStr.left(3)=="JPG")
            {
                if(filename.right(3)!="jpg")
                {
                    filename+=".jpg";
                }
            }
            if(slcStr.left(3)=="PNG")
            {
                if(filename.right(3)!="png")
                {
                    filename+=".png";
                }
            }
            if(slcStr.left(3)=="BMP")
            {
                if(filename.right(3)!="bmp")
                {
                    filename+=".bmp";
                }
            }
        if(filename!=NULL)
        {
            snapImage.save(filename);
        }
    }
    else
     {
       cout<<"2"<<endl;
       disconnect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
       ui->zanting->setText("播放");
       QPixmap snapImage = QPixmap::grabWindow(QApplication::desktop()->winId());
       //QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "./未命名.jpg", "Image Files(*.jpg *.png)");
       //snapImage.save(filename);
       QString filename;
       QString slcStr;
       filename = QFileDialog::getSaveFileName(this,tr("保存图片"),"./未命名",tr("PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)"),&slcStr);//弹出保存图片的文件窗口
       if(slcStr.left(3)=="JPG")
           {
               if(filename.right(3)!="jpg")
               {
                   filename+=".jpg";
               }
           }
           if(slcStr.left(3)=="PNG")
           {
               if(filename.right(3)!="png")
               {
                   filename+=".png";
               }
           }
           if(slcStr.left(3)=="BMP")
           {
               if(filename.right(3)!="bmp")
               {
                   filename+=".bmp";
               }
           }
       if(filename!=NULL)
       {
           snapImage.save(filename);
       }
       connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataRecieve()));
       process->write("get_time_length\n");
       process->write("get_time_pos\n");
       process->write("get_percent_pos\n");
       ui->zanting->setText("暂停");
     }
}


void mplayer_video::on_lupin_clicked()
{
    if(ui->lupin->text()=="录屏")
    {
        photoStart();
        timer->start(50);
        ui->lupin->setText("停止");
    }
    else
    {
         timer->stop();
         ui->lupin->setText("录屏");
    }
}

void mplayer_video::on_lupinstart_clicked()
{
    process->write("quit\n");

    QString dirName("D:\\video");
    qdir = new QDir(dirName);
    //qdir->setSorting(QDir::Name|QDir::Reversed);
    qdir->setSorting(QDir::Reversed|QDir::Time);
    list = qdir->entryInfoList();

    number2=0;
    timer2->start(100);
}

void mplayer_video::on_stop_bofang_clicked()
{
    timer2->stop();
    ui->lupin_lable->setVisible(false);
}
