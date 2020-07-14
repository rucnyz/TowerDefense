#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QDebug>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSoundEffect>
class QMediaPlayer;

enum SoundType
{
    TowerPlaceSound,		// 放塔时的声音
    LifeLoseSound,			// 基地费血时的声音
    LaserShootSound,		// 打中敌人时的生意
    EnemyDestorySound,      // 敌人升天时的声音
    ArcherReady,            // 弓箭塔准备就绪
    ArcherHit,              // 弓箭塔攻击
    ArcherTaunt1,           // 弓箭塔升级1
    ArcherTaunt2,           // 弓箭塔升级2
    ArtilleryReady,         // 迫击炮准备就绪
    ArtilleryHit,           // 迫击炮攻击
    ArtilleryTaunt1,        // 迫击炮升级1
    ArtilleryTaunt2,        // 迫击炮升级2
    MageReady,              // 魔法塔准备就绪
//    MageHit,                // 魔法塔攻击
    MageTaunt1,             // 魔法塔升级1
    MageTaunt2,             // 魔法塔升级2
    VictoryCheer,           // 胜利的声音
    NotEnoughGold,          // 没有足够金钱
    Push,                   // 点击
    TowerSell
};


class AudioPlayer : public QObject
{
public:
    explicit AudioPlayer(QObject *parent = 0);
    AudioPlayer(int i);

    void startBGM_1();
    void stopBGM_1();

    void startBGM_2();
    void stopBGM_2();

    void startBGM_1Map();
    void stopBGM_1Map();

    void stopBGM_2Map();
    void startBGM_2Map();

    void startBGM_3Map();
    void stopBGM_3Map();

    void playWinSound();
    void playLoseSound();

    void playSound(SoundType soundType);

private:
    QMediaPlayer *m_backgroundMusic_1; // 只用来播放背景音乐
    QMediaPlayer *m_backgroundMusic_2; // 只用来播放背景音乐
    QMediaPlayer *m_backgroundMusic_3Map; // 只用来播放背景音乐
    QMediaPlayer *m_backgroundMusic_1Map; // 只用来播放背景音乐
    QMediaPlayer *m_backgroundMusic_2Map; // 只用来播放背景音乐

    QMediaPlayer *m_winMusic;
    QMediaPlayer *m_loseMusic;
};


#endif // AUDIOPLAYER_H
