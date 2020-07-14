//#pragma execution_character_set("utf-8")
#include "audioplayer.h"
#include <QDir>
#include <QCoreApplication>
#include <QTimer>
// 为了解决mac下声音无法输出,总之发现使用绝对路径可以完成目标,蛋疼,因此以此种方式暂时处理
static const QString s_curDir = "D:/homework/tower_defense";
AudioPlayer::AudioPlayer(int i)
{
    i = 1;
}
AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
    , m_backgroundMusic_1(NULL)
{
    // 创建一直播放的背景音乐
    QUrl backGroundMusicUrl_1 = QUrl::fromLocalFile(s_curDir + "/music/startmenu.mp3");
    QUrl backGroundMusicUrl_2 = QUrl::fromLocalFile(s_curDir + "/music/savage_music_theme.wav");
    QUrl backGroundMusicUrl_2Map = QUrl::fromLocalFile(s_curDir + "/music/savage_music_desert_battle.wav");
    QUrl backGroundMusicUrl_3Map = QUrl::fromLocalFile(s_curDir + "/music/savage_music_jungle_battle.wav");
    QUrl backGroundMusicUrl_1Map = QUrl::fromLocalFile(s_curDir + "/music/Map_Theme_1.wav");

    //qDebug() << backgroundMusicUrl;
    if (QFile::exists(backGroundMusicUrl_1.toLocalFile()))
    {
        m_backgroundMusic_1 = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backGroundMusicUrl_1);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // 设置背景音乐循环播放
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic_1->setPlaylist(backgroundMusicList);
        m_backgroundMusic_1->setVolume(30);
    }
    if (QFile::exists(backGroundMusicUrl_2.toLocalFile()))
    {
        m_backgroundMusic_2 = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backGroundMusicUrl_2);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // 设置背景音乐循环播放
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic_2->setPlaylist(backgroundMusicList);
        m_backgroundMusic_2->setVolume(30);
    }
    if (QFile::exists(backGroundMusicUrl_1Map.toLocalFile()))
    {
        m_backgroundMusic_1Map = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backGroundMusicUrl_1Map);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // 设置背景音乐循环播放
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic_1Map->setPlaylist(backgroundMusicList);
        m_backgroundMusic_1Map->setVolume(30);
    }
    if (QFile::exists(backGroundMusicUrl_2Map.toLocalFile()))
    {
        m_backgroundMusic_2Map = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backGroundMusicUrl_2Map);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // 设置背景音乐循环播放
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic_2Map->setPlaylist(backgroundMusicList);
        m_backgroundMusic_2Map->setVolume(30);
    }
    if (QFile::exists(backGroundMusicUrl_3Map.toLocalFile()))
    {
        m_backgroundMusic_3Map = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backGroundMusicUrl_3Map);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // 设置背景音乐循环播放
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic_3Map->setPlaylist(backgroundMusicList);
        m_backgroundMusic_3Map->setVolume(30);
    }
}

void AudioPlayer::startBGM_1()
{

    if (m_backgroundMusic_1)
    {
        //qDebug()<< m_backgroundMusic;
        m_backgroundMusic_1->play();
    }
}
void AudioPlayer::stopBGM_1()
{
    //qDebug()<<"1";
    if(m_backgroundMusic_1)
    {
        m_backgroundMusic_1->stop();
    }
}

void AudioPlayer::startBGM_2()
{
    if (m_backgroundMusic_2)
    {
        //qDebug()<< m_backgroundMusic;
        m_backgroundMusic_2->play();
    }
}
void AudioPlayer::stopBGM_2()
{
    if(m_backgroundMusic_2)
    {
        m_backgroundMusic_2->stop();
    }
}

void AudioPlayer::startBGM_1Map()
{
    if (m_backgroundMusic_1Map)
    {
        //qDebug()<< m_backgroundMusic;
        m_backgroundMusic_1Map->play();
    }
}
void AudioPlayer::stopBGM_1Map()
{
    if(m_backgroundMusic_1Map)
    {
        m_backgroundMusic_1Map->stop();
    }
}

void AudioPlayer::startBGM_2Map()
{
    if (m_backgroundMusic_2Map)
    {
        //qDebug()<< m_backgroundMusic;
        m_backgroundMusic_2Map->play();
    }
}
void AudioPlayer::stopBGM_2Map()
{
    if(m_backgroundMusic_2Map)
    {
        m_backgroundMusic_2Map->stop();
    }
}

void AudioPlayer::startBGM_3Map()
{
    if (m_backgroundMusic_3Map)
    {
        //qDebug()<< m_backgroundMusic;
        m_backgroundMusic_3Map->play();
    }
}
void AudioPlayer::stopBGM_3Map()
{
    if(m_backgroundMusic_3Map)
    {
        m_backgroundMusic_3Map->stop();
    }
}
void AudioPlayer::playWinSound(){
    if(m_winMusic)
        m_winMusic->play();
}

void AudioPlayer::playLoseSound(){
    if(m_loseMusic)
        m_loseMusic->play();
}
void AudioPlayer::playSound(SoundType soundType)
{
    static const QUrl mediasUrls[] =
    {
        QUrl::fromLocalFile(s_curDir + "/music/Sound_TowerBuilding.wav"),               // 放塔时的声音
        QUrl::fromLocalFile(s_curDir + "/music/Sound_LooseLife.wav"),                   // 基地费血时的声音
        QUrl::fromLocalFile(s_curDir + "/music/laser_shoot.wav"),                       // 打中敌人时的生意
        QUrl::fromLocalFile(s_curDir + "/music/death.wav"),                             // 敌人升天时的声音
        QUrl::fromLocalFile(s_curDir + "/music/Archer_Ready.wav"),                      // 弓箭塔准备就绪
        QUrl::fromLocalFile(s_curDir + "/music/Sound_ArrowHit2.wav"),                   // 弓箭塔攻击
        QUrl::fromLocalFile(s_curDir + "/music/Archer_Taunt1.wav"),                     // 弓箭塔升级1
        QUrl::fromLocalFile(s_curDir + "/music/Archer_Taunt2.wav"),                     // 弓箭塔升级2
        QUrl::fromLocalFile(s_curDir + "/music/Artillery_Ready.wav"),                   // 迫击炮准备就绪
        QUrl::fromLocalFile(s_curDir + "/music/Sound_Bomb1.wav"),                       // 迫击炮攻击
        QUrl::fromLocalFile(s_curDir + "/music/Artillery_Taunt1.wav"),                  // 迫击炮升级1
        QUrl::fromLocalFile(s_curDir + "/music/Artillery_Taunt2.wav"),                  // 迫击炮升级2
        QUrl::fromLocalFile(s_curDir + "/music/Mage_Ready.wav"),                        // 魔法塔准备就绪
        QUrl::fromLocalFile(s_curDir + "/music/Mage_Taunt1.wav"),                       // 魔法塔升级1
        QUrl::fromLocalFile(s_curDir + "/music/Mage_Taunt1.wav"),                       // 魔法塔升级2
        QUrl::fromLocalFile(s_curDir + "/music/Sound_VictoryCheer.wav"),                // 胜利的声音
        QUrl::fromLocalFile(s_curDir + "/music/NotEnoughSun.wav"),
        QUrl::fromLocalFile(s_curDir + "/music/push.wav"),
        QUrl::fromLocalFile(s_curDir + "/music/Sound_TowerSell.wav")
    };
//    static QSoundEffect effect;
//    effect.setSource(mediasUrls[soundType]);
//    //设置音量，0-1

//    effect.setVolume(0.8f);
//    effect.play();

    static QMediaPlayer player(this);
    if (QFile::exists(mediasUrls[soundType].toLocalFile()))
    {
        player.setMedia(mediasUrls[soundType]);
        if(soundType == Push)
        {
            player.setPosition(500);
        }
        player.play();


    }
}
