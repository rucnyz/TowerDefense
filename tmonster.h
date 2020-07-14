#ifndef TMONSTER_H
#define TMONSTER_H
#include <QObject>
#include <QPoint>
#include <QString>
#include <QPainter>
#include <QSize>

#include "waypoint.h"
#include "ttower.h"
#include "utility.h"
#include "audioplayer.h"


class TScene;
class TTower;
class QPainter;
class wayPoint;
class Bullet;

class Enemy:public QObject
{
    Q_OBJECT
public:
    Enemy(WayPoint * startPoint, TScene * game,QString path=":/monster/normalMonster.png");
    ~Enemy();
    virtual void draw(QPainter * painter)const;
    virtual void move();//敌人的移动

    QPoint getPos();//得到敌人的当前位置

    void getAttacked(TTower * tower);//被tower攻击
    virtual void getDamaged(int damage);//敌人被攻击受到伤害
    void getRemoved();//当血量非正时，敌人死亡，需要移除
    void getLostSight(TTower * tower);//敌人脱离tower的攻击范围
    void getSlowAttack(double value);
    void getPoisonAttack(double value);



protected slots:
    void doActive();//私有信号槽，敌人是否可以移动

protected:
    int m_maxHp;//最大血量
    int m_currentHp;//当前血量
    double m_walkingSpeed;//移动速度
    bool m_active;//是否可以移动

    WayPoint * destinationWayPoint;//目标航点的指针
    TScene * m_game;//指针
    QPoint m_pos;//当前位置
    QString m_path;//图片路径
    QList<TTower *> attackerTowerList;//正在攻击该敌人的防御塔list

    static const QSize m_fixedSize;

    AudioPlayer * audioPlayer;
};


class normalEnemy:public Enemy
{
    Q_OBJECT
public:
    normalEnemy(WayPoint *startWayPoint,
                  TScene *game, const QString &sprite = ":/monster/normalMonster.png");
    void draw(QPainter * painter)const;
    void getDamaged(int damage);
};
class bigNormalEnemy:public Enemy
{
    Q_OBJECT
public:
    bigNormalEnemy(WayPoint *startWayPoint,
                  TScene *game, const QString &sprite = ":/monster/big.png");
    void draw(QPainter * painter)const;
    void getDamaged(int damage);
};
//class iceMonster:public TMonster{
//    Q_OBJECT
//public:
//    iceMonster(WayPoint *startWayPoint,
//               TScene *game, const QPixmap &sprite = QPixmap(":/image/enemy2.png"));
//};

//class fireMonster:public TMonster{
//    Q_OBJECT
//public:
//    fireMonster(WayPoint *startWayPoint,
//                TScene *game, const QPixmap &sprite = QPixmap(":/image/enemy3.png"));
//};

class fastEnemy:public Enemy{
    Q_OBJECT
public:
    fastEnemy(WayPoint *startWayPoint,
                TScene *game, const QString &sprite = ":/monster/kuai_front/image 11010.png");
    void draw(QPainter * painter)const;
    void getDamaged(int damage);
};

class bossEnemy:public Enemy
{
    Q_OBJECT
public:
    bossEnemy(WayPoint *startWayPoint,
                TScene *game, const QString &sprite = ":/monster/boss/boss_1.png");
    void draw(QPainter * painter)const;
    void getDamaged(int damage);
};
class finalBossEnemy:public Enemy
{
    Q_OBJECT
public:
    finalBossEnemy(WayPoint *startWayPoint,
                TScene *game, const QString &sprite = ":/monster/boss/boss_3.png");
    void draw(QPainter * painter)const;
    void getDamaged(int damage);
    void move();
    void stopAndAttack();
protected:
    int m_damage = 10;
};


//inline bool collisionWithCircle(QPoint point1, int radius1, QPoint point2, int radius2)
//{
//    const int xdif = point1.x() - point2.x();
//    const int ydif = point1.y() - point2.y();
//    const int distance = qSqrt(xdif * xdif + ydif * ydif);

//    if (distance <= radius1 + radius2)
//        return true;

//    return false;
//}
#endif //TMONSTER_H
