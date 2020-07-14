#include "tobject.h"

TObject::TObject(QPoint startPos, QPoint targetPos, QString fileName)
    :pixmap(fileName)
{
    this->startPos = startPos;
    this->targetPos = targetPos;
    this->currentPos = startPos;
}
void TObject::draw(QPainter * painter)
{
    painter->drawPixmap(currentPos, pixmap);
}
void TObject::move()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "currentPos");
    animation->setDuration(2000);
    animation->setStartValue(startPos);
    animation->setEndValue(targetPos);
    animation->start();
}
QPoint TObject::getCurrentPos()
{
    return this->currentPos;
}
void TObject::setCurrentPos(QPoint pos)
{
    this->currentPos = pos;
}
