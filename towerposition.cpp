#include "towerposition.h"

#include <QSize>
#include <QPainter>
#include <QPixmap>
#include <QDebug>

const QSize TowerPosition::m_fixedSize(70,70);//设置图片的大小

TowerPosition::TowerPosition(QPoint pos, QString path):
    m_pos(pos),
    m_path(path),
    m_hasTower(false)
{
}

bool TowerPosition::hasTower()
{
    return m_hasTower;
}
bool TowerPosition::hasChooseCircle()
{
    return m_hasChooseCircle;
}
bool TowerPosition::hasUpgradeCircle()
{
    return m_hasUpgradeCircle;
}

void TowerPosition::setHasTower(bool hasTower)
{
    m_hasTower = hasTower;
}
void TowerPosition::setHasChooseCircle(bool hasChooseCircle)
{
    m_hasChooseCircle = hasChooseCircle;
}
void TowerPosition::setHasUpgradeCircle(bool hasUpgradeCircle)
{
    m_hasUpgradeCircle = hasUpgradeCircle;
}


QPoint TowerPosition::getCenterPos()
{
    QPoint tmp;
    tmp.setX(m_pos.x()+m_fixedSize.width()/2);
    tmp.setY(m_pos.y()+m_fixedSize.height()/2);
    return tmp;
}

QPoint TowerPosition::getPos()
{
    return m_pos;
}

bool TowerPosition::ContainPos(QPoint pos)
{

    bool xInHere = pos.x() > m_pos.x() + 21 && (pos.x()< 21 + m_pos.x()+m_fixedSize.width());
    bool yInHere = pos.y() > m_pos.y() + 40 && (pos.y()< 40 + m_pos.y()+m_fixedSize.height());
    //qDebug() << pos.y() - m_pos.y() -40<< pos.y() - m_pos.y() - m_fixedSize.height()-40;
    return xInHere && yInHere;
}
bool TowerPosition::ContainArcher(QPoint pos)
{
    bool xInHere= (pos.x() > m_pos.x()-40) && (pos.x() < m_pos.x()+22);
    bool yInHere = (pos.y() > m_pos.y()-30) && (pos.y() < m_pos.y() +45);

    return xInHere && yInHere;
}
bool TowerPosition::ContainArtillery(QPoint pos)
{
    bool xInHere= (pos.x() > m_pos.x()+85) && (pos.x() < m_pos.x()+147);
    bool yInHere = (pos.y() > m_pos.y()+111) && (pos.y() < m_pos.y() +186);

    return xInHere && yInHere;
}
bool TowerPosition::ContainMage(QPoint pos)
{
    bool xInHere= (pos.x() > m_pos.x()-40) && (pos.x() < m_pos.x()+22);
    bool yInHere = (pos.y() > m_pos.y()+111) && (pos.y() < m_pos.y() +186);

    return xInHere && yInHere;
}

bool TowerPosition::ContainUpgrade(QPoint pos)
{
    bool xInHere= (pos.x() > m_pos.x()+22) && (pos.x() < m_pos.x()+84);
    bool yInHere = (pos.y() > m_pos.y()-35) && (pos.y() < m_pos.y() +40);

    return xInHere && yInHere;
}

bool TowerPosition::ContainSell(QPoint pos)
{
    bool xInHere= (pos.x() > m_pos.x()+22) && (pos.x() < m_pos.x()+84);
    bool yInHere = (pos.y() > m_pos.y()+105) && (pos.y() < m_pos.y() +180);

    return xInHere && yInHere;
}
void TowerPosition::draw(QPainter *painter) const
{
    painter->drawPixmap(m_pos.x()-70,m_pos.y()-30, 260, 233, m_path);
}
void TowerPosition::setRemoveTower()
{
    setHasTower(false);
    setHasUpgradeCircle(false);
    setHasChooseCircle(false);
}
