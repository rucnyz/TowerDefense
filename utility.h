
#ifndef UTILITY_H
#define UTILITY_H

#include <QPoint>
#include <cmath>
#include <QDebug>

inline bool collisionWithCircle(QPoint p1,int r1,QPoint p2,int r2)
{//我们只创建了h文件，没有创建cpp文件，所以要定义内联函数，直接在h文件内部实现函数
    int xx=p1.x()-p2.x();
    int yy=p1.y()-p2.y();
    int dis=sqrt(xx*xx+yy*yy);
    if(dis<r1+r2)
    {
        return true;
    }
    return false;
}
#endif // UTILITY_H

