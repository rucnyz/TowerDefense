#ifndef FLYINGOBJECT_H
#define FLYINGOBJECT_H

#include <QObject>
#include <QPoint>
#include <QPainter>
#include <QString>
#include <QSize>

#include "tmonster.h"
#include "ttower.h"
#include "tscene.h"
#include "utility.h"
extern int range;
class Enemy;
class TScene;
class TTower;

class Bullet :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ getCurrentPos WRITE setCurrentPos)//子弹动态移动
public:
    Bullet();
    Bullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
           TScene * game, QString path=":/bullet/bullet.png");
    QPoint getCurrentPos();//得到子弹的当前位置
    void setCurrentPos(QPoint pos);//设置子弹的当前位置

    virtual void move();//子弹的移动
    void draw(QPainter * painter)const;//绘画子弹



protected slots:
    virtual void hitTarget();//私有信号槽，击中敌人的时候触发

protected:
    QPoint m_startPos;
    QPoint m_targetPos;
    QPoint m_currentPos;
    int m_damage;
    QString m_path;

    Enemy * m_targetEnemy;
    TScene * m_game;

    static const QSize m_fixedSize;
};
class archerBullet:public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ getCurrentPos WRITE setCurrentPos)//子弹动态移动
public:
    archerBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
           TScene * game, QString path=":/bullet/archer.png");
    void move();
protected slots:
    void hitTarget();//私有信号槽，击中敌人的时候触发
private:
    double poisonAttack;
};

class ArtilleryBullet:public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ getCurrentPos WRITE setCurrentPos)//子弹动态移动
public:

    ArtilleryBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
           TScene * game, QString path=":/bullet/Artillery.png");
    void move();
protected slots:
    void hitTarget();//私有信号槽，击中敌人的时候触发

};
class MageBullet:public Bullet
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_currentPos READ getCurrentPos WRITE setCurrentPos)//子弹动态移动
public:

    MageBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
           TScene * game, QString path=":/bullet/mage.png");
    void move();
protected slots:
    void hitTarget();//私有信号槽，击中敌人的时候触发
private:
    double slowAttack;

};
class Rocket:public Bullet
{
    Q_OBJECT
public:
    Rocket(QPoint startPos, QPoint targetPos, int damage, TTower *targetTower,
           TScene * game, QString path= ":/bullet/boss.png");
    void move();
    Enemy *targetEnemy;
    TTower *targetTower;
protected slots:
    void hitTarget();

};
#endif // FLYINGOBJECT_H
