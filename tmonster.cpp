#include "tmonster.h"
#include "tscene.h"
#include "ttower.h"
#include "waypoint.h"

#include <QPainter>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QVector2D>

const QSize Enemy::m_fixedSize(22,23);//

Enemy::Enemy(WayPoint * startWayPoint,TScene *game,QString path):
    QObject(0),
    m_game(game),
    m_pos(startWayPoint->getPos()),
    m_path(path)
{
    m_maxHp=40;
    m_currentHp=m_maxHp;
    m_walkingSpeed=2;
    m_active=false;
    destinationWayPoint=startWayPoint->getNextWayPoint();//这里就充分利用了航点的好处，我们可以直接从当前航点得到下一个航点
    audioPlayer = new AudioPlayer(1);
}

Enemy::~Enemy()
{
    attackerTowerList.clear();//清楚在攻击该敌人的防御塔
    destinationWayPoint=NULL;//全部设为空指针
    m_game=NULL;
    delete audioPlayer;
}

void Enemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=m_fixedSize.width();//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-m_fixedSize.width()/2,-m_fixedSize.height());//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-m_fixedSize.width()/2, m_pos.y()-m_fixedSize.height()/2);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}

void Enemy::move()
{
    if(!m_active)//如果不能移动，就直接return
    {
        return ;
    }

    if(collisionWithCircle(m_pos,1,destinationWayPoint->getPos(),1))//如果到达了目标航点
    {
        if(destinationWayPoint->getNextWayPoint())//如果还存在下一个航点
        {//重新设置敌人的位置，和目标航点
            m_pos=destinationWayPoint->getPos();
            destinationWayPoint=destinationWayPoint->getNextWayPoint();
        }
        else//如果没有下一个航点了，代表敌人已经到达了基地
        {
            m_game->getHpDamaged();
            m_game->removeEnemy(this);
            return ;
        }
    }

    QPoint targetPoint=destinationWayPoint->getPos();
    double movementSpeed=m_walkingSpeed;
    QVector2D normailzed(targetPoint-m_pos);
    normailzed.normalize();
    m_pos=m_pos + normailzed.toPoint()*movementSpeed;


}
void finalBossEnemy::move()
{
    static int i =0;
    if(!m_active)//如果不能移动，就直接return
    {
        return ;
    }

    if(collisionWithCircle(m_pos,1,destinationWayPoint->getPos(),1))//如果到达了目标航点
    {
        if(destinationWayPoint->getNextWayPoint())//如果还存在下一个航点
        {//重新设置敌人的位置，和目标航点
            m_pos=destinationWayPoint->getPos();
            destinationWayPoint=destinationWayPoint->getNextWayPoint();
            if(i%2 == 1)
            {
                m_active = false;
                stopAndAttack();
                m_active = true;
            }
            i++;

        }
        else//如果没有下一个航点了，代表敌人已经到达了基地
        {
            m_game->getHpDamaged();
            m_game->removeEnemy(this);
            return ;
        }
    }

    QPoint targetPoint=destinationWayPoint->getPos();
    double movementSpeed=m_walkingSpeed;
    QVector2D normailzed(targetPoint-m_pos);
    normailzed.normalize();
    m_pos=m_pos + normailzed.toPoint()*movementSpeed;


}

void Enemy::doActive()
{
    m_active=true;
}

QPoint Enemy::getPos()
{
    return m_pos;
}

void Enemy::getAttacked(TTower *tower)
{
    attackerTowerList.push_back(tower);
}
void Enemy::getSlowAttack(double value)
{
    static int i =0;
    if(i == 0)
    {
        if(m_walkingSpeed != 1)
        {
            m_walkingSpeed -= value;
        }
         i++;
    }

}
void Enemy::getPoisonAttack(double value)
{
    m_currentHp-=value;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void Enemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}

void Enemy::getRemoved()
{
    if(attackerTowerList.empty())
    {
        m_game->removeEnemy(this);
        return;
    }
    else
    {

        foreach(TTower * tower,attackerTowerList)
            tower->targetKilled();
        m_game->removeEnemy(this);
    }
}

void Enemy::getLostSight(TTower *tower)
{
    attackerTowerList.removeOne(tower);
}

normalEnemy::normalEnemy(WayPoint *startWayPoint, TScene *game, const QString &sprite/* = QPixmap(":/image/enemy1.png")*/)
    :Enemy(startWayPoint, game, sprite)
{
    m_maxHp=16;
    m_currentHp=m_maxHp;
    m_walkingSpeed=2;

}
void normalEnemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        m_game->awardGold(3);
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void normalEnemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=m_fixedSize.width();//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-m_fixedSize.width()/2,-m_fixedSize.height());//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-11, m_pos.y()-12);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}
bigNormalEnemy::bigNormalEnemy(WayPoint *startWayPoint, TScene *game, const QString &sprite/* = QPixmap(":/image/enemy1.png")*/)
    :Enemy(startWayPoint, game, sprite)
{
    m_maxHp=64;
    m_currentHp=m_maxHp;
    m_walkingSpeed=2;

}
void bigNormalEnemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        m_game->awardGold(9);
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void bigNormalEnemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=m_fixedSize.width();//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-m_fixedSize.width()/2,-m_fixedSize.height());//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-53, m_pos.y()-56);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}
//iceMonster::iceMonster(WayPoint *startWayPoint, TScene *game, const QPixmap &sprite/* = QPixmap(":/image/enemy2.png")*/)
//    :TMonster(startWayPoint, game,sprite/* = QPixmap(":/image/enemy2.png")*/)
//{
//    this->enemyKind=2;
//    this->award=250;
//    this->m_maxHp = 75;
//    this->m_currentHp = 75;
//    this->antiSlowspeed=2.0;
//    this->fireLevel=20;
//    this->iceLevel=10;
//    this->HPdamage=1;
//    this->m_rotationSprite = 0.0;
//    this->m_pos=startWayPoint->pos();
//    this->m_destinationWayPoint=startWayPoint->nextWayPoint();
//}

//fireMonster::fireMonster(WayPoint *startWayPoint, TScene *game, const QPixmap &sprite/* = QPixmap(":/image/enemy3.png")*/)
//    :TMonster(startWayPoint, game,sprite/* = QPixmap(":/image/enemy3.png")*/)
//{
//    this->enemyKind=3;
//    this->award=300;
//    this->m_maxHp = 85;
//    this->m_currentHp = 85;
//    this->antiSlowspeed=1.2;
//    this->fireattackLevel=0.5;
//    this->fireLevel=10;
//    this->iceLevel=20;
//    this->HPdamage=2;
//    this->m_rotationSprite = 0.0;
//    this->m_pos=startWayPoint->pos();
//    this->m_destinationWayPoint=startWayPoint->nextWayPoint();
//}

fastEnemy::fastEnemy(WayPoint *startWayPoint, TScene *game, const QString &sprite/* = QPixmap(":/image/enemy4.png")*/)
    :Enemy(startWayPoint, game,sprite/* = QPixmap(":/image/enemy4.png")*/)
{
    m_maxHp=20;
    m_currentHp=m_maxHp;
    m_walkingSpeed=3;
}
void fastEnemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        m_game->awardGold(3);
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void fastEnemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=m_fixedSize.width();//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-m_fixedSize.width()/2,-m_fixedSize.height());//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-m_fixedSize.width()/2, m_pos.y()-m_fixedSize.height()/2);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}
void bossEnemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        m_game->awardGold(30);
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void bossEnemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=m_fixedSize.width();//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-56,-55);//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-56, m_pos.y()-55);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}
bossEnemy::bossEnemy(WayPoint *startWayPoint, TScene *game, const QString &sprite/* = QPixmap(":/image/enemy5.png")*/)
    :Enemy(startWayPoint, game,sprite/* = QPixmap(":/image/enemy5.png")*/)
{
    m_maxHp=200;
    m_currentHp=m_maxHp;
    m_walkingSpeed=2;
}

finalBossEnemy::finalBossEnemy(WayPoint *startWayPoint, TScene *game, const QString &sprite/* = QPixmap(":/image/enemy5.png")*/)
    :Enemy(startWayPoint, game,sprite/* = QPixmap(":/image/enemy5.png")*/)
{
    m_maxHp=800;
    m_currentHp=m_maxHp;
    m_walkingSpeed=1;
}
void finalBossEnemy::getDamaged(int damage)
{
    m_currentHp-=damage;
    if(m_currentHp<=0)
    {
        //qDebug()<<"1";
        m_game->awardGold(100);
        audioPlayer->playSound(EnemyDestorySound);
        getRemoved();
    }
}
void finalBossEnemy::draw(QPainter *painter) const
{
    if(!m_active)//如果敌人不能移动，就不对它进行绘画
    {
        return ;
    }

    painter->save();
    //下面准备绘画敌人的血条
    static const int healthBarWidth=60;//设置血条的长度
    QPoint healthBarPoint=m_pos+QPoint(-30,-51);//设置血条的左上点
    painter->setPen(Qt::NoPen);//画笔的颜色
    painter->setBrush(Qt::red);//刷子的颜色，用来填充内部
    QRect healthBarBackRect(healthBarPoint,QSize(healthBarWidth,2));
    painter->drawRect(healthBarBackRect);//画出血条

    //由于敌人被攻击后会扣血，因此还要画一个显示敌人当前血量的柱形
    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint,QSize((double)m_currentHp/m_maxHp*healthBarWidth,2));
    painter->drawRect(healthBarRect);//画出当前血量血条

    //把敌人画出来
    QPoint tmp(m_pos.x()-60, m_pos.y()-51);//得到图片的左上点
    painter->drawPixmap(tmp.x(),tmp.y(),m_path);
    painter->restore();
}

void finalBossEnemy::stopAndAttack()
{
    Bullet * bullet;
    int i =0;
    auto it=m_game->towerPositionList.begin();
    foreach(TowerPosition towerPosition, m_game->towerPositionList)
    {
        if(towerPosition.hasTower())
        {
            bullet=new Rocket(m_pos,towerPosition.getPos(),
                              m_damage, towerPosition.m_tower, m_game);//构造一个子弹，准备攻击敌人

            break;
        }
        i++;
        it++;
    }
    bullet->move();
    it->setHasTower(false);
    m_game->addBullet(bullet);//将该子弹添加到 map 中
}



