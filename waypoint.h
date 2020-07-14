#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QPoint>
#include <QPainter>
#include <QColor>

// 敌人移动的航线.
class WayPoint
{
public:
    WayPoint(QPoint pos);

    void setNextWayPoint(WayPoint *nextPoint);
    WayPoint* getNextWayPoint() const;
    const QPoint getPos() const;

    void draw(QPainter *painter) const;

private:
    const QPoint		m_pos;
    WayPoint *			m_nextWayPoint;
};

#endif // WAYPOINT_H
