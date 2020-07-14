#include "ttower.h"
#include "tscene.h"
#include "tmonster.h"

const QSize TTower::m_fixedSize(150, 140);
const QSize TChooseTower::m_fixedSize(35,35);
const QSize TUpgrade::m_fixedSize(35,35);

TTower::TTower()
{
}

TTower::~TTower()
{
    delete m_fireRateTimer;
    m_fireRateTimer=NULL;
    m_chooseEnemy=NULL;
    m_game=NULL;
    delete m_chooseEnemy;
}

TTower::TTower(QPoint pos, TScene * game,QString path)://根据地图的大小，确定攻击范围
    m_attacking(false),
    m_chooseEnemy(NULL),
    m_game(game),
    m_pos(pos),
    m_path(path),
    m_attackRange(295),
    m_damage(5),//攻击力10
    m_fireRate(1000),//1000ms
    m_level(1)
{
    m_fireRateTimer=new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));

}

Archer::Archer(QPoint pos, TScene * game,QString path):
    TTower(pos, game, path)
{
    m_damage = 5;
    m_fireRate = 600;
    audioPlayer->playSound(ArcherReady);
}
Archer::~Archer()
{
    delete m_fireRateTimer;
    m_fireRateTimer=NULL;
    m_chooseEnemy=NULL;
    m_game=NULL;
    delete m_chooseEnemy;
}
void Archer::levelUp()
{
    if(m_level == 1)
    {
        audioPlayer->playSound(ArcherTaunt1);
        m_path = ":/tower/Archer/Archer_2.png";
    }
    else if(m_level == 2)
    {
        audioPlayer->playSound(ArcherTaunt2);
        m_path = ":/tower/Archer/archer_3.png";

    }
    m_level++;
    m_damage+=3;
}

Mage::Mage(QPoint pos, TScene * game,QString path):
    TTower(pos, game, path)
{
    m_damage = 16;
    m_fireRate = 1200;
    m_attackRange = 320;
    audioPlayer->playSound(MageReady);
}
Mage::~Mage()
{
    delete m_fireRateTimer;
    m_fireRateTimer=NULL;
    m_chooseEnemy=NULL;
    m_game=NULL;
    delete m_chooseEnemy;
}
void Mage::levelUp()
{
    if(m_level == 1)
    {
        audioPlayer->playSound(MageTaunt1);
        m_path = ":/tower/Mage/mage_2.png";
    }
    else if(m_level == 2)
    {
        audioPlayer->playSound(MageTaunt2);
        m_path = ":/tower/Mage/mage_3.png";
    }
    m_level++;
    m_damage+=5;

}


Artillery::Artillery(QPoint pos, TScene * game,QString path):
    TTower(pos, game, path)
{
    m_damage = 11;
    m_fireRate = 800;
    audioPlayer->playSound(ArtilleryReady);
}
Artillery::~Artillery()
{
    delete m_fireRateTimer;
    m_fireRateTimer=NULL;
    m_chooseEnemy=NULL;
    m_game=NULL;
    delete m_chooseEnemy;
}
void Artillery::levelUp()
{
    if(m_level == 1)
    {
        audioPlayer->playSound(ArtilleryTaunt1);
        m_path = ":/tower/Artillery/Artillery_2.png";
    }
    else if(m_level == 2)
    {
        audioPlayer->playSound(ArtilleryTaunt2);
        m_path = ":/tower/Artillery/Artillery_3.png";
    }
    m_level++;
    range+=2;
    m_damage+=5;
}



void TTower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::green);
    painter->drawEllipse(m_pos.x() - 120, m_pos.y() - 120, m_attackRange,
                         m_attackRange);//画出防御塔的攻击范围
    painter->drawPixmap(m_pos.x()-m_fixedSize.width()/2+20, m_pos.y()-m_fixedSize.height()/2+30,
                        150, 140, m_path);//画出防御塔的图片
    painter->restore();
}
void TTower::chooseEnemyFromAttack(Enemy *enemy)
{
    m_chooseEnemy=enemy;
    attackEnemy();
    m_chooseEnemy->getAttacked(this);//该敌人受到该防御塔的攻击
}

void TTower::attackEnemy()
{
    m_fireRateTimer->start(m_fireRate);//开始攻击
    //audioPlayer->playSound(ArcherHit);
}

void TTower::shootWeapon()
{
    Bullet * bullet=new Bullet(m_pos,m_chooseEnemy->getPos(),m_damage,m_chooseEnemy,m_game);//构造一个子弹，准备攻击敌人
    bullet->move();
    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
}
void Archer::shootWeapon()
{
    Bullet * bullet=new archerBullet(m_pos,m_chooseEnemy->getPos(),m_damage,m_chooseEnemy,m_game);//构造一个子弹，准备攻击敌人
    bullet->move();
    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
}
void Artillery::shootWeapon()
{
    Bullet * bullet=new ArtilleryBullet(m_pos,m_chooseEnemy->getPos(),m_damage,m_chooseEnemy,m_game);//构造一个子弹，准备攻击敌人
    bullet->move();
    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
}
void Mage::shootWeapon()
{
    Bullet * bullet=new MageBullet(m_pos,m_chooseEnemy->getPos(),m_damage,m_chooseEnemy,m_game);//构造一个子弹，准备攻击敌人
    bullet->move();
    m_game->addBullet(bullet);//将该子弹添加到mainwindow中
}
void TTower::targetKilled()
{
    if(m_chooseEnemy)
    {
        m_chooseEnemy=NULL;
    }
    m_fireRateTimer->stop();//敌人死亡，停止开火
}

void TTower::lostSightOfEnemy()
{
    m_chooseEnemy->getLostSight(this);
    //qDebug()<<"1";
    if(m_chooseEnemy)
    {
        m_chooseEnemy=NULL;
    }
    m_fireRateTimer->stop();
}

void TTower::checkEnemyInRange()
{
    QPoint adjustPos;
    adjustPos.setX(m_pos.x() + 27);adjustPos.setY(m_pos.y() + 27);
    if(m_chooseEnemy)//如果有了攻击的敌人
    {
        QVector2D normalized(m_chooseEnemy->getPos()-adjustPos);
        normalized.normalize();
        if(!collisionWithCircle(adjustPos, m_attackRange/2 + 10, m_chooseEnemy->getPos(),1))//当敌人不在范围内的时候
        {
            lostSightOfEnemy();
        }
    }
    else//如果没有攻击的敌人，就遍历enemylist，找到在攻击范围内的敌人
    {
        QList<Enemy * > enemyList=m_game->getEnemyList();
        foreach(Enemy * enemy,enemyList)
            if(collisionWithCircle(adjustPos, m_attackRange/2 + 10, enemy->getPos(), 1))
            {
                chooseEnemyFromAttack(enemy);
                break;
            }
    }
}

Enemy * TTower::getAttackedEnemy()
{
    return m_chooseEnemy;
}

void TTower::getRemoved()
{
    if(getAttackedEnemy())//这里要判断是不是空指针NULL
    {
        getAttackedEnemy()->getLostSight(this);//移除后，敌人被该防御塔丢失视野
    }
    audioPlayer->playSound(TowerSell);
    m_game->removeTower(this);
}

TChooseTower::TChooseTower(QPoint pos, TScene * game, QString path):
    m_pos(pos),
    m_path(path),
    m_game(game)
{
}
TChooseTower::~TChooseTower()
{
    delete this;
}

void TChooseTower::draw(QPainter *painter) const
{

    painter->save();
    painter->drawPixmap(m_pos.x()-m_fixedSize.width()/2-80, m_pos.y()-m_fixedSize.height()/2-50,
                        250, 250, m_path);
    painter->restore();
}
TUpgrade::TUpgrade(QPoint pos, TScene * game, QString path):
    m_pos(pos),
    m_path(path),
    m_game(game)
{
}
TUpgrade::~TUpgrade()
{
    delete this;
}
void TUpgrade::draw(QPainter *painter) const
{
    painter->save();
    painter->drawPixmap(m_pos.x()-m_fixedSize.width()/2-40, m_pos.y()-m_fixedSize.height()/2-50,
                        150, 200, m_path);
    painter->restore();
}
