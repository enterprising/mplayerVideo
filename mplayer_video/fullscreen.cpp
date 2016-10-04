#include "fullscreen.h"
#include "ui_fullscreen.h"
#include <QFileDialog>
#include <QDir>
//#include <kittool.h>
#include <QApplication>
#include <QClipboard>

fullscreen::fullscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fullscreen)
{
    kt = new kitTool();
    connect(kt,SIGNAL(screen_save()),this,SLOT(screen_save()));
    connect(kt,SIGNAL(esc_click()),this,SLOT(esc_click()));
    connect(this,SIGNAL(esc_click2()),kt,SLOT(esc_click2()));
    this->setWindowOpacity(1);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowState(Qt::WindowFullScreen);
//    QPushButton button = new QPushButton("hahah");

}

void fullscreen::WindowsInit()
{
    currentRectInit();
    background = QPixmap::grabWindow(QApplication::desktop()->winId());
    screen = background.toImage();
    screen_dark = background.toImage();

    int bytesPerLine = screen_dark.width() * screen_dark.depth() / 8;
    for(int i = 0;i<screen_dark.height();++i){
        uchar* lineBuf = screen_dark.scanLine(i);
        for(int j = 0;j < bytesPerLine;j++){
            lineBuf[j]/=2;
        }
    }
}

void fullscreen::currentRectInit()//区域截图窗口初始化函数
{
    end = start;
    QRect oldRect = currentRect;
    currentRect.setTopLeft(start);
    currentRect.setBottomRight(end);
    currentRect = currentRect.normalized();
    repaint(oldRect.united(currentRect));
}

void fullscreen::paintEvent(QPaintEvent *)
{
    painter=new QPainter(this);
    painter->drawImage(0, 0, screen_dark);
    painter->drawImage(currentRect,screen,currentRect,Qt::AutoColor);
}

void fullscreen::mousePressEvent(QMouseEvent *e){
    kt->hide();
    start = e->globalPos();
}

void fullscreen::mouseMoveEvent(QMouseEvent *e){
    end = e->globalPos();
    QRect oldRect = currentRect;
    currentRect.setTopLeft(start);
    currentRect.setBottomRight(end);
    currentRect = currentRect.normalized();
    repaint(oldRect.united(currentRect));
}

void fullscreen::mouseReleaseEvent(QMouseEvent *e)
{
    kt->showNormal();
    kt->move(currentRect.bottomRight().x() - 127,currentRect.bottomRight().y() + 10);
}

void fullscreen::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(currentRect.contains(e->globalPos()))
    {
        QImage img(clipImage(screen, currentRect));
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setImage(img);
        emit esc_click2();
        this->close();
    }
}

void fullscreen::keyPressEvent(QKeyEvent *k)
{
    if(k->modifiers()==Qt::ControlModifier&&k->key()==Qt::Key_S)
    {
        screencutSave();
    }
    else if(k->key()==Qt::Key_Escape)
    {
        emit esc_click2();
        emit on_show();
        this->close();
    }
}

void fullscreen::esc_click()
{
    emit on_show();
    this->close();
}

void fullscreen::screencutSave()
{
    QImage img(clipImage(screen, currentRect));
    QString filename;
    QString slcStr;
    filename = QFileDialog::getSaveFileName(this,tr("保存图片"),QDir::currentPath(),tr("PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)"),&slcStr);//弹出保存图片的文件窗口
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
        img.save(filename);
        emit esc_click2();
        emit on_show();
        this->close();
    }
}

void fullscreen::screen_save()
{
    screencutSave();
}

QImage fullscreen::clipImage(const QImage& srcImage, QRect rect)//区域截图，将指定位置的图像复制到和选框相同大小的空image上

{
    QImage image(rect.size(), QImage::Format_RGB32);
    const QImage* target = &srcImage;
    QImage targetImg;

    if ( srcImage.depth() != 32 )

    {
        targetImg = srcImage.convertToFormat(QImage::Format_RGB32);
        target = &targetImg;
    }

    int bytesPerPixel = image.depth() / 8;
    for ( int i = 0 ; i < image.height() ; ++ i )
    {
        uchar * line = image.scanLine(i);
        const uchar * srcLine = target->scanLine(rect.top() + i);
        memcpy(line,
               srcLine + rect.left() * bytesPerPixel,
               rect.width() * bytesPerPixel);
    }
    return image;
}

fullscreen::~fullscreen()
{
    delete ui;
}
