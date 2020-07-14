#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QDebug>
#include <QAbstractButton>
#include <QPropertyAnimation>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);

    MyPushButton(QString normalImg, QString pressImg = "");


    QString normalImgPath;
    QString pressImgPath;

    void mousePressEvent(QMouseEvent *e);
    void zoom1();
    void zoom2();
    void mouseReleaseEvent(QMouseEvent *e);
signals:

};

#endif // MYPUSHBUTTON_H
