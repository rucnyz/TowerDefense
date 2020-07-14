#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QSize>
#include <QPainter>
#include <QString>
#include "ttower.h"

class TowerPosition
{
public:
    TowerPosition(QPoint pos,QString path=(":/tower/keng.png"));//图片的路径
    QPoint getCenterPos();//得到防御塔坑的中心点
    QPoint getPos();//得到防御塔坑的左上点

    bool ContainPos(QPoint pos);//判断pos点是否在防御塔坑的范围内
    bool ContainArcher(QPoint pos);
    bool ContainArtillery(QPoint pos);
    bool ContainMage(QPoint pos);

    bool ContainUpgrade(QPoint pos);
    bool ContainSell(QPoint pos);

    void draw(QPainter * painter) const;

    bool hasTower();//判断该防御塔坑内有没有防御塔
    bool hasChooseCircle(); //判断该塔坑内有没有选择圈
    bool hasUpgradeCircle();

    void setHasTower(bool hasTower=true);//设置是否有防御塔
    void setHasChooseCircle(bool hasChooseCircle=true);
    void setHasUpgradeCircle(bool hasUpgradeCircle=true);

    void setRemoveTower();

    TTower * m_tower;

 private:
    QPoint m_pos;
    QString m_path;


    bool m_hasTower=false;
    bool m_hasChooseCircle=false;
    bool m_hasUpgradeCircle=false;
    static const QSize m_fixedSize;//防御塔坑的固定大小
};
#endif // TOWERPOSITION_H
