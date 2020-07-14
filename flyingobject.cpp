#include "flyingobject.h"
#include "ttower.h"
#include "tscene.h"
#include "utility.h"
#include "tmonster.h"

#include <QPoint>
#include <QPainter>
#include <QString>
#include <QPropertyAnimation>

const QSize Bullet::m_fixedSize(8,8);
int range = 20;

Bullet::Bullet()
{
}

Bullet::Bullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
               TScene * game,QString path):
    m_startPos(startPos),
    m_targetPos(targetPos),
    m_damage(damage),
    m_path(path),
    m_targetEnemy(targetEnemy),
    m_game(game)
{
}
archerBullet::archerBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
                           TScene * game,QString path):
    Bullet(startPos,targetPos,damage,targetEnemy,game,path)
{
    poisonAttack = 0.1;
}
ArtilleryBullet::ArtilleryBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
                           TScene * game,QString path):
    Bullet(startPos,targetPos,damage,targetEnemy,game,path)
{

}
MageBullet::MageBullet(QPoint startPos,QPoint targetPos,int damage,Enemy * targetEnemy,
                           TScene * game,QString path):
    Bullet(startPos,targetPos,damage,targetEnemy,game,path)
{
    slowAttack = 0.1;
}
QPoint Bullet::getCurrentPos()
{
    return m_currentPos;
}

void Bullet::setCurrentPos(QPoint pos)
{
    m_currentPos=pos;
}

void Bullet::move()
{
    static int duration=200;//子弹飞行的时间，经过100ms击中敌人
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}
void archerBullet::move()
{
    static int duration=200;//子弹飞行的时间，经过100ms击中敌人
    QPoint keyPos;
    keyPos.setX((m_startPos.x()+m_targetPos.x())/2);
    keyPos.setY((m_startPos.y()>m_targetPos.y())?(m_targetPos.y()-40):(m_startPos.y()-20));
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setKeyValueAt(0.5, keyPos);
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}
void ArtilleryBullet::move()
{
    static int duration=400;//子弹飞行的时间，经过400ms击中敌人
    QPoint keyPos;
    keyPos.setX((m_startPos.x()+m_targetPos.x())/2);
    keyPos.setY((m_startPos.y()>m_targetPos.y())?(m_targetPos.y()-40):(m_startPos.y()-20));
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setKeyValueAt(0.5, keyPos);
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}
void MageBullet::move()
{
    static int duration=200;//子弹飞行的时间，经过100ms击中敌人
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}
void Bullet::hitTarget()
{
    if(m_game->getEnemyList().indexOf(m_targetEnemy)!=-1)//如果mainwindow的敌人列表中，有子弹击中的这个敌人，该敌人受到相应的伤害
    {
        m_targetEnemy->getDamaged(m_damage);
    }
    m_game->removeBullet(this);//击中敌人后子弹就要消失
}
void archerBullet::hitTarget()
{
    if(m_game->getEnemyList().indexOf(m_targetEnemy)!=-1)//如果mainwindow的敌人列表中，有子弹击中的这个敌人，该敌人受到相应的伤害
    {
        m_targetEnemy->getPoisonAttack(this->poisonAttack);
        m_targetEnemy->getDamaged(m_damage);
    }
    m_game->removeBullet(this);//击中敌人后子弹就要消失
}
void MageBullet::hitTarget()
{
    if(m_game->getEnemyList().indexOf(m_targetEnemy)!=-1)//如果mainwindow的敌人列表中，有子弹击中的这个敌人，该敌人受到相应的伤害
    {
        m_targetEnemy->getSlowAttack(this->slowAttack);
        m_targetEnemy->getDamaged(m_damage);
    }
    m_game->removeBullet(this);//击中敌人后子弹就要消失
}
void ArtilleryBullet::hitTarget()
{
    if(m_game->getEnemyList().indexOf(m_targetEnemy)!=-1)//如果mainwindow的敌人列表中，有子弹击中的这个敌人，该敌人受到相应的伤害
    {
        m_targetEnemy->getDamaged(m_damage);

        foreach(Enemy * enemy, m_game->getEnemyList())
        {
            //qDebug()<<collisionWithCircle(m_targetEnemy->getPos(),range, enemy->getPos(), range);
            if(enemy->getPos() == m_targetEnemy->getPos())
            {
                continue;
            }
            if(collisionWithCircle(m_targetEnemy->getPos(),range, enemy->getPos(), range))
            {
                //qDebug()<<"1";
                enemy->getDamaged(m_damage);
            }
        }
    }
    m_game->removeBullet(this);//击中敌人后子弹就要消失
}
void Bullet::draw(QPainter *painter) const
{
    painter->drawPixmap(m_currentPos,m_path);
}
Rocket::Rocket(QPoint startPos, QPoint targetPos, int damage, TTower * targetTower,
               TScene * game, QString path)
    :Bullet(startPos,targetPos,damage,targetEnemy,game,path),targetTower(targetTower)
{}



void Rocket::move()
{
    static int duration = 1000;//子弹飞行的时间，经过100ms击中敌人
    QPropertyAnimation * animation=new QPropertyAnimation(this,"m_currentPos");
    animation->setDuration(duration);//设置持续时间
    animation->setStartValue(m_startPos);//设置起始位置
    animation->setEndValue(m_targetPos);//设置目标位置
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));//连接信号槽，击中敌人后，子弹动态运动结束
    animation->start();
}

void Rocket::hitTarget()
{
    targetTower->getRemoved();
    delete targetTower;

    m_game->removeBullet(this);
}

