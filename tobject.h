#ifndef TOBJECT_H
#define TOBJECT_H

#include <QObject>
#include <QPropertyAnimation>
#include <QPoint>
#include <QPixmap>
#include <QPainter>
#include <QLabel>

class TObject : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QPoint currentPos READ getCurrentPos WRITE setCurrentPos)
public:
    TObject(QPoint startPos, QPoint targetPos, QString fileName);
    void move();
    void draw(QPainter * painter);

    QPoint getCurrentPos();
    void setCurrentPos(QPoint pos);

protected:
    QPoint startPos;
    QPoint currentPos;
    QPoint targetPos;
    QPixmap pixmap;
signals:

};

#endif // TOBJECT_H
