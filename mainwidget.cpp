#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->setMouseTracking(true);
    //设置开始场景
    this->setFixedSize(1200, 1000);
    this->setWindowIcon(QIcon(":/startmenu/icon.jpg"));
    this->setWindowTitle("塔防游戏");
    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM_1();

    connect(ui->pushButton, &QPushButton::clicked, [=](){
        this->close();
    });

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/item/startgame01.png", ":/item/startgame02.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5, this->height() * 0.7);
    //选择关卡场景
    connect(startBtn, &MyPushButton::clicked, [=](){
        chooseDifficultyMenu = new ChooseDifficultyMenu;
        m_audioPlayer->stopBGM_1();
        m_audioPlayer->playSound(Push);
        QTimer::singleShot(500, this, [=](){
            this->hide();
            chooseDifficultyMenu->show();
        });
        //选择难度界面返回
        connect(this->chooseDifficultyMenu, &ChooseDifficultyMenu::getStartBack, [=](){
            m_audioPlayer->playSound(Push);
            delete chooseDifficultyMenu;
            this->show();
            m_audioPlayer->startBGM_1();
        });

    });



}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/startmenu/startmenu_change.bmp");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);



}
