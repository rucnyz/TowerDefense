#ifndef TSCENE_H
#define TSCENE_H

#include <QWidget>
#include <QMovie>
#include <QIcon>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QKeyEvent>
#include <QList>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTime>
#include "waypoint.h"
#include "audioplayer.h"
#include "mypushbutton.h"
#include "ttower.h"
#include "towerposition.h"
#include "tmonster.h"
#include "flyingobject.h"
#include "dataset.h"
class Bullet;
class Rocket;
class Enemy;
class Map_1;
class Map_2;
class Map_3;

class TScene : public QWidget
{
    Q_OBJECT
public:
    explicit TScene(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) = 0;

    QList<TowerPosition >   towerPositionList;
    QList<TTower *>			towersList;
    QList<TChooseTower *>   chooseMenuList;
    QList<TUpgrade *>       upgradeList;

    int						m_waves=0;
    bool					gameEnded ;
    bool					gameWin;
    AudioPlayer *		    audioPlayer;

    void mousePressEvent(QMouseEvent *);//鼠标点击类函数


    void getHpDamaged();//敌人进入基地内部，基地要扣血
    void awardGold(int i);
    void removeEnemy(Enemy * enemy);//敌人死亡，在mainwindow中要移除
    void removeTower(TTower * tower);

    QList<Enemy *> getEnemyList();//得到Enemy*的list

    bool canBuyTower(int moneyNeeded);

    void removeBullet(Bullet * bullet);
    void addBullet(Bullet * bullet);

   // AudioPlayer* audioPlayer() const;

protected:
    virtual bool loadWave() = 0;

    QList<QVariant>			m_wavesInfo;

//    QList<WayPoint *>		wayPointsList;


protected:
    QList<Enemy *> enemyList;//用来储存敌人的list
    QList<Bullet *> bulletList;//用来储存子弹的list
    QList<Rocket *> rocketList;
    int playerHp;//基地的血量
    int playerGold ;//初始金钱
protected slots:
    void updateMap();
    void gameStart();
};




class Map_1 : public TScene
{
    Q_OBJECT
public:
    Map_1(int dif = 0, QWidget * parent = nullptr);
    //~Map_1();
    void paintEvent(QPaintEvent *);
    void loadTowerPositions();
protected:
    //void keyPressEvent(QKeyEvent *event);
    bool loadWave();
    QList<WayPoint *>		wayPointsList;
private:
    int diff = 0;


    void addWayPoints();

    void drawWave(QPainter *painter) const;
    void drawGold(QPainter *painter) const;
    void drawHP(QPainter *painter) const;
    void drawPlayerGold();

    void doGameOver();//执行游戏结束

    void preLoadWavesInfo();


signals:
    void getMapBack();
    void stopBGM();
};

class Map_2 : public TScene
{
    Q_OBJECT
public:
    Map_2(int dif, QWidget * parent = nullptr);
    void paintEvent(QPaintEvent *);
    void loadTowerPositions();
protected:
    //void keyPressEvent(QKeyEvent *event);
    bool loadWave();
    QList<WayPoint *>		wayPointsList;
    QList<WayPoint *>		wayPointsList2;
private:
    int diff = 0;


    void addWayPoints();
    void addWayPoints2();

    void drawWave(QPainter *painter) const;
    void drawGold(QPainter *painter) const;
    void drawHP(QPainter *painter) const;
    void drawPlayerGold();

    void doGameOver();//执行游戏结束

    void preLoadWavesInfo();


signals:
    void getMapBack();
    void stopBGM();
};

class Map_3 : public TScene
{
    Q_OBJECT
public:
    Map_3(int dif, QWidget * parent = nullptr);

    void paintEvent(QPaintEvent *);
    void loadTowerPositions();
protected:
    //void keyPressEvent(QKeyEvent *event);
    bool loadWave();
    QList<WayPoint *>		wayPointsList;
private:
    int diff = 0;


    void addWayPoints();

    void drawWave(QPainter *painter) const;
    void drawGold(QPainter *painter) const;
    void drawHP(QPainter *painter) const;
    void drawPlayerGold();

    void doGameOver();//执行游戏结束

    void preLoadWavesInfo();


signals:
    void getMapBack();
    void stopBGM();
};

class ChooseMenu : public QWidget
{
    Q_OBJECT
public:
    ChooseMenu(QWidget * parent = nullptr);
    void paintEvent(QPaintEvent *);
    void setDiff(int x);
    int getDiff();
    Map_1 * map01 = NULL;
    Map_2 * map02 = NULL;
    Map_3 * map03 = NULL;

private:
    int diff = 0;
    AudioPlayer * audioPlayer;

signals:
    void getMenuBack();
};


class ChooseDifficultyMenu : public QWidget
{
    Q_OBJECT
public:
    ChooseDifficultyMenu(QWidget * parent = nullptr);
    void paintEvent(QPaintEvent *);
    ChooseMenu * chooseMenu = NULL;
private:
    AudioPlayer * audioPlayer;

signals:
    void getStartBack();

    //返回主场景



};

#endif // TSCENE_H
