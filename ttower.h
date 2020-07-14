#ifndef TTOWER_H
#define TTOWER_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QDebug>
#include <QTimer>
#include "tmonster.h"
#include "tobject.h"
#include "audioplayer.h"
class TScene;
class QPainter;
class Enemy;
class QTimer;
enum Tower{
    Archer_1,                               //1级弓箭塔
    Archer_2,                               //2级弓箭塔
    Archer_3,                               //3级弓箭塔
    Artillery_1,                            //1级炮塔
    ARTILLERY_2,                            //2级炮塔
    ARTILLERY_3,                            //3级炮塔
    Mage_1,                                 //1级魔法塔
    Mage_2,                                 //2级魔法塔
    Mage_3,                                 //3级魔法塔
};

class TTower: public QObject
{
    Q_OBJECT
public:
    TTower(QPoint pos, TScene * game, QString path=":/tower/Archer/Archer01.png");
    ~TTower();
    TTower();    
    virtual void draw(QPainter * painter) const;//画出防御塔

    void attackEnemy();//攻击敌人
    void targetKilled();//当目标被击杀后
    void chooseEnemyFromAttack(Enemy * enemy);//从可以攻击的敌人中，选出攻击的敌人

    void removeBullet();//移除防御塔产生的子弹
    void lostSightOfEnemy();//丢失攻击目标的视野
    void checkEnemyInRange();//检查敌人是否在攻击范围内
    Enemy * getAttackedEnemy();//得到正在攻击的敌人

    void getRemoved();//防御塔被移除

    virtual void levelUp() = 0;

protected slots:
    virtual void shootWeapon();//私有信号槽，实现和子弹类的连接

protected:

    bool			m_attacking;
    qreal			m_rotationSprite;

    Enemy *		    m_chooseEnemy;
    TScene *        m_game;
    QTimer *		m_fireRateTimer;
    QPoint          m_pos;//防御塔的中心点
    QString         m_path;//防御塔图片的路径

    int				m_attackRange;	// 代表塔可以攻击到敌人的距离
    int				m_damage;		// 代表攻击敌人时造成的伤害
    int				m_fireRate;		// 代表再次攻击敌人的时间间隔

    int             m_level;        // 塔等级
    AudioPlayer *   audioPlayer = new AudioPlayer(this);

    static const QSize m_fixedSize;//防御塔图片的固定大小
signals:
    void levelUP();
};

class Archer : public TTower
{
    Q_OBJECT
public:
    Archer(QPoint pos, TScene * game, QString path=":/tower/Archer/Archer01.png");
    ~Archer();
    void levelUp();
private:
    AudioPlayer *   audioPlayer = new AudioPlayer(1);
protected slots:
    void shootWeapon();//私有信号槽，实现和子弹类的连接



};

class Mage : public TTower
{
    Q_OBJECT
public:
    Mage(QPoint pos, TScene * game, QString path=":/tower/Mage/mage_1.png");
    ~Mage();
    void levelUp();
private:
    AudioPlayer *   audioPlayer = new AudioPlayer(1);
protected slots:
    void shootWeapon();//私有信号槽，实现和子弹类的连接

};

class Artillery : public TTower
{
    Q_OBJECT
public:
    Artillery(QPoint pos, TScene * game, QString path=":/tower/Artillery/Artillery_1.png");
    ~Artillery();
    void levelUp();
private:
    AudioPlayer *   audioPlayer = new AudioPlayer(1);
protected slots:
    void shootWeapon();//私有信号槽，实现和子弹类的连接
};



class TChooseTower : public QObject
{
    Q_OBJECT
public:
    TChooseTower(QPoint pos, TScene * game, QString path=":/item/circle.png");
    ~TChooseTower();
    AudioPlayer *   audioPlayer = new AudioPlayer(1);

    void draw(QPainter * painter) const;
private:
    QPoint m_pos;
    QString m_path;
    TScene * m_game;

    static const QSize m_fixedSize;//选择的大小

};
class TUpgrade: public QObject
{
    Q_OBJECT
public:
    TUpgrade(QPoint pos, TScene * game, QString path = ":/tower/upgrade.png");
    ~TUpgrade();
    AudioPlayer *   audioPlayer = new AudioPlayer(1);

    void draw(QPainter * painter) const;
private:
    QPoint m_pos;
    QString m_path;
    TScene * m_game;

    static const QSize m_fixedSize;//选择的大小

};
#endif // TTOWER_H
