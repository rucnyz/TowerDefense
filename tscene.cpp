#include "tscene.h"

TScene::TScene(QWidget *parent) :
    QWidget(parent),
    m_waves(0),
    gameEnded(false),
    gameWin(false),
    playerHp(5),
    playerGold(500)
{

}


void TScene::getHpDamaged()
{
    if(playerHp == 1)
    {
        gameEnded = true;
        return;
    }
    audioPlayer->playSound(LifeLoseSound);
    playerHp -= 1;

}
QList<Enemy *> TScene::getEnemyList()
{
    return enemyList;
}
void TScene::awardGold(int i)
{
    playerGold += i;
}

//AudioPlayer *TScene::audioPlayer() const
//{
//    return audioPlayer;
//}
void TScene::mousePressEvent(QMouseEvent * event)
{
    QPoint pressPos=event->pos();//得到鼠标点击的位置

    auto it=towerPositionList.begin();
    static auto current = it-1;
    if(current != towerPositionList.begin()-1)
    {
        chooseMenuList.clear();
        upgradeList.clear();
        if(current->hasChooseCircle() && !current->hasUpgradeCircle())
        {
            if(current->ContainArcher(pressPos))
            {
                if(canBuyTower(70))
                {
                    audioPlayer->playSound(TowerPlaceSound);
                    //m_audioPlayer->playSound(ArcherReady);
                    playerGold -= 70;
                    TTower * tower = new Archer(current->getCenterPos(), this);
                    towersList.push_back(tower);
                    current->setHasTower(true);
                    current->m_tower = tower;
                }
                else
                {
                    audioPlayer->playSound(NotEnoughGold);
                    current->setHasChooseCircle(false);


                }
            }
            else if(current->ContainArtillery(pressPos))
            {

                if(canBuyTower(125))
                {
                    audioPlayer->playSound(TowerPlaceSound);
                    playerGold -= 125;
                    TTower * tower = new Artillery(current->getCenterPos(), this);
                    towersList.push_back(tower);
                    current->setHasTower(true);
                    current->m_tower = tower;
                }
                else
                {
                    audioPlayer->playSound(NotEnoughGold);
                    current->setHasChooseCircle(false);

                }
            }
            else if (current->ContainMage(pressPos))
            {
                if(canBuyTower(100))
                {
                    audioPlayer->playSound(TowerPlaceSound);
                    playerGold -= 100;
                    TTower * tower = new Mage(current->getCenterPos(), this);
                    towersList.push_back(tower);
                    current->setHasTower(true);
                    current->m_tower = tower;
                }
                else
                {
                    audioPlayer->playSound(NotEnoughGold);
                    current->setHasChooseCircle(false);

                }
            }
            current->setHasChooseCircle(false);


        }
        else if (!current->hasChooseCircle() && current->hasUpgradeCircle() && current->hasTower())
        {
            if(current->ContainUpgrade(pressPos))
            {
                if(canBuyTower(100))
                {
                    playerGold-=100;
                    current->m_tower->levelUp();
                }
                else
                {
                    audioPlayer->playSound(NotEnoughGold);

                }
            }
            else if(current->ContainSell(pressPos))
            {
                awardGold(50);
                current->m_tower->getRemoved();
                delete current->m_tower;
                current->setRemoveTower();
            }
            current->setHasUpgradeCircle(false);
        }
        current = towerPositionList.begin()-1;
        update();
        return;

    }



    while(it!=towerPositionList.end())//遍历所有的防御塔坑
    {
        if(Qt::LeftButton==event->button())//如果是鼠标左键点击
        {
            if(it->ContainPos(pressPos))
            {
                if(!it->hasTower() && !it->hasChooseCircle())
                {
                    TChooseTower * choose = new TChooseTower(it->getCenterPos(), this);
                    chooseMenuList.push_back(choose);
                    it->setHasChooseCircle(true);
                    current = it;
                    update();
                    break;
                }
                else if (it->hasTower() && !it->hasChooseCircle())
                {
                    TUpgrade * upgrade = new TUpgrade(it->getCenterPos(), this);
                    upgradeList.push_back(upgrade);
                    it->setHasUpgradeCircle(true);
                    current = it;
                    update();
                    break;
                }
            }


        }
        ++it;
    }
}
void TScene::gameStart()
{
    loadWave();
}


void TScene::updateMap()
{
    foreach(Enemy * enemy, enemyList)
        enemy->move();

    foreach(TTower * tower, towersList)
        tower->checkEnemyInRange();
    update();
}

ChooseDifficultyMenu::ChooseDifficultyMenu(QWidget *parent) : QWidget(parent)
{
    audioPlayer = new AudioPlayer(this);
    audioPlayer->startBGM_2();
    this->setMouseTracking(true);
    //开始选难度
    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("选择关卡");

    MyPushButton * backBtn = new MyPushButton(":/item/back01.png", ":/item/back02.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            emit this->getStartBack();
        });

    });
    //难度按钮
    QString dif[3] = {"Easy", "Mid", "Hard"};
    for(int i = 0; i < 3; i++)
    {

        MyPushButton * difficulty = new MyPushButton(":/item/frame.png");
        difficulty->setParent(this);
        difficulty->move(i * 200 + 400, 400);
        QLabel * label = new QLabel(this);
        label->setFixedSize(difficulty->width(), difficulty->height());
        label->setText(dif[i]);
        label->move(i * 200 + 420, 400);
        label->setFont(QFont("Consolas", 15));
        label->setStyleSheet("color:white");

        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        connect(difficulty, &MyPushButton::clicked, [=](){

         difficulty->zoom1();
         difficulty->zoom2();
         //qDebug()<<i;
         chooseMenu = new ChooseMenu;
         audioPlayer->stopBGM_2();
         audioPlayer->playSound(Push);
         QTimer::singleShot(500, this, [=](){
            this->hide();
            chooseMenu->setDiff(i);
            chooseMenu->show();

            });
         connect(chooseMenu, &ChooseMenu::getMenuBack, [=](){
             chooseMenu->~ChooseMenu();
             this->show();
             audioPlayer->playSound(Push);
             audioPlayer->startBGM_2();
         });

        });
    }
}
void ChooseDifficultyMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/startmenu/startmenu.bmp");
    painter.drawPixmap(0, 0 , this->width(), this->height(), pix);

    //画标题
    pix.load(":/startmenu/title.png");
    painter.drawPixmap(100, 0, pix);

}

int ChooseMenu::getDiff()
{
    return diff;
}
ChooseMenu::ChooseMenu(QWidget *parent) : QWidget(parent)
{

    this->setMouseTracking(true);

    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("选择关卡");
    MyPushButton * backBtn = new MyPushButton(":/item/back01.png", ":/item/back02.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());
    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            emit this->getMenuBack();
        });

    });
    MyPushButton * level_1 = new MyPushButton(":/map/flag_1.png", ":/map/flag02.png");
    level_1->setParent(this);
    level_1->move(600, 770);
    connect(level_1, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        map01 = new Map_1(getDiff());
        //map01->setAttribute(Qt::WA_DeleteOnClose);
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            this->hide();
            map01->show();
        });
        connect(map01, &Map_1::getMapBack, [=](){
            map01->~Map_1();
            audioPlayer->playSound(Push);
            this->show();
        });

    });
    MyPushButton * level_2 = new MyPushButton(":/map/flag_1.png", ":/map/flag02.png");
    level_2->setParent(this);
    level_2->move(640, 630);
    connect(level_2, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        map02 = new Map_2(getDiff());
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            this->hide();

            map02->show();
        });
        connect(map02, &Map_2::getMapBack, [=](){
            map02->~Map_2();
            audioPlayer->playSound(Push);
            this->show();
        });

    });

    MyPushButton * level_3 = new MyPushButton(":/map/flag_1.png", ":/map/flag02.png");
    level_3->setParent(this);
    level_3->move(710, 500);
    connect(level_3, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        map03 = new Map_3(getDiff());
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            this->hide();

            map03->show();
        });
        connect(map03, &Map_3::getMapBack, [=](){
            map03->~Map_3();
            audioPlayer->playSound(Push);
            this->show();
        });

    });


}
void ChooseMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/map/map.bmp");
    painter.drawPixmap(0, 0 , this->width(), this->height(), pix);

    //画标题
    pix.load(":/startmenu/title.png");
    painter.drawPixmap(100, 0, pix);

}
void ChooseMenu::setDiff(int x)
{
    this->diff = x;
}
Map_1::Map_1(int dif, QWidget * parent):
    TScene(parent),
    diff(dif)
{
//    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "/easy.mp3");
//    m_audioPlayer = new AudioPlayer(backgroundMusicUrl,this);
//    m_audioPlayer->startBGM();
    audioPlayer = new AudioPlayer(this);
    audioPlayer->startBGM_1Map();
    this->setMouseTracking(true);

    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("关卡一");
    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/item/back01.png", ":/item/back02.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());

    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        audioPlayer->playSound(Push);

        QTimer::singleShot(500, this, [=](){
            emit this->getMapBack();

        });

    });
    preLoadWavesInfo();
    loadTowerPositions();
    addWayPoints();
    QTimer * timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateMap()));
    timer->start(30);
    QTimer::singleShot(3000,this,SLOT(gameStart()));

}
bool Map_1::loadWave()
{
    if(diff == 0)
    {
        if(m_waves>=7)
        {
            return false;
        }

        for(int i=0;i<16;++i)
        {
            if(easyMap1EnemyGene[m_waves][i] == 0)
                return true;
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy;
            switch (easyMap1EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(easyMap1EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    else if (diff == 1)
    {
        if(m_waves>=7)
        {
            return false;
        }

        for(int i=0;i<16;++i)
        {
            if(easyMap1EnemyGene[m_waves][i] == 0)
                return true;
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy;
            switch (easyMap1EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(easyMap1EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    else if (diff == 2)
    {
        if(m_waves>=10)
        {
            return false;
        }
        for(int i=0;i < 20;++i)
        {
            if(hardMap1EnemyGene[m_waves][i] == 0)
                return true;
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy;
            switch (hardMap1EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(hardMap1EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    return true;
}

//Map_1::~Map_1()
//{

//}
void TScene::removeEnemy(Enemy * enemy)
{
    Q_ASSERT(enemy);
    enemyList.removeOne(enemy);//死亡的敌人从enemylist中移除
    delete enemy;
    if(enemyList.empty())
    {
        ++m_waves;
        if(!loadWave())
        {
            gameWin=true;
        }
    }
}
void TScene::removeTower(TTower *tower)
{
    towersList.removeOne(tower);
}
//Map_1::~Map_1()
//{
//    delete audioPlayer;
//    foreach(TChooseTower * chooseTower, chooseMenuList)
//    {
//        chooseMenuList.removeOne(chooseTower);
//        delete chooseTower;
//    }
//    foreach(TUpgrade * chooseTower, upgradeList)
//    {
//        upgradeList.removeOne(chooseTower);
//        delete chooseTower;
//    }
//    foreach(WayPoint * chooseTower, wayPointsList)
//    {
//        wayPointsList.removeOne(chooseTower);
//        delete chooseTower;
//    }
//    foreach (TTower *tower, towersList)
//    {
//        Q_ASSERT(tower);
//        towersList.removeOne(tower);
//        delete tower;
//    }
//    foreach (Enemy *enemy, enemyList)
//    {
//        Q_ASSERT(enemy);
//        enemyList.removeOne(enemy);
//        delete enemy;
//    }
//    foreach (Bullet *bullet, bulletList)
//    {
//        removeBullet(bullet);
//    }
//    //delete this;
//}

void Map_1::paintEvent(QPaintEvent *)
{
    if(gameEnded || gameWin)//画出游戏结束的画面
    {
        QString text=gameEnded ? "YOU LOST":"YOU WIN";
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }

    QPainter painter(this);
    QPixmap pix;
    pix.load(":/map/map01.bmp");
    painter.drawPixmap(0, 0 , this->width(), this->height(), pix);
    pix.load(":/item/ui.png");
    painter.drawPixmap(30, 5, 100, 50, pix);
    painter.drawPixmap(30, 920, 100, 50, pix);
    painter.drawPixmap(200, 920, 100, 50, pix);

//    foreach(const WayPoint * waypoint, wayPointsList)
//        waypoint->draw(&painter);

    foreach(const TowerPosition towerposition, towerPositionList)
        towerposition.draw(&painter);

    foreach(const TTower * tower, towersList)
        tower->draw(&painter);

    foreach(const TChooseTower * choose, chooseMenuList)
        choose->draw(&painter);

    foreach(const TUpgrade * upgrade, upgradeList)
        upgrade->draw(&painter);

    foreach(const Enemy * enemy, enemyList)
        enemy->draw(&painter);

    foreach(const Bullet * bullet, bulletList)
        bullet->draw(&painter);

    foreach(const Rocket * rocket, rocketList)
        rocket->draw(&painter);

    drawHP(&painter);
    drawGold(&painter);
    drawWave(&painter);

}
void Map_1::preLoadWavesInfo()
{

}
void Map_1::addWayPoints()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(540, 1000));
    wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(550, 900));
    wayPointsList.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(570, 710));
    wayPointsList.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(700, 700));
    wayPointsList.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(770, 600));
    wayPointsList.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(700, 500));
    wayPointsList.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint7 = new WayPoint(QPoint(400, 570));
    wayPointsList.push_back(wayPoint7);
    wayPoint6->setNextWayPoint(wayPoint7);

    WayPoint *wayPoint8 = new WayPoint(QPoint(270, 400));
    wayPointsList.push_back(wayPoint8);
    wayPoint7->setNextWayPoint(wayPoint8);

    WayPoint *wayPoint9 = new WayPoint(QPoint(500, 360));
    wayPointsList.push_back(wayPoint9);
    wayPoint8->setNextWayPoint(wayPoint9);

    WayPoint *wayPoint10 = new WayPoint(QPoint(600, 300));
    wayPointsList.push_back(wayPoint10);
    wayPoint9->setNextWayPoint(wayPoint10);

    WayPoint *wayPoint11 = new WayPoint(QPoint(650, 160));
    wayPointsList.push_back(wayPoint11);
    wayPoint10->setNextWayPoint(wayPoint11);

    WayPoint *wayPoint12 = new WayPoint(QPoint(800, 160));
    wayPointsList.push_back(wayPoint12);
    wayPoint11->setNextWayPoint(wayPoint12);

    WayPoint *wayPoint13 = new WayPoint(QPoint(900, 350));
    wayPointsList.push_back(wayPoint13);
    wayPoint12->setNextWayPoint(wayPoint13);

    WayPoint *wayPoint14 = new WayPoint(QPoint(1200, 350));
    wayPointsList.push_back(wayPoint14);
    wayPoint13->setNextWayPoint(wayPoint14);
}
void Map_2::addWayPoints()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(540, 1000));
    wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(550, 880));
    wayPointsList.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(540, 760));
    wayPointsList.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(800, 760));
    wayPointsList.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(1080, 560));
    wayPointsList.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(980, 350));
    wayPointsList.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint7 = new WayPoint(QPoint(880, 200));
    wayPointsList.push_back(wayPoint7);
    wayPoint6->setNextWayPoint(wayPoint7);

    WayPoint *wayPoint8 = new WayPoint(QPoint(700, 120));
    wayPointsList.push_back(wayPoint8);
    wayPoint7->setNextWayPoint(wayPoint8);

    WayPoint *wayPoint9 = new WayPoint(QPoint(700, 0));
    wayPointsList.push_back(wayPoint9);
    wayPoint8->setNextWayPoint(wayPoint9);


}
void Map_2::addWayPoints2()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(510, 1000));
    wayPointsList2.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(520, 880));
    wayPointsList2.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(510, 760));
    wayPointsList2.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(300, 550));
    wayPointsList2.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(200, 400));
    wayPointsList2.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(240, 310));
    wayPointsList2.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint7 = new WayPoint(QPoint(380, 190));
    wayPointsList2.push_back(wayPoint7);
    wayPoint6->setNextWayPoint(wayPoint7);

    WayPoint *wayPoint8 = new WayPoint(QPoint(600, 150));
    wayPointsList2.push_back(wayPoint8);
    wayPoint7->setNextWayPoint(wayPoint8);

    WayPoint *wayPoint9 = new WayPoint(QPoint(700, 120));
    wayPointsList2.push_back(wayPoint9);
    wayPoint8->setNextWayPoint(wayPoint9);

    WayPoint *wayPoint10 = new WayPoint(QPoint(700, 0));
    wayPointsList2.push_back(wayPoint10);
    wayPoint9->setNextWayPoint(wayPoint10);


}
void Map_3::addWayPoints()
{
    //敌人航点【可改】
    WayPoint *wayPoint1 = new WayPoint(QPoint(540, 1000));
    wayPointsList.push_back(wayPoint1);

    WayPoint *wayPoint2 = new WayPoint(QPoint(550, 900));
    wayPointsList.push_back(wayPoint2);
    wayPoint1->setNextWayPoint(wayPoint2);

    WayPoint *wayPoint3 = new WayPoint(QPoint(570, 710));
    wayPointsList.push_back(wayPoint3);
    wayPoint2->setNextWayPoint(wayPoint3);

    WayPoint *wayPoint4 = new WayPoint(QPoint(700, 700));
    wayPointsList.push_back(wayPoint4);
    wayPoint3->setNextWayPoint(wayPoint4);

    WayPoint *wayPoint5 = new WayPoint(QPoint(770, 600));
    wayPointsList.push_back(wayPoint5);
    wayPoint4->setNextWayPoint(wayPoint5);

    WayPoint *wayPoint6 = new WayPoint(QPoint(700, 500));
    wayPointsList.push_back(wayPoint6);
    wayPoint5->setNextWayPoint(wayPoint6);

    WayPoint *wayPoint7 = new WayPoint(QPoint(400, 570));
    wayPointsList.push_back(wayPoint7);
    wayPoint6->setNextWayPoint(wayPoint7);

    WayPoint *wayPoint8 = new WayPoint(QPoint(270, 400));
    wayPointsList.push_back(wayPoint8);
    wayPoint7->setNextWayPoint(wayPoint8);

    WayPoint *wayPoint9 = new WayPoint(QPoint(500, 360));
    wayPointsList.push_back(wayPoint9);
    wayPoint8->setNextWayPoint(wayPoint9);

    WayPoint *wayPoint10 = new WayPoint(QPoint(600, 300));
    wayPointsList.push_back(wayPoint10);
    wayPoint9->setNextWayPoint(wayPoint10);

    WayPoint *wayPoint11 = new WayPoint(QPoint(650, 160));
    wayPointsList.push_back(wayPoint11);
    wayPoint10->setNextWayPoint(wayPoint11);

    WayPoint *wayPoint12 = new WayPoint(QPoint(800, 160));
    wayPointsList.push_back(wayPoint12);
    wayPoint11->setNextWayPoint(wayPoint12);

    WayPoint *wayPoint13 = new WayPoint(QPoint(900, 350));
    wayPointsList.push_back(wayPoint13);
    wayPoint12->setNextWayPoint(wayPoint13);

    WayPoint *wayPoint14 = new WayPoint(QPoint(1200, 350));
    wayPointsList.push_back(wayPoint14);
    wayPoint13->setNextWayPoint(wayPoint14);
}
void Map_1::loadTowerPositions()
{
    //这里和找航点是一样的，制作者需要自己不断尝试
    //找到比较合适的防御塔坑点
    QPoint pos[]=
    {
        QPoint(445,180),
        QPoint(120,380),
        QPoint(700,200),
        QPoint(370,380),
        QPoint(430,600),
        QPoint(600,550),
        QPoint(600,760)
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0;i<len;i++)
    {
        towerPositionList.push_back(pos[i]);
    }
}
void Map_2::loadTowerPositions()
{

    QPoint pos[]=
    {
        QPoint(340,200),
        QPoint(300,400),
        QPoint(550,129),
        QPoint(600,580),
        QPoint(900,400),
        QPoint(800,550)
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0;i<len;i++)
    {
        towerPositionList.push_back(pos[i]);
    }
}
void Map_3::loadTowerPositions()
{

    QPoint pos[]=
    {
        QPoint(445,180),
        QPoint(120,380),
        QPoint(700,200),
        QPoint(370,380),
        QPoint(430,600),
        QPoint(600,550),
        QPoint(600,760)
    };
    int len=sizeof(pos)/sizeof(pos[0]);
    for(int i=0;i<len;i++)
    {
        towerPositionList.push_back(pos[i]);
    }
}

//void Map_1::keyPressEvent(QKeyEvent *event)
//{
//    switch(event->key())
//    {
//    case Qt::Key_1:
//        //this->putZombie(qrand() % 5, 0);
//        break;
//    case Qt::Key_2:
//        //this->putZombie(qrand() % 5, 1);
//        break;
//    case Qt::Key_3:
//        //this->putZombie(qrand() % 5, 2);
//        break;
//    case Qt::Key_4:
//        //this->putZombie(qrand() % 5, 3);
//        break;
//    case Qt::Key_5:
//        //this->putZombie(qrand() % 5, 4);
//        break;
//    case Qt::Key_6:
//        //this->putZombie(qrand() % 5, 5);
//        break;
//    case Qt::Key_7:
//        //this->putZombie(qrand() % 5, 6);
//        break;
//    case Qt::Key_8:
//        //this->sunPoint += 100;
//        break;
//    case Qt::Key_9:
//        //this->threat = 6001;
//        break;
////    case Qt::Key_Escape:
////        emit toTitle();
////        break;
//    }
//}
void Map_1::doGameOver()
{
    if(!gameEnded)
    {
        gameEnded=true;
    }
}
void Map_2::doGameOver()
{
    if(!gameEnded)
    {
        gameEnded=true;
    }
}
void Map_3::doGameOver()
{
    if(!gameEnded)
    {
        gameEnded=true;
    }
}
bool TScene::canBuyTower(int moneyNeeded)
{
    if(playerGold >= moneyNeeded)
    {
        return true;
    }
    return false;
}

void Map_1::drawWave(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    if(diff ==0 || diff == 1)
    {
        painter->drawText(QRect(215, 935, 160, 40),QString("WAVES: %1/7").arg(m_waves+1));
    }
    else
    {
        painter->drawText(QRect(215, 935, 160, 40),QString("WAVES: %1/10").arg(m_waves+1));
    }
    painter->restore();
}

void Map_1::drawHP(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 20, 160, 40),QString("HP: %1/5").arg(playerHp));
    painter->restore();
}

void Map_1::drawGold(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 935, 160, 40),QString("GLOD: %1").arg(playerGold));
    painter->restore();
}
void Map_2::drawWave(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    if(diff ==0 || diff == 1)
    {
        painter->drawText(QRect(215, 935, 160, 40),QString("WAVES: %1/7").arg(m_waves+1));
    }
    else
    {
        painter->drawText(QRect(215, 935, 160, 40),QString("WAVES: %1/10").arg(m_waves+1));
    }
    painter->restore();
}

void Map_2::drawHP(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 20, 160, 40),QString("HP: %1/5").arg(playerHp));
    painter->restore();
}

void Map_2::drawGold(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 935, 160, 40),QString("GLOD: %1").arg(playerGold));
    painter->restore();
}
void Map_3::drawWave(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(215, 935, 160, 40),QString("WAVES: %1/7").arg(m_waves+1));
    painter->restore();
}

void Map_3::drawHP(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 20, 160, 40),QString("HP: %1/5").arg(playerHp));
    painter->restore();
}

void Map_3::drawGold(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    painter->drawText(QRect(45, 935, 160, 40),QString("GLOD: %1").arg(playerGold));
    painter->restore();
}
void TScene::removeBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    bulletList.removeOne(bullet);

}

void TScene::addBullet(Bullet *bullet)
{
    Q_ASSERT(bullet);
    bulletList.push_back(bullet);
}

Map_2::Map_2(int dif, QWidget * parent): TScene(parent), diff(dif)
{
    audioPlayer = new AudioPlayer(this);
    audioPlayer->startBGM_2Map();
    this->setMouseTracking(true);

    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("关卡二");
    MyPushButton * backBtn = new MyPushButton(":/item/back01.png", ":/item/back02.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());


    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            emit this->getMapBack();

        });

    });
    loadTowerPositions();
    addWayPoints();
    addWayPoints2();
    QTimer * timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateMap()));
    timer->start(30);
    QTimer::singleShot(3000,this,SLOT(gameStart()));
}
void Map_2::paintEvent(QPaintEvent *)
{
    if(gameEnded || gameWin)//画出游戏结束的画面
    {
        QString text=gameEnded ? "YOU LOST":"YOU WIN";
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }

    QPainter painter(this);
    QPixmap pix;
    pix.load(":/map/map02.bmp");
    painter.drawPixmap(0, 0 , this->width(), this->height(), pix);
    pix.load(":/item/ui.png");
    painter.drawPixmap(30, 5, 100, 50, pix);
    painter.drawPixmap(30, 920, 100, 50, pix);
    painter.drawPixmap(200, 920, 100, 50, pix);

//    foreach(const WayPoint * waypoint, wayPointsList)
//        waypoint->draw(&painter);


//    foreach(const WayPoint * waypoint, wayPointsList2)
//        waypoint->draw(&painter);

    foreach(const TowerPosition towerposition, towerPositionList)
        towerposition.draw(&painter);

    foreach(const TTower * tower, towersList)
        tower->draw(&painter);

    foreach(const TChooseTower * choose, chooseMenuList)
        choose->draw(&painter);

    foreach(const TUpgrade * upgrade, upgradeList)
        upgrade->draw(&painter);

    foreach(const Enemy * enemy, enemyList)
        enemy->draw(&painter);

    foreach(const Bullet * bullet, bulletList)
        bullet->draw(&painter);

    foreach(const Rocket * rocket, rocketList)
        rocket->draw(&painter);

    drawHP(&painter);
    drawGold(&painter);
    drawWave(&painter);

}


//void Map_2::keyPressEvent(QKeyEvent *event)
//{

//}
bool Map_2::loadWave()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    if(diff == 0)
    {
        if(m_waves>=7)
        {
            return false;
        }

        for(int i=0;i<16;++i)
        {
            if(easyMap2EnemyGene[m_waves][i] == 0)
                return true;
            int n = qrand() % 2;    //产生5以内的随机数
            WayPoint * startWayPoint;
            startWayPoint=(n == 1)?(wayPointsList.first()):(wayPointsList2.first());
            Enemy * enemy;
            switch (easyMap2EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(easyMap2EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    else if (diff == 1)
    {
        if(m_waves>=7)
        {
            return false;
        }

        for(int i=0;i<16;++i)
        {
            if(easyMap2EnemyGene[m_waves][i] == 0)
                return true;
            int n = qrand() % 2;    //产生5以内的随机数
            WayPoint * startWayPoint;
            startWayPoint=(n == 1)?(wayPointsList.first()):(wayPointsList2.first());
            Enemy * enemy;
            switch (easyMap2EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(easyMap2EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    else if (diff == 2)
    {
        if(m_waves>=10)
        {
            return false;
        }
        for(int i=0;i < 20;++i)
        {
            if(hardMap2EnemyGene[m_waves][i] == 0)
                return true;
            int n = qrand() % 2;    //产生5以内的随机数
            WayPoint * startWayPoint;
            startWayPoint=(n == 1)?(wayPointsList.first()):(wayPointsList2.first());
            Enemy * enemy;
            switch (hardMap2EnemyGene[m_waves][i])
            {
            case 1:
            {
                enemy=new normalEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 2:
            {
                enemy=new fastEnemy(startWayPoint,this);//创建一个新的enemy
                break;
            }
            case 3:
            {
                enemy=new bigNormalEnemy(startWayPoint, this);
                break;
            }
            case 5:
            {
                enemy = new bossEnemy(startWayPoint, this);
                break;
            }
            case 6:
            {
                enemy = new finalBossEnemy(startWayPoint, this);
                break;
            }
            }
            enemyList.push_back(enemy);
            QTimer::singleShot(hardMap2EnemyStartInterval[m_waves][i],enemy,SLOT(doActive()));
        }
    }
    return true;
}

Map_3::Map_3(int dif, QWidget * parent): TScene(parent), diff(dif)
{
    audioPlayer = new AudioPlayer(this);
    audioPlayer->startBGM_3Map();
    this->setMouseTracking(true);

    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("关卡三");
    QLabel * s = new QLabel(this);
    s->setText("此区域暂未开放！");
    s->setFont(QFont("Consolas", 30));
    s->setFixedSize(600, 100);
    s->move(500, 400);

    MyPushButton * backBtn = new MyPushButton(":/item/back01.png", ":/item/back02.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width(), this->height() - backBtn->height());


    connect(backBtn, &MyPushButton::clicked, [=](){
        //qDebug()<< "111";
        audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            emit this->getMapBack();
        });

    });
//    loadTowerPositions();
//    addWayPoints();
//    QTimer * timer=new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(updateMap()));
//    timer->start(30);
//    QTimer::singleShot(3000,this,SLOT(gameStart()));
}
void Map_3::paintEvent(QPaintEvent *)
{
    if(gameEnded || gameWin)//画出游戏结束的画面
    {
        QString text=gameEnded ? "YOU LOST":"YOU WIN";
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }

    QPainter painter(this);
    QPixmap pix;
    pix.load(":/map/map03.bmp");
    painter.drawPixmap(0, 0 , this->width(), this->height(), pix);
    pix.load(":/item/ui.png");
    painter.drawPixmap(30, 5, 100, 50, pix);
    painter.drawPixmap(30, 920, 100, 50, pix);
    painter.drawPixmap(200, 920, 100, 50, pix);

//    foreach(const WayPoint * waypoint, wayPointsList)
//        waypoint->draw(&painter);

    foreach(const TowerPosition towerposition, towerPositionList)
        towerposition.draw(&painter);

    foreach(const TTower * tower, towersList)
        tower->draw(&painter);

    foreach(const TChooseTower * choose, chooseMenuList)
        choose->draw(&painter);

    foreach(const TUpgrade * upgrade, upgradeList)
        upgrade->draw(&painter);

    foreach(const Enemy * enemy, enemyList)
        enemy->draw(&painter);

    foreach(const Bullet * bullet, bulletList)
        bullet->draw(&painter);

    foreach(const Rocket * rocket, rocketList)
        rocket->draw(&painter);

    drawHP(&painter);
    drawGold(&painter);
    drawWave(&painter);

}

bool Map_3::loadWave()
{
    if(m_waves>=6)
    {
        return false;
    }
    if(diff == 0)
    {
        int enemyStartInterval[]={100, 200, 400, 800, 1000, 2000, 4000};//敌人出现的时间
        for(int i=0;i < 7;++i)
        {
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy=new Enemy(startWayPoint,this);//创建一个新得enemy
            enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i], enemy, SLOT(doActive()));
        }
    }
    else if (diff == 1)
    {
        int enemyStartInterval[]={100, 150, 200, 250, 270, 400, 800, 1000, 2000, 4000};//敌人出现的时间
        for(int i=0;i < 10;++i)
        {
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy=new Enemy(startWayPoint,this);//创建一个新得enemy
            enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i], enemy, SLOT(doActive()));
        }
    }
    else if (diff == 2)
    {
        int enemyStartInterval[]={100, 101, 102, 150, 190, 200, 300, 400, 600,
                                  800, 900, 1000, 1500, 2000, 3000, 4000};//敌人出现的时间
        for(int i=0;i < 16;++i)
        {
            WayPoint * startWayPoint;
            startWayPoint=wayPointsList.first();
            Enemy * enemy=new Enemy(startWayPoint,this);//创建一个新得enemy
            enemyList.push_back(enemy);
            QTimer::singleShot(enemyStartInterval[i], enemy, SLOT(doActive()));
        }
    }
    return true;
}
//void Map_3::keyPressEvent(QKeyEvent *event)
//{

//}
