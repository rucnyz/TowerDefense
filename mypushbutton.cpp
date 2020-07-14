#include "mypushbutton.h"

#include <QEasingCurve>

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->setMouseTracking(true);
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret)
    {
        qDebug() <<"图片加载失败";
                   return;
    }
    this->setFixedSize(pix.width(), pix.height());

    this->setStyleSheet("QPushButton{border:0px;}");

    this->setIcon(pix);

    this->setIconSize(QSize(pix.width(), pix.height()));

}
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "")
    {
        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret)
        {
            qDebug() <<"图片加载失败";
            return;
        }
        this->setFixedSize(pix.width(), pix.height());

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pix);

        this->setIconSize(QSize(pix.width(), pix.height()));
    }

    return QPushButton::mousePressEvent(e);
}

void MyPushButton::zoom1()
{
    //创建动画
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(), this->y()+3, this->width(), this->height()));
    //设置曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}

void MyPushButton::zoom2()
{
    //创建动画
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(), this->y()+3, this->width(), this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
    //设置曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "")
    {
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret)
        {
            qDebug() <<"图片加载失败";
            return;
        }
        this->setFixedSize(pix.width(), pix.height());

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pix);

        this->setIconSize(QSize(pix.width(), pix.height()));
    }

    return QPushButton::mouseReleaseEvent(e);
}
